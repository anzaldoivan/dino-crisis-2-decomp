#!/usr/bin/env python3
"""tools/harness.py -- differential harness (T6, Phase 1.5); stdlib only, run in the container:
    dc.sh run python3 tools/harness.py [--plant PAIR | --selftest]     (normally from tools/fleet_check.sh)

A pair = two independent computations of one fact; side B never calls or imports side A's tool.
  fleet        A .run/harness/clean_run.tsv (alias sha1, written by fleet_check.sh; N = loadmap exe|code rows)
               B touch src/**/*.c, `make -j build`, sha1 build/<alias>.bin; agree iff all N equal, make rc 0 and
               every bin matches config/check.<alias>.sha.
  compiles     A units `bash tools/compile_only.sh` compiled (src/**/*.c minus its FAILED lines, K == U)
               B C-unit objects linked (build/<alias>/src/**.c.o named in build/<alias>.ld).
  coverage     A `tools/census.py --check` rows (.run/census/functions.tsv) + its `data in text` spans and totals
               B every 4-byte word of each text region [first asm|c subsegment vram (config/splat/<alias>.yaml),
               config/boundaries.tsv text-end end): covered by exactly one row or span, no row outside text,
               per-alias and fleet totals equal census's printed X/Y/D.
  matched      A tools/banked.py source scan (alias, func_<ADDR>) of src/**/*.c
               B cc1-emitted functions (`.ent` in build/<alias>/src/**.c.s; INCLUDE_ASM not) whose object .text
               bytes (relocation fields of .rel.text masked) equal retail bytes at the census extent (address from
               build/<alias>.elf; offset via loadmap base / PS-X EXE header); bytes past the symbol inside the
               extent are zero.
  oracle       tools/oracle_diff.py: agree iff rc 0, 0 disagreements; control = its own `control:` lines ok.
  denominators A tools/progress.py per-alias G/b + fleet, tools/banked.py G
               B kind=game rows/bytes of .run/census/functions.tsv per alias (`make -s print-aliases`).
Every run feeds each pair's comparator a planted disagreement in memory (control: planted caught|FAIL).
--plant PAIR: only that pair, a planted disagreement in its real input (rc 1, DISAGREE). --selftest: --plant per pair
in a subprocess, asserts rc 1 + DISAGREE. Prints `pair <name>: agree|DISAGREE (<A> vs <B>) control: ...` + up to 20
detail lines, then `harness: A of P pairs agree, D disagreements`. rc 0 iff all agree and every control caught;
1 otherwise; 2 `REFUSED: <why>` on empty/missing input (G28). Every invocation appends one line to
.run/harness/history.tsv (`utc head mode agree total disagreements elapsed_s rc`). Writes only under .run/.
"""
import argparse
import datetime
import hashlib
import os
import re
import struct
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
OUT = ROOT / ".run/harness"
CLEAN = OUT / "clean_run.tsv"
HISTORY = OUT / "history.tsv"
CENSUS_TSV = ROOT / ".run/census/functions.tsv"
RETAIL = ROOT / ".run/extracted/retail/files"
PREFIX = "extracted/retail/files/"
PLANT_START = "slus_012_79:0x8001b45c"  # not a census start: oracle_diff must flag it
FUNC = re.compile(r"^func_([0-9A-Fa-f]{8})$")
MAXD = 20


class Refused(Exception):
    pass


def run(cmd, log=None):
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    if log:
        OUT.mkdir(parents=True, exist_ok=True)
        (OUT / log).write_text(r.stdout + r.stderr)
    return r


def need(cond, why):
    if not cond:
        raise Refused(why)


def h(v):
    return "0x%08x" % v


# ---- config readers (harness's own; no tool imported) -------------------------------------------------------------

def yaml_aliases():
    return sorted(p.stem for p in (ROOT / "config/splat").glob("*.yaml"))


