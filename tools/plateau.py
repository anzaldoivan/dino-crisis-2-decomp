#!/usr/bin/env python3
"""tools/plateau.py -- plateau classifier: label a draft's residual against the game fn (T7, Phase 1.7). Runs in the image.

  plateau.py <draft.c> --target alias:start:end   compile the draft (defines func_<START>) and label it
  plateau.py .run/permute/<lever>/                 label the permuter's best candidate (lowest output-<score>-<n>/source.c,
                                                   tie: lower n) against the config/levers.tsv row named by the dir basename
  plateau.py --check                               every levers.tsv row: before_c must get the row's `label`, after_c MATCH

Compile and target exactly as tools/codegen_map.py Ctx.build/classify: tools/probe.py Probe under the pinned triple
(config/toolchains.tsv `pinned`), sym func_<START>, target words via probe.target_of; mask = the draft's relocs only.
Streams are aligned index-wise; eqm(i, j) = (draft[i] & keep[i]) == (tgt[j] & keep[i]), keep = ~draft mask of insn i.
Diff words are decoded by a pure-python MIPS-I decoder (skel = word with reg/imm fields zeroed, regs, imm); an
undecodable word is never a reg diff. Labels, first match wins (C0043):
  MATCH (equal length, no masked diff) · SIZE-MISMATCH (d = len diff, |d| > max(2, .15 nt) or |d| >= .5 nt) ·
  LENGTH-DRIFT (other d != 0; `tail` if one shift at the first diff makes the whole tail equal, else `partial`) ·
  REGALLOC-PERM (every diff same skel+imm, regs differ; `map: consistent|inconsistent`) · SCHEDULE-REORDER (masked
  multiset at the diff positions equal) · DELAY-SLOT (nop vs non-nop at >= half the diffs) · WIDTH (every opcode diff
  within the load/store family) · BRANCH-POLARITY (>= 1 inverted branch pair; every other diff branch-offset-only or the
  rest an equal masked multiset) · IMM-OFFSET (imm-only, one delta) · IMM-VALUE (imm-only, varying) · OPCODE-MIXED
  (>= 1 skel diff) · UNKNOWN.
Bucket (policy, derived at print time) and `levers:` (ids of levers.tsv rows in the label's static groups plus the
groups of rows carrying this label) follow the label. Output: one `evidence:` line (counts, diff indices, mnemonic
names only; G12), `label:`, `bucket:`, `levers:`. --check prints per-row lines, `plateau: k of P planted labelled`,
`match control: ok|FAIL`, `unknown: U`. Exit 0 ok, 1 mislabel or control FAIL, 2 usage/config error (a row label
outside the vocabulary).
"""
import argparse
import re
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import codegen_map  # noqa: E402  (levers.tsv parser; compiles nothing at import)

SCRATCH = ROOT / ".run/plateau"
LABELS = ("MATCH", "SIZE-MISMATCH", "LENGTH-DRIFT", "REGALLOC-PERM", "SCHEDULE-REORDER", "DELAY-SLOT", "WIDTH",
          "BRANCH-POLARITY", "IMM-OFFSET", "IMM-VALUE", "OPCODE-MIXED", "UNKNOWN")
BUCKET = {"MATCH": "integration", "SIZE-MISMATCH": "redraft", "LENGTH-DRIFT": "structural",
          "REGALLOC-PERM": "permuter", "SCHEDULE-REORDER": "permuter", "DELAY-SLOT": "permuter",
          "IMM-VALUE": "permuter", "WIDTH": "structural", "BRANCH-POLARITY": "structural",
          "IMM-OFFSET": "structural", "OPCODE-MIXED": "structural", "UNKNOWN": "unknown"}
STATIC_GROUPS = {"REGALLOC-PERM": {"regalloc"}, "SCHEDULE-REORDER": {"sched-reorg"}, "DELAY-SLOT": {"sched-reorg"},
                 "BRANCH-POLARITY": {"sched-reorg"}, "WIDTH": {"expand-cse"}, "IMM-OFFSET": {"loop"},
                 "IMM-VALUE": {"expand-cse"}}

# ---- MIPS-I decoder (no objdump) -------------------------------------------------------------------------------
SPECIAL = {0: "sll", 2: "srl", 3: "sra", 4: "sllv", 6: "srlv", 7: "srav", 8: "jr", 9: "jalr", 12: "syscall",
           13: "break", 16: "mfhi", 17: "mthi", 18: "mflo", 19: "mtlo", 24: "mult", 25: "multu", 26: "div",
           27: "divu", 32: "add", 33: "addu", 34: "sub", 35: "subu", 36: "and", 37: "or", 38: "xor", 39: "nor",
           42: "slt", 43: "sltu"}
