#!/usr/bin/env bash
# tools/ghidra/rebuild.sh <program> [--proof] [--keep]
#
# Rebuild ONE Ghidra program FROM TEXT + the disc, in a scratch project (build/ghidra_rebuild/proj), and
# prove it (adapted from the kit's P2 ghidra_rebuild.sh, without its build/splat steps): the committed
# config/ghidra/<program>.jsonl (ExportAnnotations.java's delta format) is the hand-authored RE work;
# everything else — the bytes, auto-analysis, the function set, the PsyQ signature names — comes from the
# extracted payload, Ghidra and psx_ldr. Makes the Ghidra database regenerable (it embeds the program bytes).
#
# ONE analyzeHeadless run, ONE analysis pass (cookbook C0004), postScripts in order:
#   1. import   the PS-X EXE via psx_ldr + auto-analysis (PsyQ Signatures analyzer) + ImportPsyqGdt.java
#               + StorePsyqSigHits.java — the same as tools/ghidra/import.sh
#   2. baseline ExportAnnotations.java -> .run/ghidra_rebuild/<program>.baseline.jsonl   (nothing hand-made yet)
#   3. import   ImportAnnotations.java config/ghidra/<program>.jsonl   (when the file exists)
#   4. export   -> .run/ghidra_rebuild/<program>.after.jsonl; delta(after, baseline) -> <program>.delta.jsonl
#   --proof     cmp <program>.delta.jsonl config/ghidra/<program>.jsonl -> "PROOF PASS" (exit 0) or FAIL (exit 1).
#   Without a committed file, steps 3-4 are skipped and the delta of the LIVE export (.run/ghidra_export/<program>.jsonl,
#   from tools/ghidra/export.sh) against the baseline is written to <program>.candidate.jsonl — the file to
#   review and commit as config/ghidra/<program>.jsonl.
#
# PRECONDITION: no agent server on :$DC2_GHIDRA_PORT; the extracted payload exists. The live project (ghidra/)
# is never touched. --keep leaves the scratch project for inspection (the next run wipes it).
set -uo pipefail
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$REPO"
PROG="${1:?usage: rebuild.sh <program> [--proof] [--keep]}"; shift
PROOF=0; KEEP=0
for a in "$@"; do case "$a" in --proof) PROOF=1 ;; --keep) KEEP=1 ;; *) echo "rebuild: unknown arg $a" >&2; exit 2 ;; esac; done
GHIDRA="${GHIDRA_INSTALL_DIR:-$HOME/ghidra_12.1.3_PUBLIC}"
HEADLESS="$GHIDRA/support/analyzeHeadless"
PY="${PY:-/opt/homebrew/opt/python@3.14/bin/python3.14}"
PORT="${DC2_GHIDRA_PORT:-8080}"
SCRIPTS="$REPO/tools/ghidra/scripts"
SCR="$REPO/.run/ghidra_rebuild"          # exports, deltas, logs (plain files)
# Ghidra refuses a project path with a component starting with '.', so the scratch PROJECT lives under
# the gitignored build/ tree.
PROJ_DIR="$REPO/build/ghidra_rebuild/proj"; PROJ="dc2"
CONF="${DC2_GHIDRA_ANN:-$REPO/config/ghidra/$PROG.jsonl}"   # override: negative controls only
LIVE="$REPO/.run/ghidra_export/$PROG.jsonl"
say() { printf 'rebuild[%s]: %s\n' "$PROG" "$*"; }
die() { printf 'rebuild[%s]: %s\n' "$PROG" "$*" >&2; exit 1; }
[ -x "$HEADLESS" ] || die "no analyzeHeadless at $HEADLESS (GHIDRA_INSTALL_DIR)"
if lsof -nP -iTCP:"$PORT" -sTCP:LISTEN >/dev/null 2>&1; then die "a server is listening on :$PORT — stop it first"; fi

# ---- what is this program? ----
case "$PROG" in
    SLUS_012.79) EXE=extracted/retail/Files/SLUS_012.79 ;;
    *) die "unknown program $PROG (only PS-X EXEs with a known payload path)" ;;
