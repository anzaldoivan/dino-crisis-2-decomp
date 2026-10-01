#!/usr/bin/env bash
# tools/ghidra/dump_functions.sh -- the Ghidra function-list oracle caches (Phase 1.5 T2.c1). Host only.
#
#   1. backs up ghidra/ to .run/ghidra_backup/ once (skipped when the backup exists);
#   2. imports every fleet program missing from the project via tools/ghidra/import_raw.sh at its loadmap base
#      (auto-analysis on); SLUS_012.79 is never re-imported or overwritten;
#   3. dumps every program read-only (DumpFunctions.java, -noanalysis -readOnly) and writes
#      config/ghidra/<prog>.functions.tsv (`start size source`, source=auto|txn; txn = recovered in a rolled-back
#      transaction from jal targets / reference targets / pointer words, see DumpFunctions.java) for all 83 binaries;
#   4. (T2.c3) copies the untracked evidence sidecars .run/ghidra_functions/<prog>.evidence.tsv into the build volume's
#      /work/.run/ghidra_functions/ (dc.sh sync keeps /work/.run; ignored files are never synced) for oracle_diff.py;
#      skipped with a note when docker or the volume is absent (oracle_diff then prints n/a evidence).
#      (T2.c5) also copies <prog>.refs.tsv (reference destinations + counts) and xprog_targets.tsv, read by
#      oracle_diff.py's `unreferenced` exception predicate; prints `merged labels: n` (DumpFunctions.java n3 merges).
# Before step 3 (T2.c4): tools/ghidra/xprog_targets.py writes .run/ghidra_functions/xprog_targets.tsv (`prog target`:
#   jal targets / pointer words in OTHER fleet programs' bytes whose loadmap window does not overlap prog's, landing in
#   prog's window), passed to DumpFunctions.java as the s4 seed file.
#
# Fleet = config/splat/*.yaml (alias = yaml stem, blob = its target_path, loadmap row = that path, class exe|code).
# Program name mapping (deterministic): alias slus_012_79 -> SLUS_012.79 (the exe keeps its name); every overlay
#   -> its census alias (yaml stem), e.g. bin_e00. A row without a base (BIN/WEP_S00.BIN) gets a `# no-text`
#   file with 0 rows and is never imported.
# Generator hash (header `# generator sha256=...`): sha256 over the bytes of tools/ghidra/scripts/DumpFunctions.java
#   then this file, then tools/ghidra/xprog_targets.py, then the text "<ghidra version>\n<loadmap sha1>\n<loadmap base>\n"; tools/oracle_diff.py
#   recomputes it and reports stale caches.
# PRECONDITION: nothing listening on :$DC2_GHIDRA_PORT (stop MCP: bash tools/ghidra/mcp_stop.sh).
set -uo pipefail
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
GHIDRA="${GHIDRA_INSTALL_DIR:-$HOME/ghidra_12.1.3_PUBLIC}"
PROJ_DIR="${DC2_GHIDRA_PROJ:-$REPO/ghidra}"; PROJ="dc2"
PORT="${DC2_GHIDRA_PORT:-8080}"
SCRIPTS="$REPO/tools/ghidra/scripts"
RAW="$REPO/.run/ghidra_functions"
OUT="$REPO/config/ghidra"
cd "$REPO"

if lsof -nP -iTCP:"$PORT" -sTCP:LISTEN >/dev/null 2>&1; then
    echo "dump_functions: ERROR -- a server is listening on :$PORT (project lock). Stop it first." >&2; exit 2
fi
[ -f "$PROJ_DIR/$PROJ.gpr" ] || { echo "dump_functions: ERROR -- no project at $PROJ_DIR/$PROJ.gpr" >&2; exit 2; }
VER="$(sed -n 's/^application.version=//p' "$GHIDRA/Ghidra/application.properties")"
[ -n "$VER" ] || { echo "dump_functions: ERROR -- no Ghidra version under $GHIDRA" >&2; exit 2; }

if [ ! -d .run/ghidra_backup ]; then
    mkdir -p .run && cp -R "$PROJ_DIR" .run/ghidra_backup && echo "dump_functions: backed up $PROJ_DIR -> .run/ghidra_backup"
fi

