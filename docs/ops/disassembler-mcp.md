# Disassembler MCP: reconnect after a restart; persist symbols headlessly

Whenever the reverse-engineering MCP server is stopped and restarted — for a headless import, or to serve a different program —
the coding agent's client connection goes stale and every call to that server times out until the client reconnects. The agent
cannot run the reconnect command itself.

**Why:** the first time it happened the agent noted "client may need reconnect" and kept going, burning turns on timeouts and
working around them. The correct move is to stop and ask.

**How to apply:** a server restart always ends with one message — "I restarted the MCP server (now serving X); please run
`/mcp` to reconnect, then I will verify and continue" — followed by one cheap verification call before any real work. Treat
the reverse-engineering database's persistence the same way: the server holds an open transaction while serving, so work is
saved only on a clean stop; renames made through the server may not persist at all — mirror symbols through a headless script
and verify with a read-only reopen.

## This project (T2.c2)

Server: GhidrAssistMCP 2.11.0, installed as a Ghidra extension (`$GHIDRA_INSTALL_DIR/Ghidra/Extensions/GhidrAssistMCP`),
run headless by `tools/ghidra/scripts/Dc2McpServer.java` as an analyzeHeadless `-preScript`. Serves one program of `ghidra/dc2`
on `127.0.0.1:8080`: SSE `GET /sse` + `POST /message` (client config `config/mcp.json.template`), streamable `POST /mcp`.

- start: `bash tools/ghidra/mcp_start.sh [prog]` (default `SLUS_012.79`; detached via nohup + perl `POSIX::setsid`; no-op if
  :8080 already listens; pid in `.run/ghidra-mcp.pid`, log `.run/ghidra-mcp.log`; wait for `serving` in the log)
- stop + save: `bash tools/ghidra/mcp_stop.sh` (touches `.run/mcp-stop.req`; waits for the port to close, for
  `Save succeeded for processed file`, and for the JVM to exit; exit 0 only when the save was seen; never SIGKILL)
- verify: `bash tools/ghidra/mcp_verify.sh <addr> <name> [prog]` (read-only reopen, `GetSymbolAt.java`; exit 0 PASS, 1 FAIL,
  2 server still up)
- env: `DC2_GHIDRA_PROJ` (project dir, default `ghidra/`), `DC2_GHIDRA_PORT`, `DC2_MCP_LOG`, `DC2_MCP_STOPREQ`, `GHIDRA_INSTALL_DIR`.

Persistence model (smoke-tested on a scratch copy, T2.c2): MCP writes apply immediately but sit in an open transaction; the
clean stop commits and saves them (`rename_symbol` FUN_80018014 survived stop + read-only reopen). Checkpoint = stop + restart.
While serving, the server holds the project lock: `tools/ghidra/import.sh` and any other analyzeHeadless on `ghidra/dc2` refuse
or fail; stop first. The reconnect rule above holds after every restart.