esac
[ -f "$EXE" ] || die "payload not found for $PROG ($EXE) — extract the disc first"

# ---- scratch project ----
rm -rf "$PROJ_DIR"; mkdir -p "$PROJ_DIR" "$SCR"
rm -f "$SCR/$PROG.baseline.jsonl" "$SCR/$PROG.after.jsonl" "$SCR/$PROG.delta.jsonl"
LOG="$SCR/$PROG.headless.log"
POST="-postScript ImportPsyqGdt.java -postScript StorePsyqSigHits.java -postScript ExportAnnotations.java $SCR/$PROG.baseline.jsonl"
[ -f "$CONF" ] && POST="$POST -postScript ImportAnnotations.java $CONF -postScript ExportAnnotations.java $SCR/$PROG.after.jsonl"

say "import + analysis + sigs/gdt + baseline export$([ -f "$CONF" ] && echo " + ImportAnnotations + export") (one headless run)"
t0=$(date +%s)
# shellcheck disable=SC2086  # POST is a word list of fixed, space-free paths
"$HEADLESS" "$PROJ_DIR" "$PROJ" -import "$EXE" -overwrite -scriptPath "$SCRIPTS" $POST >"$LOG" 2>&1
rc=$?
say "analyzeHeadless exit=$rc in $(( $(date +%s) - t0 ))s (log $LOG)"
grep -E 'SIG_HITS|DC2EXPORT|DC2ANN|ERROR|Exception' "$LOG" | sed -e 's/^.*\.java> //' -e 's/ (GhidraScript) *$//' | head -20
[ "$rc" -eq 0 ] || die "analyzeHeadless failed"
[ -s "$SCR/$PROG.baseline.jsonl" ] || die "no baseline export"

if [ ! -f "$CONF" ]; then
    [ -f "$LIVE" ] || die "no $CONF and no live export $LIVE (run tools/ghidra/export.sh $PROG first)"
    "$PY" tools/ghidra/delta.py "$LIVE" "$SCR/$PROG.baseline.jsonl" "$SCR/$PROG.candidate.jsonl" --census
    say "no committed file — CANDIDATE written: $SCR/$PROG.candidate.jsonl (review, then cp to $CONF)"
    [ "$KEEP" = 1 ] || rm -rf "$PROJ_DIR"
    exit 0
fi

# a per-row failure inside a rc-0 run is still a failure — a proof over a partial import proves nothing
grep -q 'DC2ANN .*failed=0 ' "$LOG" || die "ImportAnnotations reported failures (or no DC2ANN line) — see $LOG"
[ -s "$SCR/$PROG.after.jsonl" ] || die "no after-import export"
"$PY" tools/ghidra/delta.py "$SCR/$PROG.after.jsonl" "$SCR/$PROG.baseline.jsonl" "$SCR/$PROG.delta.jsonl" --census
if [ "$PROOF" = 1 ]; then
    if cmp -s "$SCR/$PROG.delta.jsonl" "$CONF"; then
        say "PROOF PASS — the rebuilt program's hand-authored delta == $CONF ($(wc -l < "$CONF" | tr -d ' ') rows)"
        printf 'PASS %s config-sha1 %s\n' "$(date -u +%Y-%m-%dT%H:%MZ)" "$(shasum < "$CONF" | cut -c1-12)" > "$SCR/$PROG.proof"
        [ "$KEEP" = 1 ] || rm -rf "$PROJ_DIR"
        exit 0
    fi
    say "PROOF FAIL — delta differs from $CONF (diff below, first 40 lines; scratch kept in $SCR)"
    printf 'FAIL %s config-sha1 %s\n' "$(date -u +%Y-%m-%dT%H:%MZ)" "$(shasum < "$CONF" | cut -c1-12)" > "$SCR/$PROG.proof"
    diff "$CONF" "$SCR/$PROG.delta.jsonl" | head -40
    exit 1
fi
[ "$KEEP" = 1 ] || rm -rf "$PROJ_DIR"
say "done (no --proof requested)"
