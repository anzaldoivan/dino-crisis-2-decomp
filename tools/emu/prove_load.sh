#!/usr/bin/env bash
# tools/emu/prove_load.sh <path> — prove a payload sits in RAM at its base (path relative to extracted/retail/files,
# as in config/loadmap.evidence.tsv; `tools/loadmap_evidence.py --proven-paths` lists them). Exe or a fixed overlay
# table below; unknown path → exit 2.
# Exe (SLUS_012.79): prove a PS-X EXE sits in RAM at its header t_addr.
# Reads pc0 @0x10, t_addr @0x18, t_size @0x1C of extracted/retail/files/<exe>; (re)boots the emulator with a
# pausing breakpoint at pc0 (the load-complete moment) and snapshots RAM at 3 moments: pc0 hit, +300, +600 vsyncs.
# Moment 1 (pc0): RAM[t_addr..t_addr+t_size) == file[0x800..0x800+t_size), full range, no masking.
# Moments 2,3: RAM[t_addr..TEXT_END) word-wise (data past .text mutates once main runs), with exactly two exclusions:
# libcard _patch_card_info / _patch_card2 rewrite their own code; differing words there are printed, not failures.
# Ranges and evidence: docs/memory-map.md#slus_01279. Stops the emulator at the end so runs are independent.
# Overlays (T5; docs/memory-map.md#<anchor> per overlay): file[0..size) vs RAM[base..base+size).
#   No breakpoint (ST1, WEP01; no pad input): 3 moments at fixed vsyncs inside the observed resident window, each a
#   FULL compare, no exclusions. Breakpoint (OPTION): pad script tools/emu/lua/pad.lua reaches it; moment 1 = pausing
#   breakpoint at its entry (load moment), FULL compare; moments 2,3 = entry hit +300, +600 vsyncs, [base,TEXT_END)
#   word-wise, no exclusions (TEXT_END = TextExtent.java MAX_FUNC_END on a raw import, tools/ghidra/import_raw.sh).
# exit 0 all 3 pass; 1 mismatch (differing offsets printed); 2 setup error. Snapshots: .run/ram/prove_<file>_<k>.bin.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
PY="${PY:-/opt/homebrew/opt/python@3.14/bin/python3.14}"
P="${1:?usage: prove_load.sh <path>}"; F="$ROOT/extracted/retail/files/$P"; EXE="$(basename "$P")"
API="http://127.0.0.1:${DC2_EMU_PORT:-8081}/api/v1"
[[ -f "$F" ]] || { echo "prove: no $F" >&2; exit 2; }
word() { od -A n -t u4 -j "$1" -N 4 "$F" | tr -d ' '; }
EXCL=(); AT=(); BREAK=""; PAD=""
case "$P" in
  SLUS_012.79)
    PC0=$(word 16); TADDR=$(word 24); TSIZE=$(word 28); FOFF=0x800; BREAK=$(printf '0x%08x' "$PC0")
    printf 'prove: %s pc0=0x%08x t_addr=0x%08x t_size=0x%x\n' "$EXE" "$PC0" "$TADDR" "$TSIZE"
    # .text end of SLUS_012.79 (binding, phase-ends/current/tasks/T3.md; tools/ghidra TextExtent.java)
    TEXT_END=0x80085f74
    # self-modifying libcard code, [lo,hi) absolute (T3 Findings; names from the T1 PsyQ 4.7 signature hits)
    EXCL=(0x8007e3c4:0x8007e408:_patch_card_info 0x8007e534:0x8007e5a4:_patch_card2) ;;
  # route R1; attract demo (no input) keeps ST1 resident vsync 5240-10717 (T5.c1 run A); docs/memory-map.md#ovl-st1
  PSX/BIN/ST1.BIN) TADDR=0x800d5800; AT=(6000 8000 10000) ;;
  # route R3; attract demo: WEP01 first/last seen 5399/19950 (T5.c1 run A) but WEP07 holds 0x8017e500 ~10951-16639
  # (vsync 12010 mismatched, T5.c2), so moments sit in the ST1 demo span; docs/memory-map.md#ovl-wep01
  BIN/WEP01.BIN) TADDR=0x8017e500; AT=(6000 8000 10000) ;;
  # route R2 (k=0); title menu DOWN, DOWN, CROSS → OPTION exec at its entry; docs/memory-map.md#ovl-option
  BIN/OPTION.BIN) TADDR=0x801c1500; BREAK=0x801c15f4; TEXT_END=0x801c4858; PAD="1900:DOWN 1920:DOWN 1950:CROSS" ;;
  *) echo "prove: no proof recipe for $P (exe or overlay table only)" >&2; exit 2 ;;