REGIMM = {0: "bltz", 1: "bgez", 16: "bltzal", 17: "bgezal"}
OPS = {2: "j", 3: "jal", 4: "beq", 5: "bne", 6: "blez", 7: "bgtz", 8: "addi", 9: "addiu", 10: "slti", 11: "sltiu",
       12: "andi", 13: "ori", 14: "xori", 15: "lui", 32: "lb", 33: "lh", 34: "lwl", 35: "lw", 36: "lbu", 37: "lhu",
       38: "lwr", 40: "sb", 41: "sh", 42: "swl", 43: "sw", 46: "swr"}
OPS.update({48 + z: f"lwc{z}" for z in range(4)})
OPS.update({56 + z: f"swc{z}" for z in range(4)})
COPMV = {0: "mfc", 2: "cfc", 4: "mtc", 6: "ctc"}
MEM = set(range(32, 47)) & set(OPS)
INVERTED = {frozenset(("beq", "bne")), frozenset(("blez", "bgtz")), frozenset(("bltz", "bgez")),
            frozenset(("bltzal", "bgezal")), frozenset(("beqz", "bnez")), frozenset(("beq", "bnez")),
            frozenset(("beqz", "bne"))}
BRANCHES = {"beq", "bne", "beqz", "bnez", "b", "blez", "bgtz", "bltz", "bgez", "bltzal", "bgezal", "bc0f", "bc0t",
            "bc1f", "bc1t", "bc2f", "bc2t", "bc3f", "bc3t"}


class Undecodable(Exception):
    pass


def decode(w):
    """Return (mnemonic, skel, regs, imm); skel = w with the format's reg/imm fields zeroed. Raise Undecodable."""
    op, rs, rt, rd, fn = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, w & 63
    if op == 0:
        if fn not in SPECIAL:
            raise Undecodable(w)
        if fn in (12, 13):
            return SPECIAL[fn], w & 0xFC00003F, (), (w >> 6) & 0xFFFFF
        return ("nop" if w == 0 else SPECIAL[fn]), w & 0xFC00003F, (rs, rt, rd), (w >> 6) & 31
    if op == 1:
        if rt not in REGIMM:
            raise Undecodable(w)
        return REGIMM[rt], w & 0xFC1F0000, (rs,), w & 0xFFFF
    if op in (2, 3):
        return OPS[op], w & 0xFC000000, (), w & 0x3FFFFFF
    if op in (4, 5):
        name = OPS[op] if rt else {4: "beqz", 5: "bnez"}[op]
        name = "b" if op == 4 and rs == 0 and rt == 0 else name
        return name, w & 0xFC000000, (rs, rt), w & 0xFFFF
    if op in (6, 7) or 8 <= op <= 14:
        return OPS[op], w & 0xFC000000, (rs, rt), w & 0xFFFF
    if op == 15:
        return OPS[op], w & 0xFFE00000, (rt,), w & 0xFFFF
    if 16 <= op <= 19:
        z = op - 16
        if rs & 0x10:
            return f"cop{z}", w, (), 0
        if rs == 8:
            return f"bc{z}{'t' if rt & 1 else 'f'}", w & 0xFFFF0000, (), w & 0xFFFF
        if rs in COPMV and not w & 0x7FF:
            return f"{COPMV[rs]}{z}", w & 0xFFE007FF, (rt, rd), 0
        raise Undecodable(w)
    if op in OPS:
        return OPS[op], w & 0xFC000000, (rs, rt), w & 0xFFFF
    raise Undecodable(w)


