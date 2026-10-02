#!/usr/bin/env python3
"""tools/propagate.py -- propagate a shared C body to the members of an exact duplicate family (T4, Phase 1.6).

  propagate.py --dry-run [--family F]   every family of config/families.tsv (or only F = exemplar `alias:start`)
  propagate.py --apply --family F       dry run of F; only if green, write each member unit (its fn's INCLUDE_ASM
                                        line replaced by the instantiation); rc 1 and nothing written otherwise
  propagate.py ... --plant              control mutation: the member's first func_ binding is rebound to the member
                                        fn itself (links, wrong jal) so the hash gate must FAIL; no control re-run
  propagate.py --register --family F    (T7.c27) turn the banked C exemplar F into the shared shape and write F's rows:
                                        body -> src/shared/func_<S>.inc.c (unit-local extern decls move in as block-scope
                                        externs on SHARED_<D|F><n> macros), unit-local types it needs -> guarded
                                        src/shared/func_<S>.h (renamed <T>_s<S>: member units hold other types), the
                                        exemplar unit keeps `#define` bindings + signature + `#include`; members = every
                                        kind=game fn of F's exact class in .run/census/dup.tsv; a member in no src unit
                                        is carved first (tools/carve.py, one span per alias asm piece, halved on refusal)

Registry config/families.tsv `# family role alias start unit basis`: one exemplar row per family (its unit holds the
instantiation: `#define SHARED_<X> <symbol>` lines, the signature line, `{`, `#include "../shared/func_<A>.inc.c"`,
`}`) and member rows; basis `exact:<sha1[:16] of the exact key>`.
Dry run gates the exemplar first: its unit as it stands (must `#include` src/shared/<fn>.inc.c) is scratch-built as
below and its build/<alias>.bin sha1-checked; N counts it. Then per member: exemplar and member words from the retail binary (dup_census.binary_words, ends from
.run/census/functions.tsv), masks by data-flow lui/lo pairing (dup_census.pairs, RAM range) as dup_census does for
C-defined fns; exact keys must equal each other and the basis (fail closed, C0036). Bindings: each exemplar symbol
owns the masked operands (J targets: exact match; lui/lo addresses: largest symbol <= address) and moves by the one
delta its member operands agree on. The member units of one alias are written together to a scratch tree
.run/propagate/<alias>/ (copies of config/ src/ Makefile, symlinks include/ and the extracts; fresh asm/ build/) and
`make build ONLY=<alias>` runs there once per alias (JOBS aliases in parallel); build/<alias>.bin sha1 must equal
config/check.<alias>.sha, else every member of that alias FAILs. Writes nothing outside .run/ (apply: the units).
Prints `member <alias>:<start>: hash-equal|FAIL <why>`, `propagation dry run: M of N members gated`, then (--dry-run
without --plant) `fail-closed control: ok|FAIL`: an `--apply --plant --family <first>` re-run must exit 1 on the hash
gate (`FAIL sha1`) with src/ config/ build/ tree-hash unchanged. rc 1 on any FAIL or control FAIL.
Runs where make builds (the amd64 container).
"""
import argparse
import hashlib
import os
import re
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import banked  # noqa: E402  (strip, HEAD, functions: the one C-body parser)
import dup_census as dc  # noqa: E402  (binary_words, pairs, exact_key, RAM)

FAMILIES = ROOT / "config/families.tsv"
CENSUS = ROOT / ".run/census/functions.tsv"
DUP = ROOT / ".run/census/dup.tsv"
SCRATCH = ROOT / ".run/propagate"
JOBS = 4  # parallel per-alias scratch builds (8-cpu container; each is one make)
DEFINE = re.compile(r"^#define (SHARED_\w+) ((\w*?_)([0-9A-Fa-f]{8}))\n", re.M)
SYM = re.compile(r"\b([A-Za-z]\w*?_)([0-9A-Fa-f]{8})\b")
KW = set("auto break case char const continue default do double else enum extern float for goto if int long register "
         "return short signed sizeof static struct switch typedef union unsigned void volatile while".split())


class Fail(Exception):
    pass


def families():
    out = {}
    for line in FAMILIES.read_text().splitlines():
        if line.startswith("#") or not line.strip():
            continue
        fam, role, alias, start, unit, basis = line.split("\t")
        out.setdefault(fam, []).append({"role": role, "alias": alias, "start": int(start, 16), "unit": unit,
                                        "basis": basis})
    return out


