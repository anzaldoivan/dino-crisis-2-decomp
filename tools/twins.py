#!/usr/bin/env python3
"""twins.py -- near band over the census game functions (T1, Phase 1.6); stdlib only, run in the container:
    dc.sh run python3 tools/twins.py [--check] [--fixture drop]

Inputs: .run/census/functions.tsv (kind=game rows) and .run/census/dup.tsv; both refreshed by running
tools/dup_census.py (which runs tools/census.py) when either is missing or older than its inputs.
Tokens per function: dup_census.exact_key (relocation-normalised words: J targets and %hi/%lo imm16 masked, trailing
zero words stripped); identical token sequences are deduplicated (distance 0). Prefilter on sequence pairs: lengths
within 25% (4*min >= 3*max) and the opcode-histogram bound (histogram over dup_census.near_word of each token;
distance >= max(|la-lb|, ceil(L1/2))) <= 0.7*max. Survivors: Levenshtein distance (bit-parallel Myers/Hyyro on
Python ints); ratio = 1 - dist/max(len); kept iff ratio >= 0.3.
Writes .run/twins/twins.tsv (`alias start twin_alias twin_start distance ratio twin_banked`, one row per fn per twin,
both directions; sorted distance asc, twin_banked desc, alias, start, twin_alias, twin_start). twin_banked = 1 iff
the twin is a C body func_<ADDR> in a unit build/<alias>.ld links (banked/progress helpers).
Prints `twins: T fns with >= 1 twin of G game fns`, `exact pairs reproduced: X of N` (each game pair of each dup.tsv
exact class found at distance 0 in both directions), `random-pair control: h of 10000 (<= 2%): ok|FAIL` (seed 1601,
game pairs with different exact keys, h = pairs the band keeps), `open with banked twin: k` + one line per open fn.
--check: rc 1 when X < N or the control FAILs. --fixture drop: the first exact pair's rows are dropped from the band
output (negative control: --check must rc 1). asm/ or build/*.ld missing (after dc.sh sync, before make build):
REFUSED rc 2. Notes: docs/ops/decomp-environment.md "Twin band".
"""
import argparse
import multiprocessing
import random
import subprocess
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import census  # noqa: E402
import dup_census  # noqa: E402  (exact_key/near_word/strip/load: the one masker, G33)
import progress  # noqa: E402  (census_stale/bodies/linked_units: banked = linked C body)

ROOT = census.ROOT
FUNCS, DUP = dup_census.FUNCS, dup_census.OUT
OUT = ROOT / ".run/twins/twins.tsv"
SEED, PAIRS = 1601, 10000
U = []  # unique token sequences (worker globals, inherited by fork)


def stale():
    if progress.census_stale() or not DUP.exists():
        return True
    t = DUP.stat().st_mtime
    ins = [FUNCS, ROOT / "tools/dup_census.py", *(ROOT / "asm").rglob("*.s")]
    return any(p.stat().st_mtime > t for p in ins if p.exists())


def lev(peq, m, b):
    """Levenshtein distance of a (len m, as Peq bitmasks) and b; Myers/Hyyro bit-parallel."""
    mask, hb = (1 << m) - 1, 1 << (m - 1)
    pv, mv, score = mask, 0, m
    for c in b:
        eq = peq.get(c, 0)
        xv = eq | mv
        xh = (((eq & pv) + pv) ^ pv) | eq
        ph = mv | (~(xh | pv) & mask)
        mh = pv & xh
        if ph & hb:
            score += 1
        elif mh & hb:
            score -= 1
        ph = ((ph << 1) | 1) & mask
        mh = (mh << 1) & mask
        pv = mh | (~(xv | ph) & mask)
        mv = ph & xv
    return score


