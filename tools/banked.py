#!/usr/bin/env python3
"""tools/banked.py -- count functions banked as C in src/**/*.c (T7, Phase 1.4).

banked: a top-level function definition with a C body (INCLUDE_ASM lines are not functions).
verbatim: a banked function whose body holds `__asm__` / `asm(` (G11: inline asm is not a decompilation), or any
function of a unit that #defines an include/gte.h macro name or #includes an asm header other than gte.h/dc2.h/common.h
(T10.c10: GTE ops only through gte.h's macros, names read from it).
Prints `banked: <n> of <G>` (G = census game functions of the fleet, tools/progress.py game_fleet(): census rerun
when .run/census/functions.tsv is missing or older than src/**/*.c, config/splat/*.yaml, build/*/split.stamp; none →
REFUSED rc 2), `verbatim: <m>`, the names, then `control: 1 of 2 ok` (T5, Phase 1.5: a fixture C file under
.run/banked_fixture/ with one func_<A> body + one INCLUDE_ASM func_<B>, against a 2-row in-memory game census; else
FAIL rc 1), then `gte control: ok` (a gte.h macro use passes, a unit #defining a gte.h name is refused).
Exit 0 iff verbatim == 0 and both controls hold.
Usage: python3 tools/banked.py [--quiet]
"""
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
HEAD = re.compile(r"^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{", re.M)
ASM = re.compile(r"\b(__asm__|asm)\s*(volatile\s*)?\(")
GTE_H = ROOT / "include/gte.h"  # T10.c10: the one place C may hold asm; its macro names are read from it
ASM_HEADERS = ("gte.h", "dc2.h", "common.h")  # the headers a draft or unit may take asm from
DEFINE = re.compile(r"^[ \t]*#[ \t]*define[ \t]+([A-Za-z_]\w*)", re.M)
INCLUDE = re.compile(r'^[ \t]*#[ \t]*include[ \t]*[<"]([^">]+)[">]', re.M)


def strip(src):
    """Blank comments and string/char literals, keeping offsets."""
    pat = re.compile(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'", re.S)
    return pat.sub(lambda m: re.sub(r"[^\n]", " ", m.group(0)), src)


def gte_names():
    """Macro names #defined in include/gte.h (empty when it is absent)."""
    return set(DEFINE.findall(GTE_H.read_text())) if GTE_H.is_file() else set()


def gte_refusal(src, base=None):
    """G11 (T10.c10): -> reason or None. Refused: src #defines a gte.h macro name itself, or #includes a header other
    than gte.h/dc2.h/common.h that holds asm (resolved against base, then include/)."""
    s = re.sub(r"/\*.*?\*/|//[^\n]*", " ", src, flags=re.S)
    own = sorted(set(DEFINE.findall(s)) & gte_names())
    if own:
        return f"defines gte.h name {own[0]}"
    for h in INCLUDE.findall(s):
        if pathlib.PurePath(h).name in ASM_HEADERS:
            continue
        for d in [pathlib.Path(base)] * (base is not None) + [ROOT / "include"]:
            f = d / h
            if f.is_file() and ASM.search(strip(f.read_text(errors="replace"))):
                return f"includes asm header {h}"
    return None


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
        text = path.read_text()
        unit_bad = gte_refusal(text, path.parent)  # T10.c10: a unit bypassing gte.h refuses all its bodies
        for name, body in functions(text):
            banked.append(name)
            if unit_bad or ASM.search(body):
                verbatim.append(name)
    import progress  # tools/ is on sys.path when run as a script; progress imports this module for functions()
    g = progress.game_fleet()
    if g is None:
        print("banked: REFUSED: census game functions unavailable (tools/census.py produced no functions.tsv)")
        return 2
    print(f"banked: {len(banked)} of {g}")
    print(f"verbatim: {len(verbatim)} of {len(banked)}")
    if not quiet:
        for n in banked:
            print(f"  {n}{'  VERBATIM' if n in verbatim else ''}")
    ok = control(progress) & gte_control()
    return 0 if not verbatim and ok else 1


def control(progress):
    """Known-true: one C body + one INCLUDE_ASM against a 2-row game census counts 1 of 2."""
    d = ROOT / ".run/banked_fixture"
    d.mkdir(parents=True, exist_ok=True)
    f = d / "fixture.c"
    f.write_text('#include "common.h"\n\nvoid func_80010000(void)\n{\n}\n\n'
                 'INCLUDE_ASM("asm/fixture/nonmatchings/fixture", func_80010010);\n')
    census = {0x80010000: "game", 0x80010010: "game"}
    hit = sum(1 for _, addr in progress.bodies(f) if census.get(addr) == "game")
    if (hit, len(census)) == (1, 2):
        print("control: 1 of 2 ok")
        return True
    print(f"control: FAIL {hit} of {len(census)} (want 1 of 2)")
    return False


def gte_control():
    """Known-true (T10.c10): a body using a gte.h macro passes; a unit #defining a gte.h name itself is refused."""
    names = sorted(gte_names())
    use = '#include "gte.h"\n\nvoid func_80010000(void)\n{\n    GTE_RTPS();\n}\n'
    bad = f"#define {names[0] if names else 'GTE_RTPS'}() 0\n" + use
    ok = bool(names) and gte_refusal(use) is None and gte_refusal(bad) is not None
    print(f"gte control: {'ok' if ok else 'FAIL'} ({len(names)} gte.h names)")
    return ok


if __name__ == "__main__":
    sys.exit(main())
