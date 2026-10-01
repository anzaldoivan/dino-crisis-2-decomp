#!/usr/bin/env bash
# tools/emu/emu.sh start|stop|status — detached headless PCSX-Redux with the web API on 127.0.0.1:$DC2_EMU_PORT (8081).
# BIOS: the bundled OpenBIOS by default; set DC2_BIOS=<path> to pass `-bios` (machine-local, never in git).
# Optional DC2_BREAK=<addr>: start with -debugger and pause at that PC (see lua/vsync.lua).
# CPU: -interpreter (build f7b388cc dynarec dies with SIGILL ~25 s into this game on arm64).
# State: pid .run/emu/redux.pid, log .run/emu/redux.log. Kills by pid only.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
APP="${DC2_REDUX:-$HOME/Applications/PCSX-Redux.app/Contents/MacOS/PCSX-Redux}"
PORT="${DC2_EMU_PORT:-8081}"
DIR="$ROOT/.run/emu"; PID="$DIR/redux.pid"; LOG="$DIR/redux.log"
mkdir -p "$DIR"

alive() { [[ -f "$PID" ]] && kill -0 "$(cat "$PID")" 2>/dev/null; }
up() { curl -sf -o /dev/null "http://127.0.0.1:$PORT/api/v1/execution-flow"; }

case "${1:-}" in
  start)
    if alive; then echo "emu: already running pid $(cat "$PID")"; exit 0; fi
    cue=("$ROOT"/disks/*.cue); [[ -f "${cue[0]}" ]] || { echo "emu: no disks/*.cue" >&2; exit 2; }
    [[ -x "$APP" ]] || { echo "emu: PCSX-Redux not found at $APP" >&2; exit 2; }
    args=(-no-ui -interpreter -iso "${cue[0]}" -webserver -webserver-port "$PORT" -dofile "$ROOT/tools/emu/lua/vsync.lua" -run -stdout)
    [[ -n "${DC2_BIOS:-}" ]] && args+=(-bios "$DC2_BIOS")
    [[ -n "${DC2_BREAK:-}" ]] && args+=(-debugger)  # lua/vsync.lua arms a pausing Exec breakpoint at $DC2_BREAK
    # new session so the emulator outlives the calling shell
    /usr/bin/env python3 -c 'import os,subprocess,sys; p=subprocess.Popen(sys.argv[2:],stdout=open(sys.argv[1],"w"),stderr=subprocess.STDOUT,stdin=subprocess.DEVNULL,start_new_session=True); print(p.pid)' \
      "$LOG" "$APP" "${args[@]}" > "$PID"
    # first /api/v1/lua/ request creates PCSX.WebServer (404); lua/vsync.lua then registers its route on the next vsync
    for _ in $(seq 1 60); do up && { curl -s -o /dev/null "http://127.0.0.1:$PORT/api/v1/lua/vsync"
      echo "emu: up pid $(cat "$PID") port $PORT"; exit 0; }; alive || break; sleep 0.5; done
    echo "emu: failed to come up; tail of $LOG:" >&2; tail -5 "$LOG" >&2; exit 1 ;;
  stop)
    if alive; then p="$(cat "$PID")"; kill "$p"; for _ in $(seq 1 20); do kill -0 "$p" 2>/dev/null || break; sleep 0.25; done
      kill -0 "$p" 2>/dev/null && kill -9 "$p"; echo "emu: stopped pid $p"; else echo "emu: not running"; fi
    rm -f "$PID" ;;
  status)
    if alive && up; then echo "emu: up pid $(cat "$PID") port $PORT"; else echo "emu: down"; exit 1; fi ;;
  *) echo "usage: $0 start|stop|status" >&2; exit 2 ;;
esac
