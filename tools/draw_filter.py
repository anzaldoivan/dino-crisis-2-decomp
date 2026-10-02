#!/usr/bin/env python3
"""draw_filter.py -- refuse draw candidates that cannot be drafted yet (T6, Phase 1.6); stdlib only, run in the container:
    dc.sh run python3 tools/draw_filter.py --all | --list FILE

Candidates: --all = every census game fn (.run/census/functions.tsv kind=game); --list FILE = lines `alias<ws>start`
(`#` comments; empty list: `REFUSED: empty candidate list` rc 1). Processed sorted by (alias, start); each gets the first
reason that holds, in this precedence: not-a-census-start, lib, banked, out-of-range, data-in-text, no-asm,
gte, handasm-marked, switch, pin-unproven, has-banked-twin, open-twin-sibling; none -> accepted.
  not-a-census-start  (alias, start) is not a start row of functions.tsv
  lib                 census kind lib
  banked              C body func_<ADDR> in a unit build/<alias>.ld links (progress.linked_units/bodies, as banked.py)
  out-of-range        [start,end) not inside the boundaries `text-end` row of the alias's binary (no row: loadmap base..end)
  data-in-text        [start,end) overlaps a boundaries `jtbl` table or `data-island`
  no-asm              start has no `/* off vram word */` line under asm/<alias>/ (dup_census.read_asm)
  gte                 (T12) any cop2 op in [start,end): mtc2/mfc2/ctc2/cfc2/lwc2/swc2 or a GTE command; evidence: the
                      first such mnemonic (before handasm-marked: marked fns with cop2 ops are gte)
  handasm-marked      (T12) spimdisasm `Handwritten function` marker on an instruction in [start,end), or the BIOS
                      A/B/C-table trampoline shape ($t2 loaded 0xA0/0xB0/0xC0, `jr $t2`, $t1 = call no.); evidence:
                      the marker name or `BIOS A|B|C-table trampoline`
  switch             a boundaries `jtbl` row whose basis `jr=<addr>` lies in [start,end)
  pin-unproven        census family not the family of any config/probes.tsv row (alias `self` ignored)
  has-banked-twin     a .run/twins/twins.tsv row for the fn with twin_banked 1
  open-twin-sibling   the fn is a direct twin (any twins.tsv row) of a candidate accepted earlier in this draw
Binary path -> alias via census.load_fleet(). Prints `draw: A of D accepted`, then `refused <reason>: n` for all twelve
reasons; writes .run/draw/accepted.tsv and .run/draw/refused.tsv (`# alias\tstart\treason\tevidence`).
Control (every run, outputs .run/draw/control/): a planted list from this run's evidence (one banked game fn, one census
lib fn, a game fn start+4, an unbanked marked/trampoline fn, an unbanked unmarked fn with a cop2 op, two direct twins
that pass every other reason) must give exactly banked 1, lib 1, not-a-census-start 1, handasm-marked 1, gte 1,
open-twin-sibling 1, accepted 1 (the first twin) -> `control: ok`; else `control: FAIL ...` rc 1.
Inputs: asm/ or build/*.ld missing -> `make -j build` (log .run/draw/build.log); twins.tsv missing/stale -> tools/twins.py
(which refreshes census + dup.tsv); either fails: REFUSED rc 2. Notes: docs/ops/decomp-environment.md "Draw filter".
"""
import argparse
import bisect
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import census  # noqa: E402  (functions.tsv, family_of, load_fleet: binary path -> alias)
import dup_census  # noqa: E402  (read_asm: the asm on disk)
import progress  # noqa: E402  (linked_units/bodies: the banked rule)
import twins  # noqa: E402  (stale(): census/dup refresh rule; OUT)

ROOT = census.ROOT
FUNCS = dup_census.FUNCS
TWINS = twins.OUT
OUT = ROOT / ".run/draw"
PRECEDENCE = ("not-a-census-start", "lib", "banked", "out-of-range", "data-in-text", "no-asm", "gte", "handasm-marked",
              "switch", "pin-unproven", "has-banked-twin", "open-twin-sibling")
PRINT_ORDER = ("banked", "not-a-census-start", "no-asm", "handasm-marked", "gte", "out-of-range", "lib", "data-in-text",
               "pin-unproven", "has-banked-twin", "open-twin-sibling", "switch")
