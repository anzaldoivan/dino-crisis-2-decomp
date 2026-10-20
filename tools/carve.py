#!/usr/bin/env python3
"""tools/carve.py -- carve one C unit out of a split binary (T3, Phase 1.6).

  carve.py <alias> <start> [--end E] [--unit name]
      add or narrow the config/c_units.tsv row (alias, start); regenerate that alias only
      (`splat_gen.py --force --only <alias>`); write src/<alias>/<unit>.c (`#include "common.h"` + one INCLUDE_ASM
      per census fn in the unit's range, no stub bodies, C0028) or, when it exists, drop the INCLUDE_ASM lines
      outside the range; `make split` then `make build ONLY=<alias>` + `sha1sum -c config/check.<alias>.sha`.
      A new start inside an existing unit E (T5.c32): E keeps [E.lo, start), its fns at/after --end move to a
      continuation row game_<end> (E's text pruned to each range; a C body left in no unit is refused).
      Any failure restores c_units.tsv, the yaml, the sha and the units byte-exactly and exits 1.
      T10.c40: an --end whose last fn's census row runs past its asm `nonmatching func_X, 0xN` size (trailing
      alignment nops the C unit does not emit) ends the unit at the fn's last insn; the nops stay in the next asm piece.
  carve.py <alias> <start> --rodata
      T27 switch lane: add every jtbl (config/boundaries.tsv) whose `jr=` lies in the fn's census [start, end) to the
      `rodata` column (6th) of the c_units row whose unit holds the fn; splat_gen --only, `make build ONLY=<alias>` +
      sha1 (the fn stays INCLUDE_ASM: splat migrates the table into its .s). Refusal restores c_units/yaml/sha/unit
      byte-exactly, rc 1. carve(..., rodata=True) migrates the tables of the carved range into the new unit; every
      carve re-owns an alias's listed tables to the unit holding their jr.
  carve.py --check
      every c_units row: `splat_gen.py --check --only`, unit present, relink + sha1 → `carves: C of C build
      hash-equal`; control: a planted carve at a lib-object start (config/boundaries.tsv) must be refused with
      c_units.tsv, config/splat/ and src/ hashed unchanged → `control: ok`; rodata control (T27): slus_012_79
      0x8005ea34's table (jtbl 0x8001a0a0) migrated in a propagate.scratch_build tree, fn INCLUDE_ASM, hash-equal and
      the tree unchanged → `rodata control: ok`. rc 0 iff all rows and both controls pass.
Refusals: start not a census fn start; --end not a census fn end (or a fn straddles it); a C body
(banked.functions) outside the range. Census: .run/census/functions.tsv (end exclusive). Runs where make builds
(the amd64 container: `bash tools/docker/dc.sh run python3 tools/carve.py …`).
"""
import argparse
import contextlib
import hashlib
import io
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import banked  # noqa: E402  (functions(): the one C-body parser)
import splat_gen as sg  # noqa: E402

CENSUS = ROOT / ".run/census/functions.tsv"
LOGS = ROOT / ".run/carve"
INC = re.compile(r'^INCLUDE_ASM\("[^"]*", func_([0-9A-Fa-f]{8})\);\n(\n)?', re.M)
FUNC = re.compile(r"^func_([0-9A-Fa-f]{8})$")
RODATA_CONTROL = ("slus_012_79", 0x8005EA34, 0x8001A0A0)  # T27: unit game_8005E518, one 7-word table


class Refused(Exception):
    pass


def census(alias):
    fns = []
    for line in CENSUS.read_text().splitlines():
        if line.startswith("#"):
            continue
        f = line.split("\t")
        if f[0] == alias:
            fns.append((int(f[1], 16), int(f[2], 16)))
    return sorted(fns)


