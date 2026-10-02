#!/usr/bin/env bash
# tools/ghidra/import_raw.sh <blob-path> <base-vram> <program-name>
#
# Import a RAW (flat, headerless) PSX memory image into the Ghidra project ghidra/dc2
# HEADLESS -- the counterpart to import.sh (PS-X EXEs only), adapted from the kit's P2
# ghidra_import_raw.sh. For images that load at a fixed vram with no PS-X EXE header
# (overlays, resident blobs), where the psx_ldr magic auto-detect does not apply:
#
#   - BinaryLoader at -loader-baseAddr <base-vram>, language PSX:LE:32:default,
#   - auto-analysis (PsyQ Signatures + the MIPS function finders),
#   - ImportPsyqGdt.java (psyq<ver>.gdt; ver = $DC2_PSYQ_VER, else the program's
#     detected version, else 470) + StorePsyqSigHits.java + DumpProgramInfo.java.
#
# The program is named <program-name> (the blob is staged under .run/ with that name,
# since analyzeHeadless derives the program name from the import filename).
#
# PRECONDITION: no agent server holding the project lock (guard: TCP :$DC2_GHIDRA_PORT).
set -uo pipefail
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

BLOB="${1:?usage: import_raw.sh <blob-path> <base-vram> <program-name>}"
BASE="${2:?usage: import_raw.sh <blob-path> <base-vram> <program-name>}"
NAME="${3:?usage: import_raw.sh <blob-path> <base-vram> <program-name>}"
GHIDRA="${GHIDRA_INSTALL_DIR:-$HOME/ghidra_12.1.3_PUBLIC}"
PROJ_DIR="${DC2_GHIDRA_PROJ:-$REPO/ghidra}"
PROJ="dc2"
SCRIPTS="$REPO/tools/ghidra/scripts"
LANG_ID="PSX:LE:32:default"
PORT="${DC2_GHIDRA_PORT:-8080}"
STAGE_DIR="$REPO/.run/ghidra/stage"
STAGE="$STAGE_DIR/$NAME"   # import filename -> program name

[ -f "$BLOB" ] || { echo "import-raw: ERROR -- blob not found: $BLOB" >&2; exit 2; }

# Guard: a serving agent server holds the exclusive project lock -- refuse to collide.
if lsof -nP -iTCP:"$PORT" -sTCP:LISTEN >/dev/null 2>&1; then
  echo "import-raw: ERROR -- a server is listening on :$PORT (project lock). Stop it first." >&2
  exit 2
fi
# Clear a stale lock only when no analyzer is alive.
if ! pgrep -f 'ghidra.app.util.headless.AnalyzeHeadless' >/dev/null 2>&1; then
  rm -f "$PROJ_DIR/$PROJ.lock" "$PROJ_DIR/$PROJ.lock~" 2>/dev/null || true
fi

mkdir -p "$STAGE_DIR" "$PROJ_DIR"
cp -f "$BLOB" "$STAGE"

GDT_ARGS=()
[ -n "${DC2_PSYQ_VER:-}" ] && GDT_ARGS=("$GHIDRA/Ghidra/Extensions/ghidra_psx_ldr/data/psyq${DC2_PSYQ_VER}.gdt")

echo "import-raw: ===== importing '$BLOB' as program '$NAME' @ $BASE ($LANG_ID) ====="
"$GHIDRA/support/analyzeHeadless" "$PROJ_DIR" "$PROJ" \
  -import "$STAGE" -overwrite \
  -loader BinaryLoader \
  -loader-baseAddr "$BASE" \
  -processor "$LANG_ID" \
  -scriptPath "$SCRIPTS" \
  -postScript ImportPsyqGdt.java ${GDT_ARGS[@]+"${GDT_ARGS[@]}"} \
  -postScript StorePsyqSigHits.java \
  -postScript DumpProgramInfo.java
rc=$?
rm -f "$STAGE"
echo "import-raw: ===== analyzeHeadless exit=$rc for program '$NAME' ====="
exit $rc