JR = re.compile(r"\bjr=0x([0-9a-fA-F]+)")
MARKER = "Handwritten function"  # spimdisasm comment line before the fn's first instruction
COP2 = {"mtc2", "mfc2", "ctc2", "cfc2", "lwc2", "swc2", "cop2", "c2", "rtps", "rtpt", "nclip", "op", "dpcs", "intpl",
        "mvmva", "ncds", "cdp", "ncdt", "nccs", "cc", "ncs", "nct", "sqr", "dcpl", "dpct", "avsz3", "avsz4", "gpf",
        "gpl", "ncct"}
LOADI = {"addiu", "addi", "ori", "li"}
T2_TABLE = re.compile(r"^\$t2,\s*(?:\$zero,\s*)?0x([ABC])0$", re.I)
T1_SET = re.compile(r"^\$t1,\s*(?:\$zero,\s*)?(?:0x[0-9A-Fa-f]+|\d+)$")


def ensure():
    """-> None when inputs are ready, else the REFUSED line."""
    if not (ROOT / "asm").is_dir() or not any((ROOT / "build").glob("*.ld")):  # dc.sh sync drops both
        log = OUT / "build.log"
        log.parent.mkdir(parents=True, exist_ok=True)
        with log.open("w") as fh:
            rc = subprocess.run(["make", "-j", "build"], cwd=ROOT, stdout=fh, stderr=subprocess.STDOUT).returncode
        if rc or not (ROOT / "asm").is_dir() or not any((ROOT / "build").glob("*.ld")):
            return "REFUSED: asm/ or build/*.ld missing; make -j build rc %d (log %s)" % (rc, log.relative_to(ROOT))
        print("draw: built split asm (make -j build rc 0)")

    def stale():
        if twins.stale() or not TWINS.exists():
            return True
        t = TWINS.stat().st_mtime
        ins = [FUNCS, twins.DUP, ROOT / "tools/twins.py", *(ROOT / "build").glob("*.ld"), *(ROOT / "src").rglob("*.c")]
        return any(p.stat().st_mtime > t for p in ins if p.exists())

    if stale():
        p = subprocess.run([sys.executable, str(ROOT / "tools/twins.py")], cwd=ROOT, capture_output=True, text=True)
        if p.returncode or stale():
            tail = " | ".join(p.stdout.splitlines()[-3:])
            return "REFUSED: tools/twins.py rc %d; twins.tsv not refreshed (%s)" % (p.returncode, tail)
    if not any(r for r in census.rows(FUNCS) if len(r) >= 6):
        return "REFUSED: empty .run/census/functions.tsv"
    return None