# fleet table: alias prog path sha1 base
FLEET=()
for y in config/splat/*.yaml; do
    alias="$(basename "$y" .yaml)"
    path="$(sed -n 's/^  target_path: extracted\/retail\/files\///p' "$y")"
    row="$(awk -F'\t' -v p="$path" '$1==p && ($4=="exe"||$4=="code") {print $3"\t"$6}' config/loadmap.tsv)"
    [ -n "$row" ] || { echo "dump_functions: ERROR -- $alias: no exe|code loadmap row for $path" >&2; exit 2; }
    prog="$alias"; [ "$alias" = slus_012_79 ] && prog="SLUS_012.79"
    FLEET+=("$alias	$prog	$path	$row")
done
echo "dump_functions: fleet ${#FLEET[@]} binaries, Ghidra $VER"

have() { grep -q ":$1:" "$PROJ_DIR/$PROJ.rep/idata/~index.dat" 2>/dev/null; }
imported=0; failed=0
for f in "${FLEET[@]}"; do
    IFS=$'\t' read -r alias prog path sha1 base <<<"$f"
    [ "$base" = "-" ] && continue
    [ "$prog" = "SLUS_012.79" ] && continue
    have "$prog" && continue
    bash tools/ghidra/import_raw.sh "extracted/retail/files/$path" "$base" "$prog" >".run/logs/import_$prog.log" 2>&1
    rc=$?
    if [ $rc -eq 0 ] && have "$prog"; then imported=$((imported + 1)); echo "dump_functions: imported $prog @ $base"
    else failed=$((failed + 1)); echo "dump_functions: IMPORT FAILED $prog rc=$rc (.run/logs/import_$prog.log)"; fi
done
echo "dump_functions: imported $imported, failed $failed"

rm -rf "$RAW"; mkdir -p "$RAW" "$OUT"
for f in "${FLEET[@]}"; do IFS=$'\t' read -r alias prog path _ <<<"$f"; printf '%s\t%s\n' "$prog" "$path"; done >"$RAW/fleet.tsv"
"${PY:-python3}" tools/ghidra/xprog_targets.py "$RAW/fleet.tsv" "$RAW/xprog_targets.tsv" || exit 2
"$GHIDRA/support/analyzeHeadless" "$PROJ_DIR" "$PROJ" -process -noanalysis -readOnly \
    -scriptPath "$SCRIPTS" -postScript DumpFunctions.java "$RAW" "$RAW/xprog_targets.tsv" >.run/logs/dump_functions_headless.log 2>&1
rc=$?
echo "dump_functions: analyzeHeadless exit=$rc ($(grep -c DC2DUMPFUNCS .run/logs/dump_functions_headless.log) programs dumped)"
echo "dump_functions: merged labels: $(grep 'DC2MERGE ' .run/logs/dump_functions_headless.log | grep -vc ' OPTION_BIN ') (fleet)"

written=0; missing=0
for f in "${FLEET[@]}"; do
    IFS=$'\t' read -r alias prog path sha1 base <<<"$f"
    hash="$({ cat "$SCRIPTS/DumpFunctions.java" "$REPO/tools/ghidra/dump_functions.sh" "$REPO/tools/ghidra/xprog_targets.py"; printf '%s\n%s\n%s\n' "$VER" "$sha1" "$base"; } | shasum -a 256 | cut -d' ' -f1)"
    dst="$OUT/$prog.functions.tsv"
    {
        echo "# config/ghidra/$prog.functions.tsv -- generated by tools/ghidra/dump_functions.sh (DumpFunctions.java); do not hand-edit."
        echo "# generator sha256=$hash ghidra=$VER loadmap_sha1=$sha1 base=$base alias=$alias"
        if [ "$base" = "-" ]; then
            echo "# no-text"
        elif [ -f "$RAW/$prog.raw.tsv" ]; then
            cat "$RAW/$prog.raw.tsv" | sed -n '1p'
        fi
        echo "# start	size	source"
        [ "$base" != "-" ] && [ -f "$RAW/$prog.raw.tsv" ] && sed '1d' "$RAW/$prog.raw.tsv"
    } >"$dst.tmp"
    if [ "$base" != "-" ] && [ ! -f "$RAW/$prog.raw.tsv" ]; then
        rm -f "$dst.tmp"; missing=$((missing + 1)); echo "dump_functions: MISSING dump for $prog"; continue
    fi
    mv "$dst.tmp" "$dst"; written=$((written + 1))
done
echo "dump_functions: wrote $written of ${#FLEET[@]} caches, missing $missing"

VOL="${DC2_VOLUME:-dc2-work}"
if command -v docker >/dev/null 2>&1 && docker volume inspect "$VOL" >/dev/null 2>&1; then
    (cd "$RAW" && COPYFILE_DISABLE=1 tar --no-xattrs --no-mac-metadata -cf - *.evidence.tsv *.refs.tsv xprog_targets.tsv) \
      | docker run -i --rm --platform linux/amd64 -v "$VOL":/work dc2-build \
          sh -c 'rm -rf /work/.run/ghidra_functions && mkdir -p /work/.run/ghidra_functions && tar -xf - -C /work/.run/ghidra_functions' \
      && echo "dump_functions: evidence sidecars copied to volume $VOL:/work/.run/ghidra_functions ($(ls "$RAW"/*.evidence.tsv | wc -l | tr -d ' '))"
else
    echo "dump_functions: evidence sidecars NOT copied (no docker or no volume $VOL)"
fi
[ $rc -eq 0 ] && [ $failed -eq 0 ] && [ $missing -eq 0 ]
