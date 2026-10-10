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
# splat64 (splitter/disassembler) in its own venv; deps pinned to the versions it resolved.
RUN python3 -m venv /opt/splat \
    && /opt/splat/bin/pip install --no-cache-dir \
      'splat64[mips]==0.41.0' spimdisasm==1.42.4 rabbitizer==1.16.2
# decomp-permuter in its own venv, pinned to main HEAD (git ls-remote …/decomp-permuter HEAD, 2026-10-02).
# Deps per its README minus the network extra (pynacl); pycparser is vendored (perm_pycparser).
ARG PERMUTER_SHA=059609d4aec73eb0650726772954e1ad575825f8
RUN python3 -m venv /opt/permuter \
    && git clone https://github.com/simonlindholm/decomp-permuter /opt/permuter/decomp-permuter \
    && git -C /opt/permuter/decomp-permuter checkout -q "$PERMUTER_SHA" \
    && test "$(git -C /opt/permuter/decomp-permuter rev-parse HEAD)" = "$PERMUTER_SHA" \
    && /opt/permuter/bin/pip install --no-cache-dir toml==0.10.2 Levenshtein==0.27.5 rapidfuzz==3.14.6
# Candidate compilers (phase 1.4 T1): old-gcc psx builds, esa PsyQ CC1PSX.EXE, maspsx, wibo → /opt/cc/<name>/.
# sha256-pinned in config/toolchains.tsv (named build context `cfg`, see dc.sh build). Pin: toolchains.tsv col 7 (psyq4.6 a2.86/G0/O2).
COPY --from=cfg toolchains.tsv /usr/local/share/toolchains.tsv
COPY fetch_toolchain.sh /usr/local/bin/fetch_toolchain.sh
RUN chmod +x /usr/local/bin/fetch_toolchain.sh \
    && TOOLCHAINS_TSV=/usr/local/share/toolchains.tsv fetch_toolchain.sh /opt/cc \
    && TOOLCHAINS_TSV=/usr/local/share/toolchains.tsv fetch_toolchain.sh --verify /opt/cc
WORKDIR /work
# Build:  docker build --platform linux/amd64 --build-context cfg=config -t <project>-build .
