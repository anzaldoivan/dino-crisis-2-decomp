#!/usr/bin/env python3
"""dup_census.py -- duplication census of the fleet (T3, Phase 1.5); stdlib only, run in the container:
    dc.sh run python3 tools/dup_census.py [--check] [--fixture nomask|nearall]

Runs tools/census.py first (regenerates .run/census/functions.tsv and the split asm), reads each census function's
words from `asm/<alias>/**/*.s` lines `/* off vram word */` (C-defined functions: from the extracted binary, hi/lo
masks by data-flow pairing), classes them by an exact key (J targets and %hi/%lo imm16 masked) and a near key
(op/rs/rt/rd/funct kept, imm16/shamt/J target masked; COP0/COP2 non-memory words whole), trailing zero words stripped.
Writes .run/census/dup.tsv (`alias start size tier class_id copies reach`) and prints tier, family, reach and
unique-tail counts with denominators. Controls: relocation (known-true) and random-pair near false positives.
Definitions: phase 1.5 PHASE_PLAN `## Interfaces`; notes: docs/ops/decomp-environment.md "Duplication census".
Exit: 0 clean; 1 census failure or (--check) a failed control (every fixture run); 2 refused (empty functions.tsv).
"""
import argparse
import random
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import census  # noqa: E402  (read-only reuse: load_fleet, parse_yaml, FAMILIES)

ROOT = census.ROOT
FUNCS = ROOT / ".run/census/functions.tsv"
OUT = ROOT / ".run/census/dup.tsv"
LINE = re.compile(r"^\s*/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*(\S*)\s*(.*)$")
DELTA = 0x12340
SEED, PAIRS = 1505, 10000
RAM = (0x80000000, 0x80200000)  # data-flow mask range for C-defined functions (KSEG0 main RAM)
LO_OPS = {0x08, 0x09, 0x0D} | set(range(0x20, 0x27)) | {0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x32, 0x3A}
NOWRITE_FUNCTS = {0x08, 0x0C, 0x0D, 0x11, 0x13, 0x18, 0x19, 0x1A, 0x1B}  # jr syscall break mthi mtlo mult div
HI_ANN = re.compile(r"%hi\(|\(0x[0-9A-Fa-f]+ >> 16\)")  # asm-annotated hi: `%hi(sym)` or splat raw `(0x… >> 16)`
LO_ANN = re.compile(r"%lo\(|\(0x[0-9A-Fa-f]+ & 0xFFFF\)")  # asm-annotated lo: `%lo(sym)` or raw `(0x… & 0xFFFF)`


def writes(w):
    """Destination GPR of word w, or None."""
    op = w >> 26
    if op == 0:
        return None if (w & 63) in NOWRITE_FUNCTS else (w >> 11) & 31
    if op == 3 or (op == 1 and ((w >> 16) & 31) in (0x10, 0x11)):
        return 31
    if 0x08 <= op <= 0x0F or 0x20 <= op <= 0x26:
        return (w >> 16) & 31
    if op in (0x10, 0x12) and ((w >> 21) & 31) in (0, 2):
        return (w >> 16) & 31
    return None


def addr_of(words, lui, lo):
    h, l = (words[lui] & 0xFFFF) << 16, words[lo] & 0xFFFF
    if words[lo] >> 26 == 0x0D:
        return h | l
    return (h + (l - 0x10000 if l & 0x8000 else l)) & 0xFFFFFFFF


def pairs(words):
    """Data-flow lui/lo pairing: lui rX, then an I-type with rs = rX until rX is redefined; `addu` with a
    lui-derived register propagates it. -> [(lui index, lo index, address)]."""
    hi, out = {}, []
    for i, w in enumerate(words):
        op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        if op == 0x0F:
            if rt:
                hi[rt] = i
            continue
        if op == 0 and (w & 63) == 0x21:
            src = rs if rs in hi else rt if rt in hi else None
            rd = (w >> 11) & 31
            if src is not None and rd:
                hi[rd] = hi[src]
                continue
        if op in LO_OPS and rs in hi:
            out.append((hi[rs], i, addr_of(words, hi[rs], i)))
        d = writes(w)
        if d is not None:
            hi.pop(d, None)
    return out


