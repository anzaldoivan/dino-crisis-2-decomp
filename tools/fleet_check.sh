#!/usr/bin/env bash
# fleet_check.sh — whole-fleet byte gate, run in the container (docs/ops/docker-host.md "Fleet check"):
#   dc.sh run bash tools/fleet_check.sh
# make clean → make extract OUT=.run/extracted/retail (from /disc) → make -j$(nproc) -k build (BASEDIR default)
# → prints `<k> of <N> byte-identical`; exit 0 iff k == N. N = config/loadmap.tsv rows with class exe|code;
# exit 1 also if the count of config/check.*.sha != N or any build/<alias>.bin is missing.
# T6: then writes .run/harness/clean_run.tsv (alias sha1) and runs `python3 tools/harness.py ${HARNESS_ARGS:-}`;
# exit non-zero also when the harness rc != 0.
set -uo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."
t0=$(date +%s)
export DC2_CUE="${DC2_CUE:-/disc/Dino Crisis 2 (USA).cue}"
n=$(awk -F'\t' '!/^#/ && ($4 == "exe" || $4 == "code")' config/loadmap.tsv | wc -l)
shas=(config/check.*.sha)
make clean >/dev/null || { echo "FAILED make clean"; exit 1; }
make extract OUT=.run/extracted/retail >.run/fleet_extract.log 2>&1 || { tail -5 .run/fleet_extract.log; echo "FAILED extract"; exit 1; }
make -j"$(nproc)" -k build >.run/fleet_build.log 2>&1
grep FAILED .run/fleet_build.log
k=0; missing=0
for s in "${shas[@]}"; do
  a=${s#config/check.}; a=${a%.sha}
  if [ ! -f "build/$a.bin" ]; then missing=$((missing + 1)); continue; fi
  (cd build && sha1sum --status -c "../$s") && k=$((k + 1))
done
echo "$k of $n byte-identical"
# T6: the clean run's hashes (harness fleet pair, side A), then the differential harness (tools/harness.py)
mkdir -p .run/harness
for s in "${shas[@]}"; do
  a=${s#config/check.}; a=${a%.sha}
  [ -f "build/$a.bin" ] && printf '%s\t%s\n' "$a" "$(sha1sum "build/$a.bin" | cut -d' ' -f1)"
done > .run/harness/clean_run.tsv
th=$(date +%s)
# shellcheck disable=SC2086  # HARNESS_ARGS is a flag list
python3 tools/harness.py ${HARNESS_ARGS:-}
hrc=$?
echo "elapsed $(( $(date +%s) - t0 )) s (check.*.sha ${#shas[@]}, missing bin $missing), harness $(( $(date +%s) - th )) s"
[ "${#shas[@]}" -eq "$n" ] && [ "$missing" -eq 0 ] && [ "$k" -eq "$n" ] && [ "$hrc" -eq 0 ]
