# Docker build host (amd64 on arm64 Mac)

Facts recorded 2026-09-30 (T2.c2). Entry point: `tools/docker/dc.sh`; image recipe `tools/docker/Dockerfile`.

## Host
- Docker Desktop 4.93.0 (240920); engine client/server 29.8.1; VM kernel 7.0.14-linuxkit, aarch64, storage driver overlayfs.
- VM resources: 8 CPUs, 8319770624 B (~7.75 GiB) memory (`docker info`).
- amd64 emulation backend: not determinable. `docker info` names none; Desktop `settings-store.json` sets no Rosetta/virtualization key (defaults apply). Containers report `uname -m` = `x86_64`.

## Image
- Name `dc2-build:latest`, id `sha256:eeae66b2b56daafeb870662b82e6a0ebc124040171e37ea1647a3d7ee24ce950`, arch amd64.
- Base `ubuntu:24.04@sha256:008173c23f95b170204355c12626cb5a965d779a7e1283b09e9cffbb1bf33ca3` (multi-arch index digest; `Dockerfile:4`).
  - Source: `docker buildx imagetools inspect ubuntu:24.04`, 2026-09-30; amd64 manifest `sha256:496754492fb28b4d3049432f2ca787449331e23fb14f0dd3fffea86bf5a93eb4`, created 2026-09-11, source git.launchpad.net/cloud-images/+oci/ubuntu-base.
- Build wall time `--no-cache`: 98 s (T2.c1, `.run/logs/dc2-build.log`).
- Package versions (`dpkg-query -W` in the image):
  - gcc-mipsel-linux-gnu 4:12.2.0-4 (reports gcc 12.4.0-2ubuntu1~24.04)
  - cpp-mipsel-linux-gnu 4:12.2.0-4
  - binutils-mipsel-linux-gnu 2.42-2ubuntu1cross5
  - python3 3.12.3-0ubuntu2.1
  - make 4.3-4.1build2
  - clang-format 1:18.0-59~exp2
- T1.c1 (phase 1.1) adds apt `cmake g++` and builds `dumpsxiso` (mkpsxiso) from source: `https://github.com/Lameguy64/mkpsxiso`
  tag `v2.30` (ARG `MKPSXISO_TAG`), commit `54fb1644ed8741223583e2dcda358b75a205e214` (ARG `MKPSXISO_SHA`, from
  `git ls-remote … refs/tags/v2.30^{}`; build fails if `git rev-parse HEAD` differs); installed `/usr/local/bin/dumpsxiso`, source removed.
  The image id above predates this layer.

## Volume and sync
- Named volume `dc2-work` (override `DC2_VOLUME`), mounted at `/work`. dc.sh touches no other volume.
- No bind mount of any host path, ever; never mounts `/Users/Shared/GameInputs`. Game data lives only in the local volume `dc2-disc`, never in the image or git.
- `dc.sh sync`: `git ls-files -co --exclude-standard -z` (paths deleted in the working tree skipped) | `COPYFILE_DISABLE=1 tar` over stdin into a container that wipes every top-level entry of `/work` except `/work/.run`, then extracts. `/work` is thus an exact copy of tracked + untracked-unignored files, plus the container scratch `/work/.run`.
- Run `dc.sh sync` before any `dc.sh run` after host edits.
- Limits: ignored paths (`disks/`, `extracted/`, RE database, host `.run/`) are never carried. `/work/.git` is not carried. Container-side edits outside `/work/.run` are lost on the next sync.

## Disc volume
- Named volume `dc2-disc` (override `DC2_DISC_VOLUME`); `dc.sh run` mounts it read-only at `/disc`.
- Loaded with `bash tools/docker/dc.sh disc /Users/Shared/GameInputs/dino-crisis-2/usa`: top-level `*.cue` + `*.bin` of the dir (no `.DS_Store`), `COPYFILE_DISABLE=1 tar` over stdin into a container that wipes then fills the volume. No bind mount. Reload only when the dump changes.
- Reference extract (T1.c1; output in `/work/.run/ref`, kept across syncs, never in git):
  `bash tools/run.sh t1-ref -- bash tools/docker/dc.sh run sh -c 'rm -rf /work/.run/ref && mkdir -p /work/.run/ref && dumpsxiso -x /work/.run/ref/files -s /work/.run/ref/layout.xml "/disc/Dino Crisis 2 (USA) (Track 1).bin" && mv /work/.run/ref/files/license_data.dat /work/.run/ref/files/ZNULL.WAV /work/.run/ref/'`
- harness: macOS tar still emits `LIBARCHIVE.xattr.com.apple.provenance` headers; GNU tar in the container prints "Ignoring unknown extended header keyword" per file. Harmless noise.

## Use
- `bash tools/docker/dc.sh build` · `bash tools/docker/dc.sh sync` · `bash tools/docker/dc.sh disc <dir>` · `bash tools/docker/dc.sh run <cmd…>` (exit code passed through).
