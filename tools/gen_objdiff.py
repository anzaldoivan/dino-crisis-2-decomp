#!/usr/bin/env python3
"""gen_objdiff.py (dc2 prototype, stdlib only; modelled on sozud/mmx4 tools/gen_objdiff.py).

Writes objdiff.json for the byte-matching build in the current directory (run after `make split build`).
Reads only tracked records (config/loadmap.tsv, config/splat/*.yaml, config/boundaries.tsv) plus build/ objects.

Unit  = one text subsegment (`c` or `asm`) of a config/splat/<alias>.yaml code segment -> one object file:
          c   -> build/<alias>/src/<alias>/<name>.c.o
          asm -> build/<alias>/asm/<alias>/<name>.s.o
target_path = that object (the build is byte-identical to the disc, so it IS the expected object).
base_path   = c unit: the same object (splat's macro.inc `nonmatching` already emits <func>.NON_MATCHING, so objdiff's
              MIPS reader drops INCLUDE_ASM functions on the base side); asm unit: build/empty.o.
build_base/build_target are false: objdiff-cli never runs make.
Category: lib-object extent (config/boundaries.tsv) -> ["sdk"]; exe game -> ["game","main"]; overlay ->
          ["game", P, P.child] (P = disc dir lower-cased, '/'->'_'; child = file stem lower-cased). Binaries with no
          text unit get no id (a 0-code category would show 100%).

  gen_objdiff.py [--out objdiff.json] [--categories-out config/categories.tsv]
"""
import argparse, json, os, re, subprocess, sys
from pathlib import Path


def rows(path):
    for line in Path(path).read_text().splitlines():
        if line and not line.startswith("#"):
            yield line.split("\t")


def parse_yaml(path):
    text = Path(path).read_text()
    tp = re.search(r"^\s+target_path:\s*extracted/retail/files/(\S+)", text, re.M).group(1)
    seg = re.search(r"^  - name: (\S+)\n    type: code\n    start: (0x[0-9a-f]+)\n    vram: (0x[0-9a-f]+)\n", text, re.M)
    if not seg:
        return tp, None, None, [], None
    start, vram = int(seg.group(2), 16), int(seg.group(3), 16)
    subs = [(int(m.group(1), 16), m.group(2), m.group(3)) for m in
            re.finditer(r"^      - \[(0x[0-9a-f]+), *([a-z.]+), *([A-Za-z0-9_]+)\]", text, re.M)]
    end = int(re.findall(r"^  - \[(0x[0-9a-f]+)\]\s*$", text, re.M)[-1], 16)
    return tp, start, vram, subs, end


def cat_ids(disc_path, exe):
    if disc_path == exe:
        return "main", None
    d, f = disc_path.rsplit("/", 1)
    return d.lower().replace("/", "_"), f.rsplit(".", 1)[0].lower()


def collect():
    lm = [r for r in rows("config/loadmap.tsv") if r[3] in ("exe", "code")]
    exe = next(r[0] for r in lm if r[3] == "exe")
    lib = {}
    for r in rows("config/boundaries.tsv"):
        if r[1] == "lib-object":
            lib.setdefault(r[0], []).append((int(r[2], 16), int(r[3], 16)))
    by_path = {}
    for y in sorted(Path("config/splat").glob("*.yaml")):
        tp, s, v, subs, e = parse_yaml(y)
        by_path[tp] = (y.stem, s, v, subs, e)
    units = []
    for r in lm:
        alias, s, v, subs, e = by_path[r[0]]
        if s is None:
            continue
        p, c = cat_ids(r[0], exe)
        game = ["game", "main"] if c is None else ["game", p, f"{p}.{c}"]
        for i, (off, typ, name) in enumerate(subs):
            if typ not in ("asm", "c"):
                continue
            nxt = subs[i + 1][0] if i + 1 < len(subs) else e
            vs, ve = v + off - s, v + nxt - s
            is_lib = any(a <= vs and ve <= b for a, b in lib.get(r[0], ()))
            if typ == "c":
                src, obj = f"src/{alias}/{name}.c", f"build/{alias}/src/{alias}/{name}.c.o"
            else:
                src, obj = f"asm/{alias}/{name}.s", f"build/{alias}/asm/{alias}/{name}.s.o"
            units.append(dict(name=f"{alias}/{name}", kind=typ, src=src, obj=obj,
                              cats=["sdk"] if is_lib else game))
    return exe, lm, units


def categories_tsv(exe, lm, units):
    used = {c for u in units for c in u["cats"]}
    rows_ = ["# config/categories.tsv -- DRAFT from gen_objdiff.py (dc2). id parent name status",
             "id\tparent\tname\tstatus",
             "game\t\tGame code (every non-SDK unit)\tactive", "sdk\t\tPsyQ SDK libraries\tactive",
             f"main\t\t{exe}\tactive"]
    parents = []
    for r in lm:
        p, c = cat_ids(r[0], exe)
        if c and f"{p}.{c}" in used and p not in parents:
            parents.append(p)
    for p in parents:
        rows_.append(f"{p}\t\t{p} overlays\tactive")
    for r in lm:
        p, c = cat_ids(r[0], exe)
        if c and f"{p}.{c}" in used:
            rows_.append(f"{p}.{c}\t{p}\t{r[0].rsplit('/', 1)[1]}\tactive")
    return rows_


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default="objdiff.json")
    ap.add_argument("--categories-out", default=None)
    a = ap.parse_args()
    exe, lm, units = collect()
    if not os.path.exists("build/empty.o"):
        subprocess.run(["mipsel-linux-gnu-as", "-EL", "-march=r3000", "-o", "build/empty.o", os.devnull], check=True)
    tsv = categories_tsv(exe, lm, units)
    if a.categories_out:
        Path(a.categories_out).write_text("\n".join(tsv) + "\n")
    cats = [r.split("\t") for r in tsv if not r.startswith(("#", "id\t"))]
    out, missing = [], []
    for u in sorted(units, key=lambda u: u["name"]):
        if not os.path.exists(u["obj"]):
            missing.append(u["obj"])
            continue
        out.append({"name": u["name"], "target_path": u["obj"],
                    "base_path": u["obj"] if u["kind"] == "c" else "build/empty.o",
                    "metadata": {"source_path": u["src"], "progress_categories": u["cats"]}})
    if missing:
        sys.exit("missing objects (did the build run?): %d, e.g. %s" % (len(missing), missing[:3]))
    cfg = {"build_base": False, "build_target": False, "units": out,
           "progress_categories": [{"id": c[0], "name": c[2]} for c in cats]}
    with open(a.out, "w") as f:
        json.dump(cfg, f, indent=2)
        f.write("\n")
    nasm = sum(1 for u in units if u["kind"] == "asm")
    print(f"{a.out}: {len(out)} units ({nasm} asm-only, {len(out)-nasm} C)")


main()