def census_end(alias, start):
    for line in CENSUS.read_text().splitlines():
        f = line.split("\t")
        if not line.startswith("#") and f[0] == alias and int(f[1], 16) == start:
            return int(f[2], 16)
    raise Fail(f"{alias}:0x{start:08x} not in {CENSUS.relative_to(ROOT)}")


def words_masks(alias, start):
    words = dc.binary_words(alias, dc.census.load_fleet()[alias].path, start, census_end(alias, start))
    masks = [False] * len(words)
    for lui, lo, ad in dc.pairs(words):
        if dc.RAM[0] <= ad < dc.RAM[1]:
            masks[lui] = masks[lo] = True
    return words, masks


def key_hash(key):
    return hashlib.sha1(b"".join(w.to_bytes(4, "little") for w in key)).hexdigest()[:16]


def operands(words, masks):
    """Masked operands -> [(kind, index, address)]: J targets and data-flow lui/lo addresses (by lo index)."""
    out = [("j", i, 0x80000000 | ((w & 0x3FFFFFF) << 2)) for i, w in enumerate(words[:dc.strip(words)])
           if w >> 26 in (2, 3)]
    return out + [("p", lo, ad) for lui, lo, ad in dc.pairs(words) if masks[lo]]


def bindings(ex_block, ew, em, mw, es, ms):
    """Exemplar defines + exemplar/member words (starts es, ms) -> {macro: member symbol}."""
    syms = [(m.group(1), m.group(3), int(m.group(4), 16)) for m in DEFINE.finditer(ex_block)]
    ops = []
    for kind, i, a in operands(ew, em):  # T7.c27: a J into the fn itself is the compiler's, not a symbol
        if kind == "j" and es <= a < es + 4 * len(ew):
            if 0x80000000 | ((mw[i] & 0x3FFFFFF) << 2) != ms + a - es:
                raise Fail(f"word {i}: member's own-body jump is not at the exemplar's offset")
            continue
        ops.append((kind, i, a))
    if not syms:
        if ops:
            raise Fail("exemplar unit has no #define SHARED_ lines")
        return {}  # a body with no relocated operand (e.g. `jr ra`) binds nothing
    deltas = {}
    for kind, i, a in ops:
        if kind == "j":
            own = [s for s in syms if s[2] == a]
            b = 0x80000000 | ((mw[i] & 0x3FFFFFF) << 2)
        else:
            own = sorted((s for s in syms if s[2] <= a), key=lambda s: s[2])[-1:]
            lui = next(lu for lu, lo, _ in dc.pairs(ew) if lo == i)
            b = dc.addr_of(mw, lui, i)
        if not own:
            raise Fail(f"exemplar operand word {i} -> 0x{a:08x} owned by no SHARED_ define")
        d = deltas.setdefault(own[0][0], b - a)
        if d != b - a:
            raise Fail(f"{own[0][0]}: operands disagree on the member delta")
    missing = [s[0] for s in syms if s[0] not in deltas]
    if missing:
        raise Fail(f"no operand binds {', '.join(missing)}")
    return {n: f"{p}{(a + deltas[n]) & 0xFFFFFFFF:08X}" for n, p, a in syms}


def block_re(start):
    return re.compile(r'((?:^#define SHARED_\w+ \w+\n)*)^([^\n]*\bfunc_%08X\b[^\n;]*)\n\{\n'
                      r'#include "\.\./shared/(func_[0-9A-F]{8})\.inc\.c"\n\}\n' % start, re.M)


