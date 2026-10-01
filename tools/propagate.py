#!/usr/bin/env python3
"""tools/propagate.py -- propagate a shared C body to the members of an exact duplicate family (T4, Phase 1.6).

  propagate.py --dry-run [--family F]   every family of config/families.tsv (or only F = exemplar `alias:start`)
  propagate.py --apply --family F       dry run of F; only if green, write each member unit (its fn's INCLUDE_ASM
                                        line replaced by the instantiation); rc 1 and nothing written otherwise
  propagate.py ... --plant              control mutation: the member's first func_ binding is rebound to the member
                                        fn itself (links, wrong jal) so the hash gate must FAIL; no control re-run

Registry config/families.tsv `# family role alias start unit basis`: one exemplar row per family (its unit holds the
instantiation: `#define SHARED_<X> <symbol>` lines, the signature line, `{`, `#include "../shared/func_<A>.inc.c"`,
`}`) and member rows; basis `exact:<sha1[:16] of the exact key>`.
Dry run per member: exemplar and member words from the retail binary (dup_census.binary_words, ends from
.run/census/functions.tsv), masks by data-flow lui/lo pairing (dup_census.pairs, RAM range) as dup_census does for
C-defined fns; exact keys must equal each other and the basis (fail closed, C0036). Bindings: each exemplar symbol
owns the masked operands (J targets: exact match; lui/lo addresses: largest symbol <= address) and moves by the one
delta its member operands agree on. The member unit is written to a scratch tree .run/propagate/<alias>/ (copies of
config/ src/ Makefile, symlinks include/ and the extracts; fresh asm/ build/) and `make build ONLY=<alias>` runs
there; build/<alias>.bin sha1 must equal config/check.<alias>.sha. Writes nothing outside .run/ (apply: the unit).
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
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import dup_census as dc  # noqa: E402  (binary_words, pairs, exact_key, RAM)

FAMILIES = ROOT / "config/families.tsv"
CENSUS = ROOT / ".run/census/functions.tsv"
SCRATCH = ROOT / ".run/propagate"
DEFINE = re.compile(r"^#define (SHARED_\w+) ((\w*?_)([0-9A-Fa-f]{8}))\n", re.M)


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


def bindings(ex_block, ew, em, mw):
    """Exemplar defines + exemplar/member words -> {macro: member symbol}."""
    syms = [(m.group(1), m.group(3), int(m.group(4), 16)) for m in DEFINE.finditer(ex_block)]
    if not syms:
        raise Fail("exemplar unit has no #define SHARED_ lines")
    deltas = {}
    for kind, i, a in operands(ew, em):
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
    return re.compile(r'((?:^#define SHARED_\w+ \w+\n)+)^([^\n]*\bfunc_%08X\b[^\n;]*)\n\{\n'
                      r'#include "\.\./shared/(func_[0-9A-F]{8})\.inc\.c"\n\}\n' % start, re.M)


def member_text(ex_text, ex, m, bind, plant):
    em = block_re(ex["start"]).search(ex_text)
    if not em:
        raise Fail(f"exemplar unit holds no shared-body instantiation of func_{ex['start']:08X}")
    if plant:
        n = next(k for k, v in bind.items() if v.startswith("func_"))
        bind = dict(bind, **{n: f"func_{m['start']:08X}"})
    block = "".join(f"#define {k} {v}\n" for k, v in bind.items())
    block += em.group(2).replace(f"func_{ex['start']:08X}", f"func_{m['start']:08X}") + "\n{\n"
    block += f'#include "../shared/{em.group(3)}.inc.c"\n}}\n'
    path = ROOT / f"src/{m['alias']}/{m['unit']}.c"
    text = path.read_text()
    for h in re.findall(r'^#include "[^/"]+"\n', ex_text, re.M):  # the exemplar's headers (dc2.h types)
        if h not in text:
            last = list(re.finditer(r'^#include "[^"]+"\n', text, re.M))
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


def scratch_build(alias, rel, text):
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
    """-> (members gated, members, {path: text} to apply)."""
    ex = next((r for r in rows if r["role"] == "exemplar"), None)
    members = [r for r in rows if r["role"] == "member"]
    ok, out = 0, {}
    for m in members:
        try:
            if ex is None:
                raise Fail(f"family {fam} has no exemplar row")
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
            bind = bindings(em_.group(1) if em_ else "", ew, em, mw)
            path, text = member_text(ex_text, ex, m, bind, plant)
            scratch_build(m["alias"], path.relative_to(ROOT), text)
            out[path] = text
            ok += 1
            print(f"member {m['alias']}:0x{m['start']:08x}: hash-equal")
        except Fail as e:
            print(f"member {m['alias']}:0x{m['start']:08x}: FAIL {e}")
    return ok, len(members), out


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
    ap.add_argument("--family", help="exemplar alias:start, e.g. psx_bin_st6:0x800d5c40")
    ap.add_argument("--plant", action="store_true", help="control mutation (wrong callee binding); must FAIL")
    a = ap.parse_args()
    if a.apply and not a.family:
        ap.error("--apply needs --family")
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
