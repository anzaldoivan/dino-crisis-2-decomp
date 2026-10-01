#!/usr/bin/env python3
"""census.py -- function census of the fleet (T1, Phase 1.5); stdlib only, run in the container:
    dc.sh run python3 tools/census.py [--check] [--only ALIAS...] [--fixture phantom|gap|datahead|libgame]

One record per `glabel` in text (`.section .text` + asm/<alias>/nonmatchings/**) plus one per C-defined
`func_<ADDR>` in src/<alias>/<unit>.c (fills the gaps of its `c` subsegment). Writes .run/census/functions.tsv
(`alias start end size kind family`, sorted, end exclusive), prints per-binary and fleet counts, runs the
known-true (config/probes.tsv, jr 0x8003500c) and negative controls (fixtures, empty --only).
Uncovered text spans that are evidenced data (data_spans) leave the denominator and are listed as `data in text`.
Kind (T4.c1): exe lib iff inside a boundaries.tsv lib-object extent, else game; overlay lib iff in a
tools/dup_census.py exact class that is lib-pure in the exe (T4.c2: >=1 exe lib, 0 exe non-lib members), else
game (unknown when the exe is not in the run).
Controls: cc_fingerprint.py lib-band functions are kind lib; probe rows are kind game (--fixture libgame flips one).
Definitions: phase 1.5 PHASE_PLAN `## Interfaces`; notes: docs/ops/decomp-environment.md "Function census".
Exit: 0 clean; 1 phantom/truncation/control failure, --check with unknown kinds (and every fixture run);
2 refused (empty alias list).
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LOADMAP = ROOT / "config/loadmap.tsv"
BOUNDARIES = ROOT / "config/boundaries.tsv"
PROBES = ROOT / "config/probes.tsv"
SPLAT = ROOT / "config/splat"
OUT = ROOT / ".run/census"
PREFIX = "extracted/retail/files/"
FLEET_N = 83
FAMILIES = {"exe": 1, "E": 13, "KOF": 14, "WEP": 20, "WEP_S": 10, "LOGO+ST": 11, "MAP": 1, "R2": 5, "RES": 3,
            "misc": 5}  # docs/memory-map.md:232
JR_CONTROL = 0x8003500C  # tools/boundaries.py CONTROL: switch jr in the exe
EXE = "slus_012_79"

INSN = re.compile(r"^\s*/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s*\*/\s*(\S*)\s*(.*)$")
FUNC_TOK = re.compile(r"\bfunc_([0-9A-Fa-f]{8})\b")
SELF_LINES = ("glabel", "endlabel", ".size", ".type", "nonmatching", "dlabel", "jlabel")
C_HEAD = re.compile(r"^[A-Za-z_][\w \t\*]*?\bfunc_([0-9A-Fa-f]{8})\s*\([^;{]*\)\s*\{", re.M)  # tools/banked.py:14
C_STRIP = re.compile(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'", re.S)
TOP_ITEM = re.compile(r"^  - \[(0x[0-9a-fA-F]+)(?:,\s*(\w+),\s*(\w+))?\]")
SUB_ITEM = re.compile(r"^      - \[(0x[0-9a-fA-F]+),\s*(\w+),\s*(\w+)\]")
BRANCH = re.compile(r"^(?:j|jal|b(?!reak$)[a-z]*)$")  # jal/j/branch mnemonics (jr/jalr: register targets)
TGT_TOK = re.compile(r"(?:\.L|\bfunc_|\bD_|\b0x)([0-9A-Fa-f]{8})\b")
JT_TOK = re.compile(r"\.L([0-9A-Fa-f]{8})\b")  # `.word .L…` = jtbl entry
DREF_TOK = re.compile(r"(?:\bD_|\b0x)([0-9A-Fa-f]{8})\b")
RAW_WORD = re.compile(r"\b0x([0-9A-Fa-f]{8})\b")
JAL_TOK = re.compile(r"(?:\.L|\bfunc_|\b0x)([0-9A-Fa-f]{8})\b")  # R1: `jal func_X|.LX|0x…` operand


def rows(path):
    return [l.split("\t") for l in path.read_text().splitlines() if l and not l.startswith("#")]


def family_of(alias):
    a = alias
    if a == "slus_012_79":
        return "exe"
    if a.startswith("bin_kof_"):
        return "KOF"
    if a.startswith("bin_wep_s"):
        return "WEP_S"
    if a.startswith("bin_wep"):
        return "WEP"
    if a == "psx_bin_logo" or a.startswith("psx_bin_st"):
        return "LOGO+ST"
    if a == "psx_data_map":
        return "MAP"
    if a in ("bin_option", "bin_save", "bin_load", "bin_subscr3", "bin_subscr6"):
        return "R2"
    if a.startswith("bin_res"):
        return "RES"
    if a in ("bin_m_result", "bin_m_title", "bin_title2", "bin_opening", "bin_ending"):
        return "misc"
    if re.fullmatch(r"bin_e[0-9a]\d", a):
        return "E"
    return "?"


def parse_yaml(path):
    """Minimal reader for tools/splat_gen.py output: target_path + code segments with their subsegments."""
    target, segs, cur = None, [], None
    for line in path.read_text().splitlines():
        m = re.match(r"^  target_path:\s*(\S+)", line)
        if m:
            target = m.group(1)
            continue
        m = TOP_ITEM.match(line)
        if m or line.startswith("  - name:"):
            off = int(m.group(1), 16) if m else None
            if cur is not None and cur["end"] is None:
                cur["end"] = off
            cur = None
            if not m:
                cur = {"type": None, "start": None, "vram": None, "subs": [], "end": None}
                segs.append(cur)
            continue
        if cur is None:
            continue
        m = re.match(r"^    (type|start|vram):\s*(\S+)", line)
        if m:
            cur[m.group(1)] = m.group(2) if m.group(1) == "type" else int(m.group(2), 16)
            continue
        m = SUB_ITEM.match(line)
        if m:
            cur["subs"].append((int(m.group(1), 16), m.group(2), m.group(3)))
    return target, segs


class Bin:
    def __init__(self, alias, path, size, family, asm_dir, src_dir):
        self.alias, self.path, self.size, self.family = alias, path, size, family
        self.asm_dir, self.src_dir = asm_dir, src_dir
        self.text = None  # (text_start, text_end) or None (raw bin, no base)
        self.c_spans = []  # (vstart, vend, unit)
        self.jd = []  # jtbl / data-island rows (start, end)
        self.jt = []  # jtbl rows (jr, start, end) -- R2
        self.lib = []  # lib-object rows (start, end)
        self.edges = []  # boundary edges for crossings
        self.tail = None  # (vstart, vend) odd tail


def load_fleet():
    lm = {r[0]: r for r in rows(LOADMAP) if len(r) > 3 and r[3] in ("exe", "code")}
    bnd = rows(BOUNDARIES)
    bins = {}
    for y in sorted(SPLAT.glob("*.yaml")):
        alias = y.stem
        target, segs = parse_yaml(y)
        path = target[len(PREFIX):] if target and target.startswith(PREFIX) else target
        if path not in lm:
            sys.exit("census: %s target %s has no exe|code loadmap row" % (alias, path))
        b = Bin(alias, path, int(lm[path][1]), family_of(alias), ROOT / "asm" / alias, ROOT / "src" / alias)
        te = [int(r[3], 16) for r in bnd if r[0] == path and r[1] == "text-end"]
        for r in bnd:
            if r[0] != path:
                continue
            s, e = int(r[2], 16), int(r[3], 16)
            if r[1] in ("jtbl", "data-island"):
                b.jd.append((s, e)); b.edges += [s, e]
                m = re.search(r"\bjr=0x([0-9a-fA-F]+)", r[4] if len(r) > 4 else "")
                if r[1] == "jtbl" and m:
                    b.jt.append((int(m.group(1), 16), s, e))
            elif r[1] == "lib-object":
                b.lib.append((s, e)); b.edges += [s, e]
            elif r[1] == "text-end":
                b.edges.append(e)
        ts = None
        for sg in segs:
            if sg["type"] != "code":
                continue
            v = lambda off, sg=sg: sg["vram"] + (off - sg["start"])
            subs = sg["subs"]
            for i, (off, kind, name) in enumerate(subs):
                nxt = subs[i + 1][0] if i + 1 < len(subs) else sg["end"]
                if kind in ("asm", "c") and ts is None:
                    ts = v(off)
                if kind == "c":
                    b.c_spans.append((v(off), v(nxt), name))
                if kind == "bin" and name.endswith("_trailing"):
                    b.tail = (v(off), v(off) + b.size % 4)
        if ts is not None and te:
            b.text = (ts, te[0])
        elif ts is not None or te:
            sys.exit("census: %s: text start/end incomplete (%s, %s)" % (alias, ts, te))
        bins[alias] = b
    missing = sorted(set(lm) - {b.path for b in bins.values()})
    if missing:
        sys.exit("census: loadmap rows without a splat yaml: %s" % " ".join(missing))
    return bins


RD_FUNCTS = {0x00, 0x02, 0x03, 0x04, 0x06, 0x07, 0x09, 0x10, 0x12} | set(range(0x20, 0x28)) | {0x2A, 0x2B}
RT_OPS = set(range(0x08, 0x10)) | set(range(0x20, 0x27))  # ALU-immediate, lui, loads: write rt
MEM_OPS = set(range(0x20, 0x27)) | {0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x32, 0x3A}  # loads, stores, lwc2/swc2


def never_emitted(w):
    """R3 (T2.c7): a non-zero word that decodes to an instruction a compiler never emits -- writes $zero (SPECIAL rd
    or I-type rt = 0 on an instruction that writes it; mfc/cfc rt = 0) or a load/store with base $zero."""
    if not w:
        return False
    op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    if op == 0:
        return (w & 63) in RD_FUNCTS and rd == 0
    if op in MEM_OPS and rs == 0:
        return True
    if op in RT_OPS and rt == 0:
        return True
    return op in (0x10, 0x12) and rs in (0, 2) and rt == 0


def scan_asm(asm_dir):
    """-> dict: funcs [(name, [(vram, mnem, ops)])], refs set(UPPER hex), split_g, nm_g, words {vram: insn|data|
    invalid} (.text lines), targets set(int) (jal/j/branch operands, jlabels, `.word .L` jtbl entries), drefs
    {addr: set(src vram|None)} (%hi/%lo, la, .word operands), raw set(int) (`.word 0x…` literals), wv {vram: 32-bit
    word} (.text lines), jtw {vram: target} (`.word .L…` lines, any section), r3 (count of .text insn words R3 marks
    invalid: see never_emitted)."""
    sc = {"funcs": [], "refs": set(), "split_g": 0, "nm_g": 0, "words": {}, "targets": set(), "drefs": {},
          "raw": set(), "wv": {}, "jtw": {}, "r3": 0}
    funcs, refs = sc["funcs"], sc["refs"]
    if not asm_dir.is_dir():
        return sc
    for f in sorted(asm_dir.rglob("*.s")):
        nm = "nonmatchings" in f.relative_to(asm_dir).parts
        sect = ".text" if nm else None
        cur = None
        for line in f.read_text(errors="replace").splitlines():
            s = line.strip()
            if not s.startswith(SELF_LINES):
                for h in FUNC_TOK.findall(line):
                    refs.add(h.upper())
            if s.startswith(".section"):
                sect = s.split()[1].rstrip(",")
                cur = None
                continue
            m = INSN.match(line)
            src, mn, ops = (int(m.group(1), 16), m.group(2), m.group(3)) if m else (None, (s.split() or [""])[0], s)
            if mn == ".word":
                sc["raw"].update(int(h, 16) for h in RAW_WORD.findall(ops))
                sc["targets"].update(int(h, 16) for h in JT_TOK.findall(ops))
                jm = JT_TOK.fullmatch(ops.strip())
                if jm and m:
                    sc["jtw"][src] = int(jm.group(1), 16)
            if mn in (".word", "la") or "%hi(" in ops or "%lo(" in ops:
                for h in DREF_TOK.findall(ops):
                    sc["drefs"].setdefault(int(h, 16), set()).add(src)
            if BRANCH.match(mn) and m:
                sc["targets"].update(int(h, 16) for h in TGT_TOK.findall(ops))
            if s.startswith("jlabel "):
                sc["targets"].update(int(h, 16) for h in TGT_TOK.findall(s))
            if sect != ".text":
                continue
            if m:
                k = "invalid" if "invalid instruction" in ops else "data" if mn == ".word" else "insn"
                w = int.from_bytes(bytes.fromhex(m.group(0).split("*/")[0].split()[-1]), "little")
                if k == "insn" and never_emitted(w):  # R3 (T2.c7)
                    k = "invalid"
                    sc["r3"] += 1
                sc["words"][src], sc["wv"][src] = k, w
            if s.startswith("glabel "):
                cur = (s.split()[1], [])
                funcs.append(cur)
                sc["nm_g" if nm else "split_g"] += 1
                continue
            if s.startswith(("endlabel", "jlabel", "dlabel", ".L")) and not m:
                continue
            if m and cur is not None:
                cur[1].append((src, mn, ops.replace(" ", "")))
    return sc


def c_defs(src_dir, unit):
    f = src_dir / (unit + ".c")
    if not f.is_file():
        return []
    src = C_STRIP.sub(lambda m: " " * len(m.group(0)), f.read_text())
    return sorted({int(h, 16) for h in C_HEAD.findall(src)})


def uncond(insn):
    _, mn, ops = insn
    if mn in ("jr", "j", "b"):
        return True
    if mn == "beq" and ops.startswith("$zero,$zero,"):
        return True
    return mn == "beqz" and ops.startswith("$zero,")


def ends_flow(insns):
    k = max((i for i, x in enumerate(insns) if x[1] != "nop"), default=None)
    if k is None:
        return False
    return (uncond(insns[k]) and k + 1 < len(insns)) or (k >= 1 and uncond(insns[k - 1]))


def spans(intervals, lo, hi):
    """Coverage of [lo, hi): (covered-by-one bytes, gaps [(s,e)], overlaps [(s,e)])."""
    ev = {}
    for s, e in intervals:
        s, e = max(s, lo), min(e, hi)
        if s < e:
            ev[s] = ev.get(s, 0) + 1
            ev[e] = ev.get(e, 0) - 1
    pts = sorted(set(ev) | {lo, hi})
    one, gaps, ovl, n = 0, [], [], 0
    for a, z in zip(pts, pts[1:]):
        n += ev.get(a, 0)
        if a >= hi or z <= lo:
            continue
        lst = gaps if n == 0 else ovl if n > 1 else None
        if lst is None:
            one += z - a
        elif lst and lst[-1][1] == a:
            lst[-1] = (lst[-1][0], z)
        else:
            lst.append((a, z))
    return one, gaps, ovl


def data_spans(b, gaps, scan, glob):
    """Uncovered gaps -> (data [(s, e, dlabel-head|invalid-insn)], rest [(s, e, failing test)]); dlabel-head = some
    line outside the span references it as data, invalid-insn = only invalid-instruction evidence. A gap is data iff
    every word is spimdisasm `.word` data (or flagged invalid instruction), no control-flow target lands in it
    (fleet-wide for the exe, same-binary otherwise) and it holds an invalid instruction or a line outside it
    references it as data (%hi/%lo, la, .word; exe: fleet-wide)."""
    exe = b.family == "exe"
    tg = glob["targets"] if exe else scan["targets"]
    data, rest = [], []
    for s, e in gaps:
        kinds = [scan["words"].get(a) for a in range(s, e, 4)]
        if (e - s) % 4 or s % 4 or any(k not in ("data", "invalid") for k in kinds):
            rest.append((s, e, "not-data"))
            continue
        if any(a in tg for a in range(s, e)):
            rest.append((s, e, "cf-target"))
            continue
        inv = "invalid" in kinds
        srcs = [x for a in range(s, e) for x in (glob["drefs"].get(a, ()) if exe else
                                                 ((b.alias, y) for y in scan["drefs"].get(a, ())))]
        ref = any(al != b.alias or y is None or not s <= y < e for al, y in srcs)
        if not inv and not ref:
            rest.append((s, e, "no-evidence"))
            continue
        data.append((s, e, "dlabel-head" if ref else "invalid-insn"))  # kind = evidence: data-referenced, else
        # invalid-instruction words only
    return data, rest


CRIT = ("a ref", "b fallthrough", "b' entry", "c lib-start", "d c-def", "e raw-word", "f jal-split", "g transfer-split")


def analyze(b, glob, scan):
    """-> dict with records, counts and examples for one binary."""
    funcs, refs, split_g, nm_g = scan["funcs"], scan["refs"], scan["split_g"], scan["nm_g"]
    res = {"recs": [], "covered": 0, "B": 0, "P": 0, "T": 0, "pd": dict.fromkeys(("no-insn", "outside-text",
           "in-jtbl/data", "unconfirmed"), 0), "cf": dict.fromkeys(CRIT, 0), "sole": dict.fromkeys(CRIT, 0),
           "gaps": [], "data": [], "ovl": [], "cross": [], "pex": [], "split_g": split_g, "nm_g": nm_g, "cdefs": 0,
           "insns": {}, "jsplit": 0, "gsplit": 0, "why": {}}
    if b.text is None:
        return res
    ts, te = b.text
    res["B"] = te - ts
    recs = []  # [start, end, name, insns, src]
    for name, ins in funcs:
        h = FUNC_TOK.fullmatch(name)
        st = ins[0][0] if ins else (int(h.group(1), 16) if h else 0)
        recs.append([st, ins[-1][0] + 4 if ins else st, name, ins, "asm"])
    asm_iv = [(r[0], r[1]) for r in recs]
    cdef_starts = set()
    for cs, ce, unit in b.c_spans:
        defs = c_defs(b.src_dir, unit)
        _, gaps, _ = spans(asm_iv, max(cs, ts), min(ce, te))
        used = set()
        for g0, g1 in gaps:
            inside = [d for d in defs if g0 <= d < g1]
            for i, d in enumerate(inside):
                recs.append([d, inside[i + 1] if i + 1 < len(inside) else g1, "func_%08X" % d, [], "c"])
                used.add(d)
        for d in defs:
            if d not in used:  # def not starting/inside a gap: overlaps an asm function
                end = max([e for s, e in asm_iv if s <= d < e] or [d + 4])
                recs.append([d, end, "func_%08X" % d, [], "c"])
        cdef_starts |= set(defs)
        res["cdefs"] += len(defs)
    recs.sort(key=lambda r: (r[0], r[1], r[2]))
    # R1 (T2.c4): a jal target (this binary's census instruction lines) strictly inside a function starts a new one
    jt = {int(h, 16) for r in recs for _, mn, ops in r[3] if mn == "jal" for h in JAL_TOK.findall(ops)}
    split, jstarts = [], set()
    for st, en, name, ins, srck in recs:
        for t in sorted(x for x in jt if st < x < en):
            split.append([st, t, name, [x for x in ins if x[0] < t], srck])
            st, name, ins = t, "func_%08X" % t, [x for x in ins if x[0] >= t]
            jstarts.add(t)
        split.append([st, en, name, ins, srck])
    recs = split
    res["jsplit"] = len(jstarts)
    # R2 (T2.c7, criterion g): inside F, the first non-zero word A after an unconditional transfer + delay slot starts
    # a function unless A is a j/branch target from inside F or a target of a boundaries.tsv jtbl whose jr is in F
    # (a target in the delay slot or the zero words before A reaches A by fall-through: hand-written asm)
    wv, split, gstarts = scan["wv"], [], set()
    for st, en, name, ins, srck in recs:
        inner = {int(h, 16) for _, mn, ops in ins if BRANCH.match(mn) and mn != "jal" for h in TGT_TOK.findall(ops)}
        inner |= {scan["jtw"][a] for jr, s, e in b.jt if st <= jr < en for a in range(s, e, 4) if a in scan["jtw"]}
        cut = []
        for k, x in enumerate(ins[:-2]):
            if not uncond(x):
                continue
            a = next((y[0] for y in ins[k + 2:] if wv.get(y[0], 1)), None)
            if a is not None and not any(x[0] + 4 <= t <= a for t in inner) and (not cut or a > cut[-1]):
                cut.append(a)
        for t in cut:
            split.append([st, t, name, [x for x in ins if x[0] < t], srck])
            st, name, ins = t, "func_%08X" % t, [x for x in ins if x[0] >= t]
            gstarts.add(t)
        split.append([st, en, name, ins, srck])
    recs = split
    res["gsplit"] = len(gstarts)
    # R3 (T2.c7): leading R3/invalid words of a function that no control flow reaches leave it (data head; a
    # %hi/%lo or pointer reference to them is data evidence, not a block)
    # T2.c8: in this trim only, `syscall`/`break` with a nonzero code also count (handwritten trap words, wep0a)
    trap = lambda w: w is not None and w >> 26 == 0 and (w & 63) in (0x0C, 0x0D) and (w >> 6) & 0xFFFFF != 0
    for r in recs:
        while len(r[3]) > 1 and (scan["words"].get(r[3][0][0]) == "invalid" or trap(scan["wv"].get(r[3][0][0]))) \
                and r[3][0][0] not in (
                glob["targets"] if b.family == "exe" else scan["targets"]) and r[3][0][0] not in jstarts | gstarts:
            scan["words"][r[3][0][0]] = "invalid"  # a trimmed trap word is data-head evidence like R3 (T2.c8)
            r[3] = r[3][1:]
            r[0], r[2] = r[3][0][0], "func_%08X" % r[3][0][0]
    one, gaps, ovl = spans([(r[0], r[1]) for r in recs], ts, te)
    res["data"], res["gaps"] = data_spans(b, gaps, scan, glob)
    res["covered"], res["ovl"] = one, ovl
    res["B"] -= sum(e - s for s, e, _ in res["data"])
    dends = {e for _, e, _ in res["data"]}
    lib_starts = {s for s, _ in b.lib}
    for i, (st, en, name, ins, srck) in enumerate(recs):
        bad = []
        if srck == "asm" and not ins:
            bad.append("no-insn")
        if not ts <= st < te:
            bad.append("outside-text")
        if any(s <= st < e for s, e in b.jd):
            bad.append("in-jtbl/data")
        h = FUNC_TOK.fullmatch(name)
        key = h.group(1).upper() if h else name
        conf = dict(zip(CRIT, (
            key in (glob["refs"] if b.family == "exe" else refs),
            # T5, Phase 1.6: a C-defined predecessor has no insns here but is a complete compiled
            # function (gcc emits its epilogue), so the next start is a boundary by construction.
            bool(i > 0 and (recs[i - 1][4] == "c"
                            or (recs[i - 1][3] and ends_flow(recs[i - 1][3])))),
            st == ts or st in dends,
            st in lib_starts,
            srck == "c" and st in cdef_starts,
            st in glob["raw"],
            st in jstarts,
            st in gstarts)))
        hit = [k for k in CRIT if conf[k]]
        res["why"][st] = (srck, hit)  # T4.c2: source and confirming criteria, for the exe-lib-beyond-cc line
        for k in hit:
            res["cf"][k] += 1
        if len(hit) == 1:
            res["sole"][hit[0]] += 1
        if not hit:
            bad.append("unconfirmed")
        for k in bad:
            res["pd"][k] += 1
        if bad:
            res["P"] += 1
            res["pex"].append("%s 0x%08x %s" % (b.alias, st, ",".join(bad)))
    for st, en, name, _, _ in recs:
        for x in sorted(set(b.edges)):
            if st < x < en:
                res["cross"].append((st, x))
    res["T"] = len(res["gaps"]) + len(ovl) + len(res["cross"])
    for st, en, name, ins, srck in recs:
        if b.family == "exe":  # T4.c1: exe lib iff inside a lib-object extent; overlays: lib_pass()
            kind = "lib" if any(s <= st < e for s, e in b.lib) else "game"
        else:
            kind = "unknown"
        res["recs"].append((b.alias, st, en, kind, b.family))
        if ins:
            res["insns"][st] = (en, ins)
    return res


def write_tsv(results):
    OUT.mkdir(parents=True, exist_ok=True)
    recs = sorted(x for r in results.values() for x in r["recs"])
    (OUT / "functions.tsv").write_text(
        "# alias\tstart\tend\tsize\tkind\tfamily -- tools/census.py; end exclusive\n" + "".join(
            "%s\t0x%08x\t0x%08x\t0x%x\t%s\t%s\n" % (a, s, e, e - s, k, f) for a, s, e, k, f in recs))


def run(bins, only_full, write=True):
    """Analyze bins (dict alias -> Bin); print lines; return (totals dict, results)."""
    scans = {a: scan_asm(b.asm_dir) for a, b in sorted(bins.items())}
    glob = {"refs": set(), "targets": set(), "raw": set(), "drefs": {}}
    for a, s in scans.items():
        glob["refs"] |= s["refs"]; glob["targets"] |= s["targets"]; glob["raw"] |= s["raw"]
        for x, srcs in s["drefs"].items():
            glob["drefs"].setdefault(x, set()).update((a, y) for y in srcs)
    out, results = [], {}
    tot = {"F": 0, "b": 0, "B": 0, "P": 0, "T": 0, "gapB": 0, "gapN": 0, "ovlB": 0, "ovlN": 0, "C": 0,
           "split_g": 0, "nm_g": 0, "cdefs": 0, "jsplit": 0, "gsplit": 0, "r3": 0, "dB": 0, "dN": 0, "dlabel-head": 0, "invalid-insn": 0}
    pd, cf, sole = {}, {}, {}
    for a in sorted(bins):
        b = bins[a]
        r = analyze(b, glob, scans[a])
        results[a] = r
        line = "%s: functions: %d text bytes covered: %d of %d phantoms: %d truncations: %d" % (
            a, len(r["recs"]), r["covered"], r["B"], r["P"], r["T"])
        if b.text is None:
            line += " -- no text (raw bin, no base)"
        out.append(line)
        for s, e, k in r["data"]:
            out.append("%s: data in text 0x%08x..0x%08x %d B %s" % (a, s, e, e - s, k))
            tot["dB"] += e - s; tot["dN"] += 1; tot[k] += 1
        tot["F"] += len(r["recs"]); tot["b"] += r["covered"]; tot["B"] += r["B"]
        tot["P"] += r["P"]; tot["T"] += r["T"]
        tot["gapN"] += len(r["gaps"]); tot["gapB"] += sum(e - s for s, e, _ in r["gaps"])
        tot["ovlN"] += len(r["ovl"]); tot["ovlB"] += sum(e - s for s, e in r["ovl"])
        tot["C"] += len(r["cross"])
        for k in ("split_g", "nm_g", "cdefs", "jsplit", "gsplit"):
            tot[k] += r[k]
        tot["r3"] += scans[a]["r3"]
        for k, v in r["pd"].items():
            pd[k] = pd.get(k, 0) + v
        for k, v in r["cf"].items():
            cf[k] = cf.get(k, 0) + v
        for k, v in r["sole"].items():
            sole[k] = sole.get(k, 0) + v
    print("\n".join(out))
    if write:
        write_tsv(results)
    print("functions: %d" % tot["F"])
    print("text bytes covered: %d of %d" % (tot["b"], tot["B"]))
    print("data in text: %d B (%d spans: %d dlabel-head, %d invalid-insn)" % (
        tot["dB"], tot["dN"], tot["dlabel-head"], tot["invalid-insn"]))
    print("binaries: %d of %d" % (len(bins), FLEET_N))
    print("phantoms: %d" % tot["P"])
    print("phantom detail: %s; confirmed by %s; sole %s" % (", ".join("%s %d" % (k, pd.get(k, 0)) for k in (
        "no-insn", "outside-text", "in-jtbl/data", "unconfirmed")), ", ".join(
        "%s %d" % (k, cf.get(k, 0)) for k in CRIT), ", ".join("%s %d" % (k.split()[0], sole.get(k, 0)) for k in CRIT)))
    print("(f) jal target split: %d" % tot["jsplit"])
    print("(g) transfer split: %d" % tot["gsplit"])
    print("R3 words: %d" % tot["r3"])
    print("truncations: %d" % tot["T"])
    print("truncation detail: gaps %d B (%d spans), overlaps %d B, crossings %d" % (
        tot["gapB"], tot["gapN"], tot["ovlB"], tot["C"]))
    ex = [p for r in results.values() for p in r["pex"]][:10]
    if ex:
        print("phantom examples: " + "; ".join(ex))
    tex = []
    for a in sorted(results):
        r = results[a]
        tex += ["%s gap 0x%08x..0x%08x %s" % (a, s, e, why) for s, e, why in r["gaps"]]
        tex += ["%s overlap 0x%08x..0x%08x" % (a, s, e) for s, e in r["ovl"]]
        tex += ["%s cross 0x%08x@0x%08x" % (a, s, x) for s, x in r["cross"]]
    if tex:
        print("truncation examples: " + "; ".join(tex[:10]))
    if only_full:
        n = tot["split_g"] + tot["nm_g"] + tot["cdefs"] + tot["jsplit"] + tot["gsplit"]
        print("cross-check: glabels %d (cc_fingerprint split %d + nonmatchings %d) + C-defined %d + jal splits %d"
              " + transfer splits %d = %d, census %d: %s" % (
                  tot["split_g"] + tot["nm_g"], tot["split_g"], tot["nm_g"], tot["cdefs"], tot["jsplit"],
                  tot["gsplit"], n, tot["F"], "ok" if n == tot["F"] else "MISMATCH"))
    return tot, results


def lib_pass(bins, results, tot):
    """T4.c1: exe lib objects = distinct lib-object extents (same-start rows collapse to one, named A=B…); overlay
    functions in a lib-pure dup_census exact class -> lib, other overlay functions -> game (needs the exe in the run,
    else they stay unknown); rewrites functions.tsv; prints the fleet lib block. -> unknown count.
    T4.c2: lib-pure = >=1 exe kind-lib member and 0 exe non-lib members; a mixed class (exe game members share the
    body, so it is not library-specific, DK-10) leaves its overlay members game."""
    objs = {}  # start -> (ends, names)
    if EXE in bins:
        for r in rows(BOUNDARIES):
            if r[0] == bins[EXE].path and r[1] == "lib-object":
                o = objs.setdefault(int(r[2], 16), (set(), []))
                o[0].add(int(r[3], 16))
                o[1].append(r[4].split()[-1] if len(r) > 4 else "?")
        bad = sorted(s for s, o in objs.items() if len(o[0]) != 1)
        if bad:
            sys.exit("census: lib-object rows with one start, different ends: %s" % " ".join("0x%08x" % s for s in bad))
    nrows = sum(len(o[1]) for o in objs.values())
    ncls, eqk, cline = 0, 0, "lib exact classes: 0 (lib-pure 0, mixed 0)"
    if EXE in results and len(results) > 1:
        import dup_census  # lazy: dup_census imports census
        funcs, _ = dup_census.load(None)
        dup_census.classify(funcs)
        kind = {(x[0], x[1]): x[3] for x in results[EXE]["recs"]}
        cls = {}
        for f in funcs:
            if f["tier"] == "exact":
                cls.setdefault(f["cls"], []).append(f)
        libcls = [m for m in cls.values() if any(f["alias"] == EXE and kind[(EXE, f["start"])] == "lib" for f in m)]
        cnt = lambda m: (sum(1 for f in m if f["alias"] == EXE and kind[(EXE, f["start"])] == "lib"),
                         sum(1 for f in m if f["alias"] == EXE and kind[(EXE, f["start"])] != "lib"),
                         sum(1 for f in m if f["alias"] != EXE))
        pure = [m for m in libcls if cnt(m)[1] == 0]
        mixed = sorted((m for m in libcls if cnt(m)[1]), key=lambda m: (m[0]["size"], cnt(m)))
        olib = {(f["alias"], f["start"]) for m in pure for f in m if f["alias"] != EXE}
        ncls = sum(1 for m in pure if any(f["alias"] != EXE for f in m))
        cline = "lib exact classes: %d (lib-pure %d, mixed %d%s)" % (len(libcls), len(pure), len(mixed), "".join(
            "%s%d B, %d exe lib + %d exe game + %d overlay members each" % (
                (": " if i == 0 else "; "), m[0]["size"], *cnt(m)) for i, m in enumerate(mixed)))
        eqk = sum(1 for m in libcls for f in m if f["alias"] == EXE and kind[(EXE, f["start"])] != "lib")
        for a, r in results.items():
            if a != EXE:
                r["recs"] = [x[:3] + ("lib" if x[:2] in olib else "game",) + x[4:] for x in r["recs"]]
        write_tsv(results)
    allr = [x for r in results.values() for x in r["recs"]]
    F = len(allr)
    lib = [x for x in allr if x[3] == "lib"]
    olibr = [x for x in lib if x[0] != EXE]
    exer = results[EXE]["recs"] if EXE in results else []
    nin = lambda s, e: sum(1 for x in exer if s <= x[1] < e)
    groups = sorted(s for s, o in objs.items() if len(o[1]) > 1)
    print("lib objects: %d (%d rows, %d same-start groups resolved)" % (len(objs), nrows, len(groups)))
    for s in groups:
        e = min(objs[s][0])
        print("lib same-start 0x%08x %s: %d functions" % (s, "=".join(objs[s][1]), nin(s, e)))
    print(cline)
    print("lib functions: %d of %d (exe %d, overlays %d in %d binaries, %d exact classes, smallest %s B)" % (
        len(lib), F, len(lib) - len(olibr), len(olibr), len({x[0] for x in olibr}), ncls,
        min((x[2] - x[1] for x in olibr), default="-")))
    print("lib bytes: %d of %d" % (sum(x[2] - x[1] for x in lib), tot["b"]))
    print("game functions: %d of %d" % (sum(1 for x in allr if x[3] == "game"), F))
    U = sum(1 for x in allr if x[3] == "unknown")
    print("unknown: %d" % U)
    print("exe non-lib functions exact-equal to a lib function: %d" % eqk)
    zero = [s for s in sorted(objs) if not nin(s, min(objs[s][0]))]
    print("lib objects with 0 functions: %d%s" % (len(zero), "" if not zero else " -- " + ", ".join(
        "0x%08x %s" % (s, "=".join(objs[s][1])) for s in zero)))
    return U


def cc_lib_starts():
    """{(alias, start)} of every function tools/cc_fingerprint.py counts in its lib band: its own main() (parsing,
    lib ranges) run with in_lib/scan_func/scan_alias observed; the count is checked against its summary.txt."""
    import contextlib
    import io
    import cc_fingerprint as cc
    got, st = [], {}
    o_in, o_sf, o_sa = cc.in_lib, cc.scan_func, cc.scan_alias

    def in_lib(v, ranges):
        x = o_in(v, ranges)
        st.setdefault("first", (v, x))  # flush() asks for func[0] first
        return x

    def scan_func(body, c):
        v, x = st.pop("first")
        if x:
            got.append((st["alias"], v))
        o_sf(body, c)

    def scan_alias(alias, *a):
        st["alias"] = alias
        o_sa(alias, *a)

    cc.in_lib, cc.scan_func, cc.scan_alias = in_lib, scan_func, scan_alias
    try:
        with contextlib.redirect_stdout(io.StringIO()):
            cc.main()
    finally:
        cc.in_lib, cc.scan_func, cc.scan_alias = o_in, o_sf, o_sa
    m = re.search(r"^lib control: .*\bfunctions=(\d+)", (cc.OUT / "summary.txt").read_text(), re.M)
    if not m or int(m.group(1)) != len(got):
        sys.exit("census: cc_fingerprint lib functions %s != observed %d" % (m and m.group(1), len(got)))
    return set(got)


def odd_tails(bins):
    tails = [b for b in bins.values() if b.size % 4]
    inside = 0
    det = []
    for b in sorted(tails, key=lambda b: b.alias):
        if b.tail is None:
            det.append("%s: no trailing subsegment" % b.alias)
            inside += 1
            continue
        s, e = b.tail
        ins = b.text is not None and s < b.text[1] and e > b.text[0]
        inside += ins
        det.append("%s: 0x%08x..0x%08x (%d B) %s" % (b.alias, s, e, e - s,
                                                       "INSIDE text" if ins else "after text-end"))
    print("odd tails: %d binaries (size%%4) — inside text: %d" % (len(tails), inside))
    for d in det:
        print("odd tail " + d)
    return len(tails), inside


def controls(bins, results):
    ok = True
    n = ng = 0
    for r in rows(PROBES):
        name, alias, s, e = r[0], r[1], r[2], r[3]
        if alias == "self":
            print("%s: n/a (own object, no game address)" % name)
            continue
        if alias not in results:
            continue
        n += 1
        s, e = int(s, 16), int(e, 16)
        hit = any(x[1] == s and x[2] == e for x in results[alias]["recs"])
        ng += any(x[1] == s and x[2] == e and x[3] == "game" for x in results[alias]["recs"])
        ok &= hit
        print("control probe %s %s [0x%08x,0x%08x): %s" % (name, alias, s, e, "ok" if hit else "FAIL"))
    ok &= ng == n
    print("control game: %d of %d probes kind game: %s" % (ng, n, "ok" if ng == n else "FAIL"))
    if EXE in results:
        cl = cc_lib_starts()
        kind = {(x[0], x[1]): x[3] for x in results[EXE]["recs"]}
        k = sum(1 for x in cl if kind.get(x) == "lib")
        good = k == len(cl) >= 1
        ok &= good
        print("control lib: %d of %d cc_fingerprint lib functions kind lib: %s" % (k, len(cl), "ok" if good else "FAIL"))
        why = results[EXE]["why"]  # T4.c2: census exe lib starts cc_fingerprint does not count, with their record reason
        bey = sorted(x[1] for x in results[EXE]["recs"] if x[3] == "lib" and (EXE, x[1]) not in cl)
        print("exe lib beyond cc_fingerprint: %d%s" % (len(bey), "" if not bey else " (" + ", ".join(
            "0x%08x" % s for s in bey) + ") — " + "; ".join(
            "0x%08x src %s, criteria %s" % (s, why[s][0], "+".join(why[s][1]) or "none") for s in bey)))
    if "slus_012_79" in results:
        ins = results["slus_012_79"]["insns"]
        hit = [(st, en, i) for st, (en, i) in ins.items() if st <= JR_CONTROL < en]
        mn = [x[1] for h in hit for x in h[2] if x[0] == JR_CONTROL]
        good = len(hit) == 1 and mn == ["jr"]
        ok &= good
        print("control jr 0x%08x in function 0x%08x: %s" % (JR_CONTROL, hit[0][0] if hit else 0,
                                                            "ok" if good else "FAIL"))
    return ok, n


# --- negative-control fixtures: standard encodings written here (never copied from game asm) ---
W = {"addiu-": ("F8FFBD27", "addiu", "$sp, $sp, -0x8"), "addiu+": ("0800BD27", "addiu", "$sp, $sp, 0x8"),
     "jr": ("0800E003", "jr", "$ra"), "nop": ("00000000", "nop", ""), "jal": ("0040000C", "jal", "func_80010000")}
FX_BASE = 0x80010000
FX_FUNCS = [("A", ["addiu-", "jr", "addiu+", "nop"]),
            ("B", ["addiu-", "jal", "nop", "addiu+", "jr", "nop"]),
            ("C", ["addiu-", "jr", "addiu+", "nop"])]


def make_fixture(kind):
    d = OUT / "fixture" / kind
    asm = d / "asm" / "fx"
    asm.mkdir(parents=True, exist_ok=True)
    head = ['.include "macro.inc"', "", ".section .text, \"ax\"", ""]
    lines, dropped = list(head), list(head)
    v = FX_BASE
    if kind == "datahead":  # evidenced-looking data head (invalid insn) that B's jal jumps into: stays a truncation
        lines.append("dlabel D_%08X" % v)
        for val in (0xFC000000, 0x00000001):  # opcode 0x3F: undefined on R3000
            lines.append("    /* %X %08X %s */ .word 0x%08X%s" % (v - FX_BASE, v, val.to_bytes(4, "little").hex().upper(),
                                                              val, " /* invalid instruction */" if val >> 26 else ""))
            v += 4
        lines.append("enddlabel D_%08X" % FX_BASE)
    for fn, ws in FX_FUNCS:
        drop = kind == "gap" and fn == "C"  # C's instruction words emitted without a glabel (not data)
        out = dropped if drop else lines
        if not drop:
            lines.append("glabel func_%08X" % v)
        for i, w in enumerate(ws):
            if kind == "phantom" and fn == "B" and i == 3:
                lines.append("glabel func_%08X" % v)  # planted mid-body label, no oracle confirms it
            word, mn, ops = W[w]
            if kind == "datahead" and w == "jal":
                ops = "D_%08X" % FX_BASE
            out.append("    /* %X %08X %s */  %-10s %s" % (v - FX_BASE, v, word, mn, ops))
            v += 4
        if not drop:
            lines.append("endlabel func_%08X" % (v - 4 * len(ws)))
    (asm / "fx_0.s").write_text("\n".join(lines) + "\n")
    if len(dropped) > len(head):
        (asm / "fx_1.s").write_text("\n".join(dropped) + "\n")
    b = Bin("fx_" + kind, "FIXTURE/FX.BIN", v - FX_BASE, "fixture", asm, d / "src")
    b.text = (FX_BASE, v)
    b.edges = [v]
    return b


def run_fixture(kind):
    print("fixture %s:" % kind)
    tot, _ = run({"fx_" + kind: make_fixture(kind)}, False, write=False)
    return tot


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--only", nargs="*")
    ap.add_argument("--fixture", choices=("phantom", "gap", "datahead", "libgame"))
    a = ap.parse_args(argv)
    if a.fixture and a.fixture != "libgame":
        tot = run_fixture(a.fixture)
        return 1 if tot["P"] or tot["T"] else 0
    aliases = sorted(p.stem for p in SPLAT.glob("*.yaml"))
    if a.only is not None:
        aliases = [x for x in aliases if x in set(a.only)]
    if not aliases:
        print("REFUSED: empty alias list")
        return 2
    OUT.mkdir(parents=True, exist_ok=True)
    if not (ROOT / "extracted/retail/files").is_dir() and not (ROOT / ".run/extracted/retail/files").is_dir():
        if subprocess.run(["make", "extract", "OUT=.run/extracted/retail"], cwd=ROOT, stdout=open(
                OUT / "extract.log", "w"), stderr=subprocess.STDOUT).returncode:
            print("census: make extract failed (.run/census/extract.log)")
            return 1
    if subprocess.run(["make", "-s", "-j", "split"], cwd=ROOT, stdout=open(OUT / "split.log", "w"),
                      stderr=subprocess.STDOUT).returncode:
        print("census: make split failed (.run/census/split.log)")
        return 1
    fleet = load_fleet()
    full = a.only is None
    if full:
        fam = {}
        for b in fleet.values():
            fam[b.family] = fam.get(b.family, 0) + 1
        if fam != FAMILIES or len(fleet) != FLEET_N:
            print("census: family counts %s != %s" % (sorted(fam.items()), sorted(FAMILIES.items())))
            return 1
    bins = {x: fleet[x] for x in aliases}
    tot, results = run(bins, full)
    unk = lib_pass(bins, results, tot)
    _, tails_in = odd_tails(bins)
    if a.fixture == "libgame":  # negative control: one exe lib function flipped to game (functions.tsv untouched)
        r = results.get(EXE, {"recs": []})["recs"]
        i = next((i for i, x in enumerate(r) if x[3] == "lib"), None)
        if i is not None:
            r[i] = r[i][:3] + ("game",) + r[i][4:]
            print("fixture libgame: %s 0x%08x lib -> game" % (EXE, r[i][1]))
    cok, nprobe = controls(bins, results)
    if full:
        cok &= nprobe == 6  # was 5; T5.c1 added the func_80052634 probe row
    good = cok and tot["P"] == 0 and tot["T"] == 0 and tails_in == 0
    if a.check:
        good &= unk == 0
    if a.fixture:
        good = False
    if a.check:
        fp = run_fixture("phantom")
        neg_p = fp["P"] == 1
        print("negative control phantom: %s (phantoms %d, expect 1)" % ("ok" if neg_p else "FAIL", fp["P"]))
        fg = run_fixture("gap")
        neg_g = fg["T"] >= 1
        print("negative control gap: %s (truncations %d, expect >= 1)" % ("ok" if neg_g else "FAIL", fg["T"]))
        fd = run_fixture("datahead")
        neg_d = fd["T"] >= 1
        print("negative control datahead: %s (truncations %d, expect >= 1)" % ("ok" if neg_d else "FAIL", fd["T"]))
        neg_g &= neg_d
        p = subprocess.run([sys.executable, str(Path(__file__).resolve()), "--only"], capture_output=True, text=True)
        neg_e = p.returncode == 2 and "REFUSED: empty alias list" in p.stdout
        print("negative control empty --only: %s (rc %d)" % ("ok" if neg_e else "FAIL", p.returncode))
        cok &= neg_p and neg_g and neg_e
        print("control: %s" % ("ok" if cok else "FAIL"))
        good &= cok and full
        print("check: %s" % ("OK" if good else "FAIL"))
    else:
        print("control: %s" % ("ok" if cok else "FAIL"))
    return 0 if good else 1


if __name__ == "__main__":
    sys.exit(main())
