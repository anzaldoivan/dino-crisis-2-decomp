#!/usr/bin/env python3
"""loadmap_evidence.py -- static load-base evidence for every DC2 payload (T4; adapted from kit P2
payload_base_evidence.py, BFM slots/controls/idxtab/sig files stripped).

A payload's own bytes constrain where it loads:
  * `jal` self-calls must land ON its own function starts (prologues `addiu sp,sp,-N`, word after `jr ra`+slot);
  * absolute word pointers and lui/lo16 pairs must land inside [base, base+size);
  * jals into the exe .text [0x80018000,0x80085f74) are resident calls, never self-evidence (non-exe payloads).
Candidates are BOUNDED, never a scan: the three R2 module bases, every type-0/7 dest `a` in every PSX/DATA/*.DAT
header, the top-5 bases of the jal->start vote, and the header t_addr for the exe. A payload with zero
self-reference at every candidate is `ranked: none` with a reason, never a guess.

Output: addresses, counts and sha1s only (G12): no game bytes, strings or disassembly.

  tools/loadmap_evidence.py [payload...] [--controls] [--json OUT]
  (no payload args = every manifest .BIN + SLUS_012.79; --controls: the exe body must rank t_addr first, else exit 2)
"""
import argparse, collections, hashlib, json, os, struct, sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FILES = os.path.join(REPO, "extracted/retail/files")
MANIFEST = os.path.join(REPO, "extracted/retail/manifest.jsonl")
EXE = "SLUS_012.79"
EXE_TEXT = (0x80018000, 0x80085F74)            # binding, T3 (docs/memory-map.md#slus_01279)
FIXED = {0x800D5800: "R1/R2 base", 0x801C1500: "R1/R2 base", 0x80100000: "R2 base"}
RAM = (0x80010000, 0x80200000)
DAT_TERM = 0x6D6D7564
JR_RA = 0x03E00008

SPECIAL_OK = {0x00, 0x02, 0x03, 0x04, 0x06, 0x07, 0x08, 0x09, 0x0C, 0x0D, 0x10, 0x11, 0x12, 0x13,
              0x18, 0x19, 0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2A, 0x2B}
OP_OK = set(range(0x02, 0x10)) | {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x32, 0x3A}
MEMOPS = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B, 0x2E}


def valid_op(x):
    """True if the word decodes as an R3000 (PS1) instruction."""
    op = x >> 26
    if op == 0:
        return (x & 0x3F) in SPECIAL_OK
    if op == 1:
        return ((x >> 16) & 0x1F) in (0x00, 0x01, 0x10, 0x11)
    if op == 0x10:                                   # COP0: mfc0/mtc0/rfe
        rs = (x >> 21) & 0x1F
        return rs in (0, 4) or x == 0x42000010
    if op == 0x12:                                   # COP2 (GTE): moves or command
        rs = (x >> 21) & 0x1F
        return rs in (0, 2, 4, 6) or rs >= 0x10
    return op in OP_OK