def member_text(ex_text, ex, m, bind, plant, text):
    """-> (member unit path, `text` (its current text) with m's instantiation)."""
    em = block_re(ex["start"]).search(ex_text)
    if not em:
        raise Fail(f"exemplar unit holds no shared-body instantiation of func_{ex['start']:08X}")
    if plant:
        n = next((k for k, v in bind.items() if v.startswith("func_")), None)
        if n is None:
            raise Fail("plant: family binds no func_ symbol")
        bind = dict(bind, **{n: f"func_{m['start']:08X}"})
    block = "".join(f"#define {k} {v}\n" for k, v in bind.items())
    block += em.group(2).replace(f"func_{ex['start']:08X}", f"func_{m['start']:08X}") + "\n{\n"
    block += f'#include "../shared/{em.group(3)}.inc.c"\n}}\n'
    path = ROOT / f"src/{m['alias']}/{m['unit']}.c"
    # the exemplar's headers (dc2.h types) + the family's shared types header (T7.c27)
    hdrs = re.findall(r'^#include "(?:[^/"]+|\.\./shared/%s\.h)"\n' % em.group(3), ex_text, re.M)
    for h in hdrs:
        if h not in text:
            last = list(re.finditer(r'^#include "(?![^"]*\.inc\.c")[^"]+"\n', text, re.M))
            at = last[-1].end() if last else 0
            text = text[:at] + h + text[at:]
    inc = re.compile(r'^INCLUDE_ASM\("[^"]*", func_%08X\);\n' % m["start"], re.M)
    if inc.search(text):
        return path, inc.sub(lambda _: block, text, count=1)
    cur = block_re(m["start"]).search(text)
    if cur and not plant and cur.group(0) != block:
        raise Fail(f"{path.relative_to(ROOT)}: instantiation differs from the derived one")
    if cur:
        return path, text[:cur.start()] + block + text[cur.end():]
    raise Fail(f"{path.relative_to(ROOT)}: neither INCLUDE_ASM nor an instantiation of func_{m['start']:08X}")


def scratch_build(alias, texts):
    """Scratch tree with `texts` {rel path: text} written over src/, `make build ONLY=alias`, sha1 gate."""
    t = SCRATCH / alias
    shutil.rmtree(t, ignore_errors=True)
    t.mkdir(parents=True)
    shutil.copytree(ROOT / "config", t / "config")  # real dir: the yaml base_path ../.. must resolve to t
    shutil.copytree(ROOT / "src", t / "src")
    shutil.copy2(ROOT / "Makefile", t / "Makefile")
    (t / "include").symlink_to(os.path.relpath(ROOT / "include", t))
    for d in ("extracted", ".run/extracted"):
        if (ROOT / d).exists():
            (t / d).parent.mkdir(parents=True, exist_ok=True)
            (t / d).symlink_to(os.path.relpath(ROOT / d, (t / d).parent))
    for rel, text in texts.items():
        (t / rel).write_text(text)
    p = subprocess.run(["make", "build", f"ONLY={alias}"], cwd=t, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, text=True)
    (t / "make.log").write_text(p.stdout)
    want = (ROOT / f"config/check.{alias}.sha").read_text().split()[0]
    b = t / f"build/{alias}.bin"
    got = hashlib.sha1(b.read_bytes()).hexdigest() if b.is_file() else None
    if got != want:
        tail = " | ".join(p.stdout.strip().splitlines()[-2:])
        raise Fail(f"sha1 {got} != config/check.{alias}.sha {want} (make rc {p.returncode}: {tail}; "
                   f"{(t / 'make.log').relative_to(ROOT)})")


