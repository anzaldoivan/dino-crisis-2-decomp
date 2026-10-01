#!/usr/bin/env bash
# tools/emu/prove_load.sh <exe-name> — prove a PS-X EXE sits in RAM at its header t_addr.
# Reads pc0 @0x10, t_addr @0x18, t_size @0x1C of extracted/retail/files/<exe>; (re)boots the emulator with a
# pausing breakpoint at pc0 (the load-complete moment) and snapshots RAM at 3 moments: pc0 hit, +300, +600 vsyncs.
# Asserts RAM[t_addr..t_addr+t_size) == file[0x800..0x800+t_size) on every snapshot, full range, no masking.
# exit 0 all equal; 1 mismatch (differing offsets printed); 2 setup error. Snapshots: .run/ram/prove_<exe>_<k>.bin.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
PY="${PY:-/opt/homebrew/opt/python@3.14/bin/python3.14}"
EXE="${1:?usage: prove_load.sh <exe-name>}"; F="$ROOT/extracted/retail/files/$EXE"
API="http://127.0.0.1:${DC2_EMU_PORT:-8081}/api/v1"
[[ -f "$F" ]] || { echo "prove: no $F" >&2; exit 2; }
word() { od -A n -t u4 -j "$1" -N 4 "$F" | tr -d ' '; }
PC0=$(word 16); TADDR=$(word 24); TSIZE=$(word 28)
printf 'prove: %s pc0=0x%08x t_addr=0x%08x t_size=0x%x\n' "$EXE" "$PC0" "$TADDR" "$TSIZE"
OFF=$(( TADDR - 0x80000000 ))
(( TADDR >= 0x80000000 && OFF + TSIZE <= 0x200000 && TSIZE % 2048 == 0 )) || { echo "prove: header out of range" >&2; exit 2; }

vs() { local v; for _ in 1 2 3 4 5 6 7 8; do v=$(curl -sf "$API/lua/vsync") && { echo "$v"; return; }; sleep 0.25; done; echo -1; }
running() { curl -sf "$API/execution-flow" | grep -q '"running":true'; }
flow() { curl -sf -o /dev/null -X POST "$API/execution-flow?function=$1"; }
wait_until() {  # wait_until <max-s> <cmd...>
  local t=$1; shift; for _ in $(seq 1 $(( t * 4 ))); do "$@" && return 0; sleep 0.25; done; return 1; }

# the load moment exists only once per boot: always boot fresh with the pc0 breakpoint
bash "$ROOT/tools/emu/emu.sh" stop >/dev/null
DC2_BREAK=$(printf '0x%08x' "$PC0") bash "$ROOT/tools/emu/emu.sh" start
wait_until 120 bash -c "! curl -sf '$API/execution-flow' | grep -q '\"running\":true'" \
  || { echo "prove: pc0 breakpoint not hit in 120 s" >&2; exit 2; }
HIT=$(vs); (( HIT > 0 )) || { echo "prove: no vsync count at pc0" >&2; exit 2; }

fail=0
for k in 1 2 3; do
  if (( k > 1 )); then
    target=$(( HIT + (k - 1) * 300 ))
    flow resume
    wait_until 60 bash -c "(( \$(curl -sf '$API/lua/vsync' || echo -1) >= $target ))" \
      || { echo "prove: vsync $target not reached" >&2; exit 2; }
    flow pause
    wait_until 10 bash -c "! curl -sf '$API/execution-flow' | grep -q '\"running\":true'"
  fi
  snap="prove_${EXE}_$k"; v=$(vs)
  "$PY" "$ROOT/tools/emu/ram_probe.py" snapshot "$snap" >/dev/null
  diffs=$(cmp -l <(tail -c +2049 "$F" | head -c "$TSIZE") <(tail -c +$(( OFF + 1 )) "$ROOT/.run/ram/$snap.bin" | head -c "$TSIZE") || true)
  n=$(printf '%s' "$diffs" | grep -c . || true)
  if (( n == 0 )); then
    printf 'moment %d: vsync %s  EQUAL [0x%08x,0x%08x)\n' "$k" "$v" "$TADDR" $(( TADDR + TSIZE ))
  else
    fail=1
    words=$(printf '%s\n' "$diffs" | awk '{w=int(($1-1)/4)*4; if (w!=lw) {printf "+0x%x\n", w; lw=w}}')
    printf 'moment %d: vsync %s  MISMATCH %d bytes in %d words; first 40 word offsets from t_addr:\n' "$k" "$v" "$n" "$(wc -l <<<"$words")"
    head -40 <<<"$words" | paste -sd' ' -
  fi
done
flow resume || true
exit $fail