def words(b):
    return list(struct.unpack_from("<%dI" % (len(b) // 4), b, 0))


def dat_dests():
    """{dest: [dat basename, ...]} and {dat stem: [(type, dest, size)]} over every PSX/DATA/*.DAT archive header."""
    by_dest, by_stem = collections.defaultdict(list), {}
    for ln in open(MANIFEST):
        p = json.loads(ln)["path"]
        if not (p.startswith("PSX/DATA/") and p.endswith(".DAT")):
            continue
        with open(os.path.join(FILES, p), "rb") as f:
            b = f.read(0x800)
        recs, ok = [], False
        for o in range(0, len(b) - 0x1F, 0x20):
            t, s, a, _ = struct.unpack_from("<4I", b, o)
            if t == DAT_TERM:
                ok = True
                break
            recs.append((t, a, s))
        if not ok:
            continue
        stem = os.path.basename(p)[:-4]
        by_stem[stem] = [(t, a, s) for t, a, s in recs if t in (0, 7)]
        for t, a, s in by_stem[stem]:
            by_dest[a].append(stem)
    return by_dest, by_stem


def analyse(rel, b, is_exe, dests, by_stem):
    n = len(b)
    w = words(b)
    pro = [i * 4 for i, x in enumerate(w) if (x >> 16) == 0x27BD and (x & 0x8000)]
    jr = [i * 4 for i, x in enumerate(w) if x == JR_RA]
    # density over the code span [first prologue, last `jr ra`+slot) (whole file if none): a trailing data
    # section must not hide a module (SUBSCR3: 0.68 whole-file, 1.00 over its span)
    span = w[(pro[0] if pro else 0) // 4:(jr[-1] + 8) // 4 if jr else len(w)]
    nz = [x for x in span if x]
    density = (sum(1 for x in nz if valid_op(x)) / len(nz)) if nz else 0.0
    if n <= 8:
        hint = "stub"
    elif jr and density >= 0.80:
        hint = "code"
    else:
        hint = "data"
    rec = {"path": rel, "size": n, "sha1": hashlib.sha1(b).hexdigest(), "density": round(density, 3),
           "jr_ra": len(jr), "prologues": len(pro), "class_hint": hint}
    stem = os.path.basename(rel)[:-4]
    if stem in by_stem:
        rec["dat_segments"] = [{"type": t, "dest": "0x%08x" % a, "size": s} for t, a, s in by_stem[stem]]
    if hint == "stub":
        rec["ranked"] = "none"
        rec["reason"] = "stub (<= 8 B)"
        return rec
    text_lo = pro[0] if pro else 0
    code_end = min(n, jr[-1] + 8) if jr else n
    ptrs = [x for x in w if RAM[0] <= x < RAM[1]]
    jals, pairs = [], []
    for i in range(text_lo // 4, code_end // 4):
        x = w[i]
        op = x >> 26
        if op == 0x03:
            jals.append(0x80000000 | ((x & 0x3FFFFFF) << 2))
        elif op == 0x0F:                             # lui rt,hi -> first use of rt as base within 8 words
            rt, hi = (x >> 16) & 0x1F, (x & 0xFFFF) << 16
            for y in w[i + 1:min(i + 9, code_end // 4)]:
                yop, yrs = y >> 26, (y >> 21) & 0x1F
                if yrs == rt and (yop == 0x09 or yop in MEMOPS):
                    lo = y & 0xFFFF
                    pairs.append((hi + (lo - 0x10000 if lo & 0x8000 else lo)) & 0xFFFFFFFF)
                    break
                if yop == 0x0D and yrs == rt:
                    pairs.append(hi | (y & 0xFFFF))
                    break
                if yop == 0x0F and ((y >> 16) & 0x1F) == rt:
                    break
    resident = [t for t in jals if EXE_TEXT[0] <= t < EXE_TEXT[1]] if not is_exe else []
    own = [t for t in jals if not (EXE_TEXT[0] <= t < EXE_TEXT[1])] if not is_exe else jals
    # function starts: prologues + first non-zero word after any `jr reg`/`j`/`b` + delay slot (skips 0 padding;
    # T4: the kit's `jr ra`+8 alone missed 144 distinct exe targets, mostly after alignment padding)
    starts = set(pro) | {text_lo}
    for i, x in enumerate(w):
        if (x >> 26 == 0 and (x & 0x3F) == 8) or x >> 26 == 2 or (x >> 16) == 0x1000:
            k = i + 2
            while k < len(w) and w[k] == 0:
                k += 1
            if text_lo <= k * 4 < code_end:
                starts.add(k * 4)
    vote = collections.Counter()
    for t in set(own):
        for q in starts:
            base = t - q
            if base % 4 == 0 and RAM[0] <= base and base + n <= RAM[1]:
                vote[base] += 1
    vote_top = [bse for bse, c in sorted(vote.items(), key=lambda kv: (-kv[1], kv[0]))[:5] if c >= 2]
    cands = {}
    for bse in FIXED:
        cands[bse] = FIXED[bse]
    for bse in dests:
        cands.setdefault(bse, "DAT dest")
    for bse in vote_top:
        cands.setdefault(bse, "jal vote")
    if is_exe:
        cands[struct.unpack_from("<I", open(os.path.join(FILES, EXE), "rb").read(0x20), 0x18)[0]] = "t_addr"
    rows = []
    for base, label in cands.items():
        lo, hi = base, base + n
        if hi > RAM[1] or lo < RAM[0]:
            continue
        ip = [x for x in ptrs if lo <= x < hi]
        ip_on = sum(1 for x in ip if (x - base) in starts)
        ij = [t for t in own if lo <= t < hi]
        ij_on = sum(1 for t in ij if (t - base) in starts)
        pr = sum(1 for a in pairs if lo <= a < hi)
        score = ij_on * 100 - (len(ij) - ij_on) * 1000 + ip_on * 50 + len(ip) + pr
        rows.append({"base": "0x%08x" % base, "label": label, "self_jals_on_start": ij_on, "jals_in": len(ij),
                     "ptrs_in": len(ip), "ptrs_on_start": ip_on, "lui_pairs_in": pr, "score": score,
                     "self_ref": bool(ip or ij or pr)})
    rows.sort(key=lambda r: (-r["score"], r["base"]))
    rec.update({"resident_jals": len(resident), "self_jals": len(own), "abs_ptrs": len(ptrs), "lui_pairs": len(pairs),
                "candidates": len(rows)})
    if not any(r["self_ref"] for r in rows):
        rec["ranked"] = "none"
        rec["reason"] = "no self-reference at any of %d candidates (ptrs/jals/lui pairs all outside)" % len(rows)
    else:
        rec["ranked"] = [r for r in rows if r["self_ref"]][:3]
    return rec


def payload_list():
    out = []
    for ln in open(MANIFEST):
        p = json.loads(ln)["path"]
        if p.endswith(".BIN") or p == EXE:
            out.append(p)
    return sorted(out)


def load(rel):
    with open(os.path.join(FILES, rel), "rb") as f:
        b = f.read()
    is_exe = os.path.basename(rel) == EXE
    return (b[0x800:] if is_exe else b), is_exe


def render(r):
    head = "%s size=%d dens=%.2f jr=%d pro=%d %s" % (r["path"], r["size"], r["density"], r["jr_ra"], r["prologues"],
                                                   r["class_hint"])
    if r["ranked"] == "none":
        print(head, "| ranked: none (%s)" % r["reason"])
        return
    print(head, "|", " ".join("%s:%d[j%d/%d p%d/%d l%d]" % (c["base"], c["score"], c["self_jals_on_start"], c["jals_in"],
                                                         c["ptrs_on_start"], c["ptrs_in"], c["lui_pairs_in"])
                                for c in r["ranked"]), "| res_jals=%d" % r["resident_jals"])


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("payloads", nargs="*", help="paths relative to extracted/retail/files (default: all .BIN + exe)")
    ap.add_argument("--controls", action="store_true", help="exe body must rank its t_addr first, else exit 2")
    ap.add_argument("--json", default=None)
    a = ap.parse_args()
    by_dest, by_stem = dat_dests()
    dests = sorted(d for d in by_dest if RAM[0] <= d < RAM[1])
    out = []
    for rel in sorted(a.payloads or payload_list()):
        b, is_exe = load(rel)
        r = analyse(rel, b, is_exe, dests, by_stem)
        render(r)
        out.append(r)
    if a.controls:
        with open(os.path.join(FILES, EXE), "rb") as f:
            t_addr = struct.unpack_from("<I", f.read(0x20), 0x18)[0]
        r = next((x for x in out if x["path"] == EXE), None) or analyse(EXE, load(EXE)[0], True, dests, by_stem)
        top = r["ranked"][0]["base"] if r["ranked"] != "none" else "none"
        if top != "0x%08x" % t_addr:
            print("[controls] FAIL: exe t_addr 0x%08x not ranked first (top %s)" % (t_addr, top))
            sys.exit(2)
        print("[controls] OK: exe t_addr 0x%08x ranked first" % t_addr)
    print("[summary] payloads=%d dat_dests=%d" % (len(out), len(dests)))
    if a.json:
        with open(a.json, "w") as f:
            json.dump(sorted(out, key=lambda x: x["path"]), f, indent=1)
        print("->", a.json)


if __name__ == "__main__":
    main()