def dry_run(fam, rows, plant):
    """-> (fns gated, members + exemplar, {path: text} to apply)."""
    ex = next((r for r in rows if r["role"] == "exemplar"), None)
    members = [r for r in rows if r["role"] == "member"]
    ok, out = 0, {}
    if ex is not None:  # the exemplar is gated too: its unit as it stands, scratch-built and hash-checked
        try:
            rel = Path(f"src/{ex['alias']}/{ex['unit']}.c")
            em_ = block_re(ex["start"]).search((ROOT / rel).read_text())
            if not em_:
                raise Fail(f"{rel} does not #include a src/shared/ body for func_{ex['start']:08X}")
            if not (ROOT / f"src/shared/{em_.group(3)}.inc.c").is_file():
                raise Fail(f"src/shared/{em_.group(3)}.inc.c missing")
            scratch_build(ex["alias"], {rel: (ROOT / rel).read_text()})
            ok += 1
            print(f"member {ex['alias']}:0x{ex['start']:08x}: hash-equal", flush=True)
        except Fail as e:
            print(f"member {ex['alias']}:0x{ex['start']:08x}: FAIL {e}", flush=True)
    groups = {}  # alias -> ([members], {path: text}): one scratch build per alias (T7.c27)
    ew = em = None
    for m in members:
        try:
            if ex is None:
                raise Fail(f"family {fam} has no exemplar row")
            if ew is None:
                ew, em = words_masks(ex["alias"], ex["start"])
            mw, mm = words_masks(m["alias"], m["start"])
            ek, mk = dc.exact_key(ew, em, None), dc.exact_key(mw, mm, None)
            if ek != mk:
                raise Fail("exact key differs from the exemplar's")
            for r in (ex, m):
                if r["basis"] != f"exact:{key_hash(ek)}":
                    raise Fail(f"{r['alias']} basis {r['basis']} != exact:{key_hash(ek)}")
            ex_text = (ROOT / f"src/{ex['alias']}/{ex['unit']}.c").read_text()
            em_ = block_re(ex["start"]).search(ex_text)
            bind = bindings(em_.group(1) if em_ else "", ew, em, mw, ex["start"], m["start"])
            ms, texts = groups.setdefault(m["alias"], ([], {}))
            path = ROOT / f"src/{m['alias']}/{m['unit']}.c"
            path, text = member_text(ex_text, ex, m, bind, plant, texts.get(path) or path.read_text())
            texts[path] = text
            ms.append(m)
        except Fail as e:
            print(f"member {m['alias']}:0x{m['start']:08x}: FAIL {e}", flush=True)

    def gate(alias):
        try:
            scratch_build(alias, {p.relative_to(ROOT): t for p, t in groups[alias][1].items()})
            return None
        except Fail as e:
            return str(e)

    aliases = sorted(a for a, g in groups.items() if g[0])
    with ThreadPoolExecutor(JOBS) as pool:
        for alias, err in zip(aliases, pool.map(gate, aliases)):
            ms, texts = groups[alias]
            for m in ms:
                print(f"member {m['alias']}:0x{m['start']:08x}: " + (f"FAIL {err}" if err else "hash-equal"),
                      flush=True)
            if not err:
                ok += len(ms)
                out.update(texts)
    return ok, len(members) + (ex is not None), out


def items(text):
    """Top-level items of a C unit -> [(kind pp|fn|decl, a, b)] (offsets into text; comments excluded)."""
    s, out, i = banked.strip(text), [], 0
    while i < len(s):
        if s[i].isspace():
            i += 1
            continue
        if s[i] == "#":
            j = s.find("\n", i)
            j = len(s) if j < 0 else j + 1
            out.append(("pp", i, j))
            i = j
            continue
        m = banked.HEAD.match(s, i)
        brace = m is not None and m.group(1) not in ("if", "for", "while", "switch", "return", "sizeof")
        depth, k = 0, i
        while k < len(s):
            depth += {"{": 1, "(": 1, "}": -1, ")": -1}.get(s[k], 0)
            if depth == 0 and (s[k] == ";" or (brace and s[k] == "}")):
                break
            k += 1
        out.append(("fn" if brace else "decl", i, k + 1))
        i = k + 1
    return out


def declared(decl):
    """File-scope declaration -> (declared name or None, tags it defines, is a type declaration)."""
    flat = decl
    while re.search(r"\{[^{}]*\}", flat):
        flat = re.sub(r"\{[^{}]*\}", " ", flat)
    tags = re.findall(r"\b(?:struct|union|enum)\s+([A-Za-z_]\w*)\s*\{", decl)
    name = next((n for n in re.findall(r"\b([A-Za-z_]\w*)\s*[\[\(\);,=]", flat) if n not in KW), None)
    return name, tags, flat.lstrip().startswith("typedef") or (name is None and bool(tags))


def find_unit(alias, start):
    """-> (src unit path, 'asm'|'c') holding func_<start>, or None."""
    name = f"func_{start:08X}"
    for p in sorted((ROOT / f"src/{alias}").glob("*.c")):
        t = p.read_text()
        if re.search(r'^INCLUDE_ASM\("[^"]*", %s\);' % name, t, re.M):
            return p, "asm"
        if any(n == name for n, _ in banked.functions(t)):
            return p, "c"
    return None


