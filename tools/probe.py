#!/usr/bin/env python3
"""tools/probe.py -- standalone probe harness with controls (T3, Phase 1.4). Runs in the image (dc.sh run).

Each row of config/probes.tsv (`name alias start end c_path notes`, end exclusive) names our C function `name`;
it is compiled per triple (cc1 x maspsx --aspsx-version x -G x -O) through cpp -> cc1 -> maspsx -> as 2.42 and
its bytes (symbol value/size from readelf) are compared word by word with the target: [start,end) of the
alias's binary under $BASEDIR (read at run time only, G12), or, for alias `self`, the probe's own object under
the control triple. Relocated fields (objdump -r) are masked on both sides. Proves body shape only (G10).

  probe.py [names…] [--cc C,…] [--aspsx V,…] [-G N,…] [-O N,…]   match table per probe + controls
  probe.py --pinned [names…]                                      the `pinned` triple of config/toolchains.tsv
  probe.py --selftest                                             controls only, narrowed matrix
Controls (every run): known-true = probe vs its own object under the control triple (must match);
known-false = same probe with the other -O vs that object (must differ). Either misbehaving -> exit 1.
Scratch under .run/probe/. Exit 0 ok, 1 control/pinned failure, 2 usage/config error.
"""
import argparse
import itertools
import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import splat_gen  # noqa: E402

ROOT = splat_gen.ROOT
PROBES = ROOT / "config/probes.tsv"
TOOLCHAINS = ROOT / "config/toolchains.tsv"
SCRATCH = ROOT / ".run/probe"
DISC_CUE = Path("/disc/Dino Crisis 2 (USA).cue")
CC_DIR = Path("/opt/cc")
WIBO = CC_DIR / "wibo/wibo"
MASPSX = CC_DIR / "maspsx/maspsx.py"
CROSS = "mipsel-linux-gnu-"
DROPPED = {"psyq3.6"}  # DOS exe, wibo cannot load (docs/ops/docker-host.md "Candidate toolchains")
# maspsx README behaviour table, versions >= 2.30 (T2: ASPSX >= 2.30 fleet-wide)
ASPSX_DEFAULT = ["2.30", "2.34", "2.56", "2.67", "2.77", "2.79", "2.81", "2.86"]
G_DEFAULT = [0, 4, 8]
O_DEFAULT = [1, 2]
SELFTEST = {"cc": ["gcc-2.8.1-psx", "psyq4.4"], "aspsx": ["2.79"], "G": [0], "O": [1, 2]}
MASKS = {"R_MIPS_26": 0x03FFFFFF, "R_MIPS_HI16": 0xFFFF, "R_MIPS_LO16": 0xFFFF, "R_MIPS_GPREL16": 0xFFFF}


def basedir():
    env = os.environ.get("BASEDIR")
    if env:
        return ROOT / env
    for d in ("extracted/retail/files", ".run/extracted/retail/files"):
        if (ROOT / d / splat_gen.EXE_PATH).is_file():
            return ROOT / d
    return ROOT / "extracted/retail/files"


def ensure_extracted():
    """No BASEDIR and no extracted exe -> `make extract OUT=.run/extracted/retail`; return 0 or 2 (message printed)."""
    if os.environ.get("BASEDIR") or (basedir() / splat_gen.EXE_PATH).is_file():
        return 0
    SCRATCH.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ)
    if "DC2_CUE" not in env and DISC_CUE.is_file():  # same default as tools/fleet_check.sh (image /disc mount)
        env["DC2_CUE"] = str(DISC_CUE)
    with open(SCRATCH / "extract.log", "w") as log:
        rc = subprocess.run(["make", "extract", "OUT=.run/extracted/retail"], cwd=ROOT, env=env,
                            stdout=log, stderr=subprocess.STDOUT).returncode
    if rc != 0 or not (basedir() / splat_gen.EXE_PATH).is_file():
        print(f"probe: extraction failed (make extract OUT=.run/extracted/retail, rc {rc}); "
              "see .run/probe/extract.log", file=sys.stderr)
        return 2
    return 0


def toolchains():
    rows = splat_gen.read_tsv(TOOLCHAINS, ("name", "kind", "url", "sha256", "gcc_version", "notes", "pinned"))
    return [r for r in rows if r["kind"] in ("oldgcc", "psyq") and r["name"] not in DROPPED]


