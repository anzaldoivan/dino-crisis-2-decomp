#!/usr/bin/env python3
"""tools/progress.py -- progress with denominators read from the build (T5, Phase 1.5); stdlib only, container.

Fleet = `make -s print-aliases` (the Makefile ALIASES). Runs `python3 tools/census.py` fresh, keeps its stdout, then
reads `.run/census/functions.tsv` (`alias start end size kind family`, hex, end exclusive).
G/b (per alias) = census rows of kind game / their bytes; lib is reported apart, never in G.
C/c = census game rows whose start is a C body `func_<ADDR>` (tools/banked.py functions(); INCLUDE_ASM is not a body)
in a src C unit that build/<alias>.ld links (`build/<alias>/src/**.c.o`).
Prints `progress: C of G game functions in C, c of b bytes (<alias>)` per alias, the fleet line, the lib line, then
`denominator from build: ok|FAIL <reason>`. Checks: census aliases (its stdout lines; tsv aliases inside them, a
no-text alias has none) = ALIASES = aliases with build/<alias>.ld; per alias
game+lib rows and bytes = census stdout `<alias>: functions: n text bytes covered: B`; fleet sum = census total; every
C body has a census game row in its alias. rc 0 ok, 1 FAIL, 2 REFUSED (empty ALIASES or census, G28).
--fixture alias|functions drops one alias / one census row in memory: must FAIL (negative controls, G25).
Usage: python3 tools/progress.py [--fixture alias|functions]   (normally via `make progress`)
"""
import argparse
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import banked  # noqa: E402  (functions(): the one C-body parser)

TSV = ROOT / ".run/census/functions.tsv"
FUNC = re.compile(r"^func_([0-9A-Fa-f]{8})$")
PER = re.compile(r"^(\S+): functions: (\d+) text bytes covered: (\d+) of (\d+)")
FLEET_F = re.compile(r"^functions: (\d+)$")


def make_aliases():
    r = subprocess.run(["make", "-s", "print-aliases"], cwd=ROOT, capture_output=True, text=True)
    return r.stdout.split() if r.returncode == 0 else []


def run_census():
    """Run census.py fresh; return (rc, stdout)."""
    r = subprocess.run([sys.executable, "tools/census.py"], cwd=ROOT, capture_output=True, text=True)
    return r.returncode, r.stdout


def census_stale():
    """functions.tsv missing or older than src/**/*.c, config/splat/*.yaml, build/*/split.stamp."""
    if not TSV.exists():
        return True
    inputs = [*(ROOT / "src").rglob("*.c"), *(ROOT / "config/splat").glob("*.yaml"), *(ROOT / "build").glob("*/split.stamp")]
    t = TSV.stat().st_mtime
    return any(p.stat().st_mtime > t for p in inputs)


def load_rows():
    """[(alias, start, size, kind)] from functions.tsv."""
    rows = []
    for line in TSV.read_text().splitlines():
        if not line or line.startswith("#"):
            continue
        f = line.split("\t")
        rows.append((f[0], int(f[1], 16), int(f[3], 16), f[4]))
    return rows


def game_fleet():
    """Census game-function count of the fleet, rerunning census when stale; None when it cannot be produced."""
    if census_stale():
        run_census()
    if not TSV.exists() or census_stale():
        return None
    g = sum(1 for r in load_rows() if r[3] == "game")
    return g or None


def bodies(path):
    """[(name, addr|None)] for each C body in path; addr from `func_<ADDR>`."""
    out = []
    for name, _ in banked.functions(path.read_text()):
        m = FUNC.match(name)
        out.append((name, int(m.group(1), 16) if m else None))
    return out


def linked_units(alias):
    """src C units build/<alias>.ld links, as repo paths."""
    ld = (ROOT / f"build/{alias}.ld").read_text()
    return sorted({ROOT / s for s in re.findall(rf"build/{re.escape(alias)}/(src/\S+?\.c)\.o", ld)})


