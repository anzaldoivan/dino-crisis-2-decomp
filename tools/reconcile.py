#!/usr/bin/env python3
"""tools/reconcile.py -- reconcile ladder: bank a standalone draft into its real unit without a redraft (T5, Phase 1.6).

  reconcile.py <draft.c> --target <alias>:<start> [--apply]
  reconcile.py --check

Draft = preamble (#include/#define/typedef/struct/extern/prototype items) + exactly one fn definition. The draft
body is never edited. Rungs are cumulative, each followed by a scratch build+hash attempt (propagate.scratch_build
under .run/reconcile/scratch/<alias>/: copied config/ src/ Makefile, fresh asm/ build/, `make build ONLY=<alias>`,
build/<alias>.bin sha1 vs config/check.<alias>.sha); the verdict names the first green rung:
  as-is        preamble + fn at the fn's INCLUDE_ASM line; draft #includes the unit already has are dropped
  decl-sync    a draft extern/prototype the unit (or its headers) declares differently takes the unit's text
  callee-cast  a callee whose draft prototype decl-sync replaced is cast at its call sites to the draft's shape
               `((<ret> (*)(<args>))func_X)(...)`
  canon-sig    prototypes of fns declared in include/dc2.h are taken from dc2.h
  self-decl    draft typedefs / struct tags / #defines that dc2.h or the unit already define are removed
  carve        no INCLUDE_ASM line anywhere, or every rung above failed: `carve.py`-equivalent (carve.carve, alias
               start --end <census end>) gives the fn its own unit (common.h + the draft's preamble; then common.h +
               dc2.h + self-decl, T5.c32); --apply only (writes config/ and src/), restored on failure
  real-unit    --apply only: the unit is written in-tree, `make build ONLY=<alias>` + `sha1sum -c
               config/check.<alias>.sha`; failure restores the unit
Body hash (fn definition, whitespace-normalised, ladder-inserted casts stripped) before vs after every rung: any
difference = `failed <rung> body-changed`. Verdict (`banked <rung>` | `failed <rung> <reason>` | `no-verdict`) and a
copy of the draft in .run/reconcile/<alias>_<start>/ (+ ladder.log, make.<rung>.log). Then `reconcile: K of N banked without redraft`
and `directory gate: banked+failed+no-verdict = drafts: ok|FAIL` over every .run/reconcile/*/draft.c (a missing or
malformed verdict is FAIL), then plain `directory gate: ok` when it passed.
--check: control draft = func_80037E18's definition from src/slus_012_79/game_80037824.c + a planted
`extern short D_800AF11C;` (the unit says `extern int`), laddered against a scratch copy of that unit where
func_80037E18 is INCLUDE_ASM; prints `control: <verdict>` (must be `banked decl-sync`), then the gate; rc 1 on any
FAIL. Never writes src/. Runs where make builds (the amd64 container).
"""
import argparse
import hashlib
import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import banked  # noqa: E402  (strip, HEAD: the one C-body parser)
import carve  # noqa: E402  (carve(), build(): in-tree relink + sha1sum -c)
import propagate  # noqa: E402  (scratch_build, census_end, Fail)

OUT = ROOT / ".run/reconcile"
propagate.SCRATCH = OUT / "scratch"  # scratch_build looks the global up at call time
DC2 = ROOT / "include/dc2.h"
RUNGS = ("as-is", "decl-sync", "callee-cast", "canon-sig", "self-decl")
CONTROL = ("slus_012_79", 0x80037E18, "src/slus_012_79/game_80037824.c")
KW = {"if", "for", "while", "switch", "return", "sizeof"}


def norm(s):
    return re.sub(r"\s*([^\w\s])\s*", r"\1", " ".join(s.split()))


def fn_spans(text):
    """[(name, start, end)] of top-level fn definitions (banked.py's parser, end past the closing brace)."""
    s, out = banked.strip(text), []
    for m in banked.HEAD.finditer(s):
        if m.group(1) in KW:
            continue
        depth, i = 0, m.end() - 1
        while i < len(s):
            depth += {"{": 1, "}": -1}.get(s[i], 0)
            if depth == 0:
                break
            i += 1
        out.append((m.group(1), m.start(), i + 1))
    return out