def tname(t):
    return f"{t[0]}/a{t[1]}/G{t[2]}/O{t[3]}"


def run(cmd, cwd, stdin=None):
    p = subprocess.run(cmd, cwd=cwd, input=stdin, capture_output=True)
    if p.returncode:
        raise RuntimeError(f"{' '.join(map(str, cmd))}: rc {p.returncode}: {p.stderr.decode(errors='replace')[-400:]}")
    return p.stdout


class Probe:
    def __init__(self, row, cc_kind, scratch=SCRATCH):
        self.row, self.name, self.kind = row, row["name"], cc_kind
        self.dir = scratch / self.name
        self.dir.mkdir(parents=True, exist_ok=True)
        src = ROOT / row["c_path"]
        run([CROSS + "cpp", "-P", "-undef", "-nostdinc", "-D__GNUC__=2", str(src), "-o", "probe.i"], self.dir)
        self.cache = {}

    def asm(self, cc, g, o):
        s = f"{cc}_G{g}_O{o}.s"
        if ("s", cc, g, o) not in self.cache:
            flags = ["-quiet", f"-O{o}", f"-G{g}", "-mips1", "-fno-builtin", "probe.i", "-o", s]
            exe = [str(CC_DIR / cc / "cc1")] if self.kind[cc] == "oldgcc" else [str(WIBO), str(CC_DIR / cc / "CC1PSX.EXE")]
            run(exe + flags, self.dir)
            self.cache[("s", cc, g, o)] = s
        return s

    def obj(self, t):
        """Compile triple t; return (words, masks, relocs) of the function, or raise RuntimeError."""
        if t in self.cache:
            return self.cache[t]
        cc, a, g, o = t
        s = self.cache[("s", cc, g, o)]
        out = f"{cc}_a{a}_G{g}_O{o}.o"
        text = run([sys.executable, str(MASPSX), f"--aspsx-version={a}", f"-G{g}", s], self.dir)
        run([CROSS + "as", "-march=r3000", "-mabi=32", "-G0", "-o", out, "-"], self.dir, text)
        self.cache[t] = extract(self.dir / out, self.name)
        return self.cache[t]