def hi_reuse(words, his):
    """Exact-key mask rule kind B (asm-annotation driven, independent of pairs()/the relocator): an I-type in LO_OPS
    whose rs was last written, in program order within the function, by an asm-annotated hi line (his[i]), directly
    or through `addu` propagation as in pairs(), has its imm16 masked even when its own asm operand is raw
    (splat symbolizes only the first lo of a %hi register). -> set of word indices."""
    hi, out = set(), set()
    for i, w in enumerate(words):
        op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        if op == 0x0F:
            if rt:
                (hi.add if his[i] else hi.discard)(rt)
            continue
        if op == 0 and (w & 63) == 0x21:
            rd = (w >> 11) & 31
            if (rs in hi or rt in hi) and rd:
                hi.add(rd)
                continue
        if op in LO_OPS and rs in hi:
            out.add(i)
        d = writes(w)
        if d is not None:
            hi.discard(d)
    return out


def relocate(words, base, end, delta):
    """Independent word-level relocator -> (new words, fields moved)."""
    new = list(words)
    for i, w in enumerate(words):
        if w >> 26 in (2, 3):
            a = 0x80000000 | ((w & 0x3FFFFFF) << 2)
            if base <= a < end:
                new[i] = (w & 0xFC000000) | (((a + delta) >> 2) & 0x3FFFFFF)
    newhi = {}
    for lui, lo, a in pairs(words):
        if not base <= a < end:
            continue
        a2 = (a + delta) & 0xFFFFFFFF
        if lui not in newhi:
            newhi[lui] = (a2 >> 16) if words[lo] >> 26 == 0x0D else ((a2 + 0x8000) >> 16) & 0xFFFF
            new[lui] = (words[lui] & 0xFFFF0000) | newhi[lui]
        new[lo] = (words[lo] & 0xFFFF0000) | ((a2 - (newhi[lui] << 16)) & 0xFFFF)
    return new, sum(1 for a, b in zip(words, new) if a != b)


def strip(ws):
    n = len(ws)
    while n and ws[n - 1] == 0:
        n -= 1
    return n


def exact_key(words, masks, fixture):
    n = strip(words)
    return tuple(w & 0xFC000000 if w >> 26 in (2, 3) else
                 w & 0xFFFF0000 if masks[i] and fixture != "nomask" else w for i, w in enumerate(words[:n]))


def near_word(w):
    op = w >> 26
    if op == 0:
        return w & ~(31 << 6)
    if op in (2, 3):
        return w & 0xFC000000
    if op in (0x10, 0x12):
        return w
    return w & 0xFFFF0000


def near_key(words, fixture):
    if fixture == "nearall":
        return ()
    return tuple(near_word(w) for w in words[:strip(words)])


def read_asm(alias):
    """-> {vram: (word, annotated hi/lo, annotated hi)} from every `/* off vram word */` line; annotated = `%hi(`/
    `%lo(` or the raw-pair forms `(0x… >> 16)` / `(0x… & 0xFFFF)` (exact-key mask kind A)."""
    m_ = {}
    d = ROOT / "asm" / alias
    for f in sorted(d.rglob("*.s")):
        for line in f.read_text(errors="replace").splitlines():
            m = LINE.match(line)
            if not m:
                continue
            v = int(m.group(1), 16)
            w = int.from_bytes(bytes.fromhex(m.group(2)), "little")
            hi = bool(HI_ANN.search(m.group(4)))
            hl = hi or bool(LO_ANN.search(m.group(4)))
            if v in m_ and m_[v][0] != w:
                sys.exit("dup_census: %s 0x%x: conflicting words in asm" % (alias, v))
            o = m_.get(v, (0, False, False))
            m_[v] = (w, o[1] or hl, o[2] or hi)
    return m_


