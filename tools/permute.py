#!/usr/bin/env python3
"""tools/permute.py -- decomp-permuter driver for one lever row (T6, Phase 1.7). Runs in the image (dc.sh run).

  permute.py <lever> [--iters N] [--seed S]   permute the row's before_c against the game fn for exactly N iterations
  permute.py --check [--iters N] [--seed S]   lever = first `regalloc` row of config/levers.tsv, plus the scorer control

Sets up .run/permute/<lever>/ (wiped first): base.c (before_c through the Makefile's cpp + CPPFLAGS; register pins,
asm() and __attribute__ refused), target.o (the game fn's split asm func_<START>.s assembled with the Makefile's
AS/ASFLAGS; `make split ONLY=<alias>` when absent), settings.toml, compile.sh (shim: PERMUTE_ALIAS +
tools/permuter/compile.sh). Pre-flight: base.c compiles and has no `*/`, else refused (rc 1). Then
tools/permuter/permuter_run.py under /opt/permuter/bin/python (single worker, seeded, PYTHONHASHSEED=0, masked
word scorer; see its docstring) logs every candidate to candidates.log. Prints
`permuter: stored draft iterated W times, D distinct candidates`, `not judged: J`, `best: S of base S0`.
--check also compiles after_c and before_c and prints `scorer control: ok` iff after scores 0 and before > 0;
rc 1 when W < 100, D < 2 or the control fails.
"""
import argparse
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent / "permuter"))
import probe  # noqa: E402
import permuter_run  # noqa: E402

ROOT = probe.ROOT
LEVERS = ROOT / "config/levers.tsv"
COLS = ("lever", "group", "tell", "label", "alias", "start", "end", "before_c", "after_c", "cookbook")
VENV_PY = "/opt/permuter/bin/python"
CHECK_ITERS = 120
SEED = 1
HAZARDS = [(re.compile(r"\*/"), "`*/` in base.c"),
           (re.compile(r"\bregister\b[^;]*\b(asm|__asm__)\s*\("), "register pin (register … asm(\"$r\"))"),
           (re.compile(r"\b(asm|__asm__)\b"), "inline asm"),
           (re.compile(r"\b__attribute__\b"), "__attribute__")]


def refuse(msg):
    print(f"permute: refused: {msg}", flush=True)
    sys.exit(1)


def sh(script, alias, cwd=ROOT):
    """Run a bash script with the Makefile's triple for alias eval'd in (G69); raise on failure."""
    full = f'set -euo pipefail; eval "$(make -s --no-print-directory print-c A={alias})"; {script}'
    p = subprocess.run(["bash", "-c", full], cwd=cwd, capture_output=True, text=True)
    if p.returncode:
        raise RuntimeError(f"rc {p.returncode}: {(p.stderr or p.stdout).strip()[-300:]}")


def setup(row, d):
    alias, fn = row["alias"], f"func_{int(row['start'], 16):08X}"
    if d.exists():
        shutil.rmtree(d)
    (d / "tmp").mkdir(parents=True)
    if probe.ensure_extracted():
        refuse("game data not extracted")
    asm = sorted((ROOT / "asm" / alias / "nonmatchings").glob(f"**/{fn}.s"))
    if not asm:
        with open(d / "split.log", "w") as log:
            rc = subprocess.run(["make", "split", f"ONLY={alias}"], cwd=ROOT, stdout=log,
                                stderr=subprocess.STDOUT).returncode
        asm = sorted((ROOT / "asm" / alias / "nonmatchings").glob(f"**/{fn}.s"))
        if rc or not asm:
            refuse(f"no split asm for {fn} after `make split ONLY={alias}` (rc {rc}, {d}/split.log)")
    (d / "target.s").write_text('.include "macro.inc"\n.section .text\n' + asm[0].read_text())
    try:
        sh(f'$AS $ASFLAGS -I build/{alias}/include -o {d}/target.o {d}/target.s', alias)
        probe.extract(d / "target.o", fn)  # sized FUNC symbol (splat glabel/endlabel)
    except RuntimeError as e:
        refuse(f"target.o: {e}")
    try:
        sh(f'$CPP $CPPFLAGS {ROOT / row["before_c"]} -o {d}/base.c', alias)
    except RuntimeError as e:
        refuse(f"base.c: cpp: {e}")
    src = (d / "base.c").read_text()
    for rx, why in HAZARDS:
        if rx.search(src):
            refuse(f"base.c: {why}")
    (d / "settings.toml").write_text(f'func_name = "{fn}"\ncompiler_type = "gcc"\n'
                                     f'objdump_command = "mipsel-linux-gnu-objdump"\n')
    (d / "compile.sh").write_text(f'#!/usr/bin/env bash\nexport PERMUTE_ALIAS={alias}\n'
                                  f'exec bash {ROOT}/tools/permuter/compile.sh "$@"\n')
    if compile_c(d, d / "base.c", d / "preflight.o") is None:
        refuse(f"base.c does not compile via compile.sh ({d}/preflight.err)")
    return fn


