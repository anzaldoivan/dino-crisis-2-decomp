#!/usr/bin/env bash
# push_check.sh -- the floor's game check (`[own] gate` of config/psxdecomp.toml, which the pre-push hook runs on the
# host) for a host that cannot build: the fleet check in the build container, on its own volume
# ${DC2_PUSH_VOLUME:-dc2-push}, so the router's dc2-work is never touched.
#   bash tools/docker/push_check.sh
# The working tree goes in by `dc.sh sync`, the host's Ghidra sidecars (.run/ghidra_functions: oracle_diff reads their
# references) by `dc.sh push`; then `dc.sh run bash tools/fleet_check.sh`. Exit status: the fleet check's.
set -euo pipefail
cd "$(git rev-parse --show-toplevel)"
export DC2_VOLUME="${DC2_PUSH_VOLUME:-dc2-push}"
if [ ! -d .run/ghidra_functions ]; then
    echo "push_check.sh: .run/ghidra_functions missing (tools/ghidra/dump_functions.sh writes it): the oracle pair cannot agree"
    exit 1
fi
bash tools/docker/dc.sh sync
bash tools/docker/dc.sh push .run/ghidra_functions
exec bash tools/docker/dc.sh run bash tools/fleet_check.sh