def binary_words(alias, path, start, end):
    _, segs = census.parse_yaml(census.SPLAT / (alias + ".yaml"))
    f = next((p for p in (ROOT / ".run/extracted/retail/files" / path, ROOT / "extracted/retail/files" / path)
              if p.is_file()), None)
    for sg in sorted((g for g in segs if g["type"] == "code" and g["vram"] <= start), key=lambda g: -g["vram"])[:1]:
        if f is not None:
            off = sg["start"] + start - sg["vram"]
            data = f.read_bytes()[off:off + end - start]
            if len(data) == end - start:
                return [int.from_bytes(data[i:i + 4], "little") for i in range(0, len(data), 4)]
    sys.exit("dup_census: %s 0x%x: no binary words" % (alias, start))


def load(fixture):
    fleet = census.load_fleet()
    lm = {r[0]: r for r in census.rows(census.LOADMAP) if len(r) > 6}
    rows = [r for r in census.rows(FUNCS) if len(r) >= 6]
    if not rows:
        return None, 0
    funcs, asm, ndf = [], {}, 0
    for r in rows:
        alias, s, e, size, kind, fam = r[0], int(r[1], 16), int(r[2], 16), int(r[3], 16), r[4], r[5]
        if alias not in asm:
            asm[alias] = read_asm(alias)
        a = asm[alias]
        b = fleet[alias]
        base, end = int(lm[b.path][5], 16), int(lm[b.path][6], 16)
        vs = range(s, e, 4)
        if all(v in a for v in vs):
            words = [a[v][0] for v in vs]
            masks = [a[v][1] for v in vs]
            for i in hi_reuse(words, [a[v][2] for v in vs]):  # kind B
                masks[i] = True
        elif not any(v in a for v in vs):
            words = binary_words(alias, b.path, s, e)
            masks = [False] * len(words)
            for lui, lo, ad in pairs(words):
                if RAM[0] <= ad < RAM[1]:
                    masks[lui] = masks[lo] = True
            ndf += 1
        else:
            sys.exit("dup_census: %s 0x%x: partial asm coverage" % (alias, s))
        funcs.append({"alias": alias, "start": s, "size": size, "fam": fam, "words": words, "masks": masks,
                      "base": base, "end": end, "ex": exact_key(words, masks, fixture),
                      "nr": near_key(words, fixture)})
    return funcs, ndf