esac
if [[ "$P" != SLUS_012.79 ]]; then
  FOFF=0; TSIZE=$(wc -c < "$F" | tr -d ' ')
  printf 'prove: %s base=0x%08x size=0x%x%s\n' "$P" "$TADDR" "$TSIZE" "${BREAK:+ entry=$BREAK}"
fi
OFF=$(( TADDR - 0x80000000 ))
(( TADDR >= 0x80000000 && OFF + TSIZE <= 0x200000 )) || { echo "prove: range out of RAM" >&2; exit 2; }
[[ "$P" != SLUS_012.79 ]] || (( TSIZE % 2048 == 0 )) || { echo "prove: header out of range" >&2; exit 2; }

vs() { local v; for _ in 1 2 3 4 5 6 7 8; do v=$(curl -sf "$API/lua/vsync") && { echo "$v"; return; }; sleep 0.25; done; echo -1; }
running() { curl -sf "$API/execution-flow" | grep -q '"running":true'; }
flow() { curl -sf -o /dev/null -X POST "$API/execution-flow?function=$1"; }
wait_until() {  # wait_until <max-s> <cmd...>
  local t=$1; shift; for _ in $(seq 1 $(( t * 4 ))); do "$@" && return 0; sleep 0.25; done; return 1; }

# the load moment exists only once per boot: always boot fresh (with the breakpoint, if any)
bash "$ROOT/tools/emu/emu.sh" stop >/dev/null
rm -f "$ROOT/.run/emu/pad.log"
DC2_BREAK="$BREAK" DC2_PAD="$PAD" DC2_LUA="${PAD:+tools/emu/lua/pad.lua}" bash "$ROOT/tools/emu/emu.sh" start
if [[ -n $BREAK ]]; then
  wait_until 120 bash -c "! curl -sf '$API/execution-flow' | grep -q '\"running\":true'" \
    || { echo "prove: breakpoint $BREAK not hit in 120 s" >&2; exit 2; }
  HIT=$(vs); (( HIT > 0 )) || { echo "prove: no vsync count at breakpoint" >&2; exit 2; }
  AT=("$HIT" $(( HIT + 300 )) $(( HIT + 600 )))
fi

fail=0
for k in 1 2 3; do
  if (( k > 1 )) || [[ -z $BREAK ]]; then
    target=${AT[k-1]}
    flow resume
    wait_until $(( (target - $(vs)) / 30 + 30 )) bash -c "(( \$(curl -sf '$API/lua/vsync' || echo -1) >= $target ))" \
      || { echo "prove: vsync $target not reached" >&2; exit 2; }
    flow pause
    wait_until 10 bash -c "! curl -sf '$API/execution-flow' | grep -q '\"running\":true'"
  fi
  snap="prove_${EXE}_$k"; v=$(vs)
  "$PY" "$ROOT/tools/emu/ram_probe.py" snapshot "$snap" >/dev/null
  if (( k == 1 )) || [[ -z $BREAK ]]; then len=$TSIZE; else len=$(( TEXT_END - TADDR )); fi
  diffs=$(cmp -l <(tail -c +$(( FOFF + 1 )) "$F" | head -c "$len") <(tail -c +$(( OFF + 1 )) "$ROOT/.run/ram/$snap.bin" | head -c "$len") || true)
  words=$(printf '%s\n' "$diffs" | awk 'NF{w=int(($1-1)/4)*4; if (w!=lw) {print w; lw=w}}')
  bad=(); ex=()
  for w in $words; do
    a=$(( TADDR + w )); hit=""
    if (( k > 1 )); then for e in ${EXCL[@]+"${EXCL[@]}"}; do IFS=: read -r lo hi nm <<<"$e"
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