def read_yaml(alias):
    """(target path under files/, [(off, type, seg_start, seg_vram)])."""
    path, subs, seg = None, [], None
    for line in (ROOT / f"config/splat/{alias}.yaml").read_text().splitlines():
        s = line.split("#")[0].rstrip()
        m = re.match(r"^\s+target_path:\s*(\S+)", s)
        if m:
            path = m.group(1)[len(PREFIX):] if m.group(1).startswith(PREFIX) else m.group(1)
        if re.match(r"^  - name:", s):
            seg = {}
        m = re.match(r"^    (start|vram):\s*(0x[0-9a-fA-F]+)", s)
        if m and seg is not None:
            seg[m.group(1)] = int(m.group(2), 16)
        m = re.match(r"^      - \[(0x[0-9a-fA-F]+),\s*(\w+)", s)
        if m and seg is not None and "vram" in seg:
            subs.append((int(m.group(1), 16), m.group(2), seg["start"], seg["vram"]))
    return path, subs


def loadmap():
    """path -> (class, base|None); N = exe|code rows."""
    rows = {}
    for line in (ROOT / "config/loadmap.tsv").read_text().splitlines():
        if line.startswith("#") or not line.strip():
            continue
        f = line.split("\t")
        rows[f[0]] = (f[3], int(f[5], 16) if f[5].startswith("0x") else None)
    return rows


def text_ends():
    ends = {}
    for line in (ROOT / "config/boundaries.tsv").read_text().splitlines():
        f = line.split("\t")
        if len(f) >= 4 and f[1] == "text-end":
            ends[f[0]] = int(f[3], 16)
    return ends


def census_rows():
    """[(alias, start, end, kind)] from functions.tsv."""
    need(CENSUS_TSV.exists(), f"{CENSUS_TSV.relative_to(ROOT)} missing")
    rows = []
    for line in CENSUS_TSV.read_text().splitlines():
        if line and not line.startswith("#"):
            f = line.split("\t")
            rows.append((f[0], int(f[1], 16), int(f[2], 16), f[4]))
    need(rows, "census functions.tsv empty")
    return rows


def ld_units(alias):
    ld = (ROOT / f"build/{alias}.ld").read_text()
    return sorted(set(re.findall(rf"build/{re.escape(alias)}/(src/\S+?\.c)\.o", ld)))


def src_units():
    return sorted(str(p.relative_to(ROOT)) for p in (ROOT / "src").rglob("*.c") if not p.name.startswith("."))


# ---- ELF32 little-endian reader -------------------------------------------------------------------------------------

