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
- Phase 1.3 T1.c1 (2026-10-01) adds venv `/opt/splat` (`python3 -m venv`), pip pins (`Dockerfile`):
  - splat64 0.41.0 with extra `[mips]` (plain `splat64` pulls no spimdisasm/rabbitizer; they are `mips`/`dev` extras)
  - spimdisasm 1.42.4
  - rabbitizer 1.16.2
  - also resolved (unpinned, splat64 pins them itself or via extra): colorama 0.4.6, intervaltree 3.1.0, pylibyaml 0.1.0, PyYAML 6.0.3, tqdm 4.67.1, crunch64 0.6.2, n64img 0.3.3, pygfxd 1.0.5, pypng 0.20220715.0, sortedcontainers 2.4.0
  - image id after this layer `sha256:52431507e7454e60c623a653eba651ae908fcb983c4851c319644000a8937312`
- `mipsel-linux-gnu-as --version`: `GNU assembler (GNU Binutils for Ubuntu) 2.42`.
- splat invocation: `bash tools/docker/dc.sh run /opt/splat/bin/python -m splat <split|create_config|capy> …` (`--help` exits 0).
- Configs: `PY tools/splat_gen.py [--force] [--only <alias>] [--out-dir <d>] [--check]` (host) writes `config/splat/<alias>.yaml` + `config/check.<alias>.sha` from loadmap/boundaries; YAML `target_path` is host-canonical `extracted/retail/files/…`; `make` overrides it per alias (see Build).
- Container extract needs the cue: `bash tools/docker/dc.sh run sh -c 'DC2_CUE="/disc/Dino Crisis 2 (USA).cue" make extract OUT=.run/extracted/retail'`; its `manifest.sha1` equals the host's (T1.c1).

## Volume and sync
- Named volume `dc2-work` (override `DC2_VOLUME`), mounted at `/work`. dc.sh touches no other volume.
- No bind mount of any host path, ever; never mounts `/Users/Shared/GameInputs`. Game data lives only in the local volume `dc2-disc`, never in the image or git.
- `dc.sh sync`: `git ls-files -co --exclude-standard -z` (paths deleted in the working tree skipped) | `COPYFILE_DISABLE=1 tar --no-xattrs --no-mac-metadata` over stdin into a container that wipes every top-level entry of `/work` except `/work/.run`, then extracts. `/work` is thus an exact copy of tracked + untracked-unignored files, plus the container scratch `/work/.run`.
- Run `dc.sh sync` before any `dc.sh run` after host edits.
- Limits: ignored paths (`disks/`, `extracted/`, RE database, host `.run/`) are never carried. `/work/.git` is not carried. Container-side edits outside `/work/.run` are lost on the next sync.

## Disc volume
- Named volume `dc2-disc` (override `DC2_DISC_VOLUME`); `dc.sh run` mounts it read-only at `/disc`.
- Loaded with `bash tools/docker/dc.sh disc /Users/Shared/GameInputs/dino-crisis-2/usa`: top-level `*.cue` + `*.bin` of the dir (no `.DS_Store`), `COPYFILE_DISABLE=1 tar` over stdin into a container that wipes then fills the volume. No bind mount. Reload only when the dump changes.
- Reference extract (T1.c1; output in `/work/.run/ref`, kept across syncs, never in git):
  `bash tools/run.sh t1-ref -- bash tools/docker/dc.sh run sh -c 'rm -rf /work/.run/ref && mkdir -p /work/.run/ref && dumpsxiso -x /work/.run/ref/files -s /work/.run/ref/layout.xml "/disc/Dino Crisis 2 (USA) (Track 1).bin" && mv /work/.run/ref/files/license_data.dat /work/.run/ref/files/ZNULL.WAV /work/.run/ref/'`
- `sync` is silent since T4.c1: host bsdtar `--no-xattrs --no-mac-metadata` stops the per-file `LIBARCHIVE.xattr.com.apple.provenance` header (COPYFILE_DISABLE=1 alone did not). `dc.sh disc` still uses plain `COPYFILE_DISABLE=1 tar` (harmless warning noise).

## Use
- `bash tools/docker/dc.sh build` · `bash tools/docker/dc.sh sync` · `bash tools/docker/dc.sh disc <dir>` · `bash tools/docker/dc.sh run <cmd…>` (exit code passed through).

