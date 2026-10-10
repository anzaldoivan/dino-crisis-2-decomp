#!/usr/bin/env bash
# tools/ghidra/mcp_verify.sh <addr> <expected-name> [prog]
#
# After a save-shutdown (mcp_stop.sh), verify a recent MCP edit persisted to disk: re-opens
# the SAVED project read-only and checks the function/symbol name at <addr> (adapted from the
# kit's P2 ghidra_mcp_verify.sh). Exit 0 = PASS, 1 = FAIL, 2 = server still up.
#
#   tools/ghidra/mcp_stop.sh && tools/ghidra/mcp_verify.sh 0x80010000 SomeName
set -uo pipefail
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

ADDR="${1:?usage: mcp_verify.sh <addr e.g. 0x80010000> <expected-name> [prog]}"
EXPECT="${2:?usage: mcp_verify.sh <addr> <expected-name> [prog]}"

GHIDRA="${GHIDRA_INSTALL_DIR:-$HOME/ghidra_12.1.3_PUBLIC}"
PROJ_DIR="${DC2_GHIDRA_PROJ:-$REPO/ghidra}"
PROJ="dc2"
PROG="${3:-SLUS_012.79}"
SCRIPTS="$REPO/tools/ghidra/scripts"
PORT="${DC2_GHIDRA_PORT:-8080}"

if lsof -nP -iTCP:"$PORT" -sTCP:LISTEN >/dev/null 2>&1; then
  echo "verify: ERROR -- server still serving on :$PORT. Run mcp_stop.sh first (read-only open needs the lock free)."
  exit 2
fi

out=$("$GHIDRA/support/analyzeHeadless" "$PROJ_DIR" "$PROJ" \
  -process "$PROG" -noanalysis -readOnly \
  -scriptPath "$SCRIPTS" -postScript GetSymbolAt.java "$ADDR" 2>&1)

got=$(echo "$out" | sed -n 's/.*DC2VERIFY .* name=\[\(.*\)\].*/\1/p' | head -1)

echo "verify: read-only re-open of saved DB -> symbol@$ADDR = [${got:-<none>}]"
if [ "$got" = "$EXPECT" ]; then
  echo "verify: PASS -- '$EXPECT' persisted at $ADDR in the saved project DB"
  exit 0
else
  echo "verify: FAIL -- expected '$EXPECT' at $ADDR, saved DB has '${got:-<none>}'"
  echo "--- analyzeHeadless tail ---"; echo "$out" | tail -15
  exit 1
fi
