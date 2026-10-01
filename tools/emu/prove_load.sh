#!/usr/bin/env bash
# tools/emu/prove_load.sh <exe-name> — prove a PS-X EXE sits in RAM at its header t_addr.
# Reads pc0 @0x10, t_addr @0x18, t_size @0x1C of extracted/retail/files/<exe>; (re)boots the emulator with a
# pausing breakpoint at pc0 (the load-complete moment) and snapshots RAM at 3 moments: pc0 hit, +300, +600 vsyncs.
# Moment 1 (pc0): RAM[t_addr..t_addr+t_size) == file[0x800..0x800+t_size), full range, no masking.
# Moments 2,3: RAM[t_addr..TEXT_END) word-wise (data past .text mutates once main runs), with exactly two exclusions:
# libcard _patch_card_info / _patch_card2 rewrite their own code; differing words there are printed, not failures.
# Ranges and evidence: docs/memory-map.md#slus_01279. Stops the emulator at the end so runs are independent.
# exit 0 all 3 pass; 1 mismatch (differing offsets printed); 2 setup error. Snapshots: .run/ram/prove_<exe>_<k>.bin.
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
# .text end of SLUS_012.79 (binding, phase-ends/current/tasks/T3.md; tools/ghidra TextExtent.java)
TEXT_END=0x80085f74
# self-modifying libcard code, [lo,hi) absolute (T3 Findings; names from the T1 PsyQ 4.7 signature hits)
EXCL=(0x8007e3c4:0x8007e408:_patch_card_info 0x8007e534:0x8007e5a4:_patch_card2)
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
  if (( k == 1 )); then len=$TSIZE; else len=$(( TEXT_END - TADDR )); fi
  diffs=$(cmp -l <(tail -c +2049 "$F" | head -c "$len") <(tail -c +$(( OFF + 1 )) "$ROOT/.run/ram/$snap.bin" | head -c "$len") || true)
  words=$(printf '%s\n' "$diffs" | awk 'NF{w=int(($1-1)/4)*4; if (w!=lw) {print w; lw=w}}')
  bad=(); ex=()
  for w in $words; do
    a=$(( TADDR + w )); hit=""
    if (( k > 1 )); then for e in "${EXCL[@]}"; do IFS=: read -r lo hi nm <<<"$e"
      (( a >= lo && a < hi )) && hit=$nm; done; fi
    if [[ -n $hit ]]; then ex+=("$(printf '+0x%x(0x%08x,%s)' "$w" "$a" "$hit")"); else bad+=("$(printf '+0x%x(0x%08x)' "$w" "$a")"); fi
  done
  verdict=EQUAL; (( ${#ex[@]} )) && verdict="EQUAL-EXCL"; (( ${#bad[@]} )) && { verdict=MISMATCH; fail=1; }
  printf 'moment %d: vsync %s  %s [0x%08x,0x%08x) bad=%d excluded=%d\n' "$k" "$v" "$verdict" "$TADDR" $(( TADDR + len )) "${#bad[@]}" "${#ex[@]}"
  (( ${#bad[@]} )) && printf '  differing (first 40, +t_addr(abs)): %s\n' "$(printf '%s\n' "${bad[@]}" | head -40 | paste -sd' ' -)"
  (( ${#ex[@]} )) && printf '  excluded (+t_addr(abs,fn)): %s\n' "$(printf '%s\n' "${ex[@]}" | paste -sd' ' -)"
done
flow resume || true
bash "$ROOT/tools/emu/emu.sh" stop >/dev/null || true
echo "prove: $( (( fail )) && echo FAIL || echo PASS )"
exit $fail