# ---- classification ---------------------------------------------------------------------------------------------
def classify(draft, tgt):
    """draft = (words, masks, relocs) from probe.extract; tgt = target words. Return (label, evidence, extra)."""
    dw, dm = draft[0], draft[1]
    tw = tgt
    nd, nt = len(dw), len(tw)
    keep = [~dm.get(i, 0) & 0xFFFFFFFF for i in range(nd)]

    def eqm(i, j):
        return i < nd and j < nt and dw[i] & keep[i] == tw[j] & keep[i]

    diffs = [i for i in range(max(nd, nt)) if not eqm(i, i)]
    head = f"n_draft={nd} n_target={nt} diffs={len(diffs)} at={','.join(map(str, diffs[:8]))}" \
           + ("…" if len(diffs) > 8 else "")
    if not diffs and nd == nt:
        return "MATCH", head, {}
    d = nd - nt
    if d:
        k = diffs[0]
        if abs(d) > max(2, 0.15 * nt) or abs(d) >= 0.5 * nt:
            return "SIZE-MISMATCH", f"{head} d={d:+d}", {}
        if d > 0:
            tail = all(eqm(k + d + t, k + t) for t in range(nt - k))
        else:
            tail = all(eqm(k + t, k - d + t) for t in range(nd - k))
        how = "tail" if tail else "partial"
        return "LENGTH-DRIFT", f"{head} d={d:+d} {how}", {"d": d, "tail": tail}

    # equal lengths: decode the masked words at each diff position
    a = {i: dw[i] & keep[i] for i in diffs}
    b = {i: tw[i] & keep[i] for i in diffs}
    dec, kinds = {}, {}
    for i in diffs:
        try:
            dec[i] = (decode(a[i]), decode(b[i]))
        except Undecodable:
            kinds[i] = "unknown"
            continue
        (ma, sa, ra, ia), (mb, sb, rb, ib) = dec[i]
        if sa != sb:
            kinds[i] = "opcode"
        elif ra != rb and ia == ib:
            kinds[i] = "reg"
        elif ia != ib and ra == rb:
            kinds[i] = "imm"
        elif ia != ib:
            kinds[i] = "reg+imm"
        else:
            kinds[i] = "unknown"
    hist = Counter(kinds.values())
    head += " kinds=" + ",".join(f"{k}:{n}" for k, n in sorted(hist.items()))
    unknown = hist["unknown"]
    pairs = [(i, dec[i][0][0], dec[i][1][0]) for i in diffs if kinds[i] == "opcode"]
    pair_txt = " pairs=" + ",".join(f"{x}/{y}@{i}" for i, x, y in pairs[:6]) if pairs else ""

    if all(kinds[i] == "reg" for i in diffs):
        fwd, rev, ok = {}, {}, True
        for i in diffs:
            for x, y in zip(dec[i][0][2], dec[i][1][2]):
                if x != y and (fwd.setdefault(x, y) != y or rev.setdefault(y, x) != x):
                    ok = False
        return "REGALLOC-PERM", f"{head} map: {'consistent' if ok else 'inconsistent'}", {}
    if Counter(a.values()) == Counter(b.values()):
        return "SCHEDULE-REORDER", f"{head} span={diffs[0]}-{diffs[-1]}", {}
    nops = sum(1 for i in diffs if (a[i] == 0) != (b[i] == 0))
    if nops and nops >= len(diffs) / 2:
        return "DELAY-SLOT", f"{head} nop_positions={nops}", {}
    if unknown:
        return "UNKNOWN", head, {}
    if pairs and all(dec[i][0][1] >> 26 in MEM and dec[i][1][1] >> 26 in MEM for i, _x, _y in pairs):
        return "WIDTH", head + pair_txt, {}
    inv = [i for i, x, y in pairs if frozenset((x, y)) in INVERTED]
    if inv:
        rest = [i for i in diffs if i not in inv
                and not (kinds[i] == "imm" and dec[i][0][0] in BRANCHES and dec[i][1][0] in BRANCHES)]
        if Counter(a[i] for i in rest) == Counter(b[i] for i in rest):
            return "BRANCH-POLARITY", head + pair_txt + f" inverted={len(inv)} rest={len(rest)}", {}
    if all(kinds[i] == "imm" for i in diffs):
        deltas = {dec[i][1][3] - dec[i][0][3] for i in diffs}
        if len(deltas) == 1:
            return "IMM-OFFSET", f"{head} delta={deltas.pop()}", {}
        return "IMM-VALUE", f"{head} deltas={len(deltas)}", {}
    if pairs:
        return "OPCODE-MIXED", head + pair_txt, {}
    return "UNKNOWN", head, {}


def bucket(label, extra):
    if label == "LENGTH-DRIFT" and abs(extra.get("d", 99)) <= 2 and extra.get("tail"):
        return "permuter"
    return BUCKET[label]


def levers_for(label, rows):
    groups = set(STATIC_GROUPS.get(label, ())) | {r["group"] for r in rows if r["label"] == label}
    return sorted(r["lever"] for r in rows if r["group"] in groups)