def elf(path):
    d = path.read_bytes()
    need(d[:6] == b"\x7fELF\x01\x01", f"{path.relative_to(ROOT)}: not ELF32 LE")
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", d, 0x2e)
    secs = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    stro = secs[shstrndx][4]
    cstr = lambda base, o: d[base + o:d.index(b"\0", base + o)].decode()  # noqa: E731
    names = [cstr(stro, s[0]) for s in secs]
    syms, rels = [], {}
    for s in secs:
        if s[1] == 2:  # SHT_SYMTAB
            so = secs[s[6]][4]
            for i in range(s[5] // 16):
                n, v, sz, info, _, ndx = struct.unpack_from("<IIIBBH", d, s[4] + 16 * i)
                syms.append((cstr(so, n), v, sz, info & 0xf, ndx))
        if s[1] == 9:  # SHT_REL
            rels[s[7]] = [struct.unpack_from("<II", d, s[4] + 8 * i) for i in range(s[5] // 8)]
    return {"d": d, "secs": secs, "names": names, "syms": syms, "rels": rels}


RELMASK = {2: 0xffffffff, 4: 0x03ffffff, 5: 0xffff, 6: 0xffff, 7: 0xffff}  # R_MIPS_32/26/HI16/LO16/GPREL16


# ---- pairs -----------------------------------------------------------------------------------------------------------
# Each pair(plant) -> dict(a=, b=, diffs=[...], control=bool, extra=bool ok); Refused on empty/missing input.

def pair_fleet(plant):
    n = sum(1 for c, _ in loadmap().values() if c in ("exe", "code"))
    need(CLEAN.exists(), f"{CLEAN.relative_to(ROOT)} missing (written by tools/fleet_check.sh)")
    a = dict(l.split("\t") for l in CLEAN.read_text().splitlines() if l and not l.startswith("#"))
    need(len(a) == n, f"clean_run.tsv has {len(a)} aliases, loadmap exe|code N = {n}")
    if plant:
        k = sorted(a)[0]
        a[k] = "0" * 40
    now = time.time()
    for u in src_units():
        os.utime(ROOT / u, (now, now))
    r = run(["make", f"-j{os.cpu_count() or 1}", "build"], log="fleet_build.log")
    b, ident = {}, 0
    for al in sorted(a):
        p = ROOT / f"build/{al}.bin"
        b[al] = hashlib.sha1(p.read_bytes()).hexdigest() if p.exists() else "missing"
        chk = ROOT / f"config/check.{al}.sha"
        ident += chk.exists() and chk.read_text().split()[0] == b[al]

    def cmp(x, y):
        out = [f"{al}: clean {x.get(al, '-')[:12]} vs rebuild {y.get(al, '-')[:12]}"
               for al in sorted(set(x) | set(y)) if x.get(al) != y.get(al)]
        if r.returncode:
            out.append(f"make build rc {r.returncode} (.run/harness/fleet_build.log)")
        if ident != n:
            out.append(f"rebuild {ident} of {n} byte-identical")
        return out
    diffs = cmp(a, b)
    bad = dict(b)
    bad[sorted(bad)[0]] = "f" * 40
    return {"a": f"clean run {len(a)} sha1", "b": f"rebuild {ident} of {n} identical", "diffs": diffs,
            "control": len(cmp(a, bad)) >= 1}


def pair_compiles(plant):
    r = run(["bash", "tools/compile_only.sh"], log="compile_only.log")
    m = re.search(r"^compiled: (\d+) of (\d+) units", r.stdout, re.M)
    need(m, f"compile_only.sh printed no `compiled:` line (rc {r.returncode}, .run/harness/compile_only.log)")
    units = src_units()
    need(units, "no src/**/*.c units")
    failed = set(re.findall(r"^FAILED (\S+)", r.stderr + r.stdout, re.M))
    if plant:
        failed.add(units[0])
    k, u = int(m.group(1)) - (1 if plant else 0), int(m.group(2))
    a = set(units) - failed
    b = {x for al in yaml_aliases() if (ROOT / f"build/{al}.ld").exists() for x in ld_units(al)}
    need(b, "no C-unit object in any build/<alias>.ld")

    def cmp(x, y):
        out = [f"compiled, not linked: {u}" for u in sorted(x - y)] + [f"linked, not compiled: {u}" for u in sorted(y - x)]
        if k != u or u != len(units):
            out.append(f"compile_only: {k} of {u} units, src has {len(units)}")
        return out
    return {"a": f"compiled {len(a)} of {u}", "b": f"linked {len(b)}", "diffs": cmp(a, b),
            "control": len(cmp(a, b - {sorted(b)[0]})) >= 1}


def census_print(text):
    per, spans, fleet = {}, {}, {}
    for line in text.splitlines():
        m = re.match(r"^(\S+): functions: (\d+) text bytes covered: (\d+) of (\d+)", line)
        if m:
            per[m.group(1)] = (int(m.group(3)), int(m.group(4)))
        m = re.match(r"^(\S+): data in text (0x[0-9a-f]+)\.\.(0x[0-9a-f]+) ", line)
        if m:
            spans.setdefault(m.group(1), []).append((int(m.group(2), 16), int(m.group(3), 16)))
        m = re.match(r"^text bytes covered: (\d+) of (\d+)$", line)
        if m and "X" not in fleet:  # the first block; --check's fixtures print their own after it
            fleet["X"], fleet["Y"] = int(m.group(1)), int(m.group(2))
        m = re.match(r"^data in text: (\d+) B \((\d+) spans", line)
        if m and "D" not in fleet:
            fleet["D"], fleet["n"] = int(m.group(1)), int(m.group(2))
    return per, spans, fleet


def text_region(alias, ends):
    path, subs = read_yaml(alias)
    first = next(((o, s, v) for o, t, s, v in subs if t in ("asm", "c")), None)
    if first is None or path not in ends:
        return None
    return first[2] + first[0] - first[1], ends[path]


def cover(rows, spans, lo, hi):
    """Problems of one alias: words of [lo,hi) not covered exactly once, rows/spans outside text."""
    out = []
    cnt = bytearray((hi - lo) // 4)
    for s, e, what in rows + spans:
        if s < lo or e > hi or s % 4 or e % 4:
            out.append(f"{what} {h(s)}..{h(e)} outside text [{h(lo)},{h(hi)}) or unaligned")
            continue
        for w in range((s - lo) // 4, (e - lo) // 4):
            cnt[w] = min(cnt[w] + 1, 2)
    w = 0
    while w < len(cnt):
        if cnt[w] != 1:
            v, x = cnt[w], w
            while x < len(cnt) and cnt[x] == v:
                x += 1
            out.append(f"words {h(lo + 4 * w)}..{h(lo + 4 * x)} covered {'0' if v == 0 else '>1'} times")
            w = x
        else:
            w += 1
    return out


def pair_coverage(plant):
    r = run([sys.executable, "tools/census.py", "--check"], log="census_check.txt")
    per, spans, fleet = census_print(r.stdout)
    need(per and "Y" in fleet and "D" in fleet, f"census --check printed no totals (rc {r.returncode})")
    rows = census_rows()
    if plant:
        rows = rows[1:]
    ends = text_ends()
    by = {}
    for al, s, e, _ in rows:
        by.setdefault(al, []).append((s, e, "row"))
    regions = {al: text_region(al, ends) for al in yaml_aliases()}

    def check(by):
        out = [] if r.returncode == 0 else [f"census --check rc {r.returncode}"]
        tx = ty = td = 0
        for al in sorted(regions):
            rg, rs, sp = regions[al], by.get(al, []), [(s, e, "span") for s, e in spans.get(al, [])]
            if rg is None:
                if rs or sp or per.get(al, (0, 0)) != (0, 0):
                    out.append(f"{al}: no text region but {len(rs)} rows, {len(sp)} spans")
                continue
            lo, hi = rg
            out += [f"{al}: {x}" for x in cover(rs, sp, lo, hi)]
            x, d = sum(e - s for s, e, _ in rs), sum(e - s for s, e, _ in sp)
            if (x, hi - lo - d) != per.get(al):
                out.append(f"{al}: harness X/Y {x}/{hi - lo - d} vs census {per.get(al)}")
            tx, ty, td = tx + x, ty + hi - lo - d, td + d
        if (tx, ty, td) != (fleet["X"], fleet["Y"], fleet["D"]):
            out.append(f"fleet X/Y/D {tx}/{ty}/{td} vs census {fleet['X']}/{fleet['Y']}/{fleet['D']}")
        return out, tx, ty, td
    diffs, tx, ty, td = check(by)
    al0 = sorted(by)[0]
    dup = dict(by)
    dup[al0] = by[al0] + [by[al0][0]]
    return {"a": f"census X/Y/D {fleet['X']}/{fleet['Y']}/{fleet['D']}", "b": f"words X/Y/D {tx}/{ty}/{td}",
            "diffs": diffs, "control": len(check(dup)[0]) >= 1}


class Retail:
    def __init__(self):
        self.lm, self.cache = loadmap(), {}

    def get(self, alias):
        if alias not in self.cache:
            path, _ = read_yaml(alias)
            p = RETAIL / path
            need(p.exists(), f"retail {p.relative_to(ROOT)} missing")
            d = p.read_bytes()
            cls, base = self.lm[path]
            off = (lambda v, t=struct.unpack_from("<I", d, 0x18)[0]: v - t + 0x800) if cls == "exe" else \
                (lambda v, b=base: v - b)
            self.cache[alias] = (d, off)
        return self.cache[alias]


def matched_b(rows, retail, flip=None):
    """{(alias, start)} of cc1-emitted functions whose masked object bytes equal retail at the census extent;
    flip = (alias, start): one retail byte flipped in memory first."""
    ext = {(al, s): e for al, s, e, _ in rows}
    got, notes = set(), []
    for al in yaml_aliases():
        if not (ROOT / f"build/{al}.ld").exists():
            continue
        units = ld_units(al)
        if not units:
            continue
        lsym = {n: v for n, v, _, t, _ in elf(ROOT / f"build/{al}.elf")["syms"] if t == 2}
        d, off = retail.get(al)
        for u in units:
            o = ROOT / f"build/{al}/{u}.o"
            ents = set(re.findall(r"^\s*\.ent\s+(\S+)", (ROOT / f"build/{al}/{u}.s").read_text(), re.M))
            e = elf(o)
            ti = e["names"].index(".text")
            tsec = e["secs"][ti]
            text = e["d"][tsec[4]:tsec[4] + tsec[5]]
            mask = {}
            for ro, ri in e["rels"].get(ti, []):
                mask[ro] = RELMASK.get(ri & 0xff, 0xffffffff)
            for n, v, sz, t, ndx in e["syms"]:
                if t != 2 or ndx != ti or n not in ents:
                    continue
                start = lsym.get(n)
                end = ext.get((al, start))
                if start is None or end is None:
                    notes.append(f"{al}: {n} has no linked address or census row")
                    continue
                if sz > end - start:
                    notes.append(f"{al}: {n} size {sz} > census extent {end - start}")
                    continue
                ro = off(start)
                ret = bytearray(d[ro:ro + (end - start)])
                if flip == (al, start):
                    ret[0] ^= 0xff
                ok = len(ret) == end - start and not any(ret[sz:])
                for w in range(0, sz, 4):
                    m = mask.get(v + w, 0)
                    ow = struct.unpack_from("<I", text, v + w)[0] & ~m
                    rw = struct.unpack_from("<I", ret, w)[0] & ~m
                    ok &= ow == rw
                if ok:
                    got.add((al, start))
    return got, notes


def pair_matched(plant):
    import banked  # side A's own parser
    a = set()
    for p in sorted((ROOT / "src").rglob("*.c")):
        al = p.relative_to(ROOT / "src").parts[0]
        for name, _ in banked.functions(p.read_text()):
            m = FUNC.match(name)
            a.add((al, int(m.group(1), 16)) if m else (al, name))
    need(a, "banked scan found no C body")
    rows, retail = census_rows(), Retail()
    first = sorted(x for x in a if isinstance(x[1], int))[0]
    b, notes = matched_b(rows, retail, flip=first if plant else None)

    def cmp(x, y):
        fmt = lambda t: f"{t[0]} {h(t[1]) if isinstance(t[1], int) else t[1]}"  # noqa: E731
        return [f"banked, not byte-matched: {fmt(t)}" for t in sorted(x - y, key=str)] + \
            [f"byte-matched, not banked: {fmt(t)}" for t in sorted(y - x, key=str)]
    ctl = matched_b(rows, retail, flip=sorted(b)[0])[0] if b else b
    return {"a": f"banked {len(a)}", "b": f"byte-matched {len(b)}", "diffs": cmp(a, b) + notes,
            "control": len(cmp(a, ctl)) >= 1}


def pair_oracle(plant):
    r = run([sys.executable, "tools/oracle_diff.py"] + (["--plant-start", PLANT_START] if plant else []),
            log="oracle.txt")
    m = re.search(r"^oracle: (\d+) of (\d+) binaries compared, (\d+) disagreements", r.stdout, re.M)
    need(m, f"oracle_diff.py printed no `oracle:` line (rc {r.returncode})")
    ctl = re.findall(r"^control: .*$", r.stdout, re.M)
    diffs = [] if r.returncode == 0 and m.group(3) == "0" else \
        [f"oracle_diff rc {r.returncode}, {m.group(3)} disagreements (.run/harness/oracle.txt)"]
    return {"a": f"oracle {m.group(1)} of {m.group(2)}", "b": f"{m.group(3)} disagreements", "diffs": diffs,
            "control": bool(ctl) and all(c.endswith(" ok") for c in ctl)}


def pair_denominators(plant):
    r = run([sys.executable, "tools/progress.py"], log="progress.txt")
    a = {m.group(3): (int(m.group(1)), int(m.group(2)))
         for m in re.finditer(r"^progress: \d+ of (\d+) game functions in C, \d+ of (\d+) bytes \((\S+)\)$",
                              r.stdout, re.M)}
    fl = re.search(r"^progress: \d+ of (\d+) game functions in C, \d+ of (\d+) bytes \(fleet", r.stdout, re.M)
    ok = "denominator from build: ok" in r.stdout
    rb = run([sys.executable, "tools/banked.py", "--quiet"], log="banked.txt")
    bk = re.search(r"^banked: \d+ of (\d+)", rb.stdout, re.M)
    need(a and fl and bk, f"progress/banked printed no denominators (rc {r.returncode}/{rb.returncode})")
    a["fleet"] = (int(fl.group(1)), int(fl.group(2)))
    a["banked"] = (int(bk.group(1)), None)
    mk = run(["make", "-s", "print-aliases"])
    aliases = mk.stdout.split()
    need(aliases, "make -s print-aliases empty")
    rows = [x for x in census_rows() if x[3] == "game"]
    if plant:
        rows = rows[1:]

    def count(rows):
        b = {al: (0, 0) for al in aliases}
        for al, s, e, _ in rows:
            if al in b:
                b[al] = (b[al][0] + 1, b[al][1] + e - s)
        b["fleet"] = (sum(v[0] for v in b.values()), sum(v[1] for v in b.values()))
        b["banked"] = (b["fleet"][0], None)
        return b

    def cmp(x, y):
        out = [f"{k}: progress/banked {x.get(k)} vs census rows {y.get(k)}"
               for k in sorted(set(x) | set(y)) if x.get(k) != y.get(k)]
        return out + ([] if ok else ["progress: `denominator from build: ok` missing"])
    b = count(rows)
    return {"a": f"progress G {a['fleet'][0]}, banked G {a['banked'][0]}", "b": f"census game {b['fleet'][0]}",
            "diffs": cmp(a, b), "control": len(cmp(a, count(rows[1:]))) >= 1}


# Execution order: fleet touches src (census goes stale), coverage regenerates functions.tsv before matched/oracle.
PAIRS = {"fleet": pair_fleet, "compiles": pair_compiles, "coverage": pair_coverage, "matched": pair_matched,
         "oracle": pair_oracle, "denominators": pair_denominators}
# T6.c2 hook: --no-game / --scanners (config/scanners.tsv) add or filter entries of PAIRS here.


def git_head():
    try:
        r = subprocess.run(["git", "rev-parse", "--short", "HEAD"], cwd=ROOT, capture_output=True, text=True)
        return r.stdout.strip() if r.returncode == 0 and r.stdout.strip() else "-"
    except OSError:
        return "-"


def history(mode, agree, total, dis, t0, rc):
    OUT.mkdir(parents=True, exist_ok=True)
    new = not HISTORY.exists()
    with HISTORY.open("a") as f:
        if new:
            f.write("utc\thead\tmode\tagree\ttotal\tdisagreements\telapsed_s\trc\n")
        utc = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
        f.write(f"{utc}\t{git_head()}\t{mode}\t{agree}\t{total}\t{dis}\t{time.time() - t0:.0f}\t{rc}\n")


def selftest(t0):
    k = 0
    for p in PAIRS:
        r = subprocess.run([sys.executable, str(Path(__file__).resolve()), "--plant", p], cwd=ROOT,
                           capture_output=True, text=True)
        hit = r.returncode == 1 and f"pair {p}: DISAGREE" in r.stdout
        k += hit
        print(f"selftest {p}: {'caught' if hit else 'FAIL'} (rc {r.returncode})")
    print(f"selftest: {k} of {len(PAIRS)} planted disagreements caught")
    rc = 0 if k == len(PAIRS) else 1
    history("selftest", k, len(PAIRS), len(PAIRS) - k, t0, rc)
    return rc


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--plant", choices=sorted(PAIRS))
    g.add_argument("--selftest", action="store_true")
    a = ap.parse_args()
    t0 = time.time()
    if a.selftest:
        return selftest(t0)
    names = [a.plant] if a.plant else list(PAIRS)
    mode = f"plant:{a.plant}" if a.plant else "full"
    agree = dis = 0
    ctl_ok = True
    for n in names:
        try:
            res = PAIRS[n](bool(a.plant))
        except Refused as e:
            print(f"REFUSED: pair {n}: {e}")
            history(mode, agree, len(names), dis, t0, 2)
            return 2
        ok = not res["diffs"]
        agree += ok
        dis += len(res["diffs"])
        ctl_ok &= res["control"]
        print(f"pair {n}: {'agree' if ok else 'DISAGREE'} ({res['a']} vs {res['b']}) "
              f"control: {'planted caught' if res['control'] else 'FAIL'}")
        for x in res["diffs"][:MAXD]:
            print(f"  {x}")
    print(f"harness: {agree} of {len(names)} pairs agree, {dis} disagreements")
    rc = 0 if agree == len(names) and ctl_ok else 1
    history(mode, agree, len(names), dis, t0, rc)
    return rc


if __name__ == "__main__":
    sys.exit(main())