def classify(t):
    """-> (kind, name) for one preamble item text."""
    s = norm(t)
    m = re.match(r'#\s*include\s*["<]([^">]+)[">]', s)
    if m:
        return "include", m.group(1)
    m = re.match(r"#\s*define (\w+)", s)
    if m:
        return "define", m.group(1)
    if s.startswith("#"):
        return "pp", None
    if s.startswith("typedef"):
        m = re.search(r"\(\*(\w+)\)", s) or re.search(r"(\w+)(?:\[[^\]]*\])*;$", s)
        return "typedef", m.group(1) if m else None
    m = re.match(r"(?:struct|union|enum) (\w+)\{", s)
    if m:
        return "tag", m.group(1)
    m = re.search(r"\(\*(\w+)\)", s)
    if m:
        return "var", m.group(1)
    m = re.search(r"(\w+)\(", s)
    if m:
        return "proto", m.group(1)
    m = re.search(r"(\w+)(?:\[[^\]]*\])*(?:=.*)?;$", s)
    return ("var", m.group(1)) if m else ("other", None)


def items(text):
    """Comment-stripped top-level items: [{'text','kind','name'}]; fn definitions must be cut out first."""
    s = re.sub(r"/\*.*?\*/|//[^\n]*", lambda m: re.sub(r"[^\n]", " ", m.group(0)), text, flags=re.S)
    out, i, n = [], 0, len(s)
    while i < n:
        if s[i].isspace():
            i += 1
            continue
        if s[i] == "#":
            j = i
            while j < n and (s[j] != "\n" or s[j - 1] == "\\"):
                j += 1
        else:
            j, depth = i, 0
            while j < n and not (s[j] == ";" and depth == 0):
                depth += {"{": 1, "(": 1, "}": -1, ")": -1}.get(s[j], 0)
                j += 1
            j += 1
        t = s[i:j].strip()
        out.append(dict(zip(("text", "kind", "name"), (t,) + classify(t))))
        i = j
    return out


def header_items(text, seen=None):
    """Items of the quoted #includes of text, resolved in include/, transitively."""
    seen = set() if seen is None else seen
    out = []
    for it in items(text):
        if it["kind"] == "include" and it["name"] not in seen and (ROOT / "include" / it["name"]).is_file():
            seen.add(it["name"])
            h = (ROOT / "include" / it["name"]).read_text()
            out += [dict(x, header=it["name"]) for x in items(h)] + header_items(h, seen)
    return out


def unit_decls(text):
    """All items of the unit (fn bodies cut) and its headers."""
    cut, last = [], 0
    for _, a, b in fn_spans(text):
        cut.append(text[last:a])
        last = b
    rest = "".join(cut) + text[last:]
    return items(rest) + header_items(rest)


def parse_draft(text):
    fns = fn_spans(text)
    if len(fns) != 1:
        raise propagate.Fail(f"draft holds {len(fns)} fn definitions, need exactly 1")
    name, a, b = fns[0]
    return name, items(text[:a]), text[a:b]


def body_hash(fndef, casts):
    for cast, name in casts:
        fndef = fndef.replace(cast + name + ")", name)
    return hashlib.sha1(norm(fndef).encode()).hexdigest()[:16]


def rung_state(rung, pre, fndef, unit_text):
    """Cumulative transform up to rung -> (preamble items, fndef, casts, change notes)."""
    decls = unit_decls(unit_text)
    have_inc = {d["name"] for d in decls if d["kind"] == "include"}
    by_name = {}
    for d in decls:
        if d["kind"] in ("var", "proto"):
            by_name.setdefault(d["name"], d)
    defined = {d["name"] for d in decls if d["kind"] in ("typedef", "tag", "define")}
    dc2 = {d["name"]: d for d in items(DC2.read_text()) if d["kind"] == "proto"}
    k = RUNGS.index(rung)
    cur, notes, casts, orig = [], [], [], {}
    for it in pre:
        if it["kind"] == "include" and it["name"] in have_inc:
            continue
        if k >= 1 and it["kind"] in ("var", "proto") and it["name"] in by_name \
                and norm(by_name[it["name"]]["text"]) != norm(it["text"]):
            notes.append(f"decl-sync {it['name']}")
            if it["kind"] == "proto":
                orig[it["name"]] = it["text"]
            it = by_name[it["name"]]
        if k >= 3 and it["kind"] == "proto" and it["name"] in dc2 and norm(dc2[it["name"]]["text"]) != norm(it["text"]):
            notes.append(f"canon-sig {it['name']}")
            it = dc2[it["name"]]
        if k >= 4 and it["kind"] in ("typedef", "tag", "define") and it["name"] in defined:
            notes.append(f"self-decl -{it['kind']} {it['name']}")
            continue
        cur.append(it)
    if k >= 2:
        for name, t in orig.items():
            m = re.match(r"\s*(?:extern\s+|static\s+)?(.*?)\b" + name + r"\s*\((.*)\)\s*;", t, re.S)
            if not m:
                continue
            cast = f"(({norm(m.group(1)) or 'int'} (*)({norm(m.group(2))}))"
            new = re.sub(r"\b" + name + r"\s*\(", cast + name + ")(", fndef)
            if new != fndef:
                fndef = new
                casts.append((cast, name))
                notes.append(f"callee-cast {name}")
    return cur, fndef, casts, notes


