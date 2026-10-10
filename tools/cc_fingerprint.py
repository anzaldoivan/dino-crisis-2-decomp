#!/usr/bin/env python3
"""Compiler fingerprint survey over the split asm (asm/<alias>/**/*.s).

Per alias (config/loadmap.tsv class exe|code) counts ASPSX/-G tells in .text;
instructions inside any lib-object range of config/boundaries.tsv (end exclusive)
go to the separate lib control band. Writes .run/fingerprint/<alias>.tsv and
.run/fingerprint/summary.txt (counts only, deterministic). Stdlib only.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LOADMAP = ROOT / "config/loadmap.tsv"
BOUNDARIES = ROOT / "config/boundaries.tsv"
ASM = ROOT / "asm"
OUT = ROOT / ".run/fingerprint"

INSN = re.compile(r"^\s*/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s*\*/\s+([a-z][\w.]*)\s*(.*)$")
MEM = {"lb", "lbu", "lh", "lhu", "lw", "lwl", "lwr", "sb", "sh", "sw", "swl", "swr", "lwc2", "swc2"}
MULDIV = {"mult", "multu", "div", "divu"}
FP = ("$fp", "$s8", "$30")
KEYS = ("insns", "functions",
        "div_total", "break7_lo", "break7_hi", "break6_lo", "break6_hi", "tge",
        "mfx_gap0", "mfx_gap1", "mfx_gap2", "mfx_gap3plus", "mfx_no_next",
        "gp_mem", "gp_rel", "gp_addiu", "gp_sym_offset",
        "at_lui", "at_lui_mem", "at_lui_addu_mem", "at_mem",
        "li_lui_ori", "li_addiu", "li_ori", "li_pseudo", "la_lui_addiu",
        "o0_prologue")
GROUPS = (("kof_", "KOF"), ("wep_s", "WEP_S"), ("wep", "WEP"), ("res", "RES"))


def alias_of(path):  # mirrors tools/splat_gen.py:alias_of
    p = path.lower()
    if p.endswith(".bin"):
        p = p[:-4]
    return p.replace("/", "_").replace(".", "_")


def group_of(path, cls):
    if cls == "exe":
        return "EXE"
    base = Path(path).name.lower().removesuffix(".bin")
    for pre, g in GROUPS:
        if base.startswith(pre):
            return g
    if re.fullmatch(r"e[0-9a-f]{2}", base):
        return "E"
    if re.fullmatch(r"st\d", base):
        return "ST"
    return "MISC"


def rows(path):
    return [l.split("\t") for l in path.read_text().splitlines() if l and not l.startswith("#")]


def ops(rest):
    rest = rest.split("#")[0].strip()
    return [o.strip() for o in rest.split(",")] if rest else []


def base_reg(op):
    m = re.search(r"\((\$\w+)\)\s*$", op)
    return m.group(1) if m else None


def scan_func(insns, c):
    """insns: list of (mnemonic, operands) for one function in one band."""
    c["functions"] += 1
    n = len(insns)
    c["insns"] += n
    prologue = False
    for i, (mn, o) in enumerate(insns):
        nxt = insns[i + 1] if i + 1 < n else None
        nx2 = insns[i + 2] if i + 2 < n else None
        if mn in ("div", "divu"):
            c["div_total"] += 1
        elif mn == "break" and o:
            v = [x.replace(" ", "") for x in o]
            if v in (["0", "7"], ["0x0", "0x7"]):
                c["break7_lo"] += 1
            elif v in (["7"], ["0x7"], ["7", "0"]):
                c["break7_hi"] += 1
            elif v in (["0", "6"], ["0x0", "0x6"]):
                c["break6_lo"] += 1
            elif v in (["6"], ["0x6"], ["6", "0"]):
                c["break6_hi"] += 1
        elif mn == "tge":
            c["tge"] += 1
        elif mn in ("mflo", "mfhi"):
            for j in range(i + 1, n):
                if insns[j][0] in MULDIV:
                    d = j - i - 1
                    c["mfx_gap%d" % d if d < 3 else "mfx_gap3plus"] += 1
                    break
            else:
                c["mfx_no_next"] += 1
        # gp-relative
        if mn in MEM and len(o) >= 2:
            b = base_reg(o[1])
            if b == "$gp":
                c["gp_mem"] += 1
            elif b == "$at":
                c["at_mem"] += 1
        if any("%gp_rel" in x for x in o):
            c["gp_rel"] += 1
            if any(re.search(r"%gp_rel\([^)]*[+-]", x) for x in o):
                c["gp_sym_offset"] += 1
        if mn == "addiu" and len(o) == 3 and o[1] == "$gp" and o[0] != "$gp":
            c["gp_addiu"] += 1
        # $at expansions
        if mn == "lui" and o and o[0] == "$at":
            c["at_lui"] += 1
            if nxt and nxt[0] in MEM and len(nxt[1]) >= 2 and base_reg(nxt[1][1]) == "$at":
                c["at_lui_mem"] += 1
            elif (nxt and nx2 and nxt[0] == "addu" and nxt[1][:2] == ["$at", "$at"]
                  and nx2[0] in MEM and len(nx2[1]) >= 2 and base_reg(nx2[1][1]) == "$at"):
                c["at_lui_addu_mem"] += 1
        # li / la expansions
        if mn == "lui" and o and o[0] not in ("$at", "$gp") and nxt and len(nxt[1]) == 3 \
                and nxt[1][0] == o[0] and nxt[1][1] == o[0]:
            if nxt[0] == "ori":
                c["li_lui_ori"] += 1
            elif nxt[0] == "addiu" and "%lo" in nxt[1][2]:
                c["la_lui_addiu"] += 1
        if mn == "addiu" and len(o) == 3 and o[1] == "$zero":
            c["li_addiu"] += 1
        elif mn == "ori" and len(o) == 3 and o[1] == "$zero":
            c["li_ori"] += 1
        elif mn == "li":
            c["li_pseudo"] += 1
        # -O0 frame pointer: move $fp,$sp
        if not prologue and o and o[0] in FP and (
                (mn in ("addu", "or", "move") and len(o) >= 2 and o[1] == "$sp"
                 and (len(o) == 2 or o[2] == "$zero"))):
            prologue = True
    if prologue:
        c["o0_prologue"] += 1


def in_lib(v, ranges):
    return any(s <= v < e for s, e in ranges)


def scan_alias(alias, ranges, own, lib):
    if not (ASM / alias).is_dir():
        sys.exit("cc_fingerprint: no asm/%s (run make -j split)" % alias)
    for f in sorted((ASM / alias).rglob("*.s")):  # raw-bin-only aliases have none: zero row
        sect, func = None, []
        def flush():
            if func:
                inlib = in_lib(func[0][0], ranges)
                body = [(m, o) for v, m, o in func if in_lib(v, ranges) == inlib]
                scan_func(body, lib if inlib else own)
        for line in f.read_text(errors="replace").splitlines():
            s = line.strip()
            if s.startswith(".section"):
                flush(); func = []
                sect = s.split()[1].rstrip(",")
                continue
            if sect != ".text":
                continue
            if s.startswith("glabel "):
                flush(); func = []
                continue
            m = INSN.match(line)
            if m:
                func.append((int(m.group(1), 16), m.group(2), ops(m.group(3))))
        flush()


def fmt(c):
    return " ".join("%s=%d" % (k, c[k]) for k in KEYS)


def main():
    fleet = [(r[0], r[3]) for r in rows(LOADMAP) if len(r) > 3 and r[3] in ("exe", "code")]
    libr = {}
    for r in rows(BOUNDARIES):
        if r[1] == "lib-object":
            libr.setdefault(alias_of(r[0]), []).append((int(r[2], 16), int(r[3], 16)))
    OUT.mkdir(parents=True, exist_ok=True)
    lib = dict.fromkeys(KEYS, 0)
    groups = {}
    lines = []
    for path, cls in sorted(fleet, key=lambda x: alias_of(x[0])):
        a = alias_of(path)
        own = dict.fromkeys(KEYS, 0)
        scan_alias(a, sorted(libr.get(a, [])), own, lib)
        (OUT / ("%s.tsv" % a)).write_text("".join("%s\t%d\n" % (k, own[k]) for k in KEYS))
        g = groups.setdefault(group_of(path, cls), dict.fromkeys(KEYS, 0))
        for k in KEYS:
            g[k] += own[k]
        lines.append("%s %s" % (a, fmt(own)))
    out = lines + ["group %s members=%d %s" % (g, sum(1 for p, c in fleet if group_of(p, c) == g), fmt(groups[g]))
                   for g in sorted(groups)]
    out += ["lib control: ranges=%d %s" % (sum(len(v) for v in libr.values()), fmt(lib)),
            "aliases: %d" % len(fleet)]
    (OUT / "summary.txt").write_text("\n".join(out) + "\n")
    print("\n".join(lines))
    print(out[-2])
    print(out[-1])
    if len(fleet) != 83 or lib["insns"] == 0:
        sys.exit("cc_fingerprint: expected 83 aliases and a non-empty lib band")


if __name__ == "__main__":
    main()