def row(i):
    """Kept (i, j, dist) for j > i (U sorted by length asc)."""
    out = []
    a, ha, _ = U[i]
    la = len(a)
    for j in range(i + 1, len(U)):
        b, hb_, peq = U[j]
        lb = len(b)
        if 4 * la < 3 * lb:
            break
        l1 = sum(abs(v - hb_.get(k, 0)) for k, v in ha.items()) + sum(v for k, v in hb_.items() if k not in ha)
        if 10 * max(lb - la, (l1 + 1) // 2) > 7 * lb:
            continue
        d = lev(peq, lb, a)
        if 10 * d <= 7 * lb:
            out.append((i, j, d))
    return out


def banked_set():
    out = set()
    for ld in sorted((ROOT / "build").glob("*.ld")):
        alias = ld.stem
        for p in progress.linked_units(alias):
            if p.is_file():
                out |= {(alias, a) for _, a in progress.bodies(p) if a is not None}
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--fixture", choices=("drop",))
    a = ap.parse_args(argv)
    if not (ROOT / "asm").is_dir() or not any((ROOT / "build").glob("*.ld")):  # dc.sh sync drops both
        print("REFUSED: asm/ or build/*.ld missing (run `make -j build` first)")
        return 2
    if stale():
        p = subprocess.run([sys.executable, str(ROOT / "tools/dup_census.py")], cwd=ROOT, capture_output=True, text=True)
        if p.returncode or stale():
            print("twins: dup_census rc %d" % p.returncode)
            print("\n".join(p.stdout.splitlines()[-5:]))
            return 1
    kind = {(r[0], int(r[1], 16)): r[4] for r in census.rows(FUNCS) if len(r) >= 6}
    funcs, _ = dup_census.load(None)
    if not funcs:
        print("REFUSED: empty functions.tsv")
        return 2
    game = sorted((f for f in funcs if kind.get((f["alias"], f["start"])) == "game"),
                  key=lambda f: (f["alias"], f["start"]))
    G = len(game)
    groups = {}
    for f in game:
        groups.setdefault(f["ex"], []).append(f)
    keys = sorted(groups, key=lambda k: (len(k), groups[k][0]["alias"], groups[k][0]["start"]))
    U.clear()
    for k in keys:
        peq = {}
        for i, t in enumerate(k):
            peq[t] = peq.get(t, 0) | (1 << i)
        U.append((k, Counter(dup_census.near_word(t) for t in k), peq))
    idx = [i for i, k in enumerate(keys) if k]
    with multiprocessing.get_context("fork").Pool() as pool:
        kept = [t for r in pool.imap_unordered(row, idx, chunksize=8) for t in r]
    bset = banked_set()
    rows = []  # (dist, -twin_banked, alias, start, twin_alias, twin_start, ratio)

    def emit(x, y, d, m):
        r = 1.0 - d / m if m else 1.0
        rows.append((d, -int((y["alias"], y["start"]) in bset), x["alias"], x["start"], y["alias"], y["start"], r))

    for k in keys:
        g = groups[k]
        for x in g:
            for y in g:
                if x is not y:
                    emit(x, y, 0, len(k))
    for i, j, d in kept:
        m = len(keys[j])
        for x in groups[keys[i]]:
            for y in groups[keys[j]]:
                emit(x, y, d, m)
                emit(y, x, d, m)
    rows.sort()
    # dup.tsv exact classes (game members) -> pairs to reproduce
    cls = {}
    for r in census.rows(DUP):
        if len(r) >= 5 and r[3] == "exact" and kind.get((r[0], int(r[1], 16))) == "game":
            cls.setdefault(r[4], []).append((r[0], int(r[1], 16)))
    expect = sorted((m[p], m[q]) for m in cls.values() for p in range(len(m)) for q in range(p + 1, len(m)))
    if a.fixture == "drop" and expect:
        x, y = expect[0]
        rows = [r for r in rows if {(r[2], r[3]), (r[4], r[5])} != {x, y}]
    OUT.parent.mkdir(parents=True, exist_ok=True)
    with open(OUT, "w") as fh:
        fh.write("# alias\tstart\ttwin_alias\ttwin_start\tdistance\tratio\ttwin_banked -- tools/twins.py\n")
        for d, nb, al, st, ta, ts, r in rows:
            fh.write("%s\t0x%08x\t%s\t0x%08x\t%d\t%.3f\t%d\n" % (al, st, ta, ts, d, r, -nb))
    zero = {((r[2], r[3]), (r[4], r[5])) for r in rows if r[0] == 0}
    X, N = sum(1 for p, q in expect if (p, q) in zero and (q, p) in zero), len(expect)
    T = len({(r[2], r[3]) for r in rows})
    print("twins: %d fns with >= 1 twin of %d game fns" % (T, G))
    print("exact pairs reproduced: %d of %d" % (X, N))
    pairs = {((r[2], r[3]), (r[4], r[5])) for r in rows}
    rng = random.Random(SEED)
    h = got = 0
    while got < PAIRS:
        x, y = game[rng.randrange(G)], game[rng.randrange(G)]
        if x["ex"] == y["ex"]:
            continue
        got += 1
        h += ((x["alias"], x["start"]), (y["alias"], y["start"])) in pairs
    c_ok = 100 * h <= 2 * PAIRS
    print("random-pair control: %d of %d (<= 2%%): %s" % (h, PAIRS, "ok" if c_ok else "FAIL"))
    best = {}
    for d, nb, al, st, ta, ts, r in rows:  # sorted: first banked twin per open fn is its best
        if nb and (al, st) not in bset and (al, st) not in best:
            best[(al, st)] = (ta, ts, d, r)
    print("open with banked twin: %d" % len(best))
    for (al, st), (ta, ts, d, r) in sorted(best.items()):
        print("  %s 0x%08x → %s 0x%08x distance %d ratio %.3f" % (al, st, ta, ts, d, r))
    if a.check:
        return 0 if X == N and c_ok else 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
