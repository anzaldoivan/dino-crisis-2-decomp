#!/usr/bin/env python3
"""census.py -- function census of the fleet (T1, Phase 1.5); stdlib only, run in the container:
    dc.sh run python3 tools/census.py [--check] [--only ALIAS...] [--fixture phantom|gap]

One record per `glabel` in text (`.section .text` + asm/<alias>/nonmatchings/**) plus one per C-defined
`func_<ADDR>` in src/<alias>/<unit>.c (fills the gaps of its `c` subsegment). Writes .run/census/functions.tsv
(`alias start end size kind family`, sorted, end exclusive), prints per-binary and fleet counts, runs the
known-true (config/probes.tsv, jr 0x8003500c) and negative controls (fixtures, empty --only).
Definitions: phase 1.5 PHASE_PLAN `## Interfaces`; notes: docs/ops/decomp-environment.md "Function census".
Exit: 0 clean; 1 phantom/truncation/control failure (and every fixture run); 2 refused (empty alias list).
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

INSN = re.compile(r"^\s*/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s*\*/\s*(\S*)\s*(.*)$")
FUNC_TOK = re.compile(r"\bfunc_([0-9A-Fa-f]{8})\b")
SELF_LINES = ("glabel", "endlabel", ".size", ".type", "nonmatching", "dlabel", "jlabel")
C_HEAD = re.compile(r"^[A-Za-z_][\w \t\*]*?\bfunc_([0-9A-Fa-f]{8})\s*\([^;{]*\)\s*\{", re.M)  # tools/banked.py:14
C_STRIP = re.compile(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'", re.S)
TOP_ITEM = re.compile(r"^  - \[(0x[0-9a-fA-F]+)(?:,\s*(\w+),\s*(\w+))?\]")
SUB_ITEM = re.compile(r"^      - \[(0x[0-9a-fA-F]+),\s*(\w+),\s*(\w+)\]")


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


def scan_asm(asm_dir):
    """-> (functions [(name, [(vram, mnem, ops)])], refs set(UPPER hex), split_glabels, nm_glabels)."""
    funcs, refs, split_g, nm_g = [], set(), 0, 0
    if not asm_dir.is_dir():
        return funcs, refs, split_g, nm_g
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
            if sect != ".text":
                continue
            if s.startswith("glabel "):
                cur = (s.split()[1], [])
                funcs.append(cur)
                if nm:
                    nm_g += 1
                else:
                    split_g += 1
                continue
            if s.startswith(("endlabel", "jlabel", "dlabel", ".L")) and not INSN.match(line):
                continue
            m = INSN.match(line)
            if m and cur is not None:
                cur[1].append((int(m.group(1), 16), m.group(2), m.group(3).replace(" ", "")))
    return funcs, refs, split_g, nm_g


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


def analyze(b, global_refs, scan):
    """-> dict with records, counts and examples for one binary."""
    funcs, refs, split_g, nm_g = scan
    res = {"recs": [], "covered": 0, "B": 0, "P": 0, "T": 0, "pd": dict.fromkeys(("no-insn", "outside-text",
           "in-jtbl/data", "unconfirmed"), 0), "cf": dict.fromkeys(("ref", "fallthrough", "lib-start", "c-def"), 0),
           "gaps": [], "ovl": [], "cross": [], "pex": [], "split_g": split_g, "nm_g": nm_g, "cdefs": 0, "insns": {}}
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
        if srck == "c":
            res["cf"]["c-def"] += 1
        elif key in (global_refs if b.family == "exe" else refs):
            res["cf"]["ref"] += 1
        elif i > 0 and recs[i - 1][3] and ends_flow(recs[i - 1][3]):
            res["cf"]["fallthrough"] += 1
        elif st in lib_starts:
            res["cf"]["lib-start"] += 1
        else:
            bad.append("unconfirmed")
        for k in bad:
            res["pd"][k] += 1
        if bad:
            res["P"] += 1
            res["pex"].append("%s 0x%08x %s" % (b.alias, st, ",".join(bad)))
    one, gaps, ovl = spans([(r[0], r[1]) for r in recs], ts, te)
    res["covered"], res["gaps"], res["ovl"] = one, gaps, ovl
    for st, en, name, _, _ in recs:
        for x in sorted(set(b.edges)):
            if st < x < en:
                res["cross"].append((st, x))
    res["T"] = len(gaps) + len(ovl) + len(res["cross"])
    for st, en, name, ins, srck in recs:
        if srck == "c":
            kind = "game"
        elif b.family == "exe" and any(s <= st < e for s, e in b.lib):
            kind = "lib"
        else:
            kind = "unknown"
        res["recs"].append((b.alias, st, en, kind, b.family))
        if ins:
            res["insns"][st] = (en, ins)
    return res


def run(bins, only_full, write=True):
    """Analyze bins (dict alias -> Bin); print lines; return (totals dict, results)."""
    scans = {a: scan_asm(b.asm_dir) for a, b in sorted(bins.items())}
    global_refs = set().union(*(s[1] for s in scans.values())) if scans else set()
    out, results = [], {}
    tot = {"F": 0, "b": 0, "B": 0, "P": 0, "T": 0, "gapB": 0, "gapN": 0, "ovlB": 0, "ovlN": 0, "C": 0,
           "split_g": 0, "nm_g": 0, "cdefs": 0}
    pd, cf = {}, {}
    for a in sorted(bins):
        b = bins[a]
        r = analyze(b, global_refs, scans[a])
        results[a] = r
        line = "%s: functions: %d text bytes covered: %d of %d phantoms: %d truncations: %d" % (
            a, len(r["recs"]), r["covered"], r["B"], r["P"], r["T"])
        if b.text is None:
            line += " -- no text (raw bin, no base)"
        out.append(line)
        tot["F"] += len(r["recs"]); tot["b"] += r["covered"]; tot["B"] += r["B"]
        tot["P"] += r["P"]; tot["T"] += r["T"]
        tot["gapN"] += len(r["gaps"]); tot["gapB"] += sum(e - s for s, e in r["gaps"])
        tot["ovlN"] += len(r["ovl"]); tot["ovlB"] += sum(e - s for s, e in r["ovl"])
        tot["C"] += len(r["cross"])
        for k in ("split_g", "nm_g", "cdefs"):
            tot[k] += r[k]
        for k, v in r["pd"].items():
            pd[k] = pd.get(k, 0) + v
        for k, v in r["cf"].items():
            cf[k] = cf.get(k, 0) + v
    print("\n".join(out))
    if write:
        OUT.mkdir(parents=True, exist_ok=True)
        recs = sorted(x for r in results.values() for x in r["recs"])
        (OUT / "functions.tsv").write_text(
            "# alias\tstart\tend\tsize\tkind\tfamily -- tools/census.py; end exclusive\n" + "".join(
                "%s\t0x%08x\t0x%08x\t0x%x\t%s\t%s\n" % (a, s, e, e - s, k, f) for a, s, e, k, f in recs))
    print("functions: %d" % tot["F"])
    print("text bytes covered: %d of %d" % (tot["b"], tot["B"]))
    print("binaries: %d of %d" % (len(bins), FLEET_N))
    print("phantoms: %d" % tot["P"])
    print("phantom detail: %s; confirmed by %s" % (", ".join("%s %d" % (k, pd.get(k, 0)) for k in (
        "no-insn", "outside-text", "in-jtbl/data", "unconfirmed")), ", ".join("%s %d" % (k, cf.get(k, 0)) for k in (
        "ref", "fallthrough", "lib-start", "c-def"))))
    print("truncations: %d" % tot["T"])
    print("truncation detail: gaps %d B (%d spans), overlaps %d B, crossings %d" % (
        tot["gapB"], tot["gapN"], tot["ovlB"], tot["C"]))
    ex = [p for r in results.values() for p in r["pex"]][:10]
    if ex:
        print("phantom examples: " + "; ".join(ex))
    tex = []
    for a in sorted(results):
        r = results[a]
        tex += ["%s gap 0x%08x..0x%08x" % (a, s, e) for s, e in r["gaps"]]
        tex += ["%s overlap 0x%08x..0x%08x" % (a, s, e) for s, e in r["ovl"]]
        tex += ["%s cross 0x%08x@0x%08x" % (a, s, x) for s, x in r["cross"]]
    if tex:
        print("truncation examples: " + "; ".join(tex[:10]))
    if only_full:
        print("cross-check: glabels %d (cc_fingerprint split %d + nonmatchings %d) + C-defined %d = %d, census %d: %s"
              % (tot["split_g"] + tot["nm_g"], tot["split_g"], tot["nm_g"], tot["cdefs"],
                 tot["split_g"] + tot["nm_g"] + tot["cdefs"], tot["F"],
                 "ok" if tot["split_g"] + tot["nm_g"] + tot["cdefs"] == tot["F"] else "MISMATCH"))
    return tot, results


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
    n = 0
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
        ok &= hit
        print("control probe %s %s [0x%08x,0x%08x): %s" % (name, alias, s, e, "ok" if hit else "FAIL"))
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
    lines = ['.include "macro.inc"', "", ".section .text, \"ax\"", ""]
    v = FX_BASE
    for fn, ws in FX_FUNCS:
        drop = kind == "gap" and fn == "C"
        if not drop:
            lines.append("glabel func_%08X" % v)
        for i, w in enumerate(ws):
            if kind == "phantom" and fn == "B" and i == 3:
                lines.append("glabel func_%08X" % v)  # planted mid-body label, no oracle confirms it
            if not drop:
                word, mn, ops = W[w]
                lines.append("    /* %X %08X %s */  %-10s %s" % (v - FX_BASE, v, word, mn, ops))
            v += 4
        if not drop:
            lines.append("endlabel func_%08X" % (v - 4 * len(ws)))
    (asm / "fx_0.s").write_text("\n".join(lines) + "\n")
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
    ap.add_argument("--fixture", choices=("phantom", "gap"))
    a = ap.parse_args(argv)
    if a.fixture:
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
    _, tails_in = odd_tails(bins)
    cok, nprobe = controls(bins, results)
    if full:
        cok &= nprobe == 5
    good = cok and tot["P"] == 0 and tot["T"] == 0 and tails_in == 0
    if a.check:
        fp = run_fixture("phantom")
        neg_p = fp["P"] == 1
        print("negative control phantom: %s (phantoms %d, expect 1)" % ("ok" if neg_p else "FAIL", fp["P"]))
        fg = run_fixture("gap")
        neg_g = fg["T"] >= 1
        print("negative control gap: %s (truncations %d, expect >= 1)" % ("ok" if neg_g else "FAIL", fg["T"]))
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