def asm_end(alias, start):
    """T10.c40: start + N from splat's `nonmatching func_<START>, 0xN` in asm/<alias> (the fn's last insn, without
    trailing alignment nops the census row may include), or None when no asm names it."""
    p = subprocess.run(["grep", "-rhoE", f"nonmatching func_{start:08X}, 0x[0-9A-Fa-f]+", f"asm/{alias}"],
                       cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, text=True)
    sizes = {int(x.rsplit(",", 1)[1], 16) for x in p.stdout.split("\n") if x}
    return start + sizes.pop() if len(sizes) == 1 else None


def run(cmd, log):
    LOGS.mkdir(parents=True, exist_ok=True)
    p = subprocess.run(cmd, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    (LOGS / log).write_text(p.stdout)
    return p.returncode, p.stdout.strip().splitlines()[-3:]


def build(alias):
    """Relink alias (resplit when its yaml changed) and sha1-check it; return '' or the failure."""
    (ROOT / f"build/{alias}.bin").unlink(missing_ok=True)
    # T5.c32: resplit in its own make first; in one `make build` the .bin prerequisites are the asm/ files listed at
    # parse time, which a resplit deletes -> `No rule to make target asm/...s` rc 2 after every carve.
    rc, tail = run(["make", "split", f"ONLY={alias}"], f"split_{alias}.log")
    if rc:
        return f"make split ONLY={alias} rc {rc}: {' | '.join(tail)}"
    rc, tail = run(["make", "build", f"ONLY={alias}"], f"make_{alias}.log")
    if rc:
        return f"make build ONLY={alias} rc {rc}: {' | '.join(tail)}"
    rc, tail = run(["sh", "-c", f"cd build && sha1sum -c ../config/check.{alias}.sha"], f"sha_{alias}.log")
    return f"sha1 {alias}: {' | '.join(tail)}" if rc else ""


def prune(text, lo, hi):
    """text without the INCLUDE_ASM lines and func_<ADDR> C bodies outside [lo, hi) (T5.c32: enclosing-unit split),
    then without the preamble items the kept text no longer references (T14.c3: drop_dead)."""
    s, cut = banked.strip(text), []
    for m in banked.HEAD.finditer(s):
        f = FUNC.match(m.group(1))
        if not f or lo <= int(f.group(1), 16) < hi:
            continue
        depth, i = 0, m.end() - 1
        while i < len(s):
            depth += {"{": 1, "}": -1}.get(s[i], 0)
            if depth == 0:
                break
            i += 1
        cut.append((m.start(), i + 1 + (s[i + 1:i + 2] == "\n")))
    for a, b in reversed(cut):
        text = text[:a] + text[b:]
    return drop_dead(INC.sub(lambda m: m.group(0) if lo <= int(m.group(1), 16) < hi else "", text))


ITEM = re.compile(r"^(#define|typedef|struct|union|extern)\b", re.M)
FPTR = re.compile(r"\(\s*\*\s*([A-Za-z_]\w*)\s*\)")


def last_ident(x):
    """Declarator name of a `T *name[N]` fragment: its last identifier outside brackets."""
    ids = re.findall(r"[A-Za-z_]\w*", re.sub(r"\[[^\]]*\]", "", x))
    return ids[-1] if ids else None


def preamble(s):
    """Top-level preamble items of stripped text s -> [(a, b, names)] (T14.c3): `#define` lines (with `\\`
    continuations); typedefs (shapes, fn pointers, aliases); pure struct/union shapes (`{...};` only: a declarator after
    `}` defines storage); extern decls without an initializer. Any other top-level text is not an item."""
    out, i = [], 0
    for m in ITEM.finditer(s):
        a, kind = m.start(), m.group(1)
        if a < i or s.count("{", 0, a) != s.count("}", 0, a):
            continue
        if kind == "#define":
            e = a
            while True:
                n = s.find("\n", e)
                line, e = (s[e:], len(s)) if n < 0 else (s[e:n], n + 1)
                if not line.rstrip().endswith("\\") or e >= len(s):
                    break
            nm = re.match(r"#define\s+([A-Za-z_]\w*)", s[a:])
            if nm:
                out.append((a, e, {nm.group(1)}))
            i = e
            continue
        d, j = 0, a
        while j < len(s) and not (s[j] == ";" and d == 0):
            d += {"{": 1, "(": 1, "[": 1, "}": -1, ")": -1, "]": -1}.get(s[j], 0)
            j += 1
        if j >= len(s):
            break
        b, body, names = j + 1 + (s[j + 1:j + 2] == "\n"), s[a:j], set()
        i = b
        if kind in ("struct", "union"):
            t = re.match(r"(?:struct|union)\s+([A-Za-z_]\w*)\s*\{", body)
            if t and body.rstrip().endswith("}"):
                names.add(t.group(1))
        elif kind == "typedef":
            t = re.match(r"typedef\s+(?:struct|union|enum)\s+([A-Za-z_]\w*)\s*\{", body)
            if t:
                names.add(t.group(1))
            tail = body[body.rfind("}") + 1:] if "}" in body else body[len("typedef"):]
            fp = FPTR.search(tail)
            names |= {fp.group(1)} if fp else {last_ident(x) for x in tail.split(",")} - {None}
        elif "=" not in body and "{" not in body:  # extern
            fp = FPTR.search(body)
            if fp:
                names.add(fp.group(1))
            elif "(" in body:
                names.add(last_ident(body.split("(")[0]))
            else:
                names |= {last_ident(x) for x in body[len("extern"):].split(",")} - {None}
        names.discard(None)
        if names:
            out.append((a, b, names))
    return out



def included(text, seen=None):
    """Stripped text of every quoted #include of text, recursively (resolved against src/<alias>/, then include/);
    None when one does not resolve."""
    seen = set() if seen is None else seen
    out = []
    for inc in re.findall(r'^\s*#\s*include\s+"([^"]+)"', text, re.M):
        p = next((q.resolve() for q in (ROOT / "src/_" / inc, ROOT / "include" / inc) if q.is_file()), None)
        if p is None:
            return None
        if p in seen:
            continue
        seen.add(p)
        t = p.read_text(errors="replace")
        sub = included(t, seen)
        if sub is None:
            return None
        out += [banked.strip(t), sub]
    return " ".join(out)


def drop_dead(text):
    """T14.c3: text without preamble items none of whose names the rest of the text references (to a fixpoint), so a
    carve never copies a dead shape/define/extern into either half. Codegen-neutral: only unreferenced decls go; the
    quoted #includes count as referencing text (a shared body reads its unit's `#define SHARED_F0 …`), and an include
    that resolves under neither src/<alias>/ nor include/ leaves the text unchanged."""
    ext = included(text)
    if ext is None:
        return text
    while True:
        s = banked.strip(text)
        dead = [(a, b) for a, b, ns in preamble(s)
                if not any(re.search(r"\b%s\b" % re.escape(n), s[:a] + " " + s[b:] + ext) for n in ns)]
        if not dead:
            return text
        for a, b in reversed(dead):
            text = text[:a] + text[b:]


def norm_ends(lines, alias):
    """T5.c32: drop an alias row's `end` when it is another unit's start: the cut is implicit there, and
    `splat_gen --check` reads an end as a closing cut whose next piece is not C (c_unit `<unit> end` FAIL)."""
    starts = {int(f[1], 16) for f in (x.rstrip("\n").split("\t") for x in lines if not x.startswith("#"))
              if len(f) >= 3 and f[0] == alias}
    out = []
    for x in lines:
        f = x.rstrip("\n").split("\t")
        if not x.startswith("#") and len(f) >= 5 and f[0] == alias and f[4] and int(f[4], 16) in starts:
            f[4] = ""
            x = join(f)
        out.append(x)
    return out


def rows_text():
    return sg.C_UNITS.read_text().splitlines(keepends=True)


def join(f):
    """c_units row text from fields, trailing empty optional columns (end, rodata) dropped."""
    f = list(f)
    while len(f) > 4 and not f[-1]:
        f.pop()
    return "\t".join(f) + "\n"


def jtbls(alias):
    """T27: {table start: jr} of the jtbl rows of alias's binary (config/boundaries.tsv)."""
    path = next(r["path"] for r in sg.fleet()[0] if r["alias"] == alias)
    return {r["start"]: int(re.match(r"jr=(0x[0-9a-f]+)", r["basis"]).group(1), 16)
            for r in sg.boundaries().get(path, []) if r["kind"] == "jtbl"}


def tables(alias, lo, hi):
    """T27: sorted jtbl starts whose jr lies in [lo, hi)."""
    return sorted(s for s, jr in jtbls(alias).items() if lo <= jr < hi)


def reown(lines, alias, extra=()):
    """T27: lines with every listed rodata start of alias (+ extra) in the row whose unit holds its jr: the row with
    the greatest start <= jr whose end (if any) is > jr. -> (lines, {start: unit}); a start no row holds is dropped
    from every row and absent from the map."""
    jr = jtbls(alias)
    rows, want = [], set(extra)
    for i, x in enumerate(lines):
        f = (x.rstrip("\n").split("\t") + ["", ""])[:6]
        if not x.startswith("#") and f[0] == alias:
            rows.append((i, f))
            want |= {int(s, 16) for s in f[5].split(",") if s}
    own = {}
    for s in sorted(want):
        hold = [f for _, f in rows if int(f[1], 16) <= jr.get(s, -1) and (not f[4] or jr[s] < int(f[4], 16))]
        if hold:
            own[s] = max(hold, key=lambda f: int(f[1], 16))[2]
    for i, f in rows:
        old = f[5]
        f[5] = ",".join(f"0x{s:08x}" for s in sorted(s for s, u in own.items() if u == f[2]))
        if f[5] != old:
            lines[i] = join(f)
    return lines, own


def migrate_lines(alias, start):
    """T27: -> (c_units lines, unit, table starts) with the fn's tables owned by the unit holding it."""
    end = dict(census(alias)).get(start)
    if end is None:
        raise Refused(f"{alias}: 0x{start:08x} is not a census function start")
    starts = tables(alias, start, end)
    if not starts:
        raise Refused(f"{alias}: no jtbl with jr in [0x{start:08x}, 0x{end:08x})")
    lines, own = reown(rows_text(), alias, starts)
    units = {own.get(s) for s in starts}
    if len(units) != 1 or None in units:
        raise Refused(f"{alias}: 0x{start:08x} is in no C unit of config/c_units.tsv")
    return lines, units.pop(), starts


def migrate_texts(alias, start):
    """T27: ({rel: text} of config/c_units.tsv + config/splat/<alias>.yaml with the fn's tables migrated, unit,
    starts), generated under .run/carve/rodata/ (splat_gen in-process); the tree is not touched."""
    lines, unit, starts = migrate_lines(alias, start)
    d = LOGS / "rodata"
    d.mkdir(parents=True, exist_ok=True)
    cu, old = d / "c_units.tsv", sg.C_UNITS
    cu.write_text("".join(lines))
    sg.C_UNITS, out = cu, io.StringIO()
    try:
        with contextlib.redirect_stdout(out):
            sg.write(argparse.Namespace(only=alias, force=True, out_dir=str(d)))
    except SystemExit:
        raise Refused(f"splat_gen --only {alias}: {' | '.join(out.getvalue().strip().splitlines()[-3:])}")
    finally:
        sg.C_UNITS = old
    yaml = f"config/splat/{alias}.yaml"
    return {"config/c_units.tsv": cu.read_text(), yaml: (d / yaml).read_text()}, unit, starts


def migrate(alias, start):
    """T27: in-tree migration of the fn's tables into its unit, then split + build + sha1; refusal restores
    c_units/yaml/sha/unit byte-exactly. -> (unit, starts)."""
    lines, unit, starts = migrate_lines(alias, start)
    yaml, sha = ROOT / f"config/splat/{alias}.yaml", ROOT / f"config/check.{alias}.sha"
    snap = {p: (p.read_bytes() if p.exists() else None) for p in (sg.C_UNITS, yaml, sha, ROOT / f"src/{alias}/{unit}.c")}
    try:
        sg.C_UNITS.write_text("".join(lines))
        rc, tail = run([sys.executable, "tools/splat_gen.py", "--force", "--only", alias], f"splat_gen_{alias}.log")
        if rc:
            raise Refused(f"splat_gen --only {alias} rc {rc}: {' | '.join(tail)}")
        err = build(alias)
        if err:
            raise Refused(err)
    except BaseException:
        for p, b in snap.items():
            if b is None:
                p.unlink(missing_ok=True)
            else:
                p.write_bytes(b)
        raise
    return unit, starts


def carve(alias, start, end, unit, rodata=False):
    fns = census(alias)
    if not fns:
        raise Refused(f"{alias}: no census rows in {CENSUS.relative_to(ROOT)}")
    if start not in dict(fns):
        raise Refused(f"{alias}: 0x{start:08x} is not a census function start")
    if end is not None and (end not in {e for s, e in fns if s >= start} or any(s < end < e for s, e in fns)):
        raise Refused(f"{alias}: --end 0x{end:08x} is not a census function end")
    if end is not None:  # T10.c40: trailing alignment nops stay out of the C unit
        last = max(s for s, _ in fns if s < end)
        te = asm_end(alias, last)
        end = te if te and last < te < end else end
    yaml, sha = ROOT / f"config/splat/{alias}.yaml", ROOT / f"config/check.{alias}.sha"
    row = next(r for r in sg.fleet()[0] if r["alias"] == alias)
    addr = (lambda o: o - sg.EXE_HDR + sg.EXE_BASE) if row["class"] == "exe" else (lambda o: o + row["base"])

    def rng(u):  # [lo, hi) of unit u's c piece in the yaml as it stands, or None
        pieces = sg.parse(yaml.read_text())[0]
        k = next((i for i, p in enumerate(pieces) if p[1] == "c" and p[2] == u), None)
        return None if k is None else (addr(pieces[k][0]), addr(pieces[k + 1][0] if k + 1 < len(pieces) else row["size"]))

    lines, hit, encl = rows_text(), None, None
    for i, line in enumerate(lines):
        f = line.rstrip("\n").split("\t")
        if not line.startswith("#") and len(f) >= 3 and f[0] == alias and int(f[1], 16) == start:
            hit = i
    endf = "" if end is None else f"0x{end:08x}"
    if hit is None:
        unit = unit or f"game_{start:08X}"
        # T5.c32: start inside an existing unit E: E keeps [E.lo, start); E's fns at/after the closing cut go to a
        # continuation unit game_<end> (E's text, pruned) so no C body or INCLUDE_ASM points at a moved .s file.
        for i, line in enumerate(lines):
            f = (line.rstrip("\n").split("\t") + ["", "", ""])[:6]
            r = None if line.startswith("#") or f[0] != alias else rng(f[2])
            if r and r[0] < start < r[1]:
                encl = (f[2], r, f[4])
                if f[4]:
                    f[4] = f"0x{start:08x}"
                    lines[i] = join(f)
        cont = None
        if encl and end is not None and end < encl[1][1] and any(end <= s < encl[1][1] for s, _ in fns):
            cont = f"game_{end:08X}"
            lines.append(f"{alias}\t0x{end:08x}\t{cont}\tT5.c32 carve remainder of {encl[0]}"
                         + (f"\t{encl[2]}" if encl[2] else "") + "\n")
        lines.append(f"{alias}\t0x{start:08x}\t{unit}\tT3 carve" + (f"\t{endf}" if endf else "") + "\n")
    else:
        f = (lines[hit].rstrip("\n").split("\t") + ["", "", ""])[:6]
        f[2] = unit or f[2]
        unit, f[4] = f[2], endf
        lines[hit] = join(f)
    lines = norm_ends(lines, alias)
    # T27: listed tables follow their jr into the new rows; rodata=True adds the carved range's own tables
    lines = reown(lines, alias, tables(alias, start, end if end is not None else dict(fns)[start]) if rodata else ())[0]
    src = ROOT / f"src/{alias}/{unit}.c"
    esrc =ROOT / f"src/{alias}/{encl[0]}.c" if encl else None
    csrc = ROOT / f"src/{alias}/{cont}.c" if encl and cont else None
    snap = {p: (p.read_bytes() if p.exists() else None) for p in (sg.C_UNITS, yaml, sha, src, esrc, csrc) if p}
    if csrc is not None and snap[csrc] is not None:
        raise Refused(f"{csrc.relative_to(ROOT)} exists; continuation unit name taken")
    try:
        sg.C_UNITS.write_text("".join(lines))
        rc, tail = run([sys.executable, "tools/splat_gen.py", "--force", "--only", alias], f"splat_gen_{alias}.log")
        if rc:
            raise Refused(f"splat_gen --only {alias} rc {rc}: {' | '.join(tail)}")
        lo, hi = rng(unit)
        inside = [s for s, _ in fns if lo <= s < hi]
        if esrc is not None and snap[esrc] is not None:
            etext = snap[esrc].decode()
            kept = [(esrc, rng(encl[0]))] + ([(csrc, rng(cont))] if csrc else [])
            for name, _ in banked.functions(etext):
                m = FUNC.match(name)
                if m and not any(a <= int(m.group(1), 16) < b for _, (a, b) in kept):
                    raise Refused(f"{esrc.relative_to(ROOT)}: C body {name} would leave every unit")
            for p, (a, b) in kept:  # the continuation's .s files split under nonmatchings/<cont>/
                p.write_text(prune(etext, a, b).replace(f'/nonmatchings/{encl[0]}"', f'/nonmatchings/{p.stem}"'))
        if snap[src] is None:
            body = "".join(f'\nINCLUDE_ASM("asm/{alias}/nonmatchings/{unit}", func_{s:08X});\n' for s in inside)
            src.parent.mkdir(parents=True, exist_ok=True)
            src.write_text('#include "common.h"\n' + body)
        else:
            text = snap[src].decode()
            for name, _ in banked.functions(text):
                m = FUNC.match(name)
                if m and not lo <= int(m.group(1), 16) < hi:
                    raise Refused(f"{src.relative_to(ROOT)}: C body {name} outside [0x{lo:08x}, 0x{hi:08x})")
            src.write_text(INC.sub(lambda m: m.group(0) if lo <= int(m.group(1), 16) < hi else "", text))
        err = build(alias)
        if err:
            raise Refused(err)
    except BaseException:
        for p, b in snap.items():
            if b is None:
                p.unlink(missing_ok=True)
            else:
                p.write_bytes(b)
        raise
    return unit, lo, hi, len(inside)


def tree_hash():
    h = hashlib.sha1()
    for p in [sg.C_UNITS] + sorted((ROOT / "config/splat").rglob("*")) + sorted((ROOT / "src").rglob("*")):
        if p.is_file():
            h.update(str(p.relative_to(ROOT)).encode() + b"\0" + p.read_bytes())
    return h.hexdigest()


def check():
    rows = [r for rs in sg.c_units().values() for r in rs]
    built, ok = {}, 0
    for r in rows:
        a = r["alias"]
        if a not in built:
            rc, tail = run([sys.executable, "tools/splat_gen.py", "--check", "--only", a], f"check_{a}.log")
            built[a] = f"splat_gen --check --only {a}: {' | '.join(tail)}" if rc else build(a)
        err = built[a] or ("" if (ROOT / f"src/{a}/{r['unit']}.c").is_file() else f"missing src/{a}/{r['unit']}.c")
        print(f"{a} 0x{r['start']:08x} {r['unit']}: " + (f"FAIL {err}" if err else "hash-equal"))
        ok += not err
    print(f"carves: {ok} of {len(rows)} build hash-equal")
    fleet = {r["path"]: r["alias"] for r in sg.fleet()[0] if r["base"] is not None}
    lib = next(r for rs in sg.boundaries().values() for r in rs if r["kind"] == "lib-object" and r["binary"] in fleet)
    a, s = fleet[lib["binary"]], lib["start"]
    before = tree_hash()
    p = subprocess.run([sys.executable, "tools/carve.py", a, f"0x{s:08x}"], cwd=ROOT,
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    same = tree_hash() == before
    why = (p.stdout.strip().splitlines() or [""])[-1]
    print(f"control carve {a} 0x{s:08x} (lib-object {lib['basis']}): rc {p.returncode}, tree "
          + ("unchanged" if same else "CHANGED") + f"; {why}")
    ctl = p.returncode == 1 and same
    print("control: ok" if ctl else "control: FAIL")
    rctl = rodata_control()
    return 0 if rows and ok == len(rows) and ctl and rctl else 1


def rodata_control():
    """T27: migrate RODATA_CONTROL's table in a scratch tree (fn INCLUDE_ASM) -> hash-equal, tree unchanged."""
    import propagate
    propagate.SCRATCH = LOGS / "scratch"
    a, s, want = RODATA_CONTROL
    before, err = tree_hash(), ""
    try:
        texts, unit, starts = migrate_texts(a, s)
        src = (ROOT / f"src/{a}/{unit}.c").read_text()
        if starts != [want]:
            err = f"tables {','.join(f'0x{x:08x}' for x in starts)} != 0x{want:08x}"
        elif not re.search(rf'^INCLUDE_ASM\("[^"]*", func_{s:08X}\);', src, re.M):
            err = f"func_{s:08X} is not INCLUDE_ASM in src/{a}/{unit}.c"
        else:
            propagate.scratch_build(a, texts)
    except (Refused, propagate.Fail) as e:
        err = str(e)
    same = tree_hash() == before
    print(f"rodata control {a} 0x{s:08x} jtbl 0x{want:08x}: " + (f"FAIL {err}" if err else "scratch hash-equal")
          + ", tree " + ("unchanged" if same else "CHANGED"))
    ok = not err and same
    print("rodata control: ok" if ok else "rodata control: FAIL")
    return ok


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("alias", nargs="?")
    ap.add_argument("start", nargs="?", type=lambda x: int(x, 16))
    ap.add_argument("--end", type=lambda x: int(x, 16))
    ap.add_argument("--unit")
    ap.add_argument("--rodata", action="store_true", help="T27: migrate the fn's jump tables into its unit")
    ap.add_argument("--check", action="store_true")
    args = ap.parse_args()
    if args.check:
        sys.exit(check())
    if args.alias is None or args.start is None:
        ap.error("alias and start are required")
    if args.rodata:
        try:
            unit, starts = migrate(args.alias, args.start)
        except Refused as e:
            print(f"REFUSED {e}")
            sys.exit(1)
        print(f"migrated {args.alias} 0x{args.start:08x} rodata {','.join(f'0x{x:08x}' for x in starts)} -> {unit}, "
              "build hash-equal")
        return
    try:
        unit, lo, hi, n = carve(args.alias, args.start, args.end, args.unit)
    except Refused as e:
        print(f"REFUSED {e}")
        sys.exit(1)
    print(f"carved {args.alias} {unit} [0x{lo:08x}, 0x{hi:08x}): {n} fns, build hash-equal")


if __name__ == "__main__":
    main()
