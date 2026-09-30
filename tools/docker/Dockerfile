# Build host for a PlayStation / PsyQ matching decompilation (x86-64 Linux, Ubuntu 24.04).
# Use it when the host is not x86-64 Linux, for example an arm64 Mac.
# Keep the source tree in a named volume. Do not build on a host bind mount.
FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive LANG=C.UTF-8
RUN apt-get update && apt-get install -y --no-install-recommends \
      bchunk binutils-mipsel-linux-gnu bsdmainutils clang-format coreutils curl gcc-mipsel-linux-gnu \
      cpp-mipsel-linux-gnu git libelf-dev make ninja-build p7zip-full python3 python3-pip python3-venv \
      python3-dev build-essential unzip wget file xxd ca-certificates time \
    && rm -rf /var/lib/apt/lists/*
RUN git config --global --add safe.directory '*'
WORKDIR /work
# Build:  docker build --platform linux/amd64 -t <project>-build .
# TODO(phase-4): add the pinned compiler's own packages when the pin needs them.