def to_shared(fam, ex_alias, s):
    """Rewrite the exemplar's unit into the shared shape (idempotent); writes src/shared/func_<S>.{inc.c,h}."""
    found = find_unit(ex_alias, s)
    if not found or found[1] != "c":
        raise Fail(f"func_{s:08X} has no C body in src/{ex_alias}/")
    path, S = found[0], f"{s:08X}"
    text = path.read_text()
    if block_re(s).search(text):
        return path
    its = items(text)
    st = banked.strip(text)
    fn = next(it for it in its if it[0] == "fn" and banked.HEAD.match(st, it[1]).group(1) == f"func_{S}")
    ftext = text[fn[1]:fn[2]]
    decls = [(it, text[it[1]:it[2]]) + declared(text[it[1]:it[2]]) for it in its if it[0] == "decl"]
    need = set(re.findall(r"\b[A-Za-z_]\w*\b", banked.strip(ftext))) - {f"func_{S}"}
    kept, grew = [], True
    while grew:
        grew = False
        for d in decls:
            if d not in kept and d[2] != f"func_{S}" and ({d[2]} | set(d[3])) & need:
                kept.append(d)
                need |= set(re.findall(r"\b[A-Za-z_]\w*\b", banked.strip(d[1])))
                grew = True
    kept.sort(key=lambda d: d[0][1])
    types = [d for d in kept if d[4]]
    externs = [d for d in kept if not d[4]]
    for d in externs:
        if not re.match(r"\s*extern\b", d[1]) and not re.search(r"\b%s\s*\(" % d[2], d[1]):
            raise Fail(f"{path.relative_to(ROOT)}: `{d[2]}` is defined, not declared, at file scope")
    tnames = {n for d in types for n in [d[2]] + d[3] if n}
    rename = {n: f"{n}_s{S}" for n in tnames}
    lb, rb = ftext.index("{"), ftext.rindex("}")
    sig = " ".join(ftext[:lb].split())
    body = ftext[lb + 1:rb].strip("\n").rstrip()
    ext = "".join("    " + " ".join(d[1].split()) + "\n" for d in externs)
    syms, own = [], {f"func_{S}"} | tnames
    for m in SYM.finditer(banked.strip(ext + body)):
        if m.group(0) not in own and m.group(0) not in syms:
            syms.append(m.group(0))
    declared_syms = {d[2] for d in externs}
    macro, count = {}, {"F": 0, "D": 0}
    for sym in syms:
        if sym not in declared_syms and not sym.startswith("func_"):
            raise Fail(f"{path.relative_to(ROOT)}: {sym} is declared outside the unit (a header): cannot rebind")
        c = "F" if sym.startswith("func_") else "D"
        macro[sym] = f"SHARED_{c}{count[c]}"
        count[c] += 1

    def sub(t, table):
        return re.sub(r"\b(%s)\b" % "|".join(map(re.escape, table)), lambda m: table[m.group(1)], t) if table else t

    ext, body = sub(sub(ext, macro), rename), sub(sub(body, macro), rename)
    sig = sub(sig, rename)
    binds = "".join(f" *   {v:<11} exemplar {k}\n" for k, v in macro.items()) or " *   (none: no relocated operand)\n"
    inc = (f"/* func_{S}.inc.c -- shared C body of family {fam} (exact class, config/families.tsv; T7.c27\n"
           f" * propagate.py --register). Included inside a unit's function braces after its signature line, never\n"
           f" * compiled on its own (*.inc.c is not a unit). The unit binds, by one-line #define:\n{binds}"
           f" * tools/propagate.py derives a member's bindings from its relocated operands. */\n"
           + ext + (body + "\n" if body.strip() else "") + "".join(f"#undef {v}\n" for v in macro.values()))
    shared = ROOT / "src/shared"
    (shared / f"func_{S}.inc.c").write_text(inc)
    if types:
        (shared / f"func_{S}.h").write_text(
            f"/* func_{S}.h -- unit-local types of the shared body of family {fam} (T7.c27), renamed <T>_s{S} so\n"
            f" * a member unit's own types never collide; guarded: a unit may instantiate the family twice. */\n"
            f"#ifndef SHARED_H_{S}\n#define SHARED_H_{S}\n\n"
            + "\n".join(sub(d[1].strip(), rename) for d in types) + f"\n\n#endif\n")
    # the unit: drop moved decls no other item still names, then the instantiation in place of the definition
    rest = banked.strip("".join(text[it[1]:it[2]] for it in its if it != fn and all(it != d[0] for d in kept)))
    used = set(re.findall(r"\b[A-Za-z_]\w*\b", rest))
    cut = [d[0] for d in kept if not ({d[2]} | set(d[3])) & used]
    block = ("".join(f"#define {v} {k}\n" for k, v in macro.items()) + f"{sig}\n{{\n"
             f'#include "../shared/func_{S}.inc.c"\n}}')
    for a, b, new in sorted([(fn[1], fn[2], block)] + [(d[1], d[2], "") for d in cut], reverse=True):
        if not new:  # a removed decl takes its line ending (and one following blank line) with it
            b += len(re.match(r"[ \t]*\n?(?:[ \t]*\n)?", text[b:]).group(0))
        text = text[:a] + new + text[b:]
    if types:
        last = list(re.finditer(r'^#include "(?![^"]*\.inc\.c")[^"]+"\n', text, re.M))
        at = last[-1].end() if last else 0
        text = text[:at] + f'#include "../shared/func_{S}.h"\n' + text[at:]
    path.write_text(text)
    return path