def compile_c(d, c, o):
    p = subprocess.run(["bash", str(d / "compile.sh"), str(c), "-o", str(o)], capture_output=True, text=True)
    if p.returncode:
        (d / "preflight.err").write_text(p.stdout + p.stderr)
        return None
    return o


def control(row, d, fn):
    target = probe.extract(d / "target.o", fn)
    res = {}
    for k in ("after_c", "before_c"):
        o = compile_c(d, ROOT / row[k], d / f"control_{k}.o")
        res[k] = (None, "compile failed", None) if o is None else permuter_run.score_o(o, fn, target)
    a, b = res["after_c"][0], res["before_c"][0]
    print(f"scorer control: after_c {a if a is not None else res['after_c'][1]}, "
          f"before_c {b if b is not None else res['before_c'][1]}", flush=True)
    ok = a == 0 and b is not None and b > 0
    if ok:
        print("scorer control: ok", flush=True)
    return ok


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("lever", nargs="?")
    ap.add_argument("--iters", type=int, default=None)
    ap.add_argument("--seed", type=int, default=SEED)
    ap.add_argument("--check", action="store_true")
    a = ap.parse_args()
    rows = [dict(zip(COLS, ln.split("\t"))) for ln in LEVERS.read_text().splitlines() if ln and not ln.startswith("#")]
    if a.check:
        rows = [r for r in rows if r["group"] == "regalloc"]
    elif a.lever:
        rows = [r for r in rows if r["lever"] == a.lever]
    else:
        ap.error("a lever or --check is required")
    if not rows:
        refuse("no matching row in config/levers.tsv")
    row = rows[0]
    iters = a.iters if a.iters is not None else (CHECK_ITERS if a.check else 1000)
    d = ROOT / ".run/permute" / row["lever"]
    fn = setup(row, d)
    print(f"permute: {row['lever']} {row['alias']}:{fn} seed {a.seed} iters {iters} dir {d.relative_to(ROOT)}",
          flush=True)
    env = dict(os.environ, PYTHONHASHSEED="0", PYTHONUNBUFFERED="1", TMPDIR=str(d / "tmp"))
    with open(d / "permuter.out", "w") as out:
        rc = subprocess.run([VENV_PY, str(ROOT / "tools/permuter/permuter_run.py"), str(d), "--fn", fn,
                             "--iters", str(iters), "--seed", str(a.seed)], cwd=ROOT, env=env,
                            stdout=out, stderr=subprocess.STDOUT).returncode
    if rc:
        print(f"permute: permuter rc {rc}; see {d.relative_to(ROOT)}/permuter.out", flush=True)
        return 1
    log = [ln.split("\t") for ln in (d / "candidates.log").read_text().splitlines()]
    base = [r for r in log if r[0] == "0"]
    its = [r for r in log if r[0] != "0"]
    w = len(its)
    dist = len({r[3] for r in its if r[3] != "-"})
    nj = sum(1 for r in its if r[1].startswith("not-judged"))
    scores = [int(r[2]) for r in log if r[2] != "-"]
    s0 = base[0][2] if base else "-"
    print(f"permuter: stored draft iterated {w} times, {dist} distinct candidates", flush=True)
    print(f"not judged: {nj}", flush=True)
    print(f"best: {min(scores) if scores else '-'} of base {s0}", flush=True)
    if not a.check:
        return 0
    ok = control(row, d, fn)
    return 0 if ok and w >= 100 and dist >= 2 else 1


if __name__ == "__main__":
    sys.exit(main())
