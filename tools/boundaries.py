#!/usr/bin/env python3
"""tools/boundaries.py -- generate config/boundaries.tsv: split boundaries per executable (T7).

Rows `binary<TAB>kind<TAB>start<TAB>end<TAB>basis` (start/end `0x%08x`, end exclusive), sorted (binary, start, kind),
for SLUS_012.79 + every class=code row of config/loadmap.tsv whose base != `-`. Kinds:
  jtbl         computed-jump table, own MIPS scan: `jr rY` (rY != ra) back-sliced through `lw rY,lo(rZ)` / `addu` /
               `sll` / `lui`+`addiu`|`%lo` to the table base; extent = the `sltiu` bound when present, else the run
               of consecutive words pointing into the jr's function. basis = `jr=0x… sltiu|run`.
  lib-object   PsyQ object extents (exe only) from the cached psx_ldr signature dump (non-low-entropy matches).
  text-end     [base, text_end): exe = the binding T3 value (asserted equal to the scan); overlays = the scan
               (end of the last `jr ra` + delay slot).
  data-island  [text_end, end) where end = the loadmap end.
Addresses, sizes and names only; never bytes or disassembly (G12).

  PY tools/boundaries.py            write config/boundaries.tsv (from extracted/retail/files + the Ghidra caches)
  PY tools/boundaries.py --check    regenerate twice to .run/t7/ and cmp with the tracked file; exe jtbl vs the
                                    Ghidra switch dump (0 disagreements); control switch found; exe text-end
  PY tools/boundaries.py --ghidra   refresh the Ghidra caches (headless, read-only; refuses while :8080 listens)
Caches (addresses + names only): config/ghidra/SLUS_012.79.switch_tables.tsv, config/ghidra/SLUS_012.79.psyq_objects.tsv
"""
import os
import pathlib
import re
import struct
import subprocess
import sys

REPO = pathlib.Path(__file__).resolve().parent.parent
FILES = REPO / "extracted" / "retail" / "files"
LOADMAP = REPO / "config" / "loadmap.tsv"
OUT = REPO / "config" / "boundaries.tsv"
SCRATCH = REPO / ".run" / "t7"
EXE = "SLUS_012.79"
GH_SWITCH = REPO / "config" / "ghidra" / f"{EXE}.switch_tables.tsv"
GH_OBJS = REPO / "config" / "ghidra" / f"{EXE}.psyq_objects.tsv"
EXE_TEXT_END = 0x80085F74                       # binding, T3 (docs/memory-map.md#slus_01279)
CONTROL = (0x8003500C, 0x80018624, 0x800186F8)  # (jr, start, end): Ghidra switchdataD_80018624, 53 entries
COMMENT_EVERY = 50
JR_RA = 0x03E00008
SLICE = 48                                      # max instructions walked back from a jr / an sll


def u32(img, off):
    return struct.unpack_from("<I", img, off)[0]


def writes(w):
    """Destination GPR written by instruction word w, or None."""
    op, rs, rt, rd, fn = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, w & 63
    if op == 0:
        return None if fn in (0x08, 0x0C, 0x0D, 0x11, 0x13, 0x18, 0x19, 0x1A, 0x1B) else rd
    if op == 3:
        return 31
    if 0x08 <= op <= 0x0F or 0x20 <= op <= 0x26:
        return rt
    if op in (0x10, 0x12) and rs in (0, 2):     # mfc0 / mfc2 / cfc2
        return rt
    return None