INC_RE = r'^INCLUDE_ASM\("[^"]*", {name}\);\n'


def compose(unit_text, name, pre, fndef):
    block = "".join(it["text"] + "\n" for it in pre) + ("\n" if pre else "") + fndef + "\n"
    m = re.search(INC_RE.format(name=name), unit_text, re.M)
    return unit_text[:m.start()] + block + unit_text[m.end():]


def placed_hash(text, name, casts):
    for n, a, b in fn_spans(text):
        if n == name:
            return body_hash(text[a:b], casts)
    return None


def find_unit(alias, name):
    for p in sorted((ROOT / "src" / alias).glob("*.c")):
        t = p.read_text()
        if re.search(INC_RE.format(name=name), t, re.M):
            return p, t
        if any(n == name for n, _, _ in fn_spans(t)):
            return p, None
    return None, None


def short(err):
    return "sha1-mismatch" if "make rc 0" in err else "build-error"


def ladder(draft, alias, start, apply, log, d, unit_override=None):
    """-> verdict string. unit_override = (path, text) replaces the real unit (control)."""
    name, pre, fndef = parse_draft(draft)
    if name != f"func_{start:08X}":
        return f"failed as-is draft-defines-{name}"
    before = body_hash(fndef, [])
    log(f"body hash before: {before}")
    path, text = unit_override or find_unit(alias, name)
    if path is not None and text is None:
        return "failed as-is already-banked"
    tried, green = {}, None
    if path is not None:
        rel = path.relative_to(ROOT)
        for rung in RUNGS:
            cur, fd, casts, notes = rung_state(rung, [dict(i) for i in pre], fndef, text)
            out = compose(text, name, cur, fd)
            h = placed_hash(out, name, casts)
            if h != before:
                log(f"rung {rung}: body hash {h} != {before}")
                return f"failed {rung} body-changed"
            if out in tried:
                log(f"rung {rung}: {', '.join(notes) or 'no change'}; build skipped: same unit text as {tried[out]}")
                continue
            tried[out] = rung
            try:
                try:
                    propagate.scratch_build(alias, rel, out)
                finally:
                    shutil.copy2(propagate.SCRATCH / alias / "make.log", d / f"make.{rung}.log")
                log(f"rung {rung}: {', '.join(notes) or 'no change'}; scratch build hash-equal")
                green = (rung, out)
                break
            except propagate.Fail as e:
                log(f"rung {rung}: {', '.join(notes) or 'no change'}; scratch FAIL {e}")
    if green is None:
        return carve_rung(alias, start, pre, fndef, name, before, apply, log)
    rung, out = green
    if apply:
        old = path.read_text()
        path.write_text(out)
        err = carve.build(alias)
        if err:
            path.write_text(old)
            log(f"rung real-unit: FAIL {err}; {rel} restored")
            return f"failed real-unit {'sha1-mismatch' if err.startswith('sha1') else 'build-error'}"
        log(f"rung real-unit: {rel} written; make build ONLY={alias} + sha1sum -c ok")
    return f"banked {rung}"


