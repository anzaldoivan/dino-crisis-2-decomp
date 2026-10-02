#!/usr/bin/env python3
"""types_check.py — canonical-types scanner (T2, Phase 1.6). Stdlib only.

Scans src/**/*.c (headers are not scanned: include/dc2.h is the canonical layer) for
  duplicate         a struct/union shape (`typedef struct ... Name;` or `struct Name {`) whose name is defined in
                    include/dc2.h, or whose name or normalised body (comments+whitespace stripped) appears in another unit;
  raw-address-cast  a cast to a pointer type applied to a nonzero integer literal (hex or decimal), ignoring comments,
                    string literals and INCLUDE_ASM lines; `#define` lines count.
Prints `units scanned: U`, `types: D duplicates, R raw address casts`, one `path:line kind name` per offender, then the
every-run control line (planted fixtures under .run/types_fixture/). rc 1 when D>0, R>0 or a control fails.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CANON = ROOT / "include" / "dc2.h"
FIXTURE = ROOT / ".run" / "types_fixture"

SHAPE = re.compile(r"\b(typedef\s+)?(struct|union)\s*(\w+)?\s*\{")
CAST = re.compile(
    r"\(\s*((?:(?:const|volatile|unsigned|signed|struct|union)\s+)*\w+(?:\s+\w+)?\s*\*[\s*]*)\)"
    r"\s*\(?\s*(0[xX][0-9A-Fa-f]+|\d+)[uUlL]*")


def strip(text):
    """Blank comments, string/char literals and INCLUDE_ASM lines; keep newlines so line numbers hold."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append(re.sub(r"[^\n]", " ", text[i:j]))
            i = j
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i))
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            out.append(c + re.sub(r"[^\n]", " ", text[i + 1:j - 1]) + c if j - i >= 2 else text[i:j])
            i = j
        else:
            out.append(c)
            i += 1
    lines = "".join(out).split("\n")
    return "\n".join("" if re.match(r"\s*INCLUDE_ASM\b", ln) else ln for ln in lines)


def shapes(code):
    """Yield (line, name, normalised body) for each struct/union definition with a body."""
    for m in SHAPE.finditer(code):
        start = m.end() - 1
        depth, j = 0, start
        while j < len(code):
            depth += {"{": 1, "}": -1}.get(code[j], 0)
            if depth == 0:
                break
            j += 1
        body = re.sub(r"\s+", "", code[start:j + 1])
        name = m.group(3)
        if m.group(1):
            t = re.match(r"\s*(\w+)\s*;", code[j + 1:])
            name = t.group(1) if t else name
        if name:
            yield code.count("\n", 0, m.start()) + 1, name, body


def raw_casts(code):
    for m in CAST.finditer(code):
        lit = m.group(2)
        if int(lit, 16 if lit[:2].lower() == "0x" else 10) != 0:
            yield code.count("\n", 0, m.start()) + 1, re.sub(r"\s+", "", m.group(0))


def scan(src_root, rel_to):
    canon = {name for _, name, _ in shapes(strip(CANON.read_text()))} if CANON.exists() else set()
    units = sorted(src_root.rglob("*.c"))
    defs, offenders, raws = [], [], 0
    for u in units:
        code = strip(u.read_text(errors="replace"))
        rel = u.relative_to(rel_to).as_posix()
        defs += [(rel, ln, name, body) for ln, name, body in shapes(code)]
        for ln, cast in raw_casts(code):
            offenders.append((rel, ln, "raw-address-cast", cast))
            raws += 1
    dups = 0
    for rel, ln, name, body in defs:
        other = [d for d in defs if d[0] != rel and (d[2] == name or d[3] == body)]
        if name in canon or other:
            offenders.append((rel, ln, "duplicate", name))
            dups += 1
    offenders.sort()
    return len(units), dups, raws, offenders


def controls():
    dup_root, raw_root = FIXTURE / "dup" / "src", FIXTURE / "raw" / "src"
    for d in (dup_root, raw_root):
        d.mkdir(parents=True, exist_ok=True)
        for f in d.glob("*.c"):
            f.unlink()
    (dup_root / "plant_dup.c").write_text(
        '#include "dc2.h"\n\ntypedef struct {\n    char pad0[0x98];\n    int x98;\n} Obj;\n')
    (raw_root / "plant_raw.c").write_text(
        '#include "common.h"\n\n/* *(int *)0x1F800000 = 0; */\n'
        'INCLUDE_ASM("asm/x/nonmatchings/y", func_1F800000); /* (int *)0x1F800000 */\n\n'
        'void f(void)\n{\n    *(int *)0x1F800000 = 0;\n}\n')
    _, d, _, _ = scan(dup_root, FIXTURE)
    _, _, r, _ = scan(raw_root, FIXTURE)
    return d == 1, r == 1


def main():
    units, dups, raws, offenders = scan(ROOT / "src", ROOT)
    print(f"units scanned: {units}")
    print(f"types: {dups} duplicates, {raws} raw address casts")
    for rel, ln, kind, name in offenders:
        print(f"{rel}:{ln} {kind} {name}")
    dup_ok, raw_ok = controls()
    if dup_ok and raw_ok:
        print("control: planted duplicate refused, planted raw cast refused")
    else:
        print(f"control: FAIL duplicate={'ok' if dup_ok else 'missed'} raw cast={'ok' if raw_ok else 'missed'}")
    return 1 if dups or raws or not (dup_ok and raw_ok) else 0


if __name__ == "__main__":
    sys.exit(main())