class Evidence:
    """Everything a reason needs, derived from this run's inputs."""

    def __init__(self):
        self.fn = {(r[0], int(r[1], 16)): (int(r[2], 16), r[4], r[5]) for r in census.rows(FUNCS) if len(r) >= 6}
        fleet = census.load_fleet()
        self.path = {a: b.path for a, b in fleet.items()}
        lm = {r[0]: r for r in census.rows(census.LOADMAP) if len(r) > 6}
        self.text, self.jtbl, self.island = {}, {}, {}
        for r in census.rows(census.BOUNDARIES):
            if len(r) < 4:
                continue
            s, e = int(r[2], 16), int(r[3], 16)
            if r[1] == "text-end":
                self.text[r[0]] = (s, e, "boundaries text-end %s [0x%08x,0x%08x)" % (r[0], s, e))
            elif r[1] == "jtbl":
                m = JR.search(r[4] if len(r) > 4 else "")
                self.jtbl.setdefault(r[0], []).append((s, e, int(m.group(1), 16) if m else None))
            elif r[1] == "data-island":
                self.island.setdefault(r[0], []).append((s, e))
        for a, p in self.path.items():  # no text-end row: loadmap base..end, said in evidence
            if p not in self.text and p in lm and lm[p][5].startswith("0x") and lm[p][6].startswith("0x"):
                s, e = int(lm[p][5], 16), int(lm[p][6], 16)
                self.text[p] = (s, e, "no text-end row for %s; loadmap base..end [0x%08x,0x%08x)" % (p, s, e))
        self.banked = {}
        for ld in sorted((ROOT / "build").glob("*.ld")):
            for u in progress.linked_units(ld.stem):
                if u.is_file():
                    for _, ad in progress.bodies(u):
                        if ad is not None:
                            self.banked[(ld.stem, ad)] = str(u.relative_to(ROOT))
        pins = sorted({r[1] for r in census.rows(census.PROBES) if len(r) > 1 and r[1] != "self"})
        self.pinned = {census.family_of(a): a for a in pins}
        self.twins = {}
        for r in census.rows(TWINS):
            if len(r) >= 7:
                self.twins.setdefault((r[0], int(r[1], 16)), []).append((r[2], int(r[3], 16), r[6] == "1"))
        self.asm = {}
        self.ops = {}

    def has_asm(self, alias, start):
        if alias not in self.asm:
            self.asm[alias] = set(dup_census.read_asm(alias)) if (ROOT / "asm" / alias).is_dir() else set()
        return start in self.asm[alias]

    def notes(self, alias):
        """-> sorted [(vram, kind, mnemonic, operands)] of the asm lines the T12 classes read: kind `marker` (first
        instruction after a `Handwritten function` line), `cop2`, `t2`/`t1` (immediate load), `jr2` (jr $t2)."""
        if alias not in self.ops:
            out, pend = set(), False
            d = ROOT / "asm" / alias
            for f in (sorted(d.rglob("*.s")) if d.is_dir() else ()):
                for line in f.read_text(errors="replace").splitlines():
                    if MARKER in line and not dup_census.LINE.match(line):
                        pend = True
                        continue
                    m = dup_census.LINE.match(line)
                    if not m:
                        continue
                    v, mn, ops = int(m.group(1), 16), m.group(3).lower(), m.group(4).split("/*", 1)[0].strip()
                    if pend:
                        out.add((v, "marker", MARKER, ""))
                        pend = False
                    if mn in COP2:
                        out.add((v, "cop2", mn, ""))
                    elif mn in LOADI and T2_TABLE.match(ops):
                        out.add((v, "t2", mn, T2_TABLE.match(ops).group(1).upper()))
                    elif mn in LOADI and T1_SET.match(ops):
                        out.add((v, "t1", mn, ""))
                    elif mn == "jr" and ops == "$t2":
                        out.add((v, "jr2", mn, ""))
            self.ops[alias] = sorted(out)
        return self.ops[alias]

    def t12(self, alias, start, end):
        """(reason, evidence) for gte / handasm-marked over [start,end), or None. Evidence: names only (G73)."""
        ns = self.notes(alias)
        ins = ns[bisect.bisect_left(ns, (start,)):bisect.bisect_left(ns, (end,))]
        kinds = {n[1]: n for n in reversed(ins)}  # first occurrence of each kind
        if "cop2" in kinds:  # gte first: marked fns with cop2 ops are GTE work (T12 known cases)
            return "gte", kinds["cop2"][2]
        if "marker" in kinds:
            return "handasm-marked", MARKER
        if "t2" in kinds and "jr2" in kinds and "t1" in kinds:
            return "handasm-marked", "BIOS %s-table trampoline" % kinds["t2"][3]
        return None

    def base(self, alias, start):
        """(reason, evidence) of the first reason before open-twin-sibling, or None."""
        k = (alias, start)
        if k not in self.fn:
            return "not-a-census-start", "no start row %s 0x%08x in .run/census/functions.tsv" % k
        end, kind, fam = self.fn[k]
        if kind == "lib":
            return "lib", "functions.tsv kind=lib"
        if k in self.banked:
            return "banked", "C body func_%08X in %s (linked by build/%s.ld)" % (start, self.banked[k], alias)
        p = self.path.get(alias)
        t = self.text.get(p)
        if t is None:
            return "out-of-range", "no text-end row and no loadmap base..end for %s (%s)" % (p, alias)
        if not (t[0] <= start and end <= t[1]):
            return "out-of-range", "[0x%08x,0x%08x) outside %s" % (start, end, t[2])
        for s, e, _ in self.jtbl.get(p, ()):
            if s < end and start < e:
                return "data-in-text", "boundaries jtbl %s [0x%08x,0x%08x)" % (p, s, e)
        for s, e in self.island.get(p, ()):
            if s < end and start < e:
                return "data-in-text", "boundaries data-island %s [0x%08x,0x%08x)" % (p, s, e)
        if not self.has_asm(alias, start):
            return "no-asm", "no asm line at 0x%08x under asm/%s/" % (start, alias)
        r = self.t12(alias, start, end)
        if r is not None:
            return r
        for s, e, jr in self.jtbl.get(p, ()):
            if jr is not None and start <= jr < end:
                return "switch", "boundaries jtbl %s [0x%08x,0x%08x) jr=0x%08x" % (p, s, e, jr)
        if fam not in self.pinned:
            return "pin-unproven", "family %s not in config/probes.tsv families (%s)" % (fam, ",".join(sorted(self.pinned)))
        for ta, ts, tb in self.twins.get(k, ()):
            if tb:
                return "has-banked-twin", "twin %s:0x%08x banked" % (ta, ts)
        return None


