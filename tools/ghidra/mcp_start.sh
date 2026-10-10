#!/usr/bin/env bash
# tools/ghidra/mcp_start.sh [prog]
# Idempotent, fully-detached start of the headless GhidrAssistMCP server (adapted from the
# kit's P2 ghidra_mcp_start.sh), using scripts/Dc2McpServer.java (server + clean
# save-on-shutdown -- see that file). Serves one program of the Ghidra project ghidra/dc2
# over MCP on 127.0.0.1:$DC2_GHIDRA_PORT (default 8080): SSE GET /sse + POST /message,
# streamable POST /mcp. Program defaults to SLUS_012.79; only ONE program per session
# (stop with mcp_stop.sh to switch).
#
#   * no-op if something already listens on the port (safe to run every session),
#   * the launched server is detached (nohup + perl POSIX::setsid + disown) so it outlives
#     the launching shell and the agent session.
#
# GhidrAssistMCP must be installed as a Ghidra extension (its lib/*.jar then sit on the
# analyzeHeadless classpath): $GHIDRA/Ghidra/Extensions/GhidrAssistMCP. Silent no-op when
# Ghidra, the extension or the project is absent (a contributor's clone has none of them).
#
# After "serving" appears in the log, attach the client once per session (a running session
# cannot self-reconnect).
#
# PERSISTENCE: MCP writes are saved only on CLEAN SHUTDOWN (analyzeHeadless commits the open
# transaction + saves on close). Save+stop with mcp_stop.sh; resume by re-running this.
set -uo pipefail
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

GHIDRA="${GHIDRA_INSTALL_DIR:-$HOME/ghidra_12.1.3_PUBLIC}"
PROJ_DIR="${DC2_GHIDRA_PROJ:-$REPO/ghidra}"
PROJ="dc2"
PROG="${1:-SLUS_012.79}"
PORT="${DC2_GHIDRA_PORT:-8080}"
RUNDIR="$REPO/.run"; mkdir -p "$RUNDIR"
LOG="${DC2_MCP_LOG:-$RUNDIR/ghidra-mcp.log}"
SCRIPTS="$REPO/tools/ghidra/scripts"
STOPREQ="${DC2_MCP_STOPREQ:-$RUNDIR/mcp-stop.req}"
PIDFILE="$RUNDIR/ghidra-mcp.pid"

# 0) Not this machine's job? silent, successful no-op.
if [ ! -x "$GHIDRA/support/analyzeHeadless" ] || [ ! -d "$PROJ_DIR/$PROJ.rep" ] \
   || [ ! -f "$GHIDRA/Ghidra/Extensions/GhidrAssistMCP/lib/GhidrAssistMCP.jar" ]; then
  exit 0
fi

# 1) Already serving? no-op.
if lsof -nP -iTCP:"$PORT" -sTCP:LISTEN >/dev/null 2>&1; then
  echo "ghidra-mcp: already serving on :$PORT -- no-op"
  exit 0
fi

# 2) Clear a stale stop sentinel. A project lock is left alone: another analyzeHeadless may
#    own it (Ghidra then refuses to open, and the log says so).
rm -f "$STOPREQ" 2>/dev/null || true

# 3) Launch detached (macOS has no setsid(1): perl's POSIX::setsid starts a new session).
echo "ghidra-mcp: starting headless server (detached) -> $LOG"
nohup perl -MPOSIX=setsid -e 'setsid(); exec @ARGV or die "exec: $!"' \
  "$GHIDRA/support/analyzeHeadless" "$PROJ_DIR" "$PROJ" \
  -process "$PROG" -noanalysis \
  -scriptPath "$SCRIPTS" \
  -preScript Dc2McpServer.java host=127.0.0.1 "port=$PORT" "stopreq=$STOPREQ" \
  >"$LOG" 2>&1 </dev/null &
echo $! > "$PIDFILE"
disown 2>/dev/null || true

echo "ghidra-mcp: launched (pid $(cat "$PIDFILE"), $PIDFILE). Project load takes ~10-60s."
echo "ghidra-mcp: wait for 'serving' in $LOG, then attach the client."
