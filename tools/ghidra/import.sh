#!/usr/bin/env bash
# tools/ghidra/import.sh <exe-path>
# tools/ghidra/import.sh --info <program-name>
#
# Import a PS-X EXE into the Ghidra project ghidra/dc2 HEADLESS (adapted from the kit's
# P2 ghidra_import.sh): the psx_ldr loader is auto-detected; auto-analysis runs, and its
# "PsyQ Signatures" analyzer applies the PsyQ sigs for the version the loader's DetectPsyQ
# found (Program Info "PsyQ Version"); then ImportPsyqGdt.java resolves psyq<detected>.gdt,
# StorePsyqSigHits.java stores + prints the sig-hit count, DumpProgramInfo.java prints the
# metadata. Program name = the EXE's basename; -overwrite makes re-import idempotent.
#
# --info: read-only, no analysis; prints language, image base, header t_addr, function
# count, PsyQ version, sig-hit count. Exit 0 only if the program exists and hits > 0.
#
# PRECONDITION: no agent server holding the project lock (guard: TCP :$DC2_GHIDRA_PORT).
set -uo pipefail
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

GHIDRA="${GHIDRA_INSTALL_DIR:-$HOME/ghidra_12.1.3_PUBLIC}"
PROJ_DIR="${DC2_GHIDRA_PROJ:-$REPO/ghidra}"
PROJ="dc2"
SCRIPTS="$REPO/tools/ghidra/scripts"
PORT="${DC2_GHIDRA_PORT:-8080}"
OUT_DIR="$REPO/.run/ghidra"

usage() { echo "usage: import.sh <ps-x-exe> | import.sh --info <program>" >&2; exit 2; }
[ $# -ge 1 ] || usage

# Guard: a serving agent server holds the exclusive project lock -- refuse to collide.
if lsof -nP -iTCP:"$PORT" -sTCP:LISTEN >/dev/null 2>&1; then
  echo "import: ERROR -- a server is listening on :$PORT (project lock). Stop it first." >&2
  exit 2
fi
# Clear a stale lock only when no analyzer is alive.
if ! pgrep -f 'ghidra.app.util.headless.AnalyzeHeadless' >/dev/null 2>&1; then
  rm -f "$PROJ_DIR/$PROJ.lock" "$PROJ_DIR/$PROJ.lock~" 2>/dev/null || true
fi
mkdir -p "$PROJ_DIR" "$OUT_DIR"

# hits_ok <output-file>: exit 0 iff the program printed a sig-hit count > 0.
hits_ok() {
  local hits
  hits="$(grep -o 'SIG_HITS: [0-9]*' "$1" | tail -1 | awk '{print $2}')"
  [ -n "$hits" ] && [ "$hits" -gt 0 ]
}

if [ "$1" = "--info" ]; then
  PROG="${2:-}"; [ -n "$PROG" ] || usage
  OUT="$OUT_DIR/info-$PROG.out"
  "$GHIDRA/support/analyzeHeadless" "$PROJ_DIR" "$PROJ" \
    -process "$PROG" -noanalysis -readOnly \
    -scriptPath "$SCRIPTS" \
    -postScript DumpProgramInfo.java >"$OUT" 2>&1
  rc=$?
  grep -E 'DumpProgramInfo.java> ' "$OUT" | sed -e 's/.*DumpProgramInfo.java> //' -e 's/ (GhidraScript) *$//'
  [ $rc -eq 0 ] || { echo "info: analyzeHeadless exit=$rc (see $OUT)" >&2; exit 1; }
  hits_ok "$OUT" || { echo "info: FAIL -- program '$PROG' missing or sig hits = 0 (see $OUT)" >&2; exit 1; }
  grep -q 'BASE_CHECK: ok' "$OUT" || { echo "info: FAIL -- t_addr block check (see $OUT)" >&2; exit 1; }
  echo "info: OK"
  exit 0
fi

EXE="$1"
[ -f "$EXE" ] || { echo "import: ERROR -- exe not found: $EXE" >&2; exit 2; }
OUT="$OUT_DIR/import-$(basename "$EXE").out"

echo "import: ===== importing '$EXE' into project '$PROJ' (program = basename, -overwrite) ====="
"$GHIDRA/support/analyzeHeadless" "$PROJ_DIR" "$PROJ" \
  -import "$EXE" -overwrite \
  -scriptPath "$SCRIPTS" \
  -postScript ImportPsyqGdt.java \
  -postScript StorePsyqSigHits.java \
  -postScript DumpProgramInfo.java 2>&1 | tee "$OUT"
rc=${PIPESTATUS[0]}
echo "import: ===== analyzeHeadless exit=$rc for '$EXE' (full output: $OUT) ====="
[ $rc -eq 0 ] || exit $rc
hits_ok "$OUT" || { echo "import: FAIL -- sig hits = 0" >&2; exit 1; }
exit 0