## Build (phase 1.3 T3.c1, 2026-10-01)
- `bash tools/docker/dc.sh sync && bash tools/run.sh <name> -- bash tools/docker/dc.sh run make -j build [ONLY="<alias> …"]`; green = one `<alias>.bin: OK` per alias, exit 0. Base files missing → the container extract above.
- Targets: `split` (asm only), `build`, `expected`, `clean`. `ONLY` default = every `config/splat/*.yaml`; an unknown alias is a make error.
- `BASEDIR ?=` first existing of `extracted/retail/files` (host), `.run/extracted/retail/files` (container), else the host path; no need to pass it.
- `clean` (T4.c1): removes generated outputs only: `asm/`, `build/overlays.mk`, per alias `build/<alias>/` + `build/<alias>.{bin,bin.bad,bin.tmp,elf,map,ld,override.yaml}`. Any other `build/` entry (e.g. ignored `build/ghidra_rebuild/`) survives. `expected/` is not touched.
- `expected` (T4.c1): depends on all N aliases' `build/<alias>.bin` (ignores `ONLY`); when all are green copies `build/<alias>.bin` → `expected/<alias>.bin`; any failure → rc ≠ 0, `expected/` untouched. After a green fleet_check it rebuilds nothing (prints `expected: 83 binaries`).
- Fleet check (T4.c1): `bash tools/docker/dc.sh sync && bash tools/run.sh <name> -- bash tools/docker/dc.sh run bash tools/fleet_check.sh` → `make clean`, `make extract OUT=.run/extracted/retail` (`DC2_CUE` default `/disc/Dino Crisis 2 (USA).cue`), `make -j$(nproc) -k build`, prints `<k> of <N> byte-identical` + elapsed; rc 0 iff k == N (N = loadmap rows class exe|code; also rc 1 if #check.*.sha ≠ N or a bin is missing). Logs in container `/work/.run/fleet_{extract,build}.log`. Runtime ~51 s (8 cores).
- Overlay data head (T4.c1, `tools/splat_gen.py head_end`): when a word between text_start and the first `addiu $sp,$sp,-N` decodes as MIPS II+ (gas `-march=r3000` rejects it), that whole head is `rodata`, text starts at the prologue.
- Out-of-overlay `j func_<ADDR>` targets (splat leaves them out of `undefined_funcs_auto.txt`): make writes `build/<alias>/undefined_jumps_auto.txt` (`func_X = 0xX;` for each referenced, undefined, unlisted `func_X`) and links it as a third `-T`.
- `sync` wipes `/work/build` + `/work/asm` (not under `.run`), so the first build after a sync is a full rebuild.
- `build/overlays.mk` is generated by make from the YAML list (`$(eval $(call alias_rules,<alias>,<path>))` per alias, path from `target_path`).
- Override mechanism: make writes `build/<alias>.override.yaml` (`options: target_path: $(BASEDIR)/<path>`, rewritten only on change) and runs `splat split config/splat/<alias>.yaml build/<alias>.override.yaml`; splat 0.41.0 `conf.load` merges later files into earlier (scalars replace), `base_path` resolves from the first file. YAMLs are never edited.
- Split env: `SPIMDISASM_SYMBOL_ALIGNMENT_REQUIRES_ALIGNED_SECTION=True` (else GCC profile emits `.align 3` before a jtbl in a non-8-aligned file → shifted bytes). Split log: `build/<alias>/split.log`.
- Assembler: `mipsel-linux-gnu-as -march=r3000 -mabi=32 -G0 -no-pad-sections -I build/<alias>/include` for every `.s`; a splat `bin` asset → `printf '.section .data\n.incbin "<file>"\n' | mipsel-linux-gnu-as -march=r3000 -mabi=32 -G0 -no-pad-sections -o <obj> -`. Object list = every `build/<alias>/….o` named in `build/<alias>.ld`.
- Link: `mipsel-linux-gnu-ld -nostdlib --no-check-sections -Map build/<alias>.map -T build/<alias>.ld -T build/<alias>/undefined_syms_auto.txt -T build/<alias>/undefined_funcs_auto.txt -o build/<alias>.elf`; `mipsel-linux-gnu-objcopy -O binary`.
- Odd-size tail: shrink-only trim to the target size when `size(build) > size(target)` and the excess ≤ 3 B (empty input sections under `SUBALIGN(4)` pad the end).
- Check: `(cd build && sha1sum -c ../config/check.<alias>.sha)`; on mismatch the image is kept as `build/<alias>.bin.bad`, make prints `FAILED sha1: <alias>` and exits non-zero. Also `FAILED split|assemble|link: <alias>`.
- Layout options are in the generated YAMLs (`tools/splat_gen.py`): `subalign: 4`, `ld_align_section_vram_end: False`, `asset_path: asm/<alias>/assets`, `generated_asm_macros_directory: build/<alias>/include`.