# ---- compile (as tools/codegen_map.py Ctx.build / classify) -----------------------------------------------------
class Ctx:
    """probe.py module, cc kinds, pinned triple, fleet by alias. Imported lazily (container only)."""

    def __init__(self):
        import probe
        import splat_gen
        self.probe = probe
        if probe.ensure_extracted():
            raise SystemExit(2)
        tcs = probe.toolchains()
        self.kind = {r["name"]: r["kind"] for r in tcs}
        pins = [(r["name"], r.get("pinned", "-")) for r in tcs if r.get("pinned", "-") not in ("-", "", None)]
        if len(pins) != 1:
            raise SystemExit(f"plateau: {len(pins)} rows of config/toolchains.tsv carry a pinned value; want 1")
        cc, v = pins[0]
        av, gv, ov = v.split("/")
        self.t = (cc, av[1:], int(gv[1:]), int(ov[1:]))
        self.fleet = {a["alias"]: a for a in splat_gen.fleet()[0]}

    def label(self, tag, c_path, alias, start, end):
        """Compile c_path (defines func_<START>) under the pinned triple; return (label, evidence, extra)."""
        sym = f"func_{int(start, 16):08X}"
        p = self.probe.Probe({"name": sym, "c_path": str(c_path)}, self.kind, scratch=SCRATCH / tag)
        cc, _, g, o = self.t
        p.asm(cc, g, o)
        draft = p.obj(self.t)
        tgt = self.probe.target_of({"name": tag, "alias": alias, "start": start, "end": end}, self.fleet)
        return classify(draft, tgt[0])


def report(res, rows):
    label, ev, extra = res
    print(f"evidence: {ev}")
    print(f"label: {label}")
    print(f"bucket: {bucket(label, extra)}")
    print(f"levers: {' '.join(levers_for(label, rows)) or 'none'}")


def check(ctx, rows):
    k = bad = unk = 0
    ctl = True
    for r in rows:
        try:
            lb, evb, _ = ctx.label(f"{r['lever']}/before", r["before_c"], r["alias"], r["start"], r["end"])
            la, _, _ = ctx.label(f"{r['lever']}/after", r["after_c"], r["alias"], r["start"], r["end"])
        except RuntimeError as e:
            print(f"{r['lever']}: compile error: {str(e)[:200]}")
            bad, ctl = bad + 1, False
            continue
        ok = lb == r["label"]
        k += ok
        bad += not ok
        unk += lb == "UNKNOWN"
        ctl &= la == "MATCH"
        print(f"{r['lever']}: before {lb} (row {r['label']}) {'ok' if ok else 'MISLABEL'}; "
              f"after {la} {'ok' if la == 'MATCH' else 'FAIL'}; evidence: {evb}")
    print(f"plateau: {k} of {len(rows)} planted labelled")
    print(f"match control: {'ok' if ctl else 'FAIL'}")
    print(f"unknown: {unk}")
    return 1 if bad or not ctl else 0


def best_candidate(d):
    cands = []
    for c in d.glob("output-*-*/source.c"):
        m = re.fullmatch(r"output-(\d+)-(\d+)", c.parent.name)
        if m:
            cands.append((int(m[1]), int(m[2]), c))
    return min(cands, key=lambda x: x[:2]) if cands else None


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("draft", nargs="?", help="draft .c, or a .run/permute/<lever>/ dir")
    ap.add_argument("--target", help="alias:start:end (hex, end exclusive)")
    ap.add_argument("--check", action="store_true")
    a = ap.parse_args()
    rows = codegen_map.read_rows()
    if isinstance(rows, str):
        print(f"plateau: {rows}", file=sys.stderr)
        return 2
    if a.check:
        bad = [r["lever"] for r in rows if r["label"] not in LABELS]
        if bad:
            print(f"plateau: config/levers.tsv label outside the vocabulary: {', '.join(bad)}", file=sys.stderr)
            return 2
        return check(Ctx(), rows)
    if not a.draft:
        ap.error("draft or --check required")
    path = Path(a.draft)
    if path.is_dir():
        row = next((r for r in rows if r["lever"] == path.resolve().name), None)
        if row is None:
            print(f"plateau: no config/levers.tsv row {path.resolve().name!r}", file=sys.stderr)
            return 2
        best = best_candidate(path)
        if best is None:
            print(f"plateau: no output-<score>-<n>/source.c under {path}", file=sys.stderr)
            return 2
        s, n, c = best
        print(f"candidate: output-{s}-{n} (permuter score {s})")
        report(Ctx().label(f"permute/{row['lever']}", c.resolve(), row["alias"], row["start"], row["end"]), rows)
        return 0
    m = re.fullmatch(r"([A-Za-z0-9_]+):(0x[0-9A-Fa-f]+):(0x[0-9A-Fa-f]+)", a.target or "")
    if not m:
        ap.error("--target alias:start:end required")
    report(Ctx().label(f"draft/{path.stem}", path.resolve(), m[1], m[2], m[3]), rows)
    return 0


if __name__ == "__main__":
    sys.exit(main())
