#!/usr/bin/env bash
# cc1_dumps.sh <alias> <src.c> [name] — per-pass RTL dumps of one TU under the pinned triple (container; T1 Phase 1.7).
# Flags come from `make print-c A=<alias>` (G69, nothing retyped); cpp → cc1 $CFLAGS -da (gcc 2.95: every dump).
# Dumps land in .run/dumps/<name>/ (default name = src stem; scratch, never committed: G12). Prints present/missing
# dump suffixes per pass group, then the control: the .s with -da byte-equals the .s without.
# The pinned CC1PSX.EXE under wibo accepts -da and writes every dump (T1.c1), so no fallback cc1 is needed.
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."
[ $# -ge 2 ] && [ $# -le 3 ] || { echo "usage: cc1_dumps.sh <alias> <src.c> [name]" >&2; exit 2; }
alias="$1"; src="$2"; name="${3:-$(basename "${src%.c}")}"
[ -f "$src" ] || { echo "cc1_dumps: no such file $src" >&2; exit 2; }
eval "$(make -s --no-print-directory print-c A="$alias")"
out=".run/dumps/$name"; rm -rf "$out"; mkdir -p "$out/plain"
# word splitting of the flag strings is intended, as in the Makefile recipe
# shellcheck disable=SC2086
$CPP $CPPFLAGS "$src" -o "$out/$name.i"
cp "$out/$name.i" "$out/plain/$name.i"
# both compiles run from their own dir with a bare input name, so the `.file` line is the same in both .s
# shellcheck disable=SC2086
(cd "$out/plain" && $CC1 $CFLAGS "$name.i" -o "$name.s")
# shellcheck disable=SC2086
(cd "$out" && $CC1 $CFLAGS -da "$name.i" -o "$name.s")

groups=("expand-cse:rtl cse gcse cse2" "loop:loop" "combine:combine regmove" "regalloc:lreg greg"
        "sched-reorg:sched sched2 dbr jump jump2")
for g in "${groups[@]}"; do
  present=(); missing=()
  for s in ${g#*:}; do
    if compgen -G "$out/$name*.$s" >/dev/null; then present+=("$s"); else missing+=("$s"); fi
  done
  echo "group ${g%%:*}: present ${present[*]:-none}; missing ${missing[*]:-none}"
done
echo "dumps: $(find "$out" -maxdepth 1 -type f ! -name "$name.i" ! -name "$name.s" | wc -l | tr -d ' ') files in $out"

if cmp -s "$out/plain/$name.s" "$out/$name.s"; then
  echo "control: .s with -da byte-equals .s without: ok"
else
  echo "control: .s with -da differs from .s without: FAIL" >&2; exit 1
fi