def parse_census(text):
    per, fleet = {}, None
    for line in text.splitlines():
        m = PER.match(line)
        if m:
            per[m.group(1)] = (int(m.group(2)), int(m.group(3)))
        m = FLEET_F.match(line)
        if m:
            fleet = int(m.group(1))
    return per, fleet


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--fixture", choices=["alias", "functions"])
    a = ap.parse_args()

    aliases = make_aliases()
    rc, out = run_census()
    per, fleet = parse_census(out)
    rows = load_rows() if TSV.exists() else []
    if not aliases or not rows or not per or rc != 0:
        print(f"REFUSED: empty fleet or census (ALIASES {len(aliases)}, census rows {len(rows)}, "
              f"census stdout aliases {len(per)}, census rc {rc})")
        return 2
    if a.fixture == "alias":
        print(f"fixture alias: dropped {aliases[-1]} from ALIASES (in memory)")
        aliases = aliases[:-1]
    elif a.fixture == "functions":
        print(f"fixture functions: dropped census row {rows[0][0]} {rows[0][1]:#x} (in memory)")
        rows = rows[1:]

    fails = []
    tsv_aliases = sorted({r[0] for r in rows})
    ld_aliases = sorted(p.name[:-3] for p in (ROOT / "build").glob("*.ld"))
    # census aliases = its stdout lines (a no-text alias has `functions: 0` and no tsv row); tsv must lie inside them
    if not (sorted(per) == sorted(aliases) == ld_aliases) or not set(tsv_aliases) <= set(per):
        diff = (set(tsv_aliases) | set(aliases) | set(ld_aliases) | set(per)) - (
            set(aliases) & set(ld_aliases) & set(per))
        fails.append(f"alias sets differ (census stdout {len(per)}, ALIASES {len(aliases)}, "
                     f"build .ld {len(ld_aliases)}, census tsv {len(tsv_aliases)}): {' '.join(sorted(diff))}")

    by = {}
    for al, start, size, kind in rows:
        by.setdefault(al, []).append((start, size, kind))
    tot = dict(C=0, G=0, c=0, b=0, L=0, l=0, F=0, B=0)
    for al in aliases:
        rs = by.get(al, [])
        gl = [r for r in rs if r[2] in ("game", "lib")]
        n, nb = len(gl), sum(r[1] for r in gl)
        if al in per and (n, nb) != per[al]:
            fails.append(f"{al}: census rows game+lib {n} / {nb} B != census stdout functions {per[al][0]} / "
                         f"{per[al][1]} B")
        game = {r[0]: r[1] for r in rs if r[2] == "game"}
        C = cb = 0
        if (ROOT / f"build/{al}.ld").exists():
            for unit in linked_units(al):
                for name, addr in bodies(unit):
                    if addr in game:
                        C, cb = C + 1, cb + game[addr]
                    else:
                        fails.append(f"{al}: C body {name} ({unit.relative_to(ROOT)}) has no census game row")
        G, b = len(game), sum(game.values())
        lib = [r for r in rs if r[2] == "lib"]
        print(f"progress: {C} of {G} game functions in C, {cb} of {b} bytes ({al})")
        tot["C"] += C; tot["c"] += cb; tot["G"] += G; tot["b"] += b
        tot["L"] += len(lib); tot["l"] += sum(r[1] for r in lib)
        tot["F"] += len(rs); tot["B"] += sum(r[1] for r in rs)
    if fleet is None or sum(v[0] for v in per.values()) != fleet or len(rows) != fleet:
        fails.append(f"fleet: census stdout total {fleet}, sum of per-alias {sum(v[0] for v in per.values())}, "
                     f"tsv rows {len(rows)}")
    print(f"progress: {tot['C']} of {tot['G']} game functions in C, {tot['c']} of {tot['b']} bytes "
          f"(fleet, {len(aliases)} binaries)")
    print(f"lib: {tot['L']} of {tot['F']} functions, {tot['l']} of {tot['B']} bytes (not in G)")
    for f in fails[:10]:
        print(f"  {f}")
    if fails:
        print(f"denominator from build: FAIL {fails[0]} ({len(fails)} failures)")
        return 1
    print("denominator from build: ok")
    return 0


if __name__ == "__main__":
    sys.exit(main())
