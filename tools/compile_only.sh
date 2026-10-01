#!/usr/bin/env bash
# compile_only.sh — no-ROM compile check (T8): every src/**/*.c through the pinned triple's cpp → cc1 → maspsx, with
# the build's exact flags (`make print-c A=<alias>`, alias = first dir under src/). No `as`, no asm/, no disc: the
# INCLUDE_ASM `.include`s are resolved only by `as`. Outputs: .run/compile_only/<path>.{i,s,m.s}.
# CCDIR: $COMPILE_ONLY_CCDIR if set (fetched if not verify-clean); else /opt/cc if verify-clean for psyq4.6, maspsx,
# wibo; else .run/cc, fetched by tools/docker/fetch_toolchain.sh (sha256 mismatch → exit non-zero, nothing compiled).
# Prints `compiled: K of U units`; exit 0 iff K == U and U ≥ 1. Usage: bash tools/compile_only.sh
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")/.."
FETCH=tools/docker/fetch_toolchain.sh
NEED=(psyq4.6 maspsx wibo)
SRC_TSV="${TOOLCHAINS_TSV:-config/toolchains.tsv}"
OUT=.run/compile_only
mkdir -p .run

# a TSV of only the needed rows, so --verify judges just those
SUB_TSV=.run/compile_only.toolchains.tsv
awk -F'\t' -v need="${NEED[*]}" 'BEGIN { split(need, a, " "); for (i in a) w[a[i]] = 1 } /^#/ || ($1 in w)' \
  "$SRC_TSV" > "$SUB_TSV"
[ "$(grep -vc '^#' "$SUB_TSV")" -eq "${#NEED[@]}" ] || { echo "compile_only: $SRC_TSV lacks a row of: ${NEED[*]}" >&2; exit 1; }
verify() { TOOLCHAINS_TSV="$SUB_TSV" bash "$FETCH" --verify "$1" >/dev/null 2>&1; }

if [ -n "${COMPILE_ONLY_CCDIR:-}" ]; then
  CCDIR="$COMPILE_ONLY_CCDIR"
elif verify /opt/cc; then
  CCDIR=/opt/cc
else
  CCDIR=.run/cc
fi
if ! verify "$CCDIR"; then
  echo "compile_only: fetching ${NEED[*]} into $CCDIR"
  TOOLCHAINS_TSV="$SUB_TSV" bash "$FETCH" --only "${NEED[@]}" "$CCDIR" \
    || { echo "compile_only: toolchain fetch failed; nothing compiled" >&2; exit 1; }
  verify "$CCDIR" || { echo "compile_only: $CCDIR not verify-clean after fetch" >&2; exit 1; }
fi
CCDIR="$(cd "$CCDIR" && pwd)"
echo "compile_only: CCDIR=$CCDIR"

mapfile -t units < <(find src -type f -name '*.c' -not -name '.*' | LC_ALL=C sort)
U=${#units[@]}; K=0; failed=()
for c in "${units[@]}"; do
  rel="${c#src/}"; alias="${rel%%/*}"
  [ "$alias" != "$rel" ] || { echo "FAILED $c: not under src/<alias>/" >&2; failed+=("$c"); continue; }
  flags="$(make -s --no-print-directory print-c A="$alias" CCDIR="$CCDIR")" \
    || { echo "FAILED $c: make print-c" >&2; failed+=("$c"); continue; }
  (
    eval "$flags"
    o="$OUT/${c%.c}"; mkdir -p "$(dirname "$o")"
    # word splitting of the flag strings is intended, as in the Makefile recipe
    # shellcheck disable=SC2086
    $CPP $CPPFLAGS "$c" -o "$o.i" \
      && $CC1 $CFLAGS "$o.i" -o "$o.s" \
      && { echo '.include "macro.inc"'; $MASPSX $MASPSXFLAGS "$o.s"; } > "$o.m.s"
  ) && K=$((K + 1)) || { echo "FAILED $c" >&2; failed+=("$c"); }
done

echo "compiled: $K of $U units"
[ "$K" -eq "$U" ] && [ "$U" -ge 1 ] || { [ "$U" -ge 1 ] || echo "compile_only: no units under src/" >&2; exit 1; }