def draw(ev, cands, out):
    """-> (accepted rows, refused rows), each (alias, start, reason, evidence); writes out/{accepted,refused}.tsv."""
    acc, ref = [], []
    taken = set()
    for k in sorted(set(cands)):
        r = ev.base(*k)
        if r is None:
            sib = next(((ta, ts) for ta, ts, _ in ev.twins.get(k, ()) if (ta, ts) in taken), None)
            if sib is not None:
                r = ("open-twin-sibling", "twin of accepted %s:0x%08x" % sib)
        if r is None:
            taken.add(k)
            acc.append((k[0], k[1], "accepted", "-"))
        else:
            ref.append((k[0], k[1], r[0], r[1]))
    out.mkdir(parents=True, exist_ok=True)
    for name, rows in (("accepted.tsv", acc), ("refused.tsv", ref)):
        with open(out / name, "w") as fh:
            fh.write("# alias\tstart\treason\tevidence\n")
            for a, s, rs, e in rows:
                fh.write("%s\t0x%08x\t%s\t%s\n" % (a, s, rs, e))
    return acc, ref


def control(ev):
    """Planted list from this run's evidence; -> None when ok, else what differed."""
    game = sorted(k for k, v in ev.fn.items() if v[1] == "game")
    banked = next((k for k in game if k in ev.banked), None)
    lib = next((k for k, v in sorted(ev.fn.items()) if v[1] == "lib"), None)
    off = next(((a, s + 4) for a, s in game if ev.fn[(a, s)][0] - s > 4 and (a, s + 4) not in ev.fn), None)
    t12 = {}  # T12: first unbanked game fn the class detector flags, per class
    for k in game:
        r = None if k in ev.banked else ev.t12(k[0], k[1], ev.fn[k][0])
        if r is not None and r[0] not in t12:
            t12[r[0]] = k
        if len(t12) == 2:
            break
    hand, gte = t12.get("handasm-marked"), t12.get("gte")
    pair = None
    for k in game:
        if ev.base(*k) is not None:
            continue
        pair = next(((k, (ta, ts)) for ta, ts, _ in sorted(ev.twins.get(k, ())) if (ta, ts) > k
                     and ev.base(ta, ts) is None), None)
        if pair:
            break
    missing = [n for n, v in (("banked fn", banked), ("lib fn", lib), ("start+4", off), ("handasm fn", hand),
                              ("gte fn", gte), ("twin pair", pair)) if v is None]
    if missing:
        return "no planted %s found" % ", ".join(missing)
    acc, ref = draw(ev, [banked, lib, off, hand, gte, pair[0], pair[1]], OUT / "control")
    got = {}
    for r in ref:
        got[r[2]] = got.get(r[2], 0) + 1
    got["accepted"] = len(acc)
    want = {"banked": 1, "lib": 1, "not-a-census-start": 1, "handasm-marked": 1, "gte": 1, "open-twin-sibling": 1,
            "accepted": 1}
    diff = ["%s %d (want %d)" % (n, got.get(n, 0), want.get(n, 0)) for n in sorted(set(got) | set(want))
            if got.get(n, 0) != want.get(n, 0)]
    if not diff and (acc[0][0], acc[0][1]) != pair[0]:
        diff.append("accepted %s:0x%08x, not the first twin %s:0x%08x" % (acc[0][0], acc[0][1], *pair[0]))
    return "; ".join(diff) or None


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--all", action="store_true")
    g.add_argument("--list", metavar="FILE")
    a = ap.parse_args(argv)
    if a.list:
        cands = []
        for n, line in enumerate(Path(a.list).read_text().splitlines(), 1):
            t = line.split("#", 1)[0].split()
            if not t:
                continue
            if len(t) != 2:
                print("REFUSED: %s:%d: want `alias start`" % (a.list, n))
                return 1
            cands.append((t[0], int(t[1], 16)))
        if not cands:
            print("REFUSED: empty candidate list")
            return 1
    msg = ensure()
    if msg:
        print(msg)
        return 2
    ev = Evidence()
    if a.all:
        cands = [k for k, v in ev.fn.items() if v[1] == "game"]
    acc, ref = draw(ev, cands, OUT)
    print("draw: %d of %d accepted" % (len(acc), len(set(cands))))
    for r in PRINT_ORDER:
        print("refused %s: %d" % (r, sum(1 for x in ref if x[2] == r)))
    bad = control(ev)
    print("control: ok" if bad is None else "control: FAIL %s" % bad)
    return 0 if bad is None else 1


if __name__ == "__main__":
    sys.exit(main())
