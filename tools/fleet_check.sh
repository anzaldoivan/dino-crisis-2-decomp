#!/usr/bin/env bash
# fleet_check.sh — whole-fleet byte gate, run in the container (docs/ops/docker-host.md "Fleet check"):
#   dc.sh run bash tools/fleet_check.sh
# make clean → make extract OUT=.run/extracted/retail (from /disc) → make -j$(nproc) -k build (BASEDIR default)
# → prints `<k> of <N> byte-identical`; exit 0 iff k == N. N = config/loadmap.tsv rows with class exe|code;
# exit 1 also if the count of config/check.*.sha != N or any build/<alias>.bin is missing.
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
echo "elapsed $(( $(date +%s) - t0 )) s (check.*.sha ${#shas[@]}, missing bin $missing)"
[ "${#shas[@]}" -eq "$n" ] && [ "$missing" -eq 0 ] && [ "$k" -eq "$n" ]