def extract(o, fn):
    val = size = None
    for line in run([CROSS + "readelf", "-sW", str(o)], o.parent).decode().splitlines():
        f = line.split()
        if len(f) >= 8 and f[7] == fn and f[3] == "FUNC":
            val, size = int(f[1], 16), int(f[2])
    if val is None or not size:
        raise RuntimeError(f"{o.name}: no sized FUNC symbol {fn}")
    binf = o.with_suffix(".text.bin")
    run([CROSS + "objcopy", "-O", "binary", "-j", ".text", str(o), str(binf)], o.parent)
    data = binf.read_bytes()[val:val + size]
    words = [int.from_bytes(data[i:i + 4], "little") for i in range(0, len(data) - len(data) % 4, 4)]
    masks, relocs = {}, []
    sect = False
    for line in run([CROSS + "objdump", "-r", "-j", ".text", str(o)], o.parent).decode().splitlines():
        f = line.split()
        if line.startswith("RELOCATION RECORDS"):
            sect = ".text]" in line
        elif sect and len(f) >= 2 and all(c in "0123456789abcdef" for c in f[0]):
            off = int(f[0], 16) - val
            if 0 <= off < size:
                masks[off // 4] = masks.get(off // 4, 0) | MASKS.get(f[1], 0xFFFFFFFF)
                relocs.append((off, f[1], f[2] if len(f) > 2 else ""))
    return words, masks, tuple(relocs)


def compare(a, b):
    """Return None on match, else first-diff word index (length mismatch = no match)."""
    (wa, ma), (wb, mb) = a[:2], b[:2]
    for i in range(min(len(wa), len(wb))):
        keep = ~(ma.get(i, 0) | mb.get(i, 0)) & 0xFFFFFFFF
        if wa[i] & keep != wb[i] & keep:
            return i
    return None if len(wa) == len(wb) else min(len(wa), len(wb))


def target_of(row, fleet_by_alias):
    r = fleet_by_alias.get(row["alias"])
    if r is None:
        raise SystemExit(f"probe {row['name']}: alias {row['alias']} not in config/loadmap.tsv fleet")
    start, end = int(row["start"], 16), int(row["end"], 16)
    f = basedir() / r["path"]
    if not f.exists():
        raise SystemExit(f"probe {row['name']}: target binary absent: {f} (set BASEDIR or extract)")
    off = splat_gen.off_of(r, start)
    data = f.read_bytes()[off:off + end - start]
    return [int.from_bytes(data[i:i + 4], "little") for i in range(0, len(data) - len(data) % 4, 4)], {}


def classes(probes, res, triples):
    """Group triples into output equivalence classes: key = per probe the raw (unmasked) words + reloc list."""
    out = {}
    for t in triples:
        out.setdefault(tuple((tuple(res[(p.name, t)][0]), res[(p.name, t)][2]) for p in probes), []).append(t)
    return list(out.values())


def ladder(probes, res, matches_all, triples):
    """Print `ladder: exactly 1 triple matches all K` (m == 1) or `ladder: exactly <m> triples match all K`
    (m = classes), then one line per class."""
    cls = classes(probes, res, [t for t in triples if t in matches_all])
    m = len(cls)
    print(f"ladder: exactly {m} " + ("triple matches" if m == 1 else "triples match") + f" all {len(probes)}")
    for i, c in enumerate(cls, 1):
        print(f"  class {i}: {len(c)} triple(s): {' '.join(tname(t) for t in c)}")


def build(probes, triples, pool):
    """Compile every (probe, triple); return {(name, t): (words, masks) | RuntimeError}."""
    def one_s(p, k):
        try:
            p.asm(*k)
        except RuntimeError as e:
            p.cache[("err",) + k] = e

    def one_o(p, t):
        e = p.cache.get(("err", t[0], t[2], t[3]))
        if e:
            return e
        try:
            return p.obj(t)
        except RuntimeError as e:
            return e

    keys = sorted({(t[0], t[2], t[3]) for t in triples})
    list(pool.map(lambda pk: one_s(*pk), [(p, k) for p in probes for k in keys]))
    jobs = [(p, t) for p in probes for t in triples]
    return {(p.name, t): r for (p, t), r in zip(jobs, pool.map(lambda pt: one_o(*pt), jobs))}


def controls(probe, ctl, pool):
    """Known-true and known-false for one probe under triple ctl; return (true_ok, false_ok, detail)."""
    wrong = (ctl[0], ctl[1], ctl[2], 1 if ctl[3] == 2 else 2)
    res = build([probe], [ctl, wrong], pool)
    ref, bad = res[(probe.name, ctl)], res[(probe.name, wrong)]
    if isinstance(ref, RuntimeError) or isinstance(bad, RuntimeError):
        return False, False, f"compile error under {tname(ctl)}: {ref if isinstance(ref, RuntimeError) else bad}"
    probe.cache.pop(ctl)  # recompile from scratch for the known-true side
    probe.cache.pop(("s", ctl[0], ctl[2], ctl[3]))
    again = build([probe], [ctl], pool)[(probe.name, ctl)]
    t_ok = not isinstance(again, RuntimeError) and compare(again, ref) is None
    f_ok = compare(bad, ref) is not None
    return t_ok, f_ok, f"{tname(ctl)} vs {tname(wrong)}"


def listarg(v):
    return [x for s in (v or []) for x in s.split(",") if x]


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("names", nargs="*", help="probe names (default: all rows)")
    ap.add_argument("--cc", action="append")
    ap.add_argument("--aspsx", action="append")
    ap.add_argument("-G", action="append")
    ap.add_argument("-O", action="append")
    ap.add_argument("--pinned", action="store_true")
    ap.add_argument("--selftest", action="store_true")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 4)
    a = ap.parse_args()

    tcs = toolchains()
    kind = {r["name"]: r["kind"] for r in tcs}
    rows = splat_gen.read_tsv(PROBES, ("name", "alias", "start", "end", "c_path", "notes"))
    if a.selftest:
        rows = [r for r in rows if r["alias"] == "self"]
        sel = {k: listarg(getattr(a, k)) or [str(x) for x in v] for k, v in SELFTEST.items()}
    else:
        sel = {"cc": listarg(a.cc) or [r["name"] for r in tcs], "aspsx": listarg(a.aspsx) or ASPSX_DEFAULT,
               "G": listarg(a.G) or [str(x) for x in G_DEFAULT], "O": listarg(a.O) or [str(x) for x in O_DEFAULT]}
    if a.names:
        rows = [r for r in rows if r["name"] in a.names]
    unknown = [c for c in sel["cc"] if c not in kind]
    if unknown or not rows:
        print(f"probe: unknown cc {unknown}" if unknown else "probe: no probe rows selected", file=sys.stderr)
        return 2
    order = {r["name"]: i for i, r in enumerate(tcs)}
    triples = sorted(itertools.product(sel["cc"], sel["aspsx"], map(int, sel["G"]), map(int, sel["O"])),
                     key=lambda t: (order[t[0]], tuple(map(int, t[1].split("."))), t[2], t[3]))
    selfrows = [r for r in rows if r["alias"] == "self"]
    if not selfrows:
        selfrows = [r for r in splat_gen.read_tsv(PROBES, ("name", "alias", "start", "end", "c_path", "notes"))
                    if r["alias"] == "self"][:1]
    pinned = None
    if a.pinned:
        pins = [(r["name"], r.get("pinned", "-")) for r in tcs if r.get("pinned", "-") not in ("-", "", None)]
        if len(pins) != 1:
            print("probe --pinned: no pinned triple recorded (config/toolchains.tsv column 7 `pinned`, "
                  "e.g. `a2.79/G0/O2` on exactly one cc row; see tools/probes/README.md)" if not pins else
                  f"probe --pinned: {len(pins)} rows carry a pinned value; exactly one allowed", file=sys.stderr)
            return 2
        cc, v = pins[0]
        av, gv, ov = v.split("/")
        pinned = (cc, av[1:], int(gv[1:]), int(ov[1:]))
        rows = [r for r in rows if r["alias"] != "self"]
        if not rows:
            print("probe --pinned: no game probe rows (alias != self) selected", file=sys.stderr)
            return 2

    if not a.selftest and any(r["alias"] != "self" for r in rows) and ensure_extracted():
        return 2
    SCRATCH.mkdir(parents=True, exist_ok=True)
    rc = 0
    with ThreadPoolExecutor(max(1, a.j)) as pool:
        ctl = triples[0]
        cprobe = Probe(selfrows[0], kind) if selfrows else None
        ctl_triples = triples if a.selftest else [ctl]
        t_ok = f_ok = cprobe is not None
        for t in ctl_triples:
            ok_t, ok_f, det = controls(cprobe, t, pool) if cprobe else (False, False, "no self row")
            if not (ok_t and ok_f):
                print(f"control failed: {det}: true {'ok' if ok_t else 'FAIL'}, false {'ok' if ok_f else 'FAIL'}")
            t_ok, f_ok = t_ok and ok_t, f_ok and ok_f
        if not a.selftest:
            fleet = {r["alias"]: r for r in splat_gen.fleet()[0]}
            probes = [Probe(r, kind) for r in rows]
            res = build(probes, triples + ([pinned] if pinned and pinned not in triples else []), pool)
            matches_all = set(triples)
            game = [p for p in probes if p.row["alias"] != "self"]
            for p in probes:
                tgt = res[(p.name, ctl)] if p.row["alias"] == "self" else target_of(p.row, fleet)
                print(f"probe {p.name} ({p.row['alias']} {p.row['start']}..{p.row['end']}, "
                      f"{len(tgt[0])} words)" + (f" target = own object under {tname(ctl)}" if p.row["alias"] == "self" else ""))
                hits = []
                for t in triples:
                    r = res[(p.name, t)]
                    d = "error" if isinstance(r, RuntimeError) else compare(r, tgt)
                    if d is None:
                        hits.append(t)
                    print(f"  {tname(t):32} {'match' if d is None else d if d == 'error' else f'diff@{d}'}")
                if p in game:
                    matches_all &= set(hits)
                print(f"  matching: {len(hits)} of {len(triples)}: {' '.join(tname(t) for t in hits) or '-'}")
            if pinned:
                k = sum(1 for p in probes if not isinstance(res[(p.name, pinned)], RuntimeError)
                        and compare(res[(p.name, pinned)], target_of(p.row, fleet)) is None)
                print(f"pinned {tname(pinned)}")
                print(f"probes identical: {k} of {len(probes)} under the pinned triple")
                rc = 0 if k == len(probes) else 1
            if game:
                ladder(game, res, matches_all, triples)
        print(f"controls: true {'ok' if t_ok else 'FAIL'}, false {'ok' if f_ok else 'FAIL'}")
        if not (t_ok and f_ok):
            rc = 1
    return rc


if __name__ == "__main__":
    sys.exit(main())
