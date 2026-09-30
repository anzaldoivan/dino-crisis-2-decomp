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

## Volume and sync
- Named volume `dc2-work` (override `DC2_VOLUME`), mounted at `/work`. dc.sh touches no other volume.
- No bind mount of any host path, ever; never mounts `/Users/Shared/GameInputs`. No game data enters the image or volume.
- `dc.sh sync`: `git ls-files -co --exclude-standard -z` | `COPYFILE_DISABLE=1 tar` over stdin into a container that first wipes `/work` (all top-level entries), then extracts. `/work` is thus an exact copy of tracked + untracked-unignored files.
- Limits: ignored paths (`disks/`, `extracted/`, RE database, `.run/`) are never carried; data inside the container is out of scope until phase 1.1. `/work/.git` is not carried. Container-side edits are lost on the next sync.
- harness: macOS tar still emits `LIBARCHIVE.xattr.com.apple.provenance` headers; GNU tar in the container prints "Ignoring unknown extended header keyword" per file. Harmless noise.

## Use
- `bash tools/docker/dc.sh build` · `bash tools/docker/dc.sh sync` · `bash tools/docker/dc.sh run <cmd…>` (exit code passed through).
