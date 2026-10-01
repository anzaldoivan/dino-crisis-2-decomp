#!/usr/bin/env python3
"""tools/carve.py -- carve one C unit out of a split binary (T3, Phase 1.6).

  carve.py <alias> <start> [--end E] [--unit name]
      add or narrow the config/c_units.tsv row (alias, start); regenerate that alias only
      (`splat_gen.py --force --only <alias>`); write src/<alias>/<unit>.c (`#include "common.h"` + one INCLUDE_ASM
      per census fn in the unit's range, no stub bodies, C0028) or, when it exists, drop the INCLUDE_ASM lines
      outside the range; `make build ONLY=<alias>` + `sha1sum -c config/check.<alias>.sha`. Any failure restores
      c_units.tsv, the yaml, the sha and the unit byte-exactly and exits 1.
  carve.py --check
      every c_units row: `splat_gen.py --check --only`, unit present, relink + sha1 → `carves: C of C build
      hash-equal`; control: a planted carve at a lib-object start (config/boundaries.tsv) must be refused with
      c_units.tsv, config/splat/ and src/ hashed unchanged → `control: ok`. rc 0 iff all rows and the control pass.
Refusals: start not a census fn start; --end not a census fn end (or a fn straddles it); a C body
(banked.functions) outside the range. Census: .run/census/functions.tsv (end exclusive). Runs where make builds
(the amd64 container: `bash tools/docker/dc.sh run python3 tools/carve.py …`).
"""
import argparse
import hashlib
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


def run(cmd, log):
    LOGS.mkdir(parents=True, exist_ok=True)
    p = subprocess.run(cmd, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    (LOGS / log).write_text(p.stdout)
    return p.returncode, p.stdout.strip().splitlines()[-3:]


def build(alias):
    """Relink alias (resplit when its yaml changed) and sha1-check it; return '' or the failure."""
    (ROOT / f"build/{alias}.bin").unlink(missing_ok=True)
    rc, tail = run(["make", "build", f"ONLY={alias}"], f"make_{alias}.log")
    if rc:
        return f"make build ONLY={alias} rc {rc}: {' | '.join(tail)}"
    rc, tail = run(["sh", "-c", f"cd build && sha1sum -c ../config/check.{alias}.sha"], f"sha_{alias}.log")
    return f"sha1 {alias}: {' | '.join(tail)}" if rc else ""


def rows_text():
    return sg.C_UNITS.read_text().splitlines(keepends=True)


def carve(alias, start, end, unit):
    fns = census(alias)
    if not fns:
        raise Refused(f"{alias}: no census rows in {CENSUS.relative_to(ROOT)}")
    if start not in dict(fns):
        raise Refused(f"{alias}: 0x{start:08x} is not a census function start")
    if end is not None and (end not in {e for s, e in fns if s >= start} or any(s < end < e for s, e in fns)):
        raise Refused(f"{alias}: --end 0x{end:08x} is not a census function end")
    lines, hit = rows_text(), None
    for i, line in enumerate(lines):
        f = line.rstrip("\n").split("\t")
        if not line.startswith("#") and len(f) >= 3 and f[0] == alias and int(f[1], 16) == start:
            hit = i
    endf = "" if end is None else f"0x{end:08x}"
    if hit is None:
        unit = unit or f"game_{start:08X}"
        lines.append(f"{alias}\t0x{start:08x}\t{unit}\tT3 carve" + (f"\t{endf}" if endf else "") + "\n")
    else:
        f = (lines[hit].rstrip("\n").split("\t") + ["", ""])[:5]
        f[2] = unit or f[2]
        unit, f[4] = f[2], endf
        lines[hit] = "\t".join(f if endf else f[:4]) + "\n"
    yaml, sha = ROOT / f"config/splat/{alias}.yaml", ROOT / f"config/check.{alias}.sha"
    src = ROOT / f"src/{alias}/{unit}.c"
    snap = {p: (p.read_bytes() if p.exists() else None) for p in (sg.C_UNITS, yaml, sha, src)}
    try:
        sg.C_UNITS.write_text("".join(lines))
        rc, tail = run([sys.executable, "tools/splat_gen.py", "--force", "--only", alias], f"splat_gen_{alias}.log")
        if rc:
            raise Refused(f"splat_gen --only {alias} rc {rc}: {' | '.join(tail)}")
        row = next(r for r in sg.fleet()[0] if r["alias"] == alias)
        pieces = sg.parse(yaml.read_text())[0]
        k = next(i for i, p in enumerate(pieces) if p[1] == "c" and p[2] == unit)
        addr = (lambda o: o - sg.EXE_HDR + sg.EXE_BASE) if row["class"] == "exe" else (lambda o: o + row["base"])
        lo = addr(pieces[k][0])
        hi = addr(pieces[k + 1][0] if k + 1 < len(pieces) else row["size"])
        inside = [s for s, _ in fns if lo <= s < hi]
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
    return 0 if rows and ok == len(rows) and ctl else 1


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("alias", nargs="?")
    ap.add_argument("start", nargs="?", type=lambda x: int(x, 16))
    ap.add_argument("--end", type=lambda x: int(x, 16))
    ap.add_argument("--unit")
    ap.add_argument("--check", action="store_true")
    args = ap.parse_args()
    if args.check:
        sys.exit(check())
    if args.alias is None or args.start is None:
        ap.error("alias and start are required")
    try:
        unit, lo, hi, n = carve(args.alias, args.start, args.end, args.unit)
    except Refused as e:
        print(f"REFUSED {e}")
        sys.exit(1)
    print(f"carved {args.alias} {unit} [0x{lo:08x}, 0x{hi:08x}): {n} fns, build hash-equal")


if __name__ == "__main__":
    main()
