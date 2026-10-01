#!/usr/bin/env python3
"""tools/banked.py -- count functions banked as C in src/**/*.c (T7, Phase 1.4).

banked: a top-level function definition with a C body (INCLUDE_ASM lines are not functions).
verbatim: a banked function whose body holds `__asm__` / `asm(` (G11: inline asm is not a decompilation).
Prints `banked: <n>`, `verbatim: <m>`, then the names. Exit 0 iff verbatim == 0.
Usage: python3 tools/banked.py [--quiet]
"""
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
HEAD = re.compile(r"^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{", re.M)
ASM = re.compile(r"\b(__asm__|asm)\s*(volatile\s*)?\(")


def strip(src):
    """Blank comments and string/char literals, keeping offsets."""
    pat = re.compile(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'", re.S)
    return pat.sub(lambda m: re.sub(r"[^\n]", " ", m.group(0)), src)


def functions(src):
    """Yield (name, body) for each top-level function definition."""
    s = strip(src)
    for m in HEAD.finditer(s):
        if m.group(1) in ("if", "for", "while", "switch", "return", "sizeof"):
            continue
        depth, i = 0, m.end() - 1
        while i < len(s):
            depth += {"{": 1, "}": -1}.get(s[i], 0)
            if depth == 0:
                break
            i += 1
        yield m.group(1), s[m.end():i]


def main():
    quiet = "--quiet" in sys.argv[1:]
    banked, verbatim = [], []
    for path in sorted((ROOT / "src").rglob("*.c")):
        for name, body in functions(path.read_text()):
            banked.append(name)
            if ASM.search(body):
                verbatim.append(name)
    print(f"banked: {len(banked)}")
    print(f"verbatim: {len(verbatim)}")
    if not quiet:
        for n in banked:
            print(f"  {n}{'  VERBATIM' if n in verbatim else ''}")
    return 0 if not verbatim else 1


if __name__ == "__main__":
    sys.exit(main())