def classify(funcs):
    ex, nr = {}, {}
    for f in funcs:  # funcs sorted by (alias, start): first member = first appended
        ex.setdefault(f["ex"], []).append(f)
        nr.setdefault(f["nr"], []).append(f)
    eid = {k: "e%d" % i for i, k in enumerate(sorted((k for k, v in ex.items() if len(v) >= 2),
                                                     key=lambda k: (ex[k][0]["alias"], ex[k][0]["start"])), 1)}
    for f in funcs:
        if f["ex"] in eid:
            f["tier"], f["cls"], f["copies"] = "exact", eid[f["ex"]], len(ex[f["ex"]])
        elif len(nr[f["nr"]]) >= 2:
            f["tier"], f["copies"] = "near", len(nr[f["nr"]])
        else:
            f["tier"], f["cls"], f["copies"] = "unique", "-", 1
    nk = sorted({f["nr"] for f in funcs if f["tier"] == "near"}, key=lambda k: (nr[k][0]["alias"], nr[k][0]["start"]))
    nid = {k: "n%d" % i for i, k in enumerate(nk, 1)}
    for f in funcs:
        if f["tier"] == "near":
            f["cls"] = nid[f["nr"]]
    classes = [("exact", eid[k], ex[k]) for k in eid] + [("near", nid[k], nr[k]) for k in nid]
    return classes


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--fixture", choices=("nomask", "nearall"))
    a = ap.parse_args(argv)
    p = subprocess.run([sys.executable, str(ROOT / "tools/census.py")], cwd=ROOT, capture_output=True, text=True)
    if p.returncode:
        print("dup_census: tools/census.py rc %d" % p.returncode)
        print("\n".join(p.stdout.splitlines()[-5:]))
        return 1
    funcs, ndf = load(a.fixture)
    if not funcs:
        print("REFUSED: empty functions.tsv")
        return 2
    classes = classify(funcs)
    F, B = len(funcs), sum(f["size"] for f in funcs)
    print("mask source data-flow: %d functions" % ndf)
    for tier in ("exact", "near"):
        mem = [f for f in funcs if f["tier"] == tier]
        print("%s tier: %d classes, %d of %d functions, %d of %d bytes"
              % (tier, len({f["cls"] for f in mem}), len(mem), F, sum(f["size"] for f in mem), B))
    for fam in census.FAMILIES:
        mem = [f for f in funcs if f["fam"] == fam]
        c = {t: sum(1 for f in mem if f["tier"] == t) for t in ("exact", "near", "unique")}
        print("family %s: functions %d, exact %d, near %d, unique %d, bytes %d"
              % (fam, len(mem), c["exact"], c["near"], c["unique"], sum(f["size"] for f in mem)))
    top = sorted(classes, key=lambda c: (-c[2][0]["size"] * len(c[2]), c[2][0]["alias"], c[2][0]["start"]))[:20]
    print("top reach: class tier code_bytes copies reach families first")
    for tier, cid, mem in top:
        print("  %s %s %d %d %d %s %s:0x%x" % (cid, tier, mem[0]["size"], len(mem), mem[0]["size"] * len(mem),
                                             ",".join(sorted({f["fam"] for f in mem})), mem[0]["alias"],
                                             mem[0]["start"]))
    un = [f for f in funcs if f["tier"] == "unique"]
    print("unique tail: %d of %d functions, %d of %d bytes" % (len(un), F, sum(f["size"] for f in un), B))
    if not a.fixture:
        with open(OUT, "w") as fh:
            fh.write("# alias\tstart\tsize\ttier\tclass_id\tcopies\treach -- tools/dup_census.py\n")
            for f in funcs:
                fh.write("%s\t0x%08x\t0x%x\t%s\t%s\t%d\t0x%x\n" % (f["alias"], f["start"], f["size"], f["tier"],
                                                                 f["cls"], f["copies"], f["size"] * f["copies"]))
    # Control 1 (known-true): relocated copy keeps its exact key.
    n = k = m = 0
    fails = []
    for f in funcs:
        new, moved = relocate(f["words"], f["base"], f["end"], DELTA)
        if not moved:
            continue
        n, m = n + 1, m + moved
        if exact_key(new, f["masks"], a.fixture) == f["ex"]:
            k += 1
        else:
            fails += ["%s:0x%x:+0x%x" % (f["alias"], f["start"], 4 * i)
                      for i, (x, y) in enumerate(zip(f["words"], new)) if x != y and
                      exact_key([x], [f["masks"][i]], a.fixture) != exact_key([y], [f["masks"][i]], a.fixture)][:1]
    c1 = k == n >= 1 and m >= 1
    print("relocation control: delta 0x%x, %d of %d functions stay exact (%d fields moved): %s"
          % (DELTA, k, n, m, "ok" if c1 else "FAIL"))
    if fails:
        print("  failing: %d functions, first: %s" % (len(fails), " ".join(fails[:10])))
    # Control 2: near-key false positives among distinct exact classes.
    rng = random.Random(SEED)
    h = got = 0
    while got < PAIRS:
        x, y = rng.randrange(F), rng.randrange(F)
        if x == y or funcs[x]["ex"] == funcs[y]["ex"]:
            continue
        got += 1
        h += funcs[x]["nr"] == funcs[y]["nr"]
    pct = 100.0 * h / PAIRS
    c2 = pct <= 1.0
    print("near control: seed %d, %d of %d pairs (%.2f%%): %s" % (SEED, h, PAIRS, pct, "ok" if c2 else "FAIL"))
    ok = c1 and c2
    print("control: %s" % ("ok" if ok else "FAIL"))  # both controls; the milestone's line
    if a.check:
        print("check: %s" % ("OK" if ok else "FAIL"))
        return 0 if ok else 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
