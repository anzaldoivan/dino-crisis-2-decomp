"""xprog_targets.py -- s4 seed list for DumpFunctions.java (Phase 1.5 T2.c4); host, stdlib, called by dump_functions.sh:
    python3 tools/ghidra/xprog_targets.py <fleet.tsv> <out.tsv> [<bodies dir>]

fleet.tsv: `prog<TAB>path` (dump_functions.sh FLEET). Window of a program = loadmap [base, end) (rows without a base are
skipped). From each program's bytes only (extracted/retail/files/<path>; a `PS-X EXE` header's first 0x800 bytes are
skipped) take every aligned word: opcode 000011 -> jal target 0x80000000 | (w & 0x3FFFFFF) << 2; else a 4-aligned
value in [0x80000000, 0xA0000000) -> pointer. A target is written for program Q iff it lies in Q's window and the
source program's window does not overlap Q's. Output `prog<TAB>0x%08x` sorted, unique (addresses only, G12).
T2.c7: with <bodies dir> (<prog>.bodies.tsv, DumpFunctions.java bodies mode: Ghidra function body ranges after seeding),
a jal word counts only when its own address (window base + offset) lies inside a body of its program (a data word
decoding as jal is not a call); a program without a bodies file contributes no jal targets.
Never reads census or splat output.
"""
import bisect
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent


def bodies(d, prog):
    f = Path(d) / (prog + ".bodies.tsv")
    if not f.exists():
        return []
    return sorted(tuple(int(x, 16) for x in l.split("\t")[:2]) for l in f.read_text().splitlines()
                  if l and not l.startswith("#"))


def main(fleet_tsv, out_tsv, bodies_dir=None):
    lm = {}
    for l in (ROOT / "config/loadmap.tsv").read_text().splitlines():
        r = l.split("\t")
        if l and not l.startswith("#") and len(r) > 6 and r[5] != "-":
            lm[r[0]] = (int(r[5], 16), int(r[6], 16))
    progs = []
    for l in Path(fleet_tsv).read_text().splitlines():
        prog, path = l.split("\t")
        if path in lm:
            progs.append((prog, path) + lm[path])
    tg = {}
    for prog, path, base, _ in progs:
        data = (ROOT / "extracted/retail/files" / path).read_bytes()
        if data[:8] == b"PS-X EXE":
            data = data[0x800:]
        s = set()
        bd = bodies(bodies_dir, prog) if bodies_dir else None
        for off in range(0, len(data) - 3, 4):
            w = int.from_bytes(data[off:off + 4], "little")
            if w >> 26 == 3:
                i = bisect.bisect_right(bd, (base + off, 0xFFFFFFFF)) - 1 if bd is not None else 0
                if bd is None or (i >= 0 and bd[i][0] <= base + off < bd[i][1]):
                    s.add(0x80000000 | (w & 0x3FFFFFF) << 2)
            elif w % 4 == 0 and 0x80000000 <= w < 0xA0000000:
                s.add(w)
        tg[prog] = s
    out = set()
    for q, _, qb, qe in progs:
        for p, _, pb, pe in progs:
            if pb < qe and qb < pe:  # overlapping windows (includes q itself)
                continue
            out.update((q, t) for t in tg[p] if qb <= t < qe)
    Path(out_tsv).write_text("".join("%s\t0x%08x\n" % x for x in sorted(out)))
    print("xprog_targets: %d programs, %d targets" % (len(progs), len(out)))


if __name__ == "__main__":
    main(*sys.argv[1:4])