def piece_of(alias, start):
    """-> (index, type) of the splat piece of config/splat/<alias>.yaml holding address start."""
    import splat_gen as sg
    row = next(r for r in sg.fleet()[0] if r["alias"] == alias)
    addr = (lambda o: o - sg.EXE_HDR + sg.EXE_BASE) if row["class"] == "exe" else (lambda o: o + row["base"])
    pieces = sg.parse((ROOT / f"config/splat/{alias}.yaml").read_text())[0]
    for k, p in enumerate(pieces):
        hi = addr(pieces[k + 1][0]) if k + 1 < len(pieces) else addr(row["size"])
        if addr(p[0]) <= start < hi:
            return k, p[1]
    return None, None


def carve_span(alias, ms):
    """Carve [first, last end) of ms (sorted starts) as one unit; halve on refusal. -> {start: why} not carved."""
    import carve
    lo, hi = ms[0], census_end(alias, ms[-1])
    try:
        unit, a, b, n = carve.carve(alias, lo, hi, None)
        print(f"carved {alias} {unit} [0x{a:08x}, 0x{b:08x}): {n} fns, {len(ms)} members", flush=True)
        return {}
    except carve.Refused as e:
        if len(ms) == 1:
            return {lo: f"carve refused: {e}"}
        print(f"carve {alias} [0x{lo:08x}, 0x{hi:08x}) refused, halving: {e}", flush=True)
        h = len(ms) // 2
        return {**carve_span(alias, ms[:h]), **carve_span(alias, ms[h:])}


