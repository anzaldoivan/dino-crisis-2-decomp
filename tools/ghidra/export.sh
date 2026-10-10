#!/usr/bin/env bash
# tools/ghidra/export.sh [--proj DIR] [--out DIR] [PROG ...]
#
# Read-only export of a Ghidra program's annotations (types, signatures, data, comments, bookmarks,
# equates, labels) to byte-stable JSON-Lines via tools/ghidra/scripts/ExportAnnotations.java (adapted
# from the kit's P2 ghidra_export_annotations.sh). With no PROG every program in the project is exported
# (headless `-process` with no name). Output: <out>/<PROG>.jsonl (default .run/ghidra_export/).
#
# PRECONDITION: no agent server holding the project lock (guard: TCP :$DC2_GHIDRA_PORT) — even a
# read-only open needs the lock free. Never writes to the project (-readOnly).
set -uo pipefail
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
GHIDRA="${GHIDRA_INSTALL_DIR:-$HOME/ghidra_12.1.3_PUBLIC}"
PROJ_DIR="${DC2_GHIDRA_PROJ:-$REPO/ghidra}"; PROJ="dc2"
PORT="${DC2_GHIDRA_PORT:-8080}"
OUT="$REPO/.run/ghidra_export"
SCRIPTS="$REPO/tools/ghidra/scripts"
while [ $# -gt 0 ]; do
    case "$1" in
        --proj) PROJ_DIR="$2"; shift 2 ;;
        --out) OUT="$2"; shift 2 ;;
        *) break ;;
    esac
done
if lsof -nP -iTCP:"$PORT" -sTCP:LISTEN >/dev/null 2>&1; then
    echo "export: ERROR -- a server is listening on :$PORT (project lock). Stop it first." >&2; exit 2
fi
[ -f "$PROJ_DIR/$PROJ.gpr" ] || { echo "export: ERROR -- no project at $PROJ_DIR/$PROJ.gpr" >&2; exit 2; }
mkdir -p "$OUT"
cd "$REPO"
rc=0
if [ $# -eq 0 ]; then
    "$GHIDRA/support/analyzeHeadless" "$PROJ_DIR" "$PROJ" -process -noanalysis -readOnly \
        -scriptPath "$SCRIPTS" -postScript ExportAnnotations.java "$OUT" 2>&1 \
        | grep -E 'DC2EXPORT|ERROR|Exception' ; rc=${PIPESTATUS[0]}
else
    for p in "$@"; do
        "$GHIDRA/support/analyzeHeadless" "$PROJ_DIR" "$PROJ" -process "$p" -noanalysis -readOnly \
            -scriptPath "$SCRIPTS" -postScript ExportAnnotations.java "$OUT" 2>&1 \
            | grep -E 'DC2EXPORT|ERROR|Exception' ; r=${PIPESTATUS[0]}; [ "$r" -eq 0 ] || rc=$r
    done
fi
echo "export: analyzeHeadless exit=$rc -> $OUT/"
exit $rc
