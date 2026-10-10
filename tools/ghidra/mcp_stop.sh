#!/usr/bin/env bash
# tools/ghidra/mcp_stop.sh -- clean stop WITH save (the way headless MCP work is persisted;
# adapted from the kit's P2 ghidra_mcp_stop.sh). Touches the stopreq sentinel; Dc2McpServer
# stops the MCP server and returns, then analyzeHeadless COMMITS the pending transaction and
# saves+closes the project (releasing the lock). Never SIGKILL: that loses the save.
set -uo pipefail
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

PORT="${DC2_GHIDRA_PORT:-8080}"
RUNDIR="$REPO/.run"
LOG="${DC2_MCP_LOG:-$RUNDIR/ghidra-mcp.log}"
STOPREQ="${DC2_MCP_STOPREQ:-$RUNDIR/mcp-stop.req}"
PIDFILE="$RUNDIR/ghidra-mcp.pid"

listening() { lsof -nP -iTCP:"$PORT" -sTCP:LISTEN >/dev/null 2>&1; }

if ! listening; then
  echo "ghidra-mcp: not serving on :$PORT -- nothing to stop"
  exit 0
fi

before=$(wc -l < "$LOG" 2>/dev/null || echo 0)
echo "ghidra-mcp: requesting clean save+stop (touch $STOPREQ)"
: > "$STOPREQ"

# Wait for the port to free (server stopped)...
port_closed=0
for i in $(seq 1 90); do
  if ! listening; then echo "ghidra-mcp: server stopped after ${i}s"; port_closed=1; break; fi
  sleep 1
done
[ "$port_closed" -eq 1 ] || { echo "WARN: still serving on :$PORT after 90s -- inspect $LOG (do NOT SIGKILL; that loses the save)"; exit 1; }

# ...then for analyzeHeadless to report the save (commit happens during project close).
saved=0
for i in $(seq 1 45); do
  if tail -n +"$((before+1))" "$LOG" 2>/dev/null | grep -qiE 'Save succeeded for processed file'; then
    echo "ghidra-mcp: Save succeeded (committed on close) after ${i}s"; saved=1; break
  fi
  sleep 1
done
[ "$saved" -eq 1 ] || echo "WARN: did not see 'Save succeeded' in $LOG within 45s -- verify before relying on the save"

# ...and for the JVM to exit (lock released).
if [ -f "$PIDFILE" ]; then
  pid=$(cat "$PIDFILE")
  for i in $(seq 1 30); do kill -0 "$pid" 2>/dev/null || break; sleep 1; done
  kill -0 "$pid" 2>/dev/null && echo "WARN: pid $pid still alive after 30s" || rm -f "$PIDFILE"
fi

echo "=== shutdown log tail ==="
tail -n +"$((before+1))" "$LOG" 2>/dev/null | grep -iE 'stop requested|server stopped|Save succeeded|FAILED|lock|read-only' | tail -10
[ "$saved" -eq 1 ]
