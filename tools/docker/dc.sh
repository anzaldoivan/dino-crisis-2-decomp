#!/usr/bin/env bash
# dc.sh — the one entrypoint to the amd64 build host (see docs/ops/docker-host.md).
#   dc.sh build [docker-build args…]  build image dc2-build from tools/docker/Dockerfile
#   dc.sh sync                        replace /work on the volume with the working tree
#                                     (tracked + untracked-unignored, deleted skipped; ignored never carried);
#                                     /work/.run (container scratch, e.g. the reference extract) is kept
#   dc.sh disc <dir>                  replace volume ${DC2_DISC_VOLUME:-dc2-disc} with <dir>'s top-level
#                                     *.cue + *.bin (piped over stdin; machine-local game data)
#   dc.sh run <cmd…>                  run <cmd…> in /work on the volume, disc volume read-only at /disc;
#                                     exit code passed through
# Volumes: ${DC2_VOLUME:-dc2-work}, ${DC2_DISC_VOLUME:-dc2-disc}. No bind mounts, ever. Touches no other volume.
set -euo pipefail

cd "$(git -C "$(dirname "${BASH_SOURCE[0]}")" rev-parse --show-toplevel)"

VOL="${DC2_VOLUME:-dc2-work}"
DISC_VOL="${DC2_DISC_VOLUME:-dc2-disc}"
IMG=dc2-build
PLAT=linux/amd64

usage() { echo "usage: dc.sh build [docker-build args…] | sync | disc <dir> | run <cmd…>" >&2; exit 2; }

[ $# -ge 1 ] || usage
sub="$1"; shift
case "$sub" in
  build)
    # context stays tools/docker (repo root holds ignored game data); `cfg` = config/ for toolchains.tsv
    exec docker build --platform "$PLAT" -t "$IMG" --build-context cfg=config "$@" -f tools/docker/Dockerfile tools/docker
    ;;
  sync)
    docker volume inspect "$VOL" >/dev/null 2>&1 || docker volume create "$VOL" >/dev/null
    git ls-files -co --exclude-standard -z \
      | while IFS= read -r -d '' f; do [ -e "$f" ] && printf '%s\0' "$f"; done \
      | COPYFILE_DISABLE=1 tar --no-xattrs --no-mac-metadata --null -T - -cf - \
      | docker run -i --rm --platform "$PLAT" -v "$VOL":/work -w /work "$IMG" \
          sh -c 'find /work -mindepth 1 -maxdepth 1 ! -name .run -exec rm -rf {} + && tar -xf - -C /work'
    ;;
  disc)
    [ $# -eq 1 ] && [ -d "$1" ] || usage
    docker volume inspect "$DISC_VOL" >/dev/null 2>&1 || docker volume create "$DISC_VOL" >/dev/null
    (cd "$1/" && find . -maxdepth 1 -type f \( -name '*.cue' -o -name '*.bin' \) ! -name .DS_Store -print0 \
      | COPYFILE_DISABLE=1 tar --null -T - -cf -) \
      | docker run -i --rm --platform "$PLAT" -v "$DISC_VOL":/disc -w /disc "$IMG" \
          sh -c 'find /disc -mindepth 1 -maxdepth 1 -exec rm -rf {} + && tar -xf - -C /disc && ls -l /disc'
    ;;
  run)
    [ $# -ge 1 ] || usage
    exec docker run --rm --platform "$PLAT" -v "$VOL":/work -v "$DISC_VOL":/disc:ro -w /work "$IMG" "$@"
    ;;
  *) usage ;;
esac
