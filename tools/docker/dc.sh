#!/usr/bin/env bash
# dc.sh — the one entrypoint to the amd64 build host (see docs/ops/docker-host.md).
#   dc.sh build [docker-build args…]  build image dc2-build from tools/docker/Dockerfile
#   dc.sh sync                        replace /work on the volume with the working tree
#                                     (tracked + untracked-unignored; ignored data never carried)
#   dc.sh run <cmd…>                  run <cmd…> in /work on the volume; exit code passed through
# Volume: ${DC2_VOLUME:-dc2-work}. No bind mounts, ever. Touches no other volume.
set -euo pipefail

cd "$(git -C "$(dirname "${BASH_SOURCE[0]}")" rev-parse --show-toplevel)"

VOL="${DC2_VOLUME:-dc2-work}"
IMG=dc2-build
PLAT=linux/amd64

usage() { echo "usage: dc.sh build [docker-build args…] | sync | run <cmd…>" >&2; exit 2; }

[ $# -ge 1 ] || usage
sub="$1"; shift
case "$sub" in
  build)
    exec docker build --platform "$PLAT" -t "$IMG" "$@" -f tools/docker/Dockerfile tools/docker
    ;;
  sync)
    docker volume inspect "$VOL" >/dev/null 2>&1 || docker volume create "$VOL" >/dev/null
    git ls-files -co --exclude-standard -z \
      | COPYFILE_DISABLE=1 tar --null -T - -cf - \
      | docker run -i --rm --platform "$PLAT" -v "$VOL":/work -w /work "$IMG" \
          sh -c 'find /work -mindepth 1 -maxdepth 1 -exec rm -rf {} + && tar -xf - -C /work'
    ;;
  run)
    [ $# -ge 1 ] || usage
    exec docker run --rm --platform "$PLAT" -v "$VOL":/work -w /work "$IMG" "$@"
    ;;
  *) usage ;;
esac
