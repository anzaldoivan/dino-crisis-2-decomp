#!/usr/bin/env bash
# tools/permuter/compile.sh — the permuter's compile command (T6, Phase 1.7): <in.c> -o <out.o> through the pinned
# triple cpp → cc1 → (`.include "macro.inc"` + maspsx) → as, exactly as the Makefile's .c recipe. Alias from env
# PERMUTE_ALIAS; every tool and flag from `make print-c A=$PERMUTE_ALIAS` (G69: no flag copies here).
# Intermediates (<out>.i/.s/.m.s) sit next to <out.o> and are removed. Exit non-zero on any failure.
# Usage: PERMUTE_ALIAS=<alias> bash tools/permuter/compile.sh <in.c> -o <out.o>
set -euo pipefail

[ $# -eq 3 ] && [ "$2" = "-o" ] && [ -n "${PERMUTE_ALIAS:-}" ] \
  || { echo "usage: PERMUTE_ALIAS=<alias> compile.sh <in.c> -o <out.o>" >&2; exit 2; }
abs() { case "$1" in /*) printf '%s' "$1" ;; *) printf '%s/%s' "$PWD" "$1" ;; esac; }
in="$(abs "$1")"; out="$(abs "$3")"
cd "$(dirname "${BASH_SOURCE[0]}")/../.."

eval "$(make -s --no-print-directory print-c A="$PERMUTE_ALIAS")"
b="${out%.o}"
trap 'rm -f "$b.i" "$b.s" "$b.m.s"' EXIT
# word splitting of the flag strings is intended, as in the Makefile recipe
# shellcheck disable=SC2086
$CPP $CPPFLAGS "$in" -o "$b.i"
$CC1 $CFLAGS "$b.i" -o "$b.s"
{ echo '.include "macro.inc"'; $MASPSX $MASPSXFLAGS "$b.s"; } > "$b.m.s"
$AS $ASFLAGS -I "build/$PERMUTE_ALIAS/include" -o "$out" "$b.m.s"