class Image:
    def __init__(self, img, base):
        self.img, self.base, self.n = img, base, len(img) // 4

    def word(self, i):
        return u32(self.img, i * 4)

    def def_before(self, reg, i):
        for j in range(i - 1, max(-1, i - 1 - SLICE), -1):
            w = self.word(j)
            if w == JR_RA:
                return None
            if writes(w) == reg:
                return j
        return None

    def ev(self, reg, i, depth=0):
        """Symbolic value of reg just before instruction i: ('c',v) | ('x',src,j) | ('t',c,x) | ('tbl',addr,x)."""
        if reg == 0:
            return ("c", 0)
        if depth > 8:
            return None
        j = self.def_before(reg, i)
        if j is None:
            return None
        w = self.word(j)
        op, rs, rt, fn, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 63, w & 0xFFFF
        simm = imm - 0x10000 if imm & 0x8000 else imm
        if op == 0x0F:                                            # lui
            return ("c", imm << 16)
        if op == 0x09:                                            # addiu
            v = self.ev(rs, j, depth + 1)
            return ("c", (v[1] + simm) & 0xFFFFFFFF) if v and v[0] == "c" else None
        if op == 0x0D:                                            # ori
            v = self.ev(rs, j, depth + 1)
            return ("c", v[1] | imm) if v and v[0] == "c" else None
        if op == 0 and fn == 0x00 and ((w >> 6) & 31) == 2:       # sll rd, rt, 2
            return ("x", rt, j)
        if op == 0 and fn in (0x21, 0x25):                        # addu / or
            if rt == 0:
                return self.ev(rs, j, depth + 1)
            if rs == 0:
                return self.ev(rt, j, depth + 1)
            if fn != 0x21:
                return None
            a, b = self.ev(rs, j, depth + 1), self.ev(rt, j, depth + 1)
            for c, x in ((a, b), (b, a)):
                if c and x and c[0] == "c" and x[0] == "x":
                    return ("t", c[1], x)
            return None
        if op == 0x23:                                            # lw
            v = self.ev(rs, j, depth + 1)
            return ("tbl", (v[1] + simm) & 0xFFFFFFFF, v[2]) if v and v[0] == "t" else None
        return None

    def bound(self, x):
        _, src, j = x
        for k in range(j - 1, max(-1, j - 1 - SLICE), -1):
            w = self.word(k)
            if w == JR_RA:
                return None
            if w >> 26 == 0x0B and ((w >> 21) & 31) == src:      # sltiu t, src, N
                return w & 0xFFFF
        return None

    def func_range(self, i):
        lo = hi = None
        for j in range(i - 1, -1, -1):
            if self.word(j) == JR_RA:
                lo = j + 2
                break
        for j in range(i, self.n):
            if self.word(j) == JR_RA:
                hi = j + 2
                break
        lo = 0 if lo is None else lo
        hi = self.n if hi is None else hi
        return self.base + 4 * lo, self.base + 4 * hi

    def text_end(self):
        """After the last `jr ra` + delay slot; a `jr ra` adjacent to another (illegal in a delay slot) is data."""
        for i in range(self.n - 2, -1, -1):
            if self.word(i) == JR_RA and self.word(i + 1) != JR_RA and (i == 0 or self.word(i - 1) != JR_RA):
                return self.base + 4 * (i + 2)
        return self.base

    def jtbls(self, limit):
        """[(jr, start, end, how)] for every jr rY (rY != ra) below limit that slices to a table."""
        found = []
        for i in range((limit - self.base) // 4):
            w = self.word(i)
            if w & 0xFC1FFFFF != 0x00000008 or (w >> 21) & 31 == 31:
                continue
            v = self.ev((w >> 21) & 31, i)
            if not v or v[0] != "tbl":
                continue
            found.append((i, v[1], self.bound(v[2])))
        starts = sorted(t for _, t, _ in found)
        rows = []
        for i, start, n in found:
            jr = self.base + 4 * i
            if n is not None:
                rows.append((jr, start, start + 4 * n, "sltiu"))
                continue
            if not (self.base <= start < self.base + 4 * self.n):
                continue
            flo, fhi = self.func_range(i)
            nxt = min([s for s in starts if s > start], default=self.base + 4 * self.n)
            a = start
            while a < nxt and a < self.base + 4 * self.n and flo <= u32(self.img, a - self.base) < fhi:
                a += 4
            if a > start:
                rows.append((jr, start, a, "run"))
        return rows


def loadmap_rows():
    out = []
    for ln in LOADMAP.read_text(encoding="utf-8").splitlines():
        if not ln or ln.startswith("#"):
            continue
        path, _size, _sha, cls, _route, base, end, _st = ln.split("\t")
        if cls == "exe" or (cls == "code" and base != "-"):
            out.append((path, cls, int(base, 16), int(end, 16)))
    return out


def read_cache(path):
    rows = []
    for ln in path.read_text(encoding="utf-8").splitlines():
        if ln and not ln.startswith("#"):
            rows.append(ln.split("\t"))
    return rows


def generate():
    rows = []
    for path, cls, base, end in loadmap_rows():
        data = (FILES / path).read_bytes()
        if cls == "exe":
            t_addr, t_size = struct.unpack_from("<II", data, 0x18)
            if t_addr != base:
                sys.exit(f"boundaries: {path} header t_addr 0x{t_addr:08x} != loadmap base 0x{base:08x}")
            img = Image(data[0x800:0x800 + t_size], t_addr)
            scan_end = img.text_end()
            if scan_end != EXE_TEXT_END:
                sys.exit(f"boundaries: {path} scanned text-end 0x{scan_end:08x} != binding 0x{EXE_TEXT_END:08x}")
            te, te_basis = EXE_TEXT_END, "T3 binding; scan agrees"
        else:
            img = Image(data[: len(data) & ~3], base)
            te, te_basis = img.text_end(), "scan: last jr ra + delay slot"
        rows.append((path, "text-end", base, te, te_basis))
        if te < end:
            rows.append((path, "data-island", te, end, "text-end..loadmap end"))
        for jr, start, tend, how in img.jtbls(te):
            rows.append((path, "jtbl", start, tend, f"jr=0x{jr:08x} {how}"))
        if cls == "exe":
            for start, size, lib, obj, low in read_cache(GH_OBJS):
                if low == "0":
                    s = int(start, 16)
                    rows.append((path, "lib-object", s, s + int(size, 16), f"psyq470 {lib}/{obj}"))
    rows = sorted(set(rows), key=lambda r: (r[0], r[2], r[1], r[3], r[4]))
    lines = ["# config/boundaries.tsv -- generated by tools/boundaries.py from extracted/retail/files + config/loadmap.tsv"
             " + config/ghidra/SLUS_012.79.{switch_tables,psyq_objects}.tsv; do not hand-edit.",
             f"# rows = {len(rows)}; end exclusive; see docs/memory-map.md#boundaries",
             "# binary\tkind\tstart\tend\tbasis"]
    for k, (b, kind, s, e, basis) in enumerate(rows):
        if k and k % COMMENT_EVERY == 0:
            lines.append(f"# rows {k + 1}..")
        lines.append(f"{b}\t{kind}\t0x{s:08x}\t0x{e:08x}\t{basis}")
    return "\n".join(lines) + "\n", rows


def write_cache(path, header, rows):
    lines = [header]
    for k, r in enumerate(rows):
        if k and k % COMMENT_EVERY == 0:
            lines.append(f"# rows {k + 1}..")
        lines.append("\t".join(r))
    path.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")


def refresh_ghidra():
    ghidra = pathlib.Path(os.environ.get("GHIDRA_INSTALL_DIR", pathlib.Path.home() / "ghidra_12.1.3_PUBLIC"))
    port = os.environ.get("DC2_GHIDRA_PORT", "8080")
    if subprocess.run(["lsof", "-nP", f"-iTCP:{port}", "-sTCP:LISTEN"], capture_output=True).returncode == 0:
        sys.exit(f"boundaries: a server is listening on :{port} (project lock). Stop it first.")
    SCRATCH.mkdir(parents=True, exist_ok=True)
    log = SCRATCH / "ghidra.out"
    with open(log, "w") as fh:
        rc = subprocess.run([str(ghidra / "support" / "analyzeHeadless"), str(REPO / "ghidra"), "dc2",
                             "-process", EXE, "-noanalysis", "-readOnly",
                             "-scriptPath", str(REPO / "tools" / "ghidra" / "scripts"),
                             "-postScript", "DumpSwitchTables.java", "-postScript", "DumpPsyqObjects.java"],
                            stdout=fh, stderr=subprocess.STDOUT).returncode
    text = log.read_text(encoding="utf-8", errors="replace")
    sw = re.findall(r"SWITCH jr=(0x[0-9a-f]{8}|none) start=(0x[0-9a-f]{8}) end=(0x[0-9a-f]{8}) n=(\d+)", text)
    ob = re.findall(r"OBJ start=(0x[0-9a-f]{8}) size=(0x[0-9a-f]+) lib=(\S+) obj=(\S+) low=([01])", text)
    m1, m2 = re.search(r"SWITCH_COUNT (\d+)", text), re.search(r"OBJ_COUNT (\d+)", text)
    if rc or not m1 or not m2 or int(m1[1]) != len(sw) or int(m2[1]) != len(ob):
        sys.exit(f"boundaries: ghidra dump failed rc={rc} (see {log.relative_to(REPO)})")
    write_cache(GH_SWITCH, "# Ghidra DumpSwitchTables.java on program SLUS_012.79 (ghidra/dc2, read-only); "
                "jr<TAB>start<TAB>end<TAB>entries; end exclusive; refresh: PY tools/boundaries.py --ghidra",
                [list(r) for r in sw])
    write_cache(GH_OBJS, "# Ghidra DumpPsyqObjects.java (psx_ldr SigApplier, PsyQ 470, rolled back) on SLUS_012.79; "
                "start<TAB>size<TAB>lib<TAB>obj<TAB>low; refresh: PY tools/boundaries.py --ghidra",
                [list(r) for r in ob])
    print(f"ghidra: switch={len(sw)} objs={len(ob)}")


def check():
    SCRATCH.mkdir(parents=True, exist_ok=True)
    text1, rows = generate()
    text2, _ = generate()
    ok = True
    (SCRATCH / "boundaries.check1.tsv").write_text(text1, encoding="utf-8", newline="\n")
    (SCRATCH / "boundaries.check2.tsv").write_text(text2, encoding="utf-8", newline="\n")
    if text1 != text2:
        print("FAIL determinism: two regenerations differ"); ok = False
    if not OUT.exists() or OUT.read_text(encoding="utf-8") != text1:
        print(f"FAIL stale: {OUT.relative_to(REPO)} differs from a regeneration"); ok = False
    ours = {}
    for b, kind, s, e, basis in rows:
        if b == EXE and kind == "jtbl":
            ours[int(basis.split()[0][3:], 16)] = (s, e)
    theirs = {}
    for jr, s, e, _n in read_cache(GH_SWITCH):
        theirs[jr] = (int(s, 16), int(e, 16))
    dis = 0
    keys = sorted({f"0x{k:08x}" for k in ours} | set(theirs))
    for k in keys:
        a = ours.get(int(k, 16)) if k != "none" else None
        g = theirs.get(k)
        if a != g:
            dis += 1
            fa = "-" if a is None else f"[0x{a[0]:08x},0x{a[1]:08x})"
            fg = "-" if g is None else f"[0x{g[0]:08x},0x{g[1]:08x})"
            print(f"DISAGREE jr={k} ours={fa} ghidra={fg}")
    print(f"exe jtbl: ours={len(ours)} ghidra={len(theirs)} disagreements={dis}")
    if dis:
        ok = False
    jr, cs, ce = CONTROL
    if ours.get(jr) != (cs, ce) or theirs.get(f"0x{jr:08x}") != (cs, ce):
        print(f"FAIL control switch jr=0x{jr:08x} [0x{cs:08x},0x{ce:08x}) not found by both"); ok = False
    else:
        print(f"control switch jr=0x{jr:08x} [0x{cs:08x},0x{ce:08x}): found")
    te = [r for r in rows if r[0] == EXE and r[1] == "text-end"]
    if len(te) != 1 or te[0][3] != EXE_TEXT_END:
        print("FAIL exe text-end"); ok = False
    else:
        print(f"exe text-end 0x{EXE_TEXT_END:08x}: ok")
    counts = {}
    for b, kind, *_ in rows:
        cls = "exe" if b == EXE else "code"
        counts[(cls, kind)] = counts.get((cls, kind), 0) + 1
    print("counts: " + " ".join(f"{c}/{k}={n}" for (c, k), n in sorted(counts.items())))
    if not counts.get(("exe", "lib-object")):
        print("FAIL no lib-object rows"); ok = False
    print("check: OK" if ok else "check: FAIL")
    return 0 if ok else 1


def main(argv):
    if argv[:1] == ["--ghidra"]:
        refresh_ghidra()
        return 0
    if argv[:1] == ["--check"]:
        return check()
    if argv:
        print(__doc__)
        return 2
    text, rows = generate()
    OUT.write_text(text, encoding="utf-8", newline="\n")
    print(f"boundaries: rows={len(rows)} -> {OUT.relative_to(REPO)}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