def carve_rung(alias, start, pre, fndef, name, before, apply, log):
    if not apply:
        log("rung carve: needs --apply (writes config/ and src/)")
        return "failed carve needs-apply"
    end = propagate.census_end(alias, start)
    import splat_gen as sg
    files = [sg.C_UNITS, ROOT / f"config/splat/{alias}.yaml", ROOT / f"config/check.{alias}.sha"]
    files += sorted((ROOT / "src" / alias).glob("*.c"))
    snap = {p: p.read_bytes() for p in files}
    try:
        unit, _, _, _ = carve.carve(alias, start, end, None)
    except carve.Refused as e:
        log(f"rung carve: refused {e}")
        return "failed carve refused"
    path = ROOT / f"src/{alias}/{unit}.c"
    base = path.read_text()
    # T5.c32: the draft's own preamble first (as plateau.py compiles it); dc2.h + self-decl only as the fallback,
    # since self-decl drops a draft typedef/tag that dc2.h defines differently (`structure has no member`).
    variants = [("draft-decls", base, "as-is"),
                ("dc2", base if '#include "dc2.h"' in base else
                 base.replace('#include "common.h"\n', '#include "common.h"\n#include "dc2.h"\n', 1), "self-decl")]
    err, tried, notes, tag = None, set(), [], None
    for tag, text, rung in variants:
        cur, fd, casts, notes = rung_state(rung, [dict(i) for i in pre], fndef, text)
        out = compose(text, name, cur, fd)
        if out in tried:
            continue
        tried.add(out)
        err = "body-changed" if placed_hash(out, name, casts) != before else None
        if err is None:
            path.write_text(out)
            err = carve.build(alias) or None
        if err is None:
            break
        log(f"rung carve ({tag}): FAIL {err}")
    if err:
        for p in set(snap) | {path} | set((ROOT / "src" / alias).glob("*.c")):
            if p in snap:
                p.write_bytes(snap[p])
            else:
                p.unlink(missing_ok=True)
        subprocess.run([sys.executable, "tools/splat_gen.py", "--force", "--only", alias], cwd=ROOT,
                       stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)
        log(f"rung carve: FAIL {err}; carve reverted")
        return "failed carve body-changed" if err == "body-changed" else f"failed carve {short(err)}"
    log(f"rung carve ({tag}): {path.relative_to(ROOT)} carved, {', '.join(notes) or 'no change'}; build + sha1 ok")
    return "banked carve"


def run(draft_text, alias, start, apply, override=None):
    d = OUT / f"{alias}_0x{start:08x}"
    shutil.rmtree(d, ignore_errors=True)
    d.mkdir(parents=True)
    (d / "draft.c").write_text(draft_text)
    (d / "verdict").write_text("no-verdict\n")
    lines = []

    def log(s):
        lines.append(s)
        print(f"  {s}")

    try:
        v = ladder(draft_text, alias, start, apply, log, d, override)
    except propagate.Fail as e:
        log(f"refused: {e}")
        v = "failed as-is " + re.sub(r"\W+", "-", str(e))[:60]
    (d / "ladder.log").write_text("\n".join(lines) + "\n")
    (d / "verdict").write_text(v + "\n")
    return v


def gate():
    V = re.compile(r"^(banked \S+|failed \S+ .+|no-verdict)$")
    dirs = sorted(p.parent for p in OUT.glob("*/draft.c"))
    b = f = nv = 0
    for d in dirs:
        v = (d / "verdict").read_text().strip() if (d / "verdict").is_file() else ""
        if not V.match(v):
            print(f"  {d.name}: verdict missing or malformed")
            continue
        b, f, nv = b + v.startswith("banked"), f + v.startswith("failed"), nv + (v == "no-verdict")
        print(f"  {d.name}: {v}")
    ok = len(dirs) >= 1 and b + f + nv == len(dirs)
    print(f"reconcile: {b} of {len(dirs)} banked without redraft")
    print(f"directory gate: banked+failed+no-verdict = drafts: {'ok' if ok else 'FAIL'}")
    if ok:
        print("directory gate: ok")  # plain verdict line for the scanner control regex
    return ok


def check():
    alias, start, rel = CONTROL
    name = f"func_{start:08X}"
    text = (ROOT / rel).read_text()
    span = next(((a, b) for n, a, b in fn_spans(text) if n == name), None)
    if span is None:
        print(f"control: FAIL {name} has no C body in {rel}")
        return 1
    a, b = span
    unit = text[:a] + f'INCLUDE_ASM("asm/{alias}/nonmatchings/{Path(rel).stem}", {name});' + text[b:]
    draft = ('#include "common.h"\n#include "dc2.h"\n\nextern short D_800AF11C; /* planted: the unit says int */\n\n'
             + text[a:b] + "\n")
    v = run(draft, alias, start, False, (ROOT / rel, unit))
    print(f"control: {v}")
    ok = gate()
    return 0 if ok and v == "banked decl-sync" else 1


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    ap.add_argument("draft", nargs="?")
    ap.add_argument("--target", help="alias:start, e.g. slus_012_79:0x80052634")
    ap.add_argument("--apply", action="store_true", help="write the unit in-tree when the ladder is green")
    ap.add_argument("--check", action="store_true", help="planted decl-sync control + directory gate")
    a = ap.parse_args()
    if a.check:
        return check()
    if not a.draft or not a.target or ":" not in a.target:
        ap.error("need <draft.c> --target <alias>:<start> (or --check)")
    alias, start = a.target.split(":")
    v = run(Path(a.draft).read_text(), alias, int(start, 16), a.apply)
    print(f"verdict {alias}:{start}: {v}")
    ok = gate()
    return 0 if ok and v.startswith("banked") else 1


if __name__ == "__main__":
    sys.exit(main())