def register(fam):
    """--register: shared shape for exemplar fam + families.tsv rows for every addressable kind=game class member."""
    ex_alias, s = fam.split(":")[0], int(fam.split(":")[1], 16)
    kind = {}
    for line in CENSUS.read_text().splitlines():
        f = line.split("\t")
        if not line.startswith("#"):
            kind[(f[0], int(f[1], 16))] = f[4]
    dup = [ln.split("\t") for ln in DUP.read_text().splitlines() if not ln.startswith("#")]
    row = next((r for r in dup if r[0] == ex_alias and int(r[1], 16) == s), None)
    if row is None or row[3] != "exact":
        print(f"register {fam}: tier {row[3] if row else 'absent'} in {DUP.relative_to(ROOT)}, not an exact class")
        return 1
    members = sorted((r[0], int(r[1], 16)) for r in dup if r[4] == row[4] and (r[0], int(r[1], 16)) != (ex_alias, s)
                     and kind.get((r[0], int(r[1], 16))) == "game")
    print(f"register {fam}: class {row[4]}, {int(row[5]) - 1} other copies, {len(members)} kind=game", flush=True)
    if not members:
        print(f"register {fam}: no kind=game member; not registered")
        return 1
    try:
        ex_path = to_shared(fam, ex_alias, s)
    except Fail as e:
        print(f"register {fam}: FAIL {e}")
        return 1
    skip, missing = {}, {}
    for a, m in members:
        u = find_unit(a, m)
        if u is None:
            missing.setdefault(a, []).append(m)
        elif u[1] == "c" and not block_re(m).search(u[0].read_text()):
            skip[(a, m)] = f"already C in {u[0].relative_to(ROOT)}"
    for a in sorted(missing):  # batch: one carve per (alias, asm piece), never one per member
        spans = {}
        for m in missing[a]:
            k, t = piece_of(a, m)
            if t != "asm":
                skip[(a, m)] = f"in a {t} piece, not asm"
            else:
                spans.setdefault(k, []).append(m)
        for k in sorted(spans):
            skip.update({(a, m): why for m, why in carve_span(a, sorted(spans[k])).items()})
    ew, em = words_masks(ex_alias, s)
    ek = dc.exact_key(ew, em, None)
    basis = f"exact:{key_hash(ek)}"
    rows = [f"{fam}\texemplar\t{ex_alias}\t0x{s:08x}\t{ex_path.stem}\t{basis}"]
    for a, m in members:
        if (a, m) not in skip:
            mw, mm = words_masks(a, m)
            u = find_unit(a, m)
            if dc.exact_key(mw, mm, None) != ek:
                skip[(a, m)] = "exact key differs from the exemplar's"
            elif u is None:
                skip[(a, m)] = "in no src unit after carving"
            else:
                rows.append(f"{fam}\tmember\t{a}\t0x{m:08x}\t{u[0].stem}\t{basis}")
    for (a, m), why in sorted(skip.items()):
        print(f"member {a}:0x{m:08x}: not registered: {why}")
    lines = [ln for ln in FAMILIES.read_text().splitlines() if ln.startswith("#") or ln.split("\t")[0] != fam]
    FAMILIES.write_text("\n".join(lines + rows) + "\n")
    print(f"registered {fam}: {len(rows) - 1} members ({len(skip)} not registered), basis {basis}", flush=True)
    return 0


def tree_hash():
    h = hashlib.sha1()
    for d in ("src", "config", "build"):
        for p in sorted((ROOT / d).rglob("*")):
            if p.is_file() and not p.is_symlink():
                h.update(str(p.relative_to(ROOT)).encode() + b"\0" + p.read_bytes())
    return h.hexdigest()


def control(fam):
    before = tree_hash()
    p = subprocess.run([sys.executable, str(Path(__file__).resolve()), "--apply", "--plant", "--family", fam],
                       cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    same = tree_hash() == before
    why = next((ln for ln in p.stdout.splitlines() if ": FAIL" in ln), p.stdout.strip()[-200:])
    print(f"control --apply --plant --family {fam}: rc {p.returncode}, tree {'unchanged' if same else 'CHANGED'}; "
          f"{why}")
    good = p.returncode == 1 and same and ": FAIL sha1 " in why  # the hash gate caught the plant
    print("fail-closed control: ok" if good else "fail-closed control: FAIL")
    return good


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    mode = ap.add_mutually_exclusive_group(required=True)
    mode.add_argument("--dry-run", action="store_true", help="gate every member (or --family F's); writes .run/ only")
    mode.add_argument("--apply", action="store_true", help="dry run of --family F, then write its member units")
    mode.add_argument("--register", action="store_true",
                      help="shared shape for banked exemplar F + its families.tsv rows (carves members first)")
    ap.add_argument("--family", help="exemplar alias:start, e.g. psx_bin_st6:0x800d5c40")
    ap.add_argument("--plant", action="store_true", help="control mutation (wrong callee binding); must FAIL")
    a = ap.parse_args()
    if (a.apply or a.register) and not a.family:
        ap.error("--apply/--register needs --family")
    if a.register:
        return register(a.family.lower())
    fams = families()
    if a.family and a.family not in fams:
        print(f"propagate: family {a.family} not in {FAMILIES.relative_to(ROOT)}")
        return 1
    ok = n = 0
    out = {}
    for fam in ([a.family] if a.family else sorted(fams)):
        k, m, o = dry_run(fam, fams[fam], a.plant)
        ok, n = ok + k, n + m
        out.update(o)
    print(f"propagation dry run: {ok} of {n} members gated")
    green = n >= 1 and ok == n
    if a.apply and green:
        for path, text in out.items():
            path.write_text(text)
            print(f"applied {path.relative_to(ROOT)}")
    if not a.plant and not a.apply:
        green = control(a.family or sorted(fams)[0]) and green
    return 0 if green else 1


if __name__ == "__main__":
    sys.exit(main())
