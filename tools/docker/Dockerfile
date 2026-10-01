# Build host for a PlayStation / PsyQ matching decompilation (x86-64 Linux, Ubuntu 24.04).
# Use it when the host is not x86-64 Linux, for example an arm64 Mac.
# Keep the source tree in a named volume. Do not build on a host bind mount.
FROM ubuntu:24.04@sha256:008173c23f95b170204355c12626cb5a965d779a7e1283b09e9cffbb1bf33ca3
ENV DEBIAN_FRONTEND=noninteractive LANG=C.UTF-8
RUN apt-get update && apt-get install -y --no-install-recommends \
      bchunk binutils-mipsel-linux-gnu bsdmainutils clang-format coreutils curl gcc-mipsel-linux-gnu \
      cpp-mipsel-linux-gnu git libelf-dev make ninja-build p7zip-full python3 python3-pip python3-venv \
      python3-dev build-essential unzip wget file xxd ca-certificates time cmake g++ \
    && rm -rf /var/lib/apt/lists/*
RUN git config --global --add safe.directory '*'
# dumpsxiso (mkpsxiso): reference extractor of the disc layout. Tag pinned to its commit sha
# (git ls-remote https://github.com/Lameguy64/mkpsxiso refs/tags/v2.30^{}, 2026-09-30).
ARG MKPSXISO_TAG=v2.30
ARG MKPSXISO_SHA=54fb1644ed8741223583e2dcda358b75a205e214
RUN git clone --depth 1 --branch "$MKPSXISO_TAG" --recurse-submodules --shallow-submodules \
      https://github.com/Lameguy64/mkpsxiso /tmp/mkpsxiso \
    && test "$(git -C /tmp/mkpsxiso rev-parse HEAD)" = "$MKPSXISO_SHA" \
    && cmake -S /tmp/mkpsxiso -B /tmp/mkpsxiso/build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build /tmp/mkpsxiso/build -j"$(nproc)" \
    && install -m 0755 /tmp/mkpsxiso/build/dumpsxiso /usr/local/bin/dumpsxiso \
    && rm -rf /tmp/mkpsxiso
WORKDIR /work
# Build:  docker build --platform linux/amd64 -t <project>-build .
# TODO(phase-4): add the pinned compiler's own packages when the pin needs them.
