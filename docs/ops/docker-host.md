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

## Candidate toolchains (phase 1.4 T1)
- Fetched 2026-10-01 into the image at `/opt/cc/<name>/` by `tools/docker/fetch_toolchain.sh` from `config/toolchains.tsv` (url + sha256 per row; mismatch → exit 1, nothing unpacked; stamp `/opt/cc/<name>/.sha256`). No candidate is the pin. binutils stays 2.42.
- Build context stays `tools/docker`; `dc.sh build` adds named context `--build-context cfg=config` (Dockerfile `COPY --from=cfg toolchains.tsv`). In-image copies: `/usr/local/share/toolchains.tsv`, `/usr/local/bin/fetch_toolchain.sh`.
- Image id after this layer `sha256:bb40c3916d1df66e70fc3936d5b41c31ad147d3e11b810aee02d5a2f1dbf876c`.
- Commands:
  - fetch: `fetch_toolchain.sh [--only <name>…] <dest>` (image build runs it with `TOOLCHAINS_TSV=/usr/local/share/toolchains.tsv … /opt/cc`).
  - verify: `bash tools/docker/dc.sh sync && bash tools/docker/dc.sh run bash tools/docker/fetch_toolchain.sh --verify /opt/cc` → `loader: 1 ok`, `toolchains: 13 ok`, rc 0.
  - sha mismatch check: a scratch tsv with the wibo sha zeroed, `TOOLCHAINS_TSV=.run/t1/bad.tsv fetch_toolchain.sh --only wibo .run/t1/badcc` → `sha256 mismatch for wibo`, rc 1, dest empty.
  - smoke (scratch `/work/.run/t1/smoke`, not tracked), per candidate: `mipsel-linux-gnu-cpp -P -undef -nostdinc -D__GNUC__=2 smoke.c -o smoke.i`; `<cc1> -quiet -O2 -G0 -mips1 -fno-builtin smoke.i -o <n>.s` (old-gcc: `/opt/cc/<n>/cc1`; esa: `/opt/cc/wibo/wibo /opt/cc/<n>/CC1PSX.EXE`); `python3 /opt/cc/maspsx/maspsx.py --aspsx-version=2.79 <n>.s | mipsel-linux-gnu-as -march=r3000 -mabi=32 -G0 -o <n>.o -`. Banner: `<cc1> -version smoke.i -o /dev/null`. smoke.c = 5 lines (`int g;` + `add(a,b)`).
- Smoke "ok" = `<n>.o` is `ELF 32-bit LSB relocatable, MIPS`. 11 of 12 cc1 builds ok; psyq3.6 DROPPED.
- old-gcc release 0.17, url `https://github.com/decompals/old-gcc/releases/download/0.17/<name>.tar.gz` (flat tar: cc1 cc1plus cpp g++ gcc). `file` of every cc1: `ELF 32-bit LSB executable, Intel 80386, version 1 (GNU/Linux), statically linked` (runs under the amd64 emulation as is).
  - gcc-2.7.2-psx · sha256 `500a459b3485e885a8d302cac23c2a4632f3900e03a09153f6190699fd723571` · `GNU C version 2.7.2 [AL 1.1, MM 40] Sony Playstation compiled by GNU C version 9.4.0.` · smoke ok
  - gcc-2.8.0-psx · `1a3c956fe8aea5ebdb251749d95de2c84f023530584d7bd663744b5ec24050b7` · `GNU C version 2.8.0 (mips-sony-psx) compiled by GNU C version 9.4.0.` · ok
  - gcc-2.8.1-psx · `f6f6e883942d4d3289d048236c672e71ed410e546aaae8ff655952f1567e1be0` · `GNU C version 2.8.1 (mips-sony-psx) compiled by GNU C version 9.4.0.` · ok
  - gcc-2.91.66-psx · `f773a0a9659fa4ff74313ac4363d939312e8125675cd09ad9f8c1202f587f1fd` · `GNU C version egcs-2.91.66 19990314 (egcs-1.1.2 release) (mips-sony-psx) compiled by GNU C version 9.4.0.` · ok
  - gcc-2.95.2-psx · `932ed3669710a82b12570c29c46ff42989b6505b2af10db84d4551d55dfc0b1c` · `GNU C version 2.95.2 19991024 (release) (mips-sony-psx) compiled by GNU C version 9.4.0.` · ok
- esa, url `https://github.com/mkst/esa/releases/download/psyq-binaries/<name>.tar.gz` (one top dir, stripped). `file` of CC1PSX.EXE: `PE32 executable (console) Intel 80386, for MS Windows, 4|5 sections` except psyq3.6.
  - psyq3.6 · `9445f4cd871ca85b764b8d2746b6122750af17ecfe2d6e8aae41d15461d8c41d` · `file`: `MS-DOS executable, MZ for MS-DOS, COFF` · no banner (README: `GNU C++ 2.7.2.SN.1 [AL 1.1, MM 40] Sony Playstation`) · **DROPPED**: `Failed to load PE image /opt/cc/psyq3.6/CC1PSX.EXE` (DOS exe; the tarball ships a `dosemurc`, i.e. needs a DOS emulator, not wibo). Fetched + verified anyway.
  - psyq4.0 · `f25a4f6f044eb1b344bbbd3291aa9a4fc1a1124fc637eae9522c0a117d940e28` · `GNU C version 2.7.2.SN32.3.7.0002 [AL 1.1, MM 40] Sony Playstation compiled by CC.` · ok
  - psyq4.1 · `2a2650ceb5eaa73fdc581bec4a85ccaa6ff9eeea8a4810be25a638f3cd2ebac4` · `GNU C version cygnus-2.7.2-970404 SN32.3.7.0004 (SonyPSX) compiled by CC.` · ok
  - psyq4.3 · `577038d66507d3aa5423de0ba3f540e121a4f60f637f4794aa4350135d4f9a46` · `GNU C version 2.8.0 SN32 Build 4.0.0007 (SonyPlayStation) compiled by CC.` · ok
  - psyq4.4 · `72e73934bab0d51933eb95af514afb14f3d432f01530eac3ddb16dfbb57ab66c` · `GNU C version 2.8.1 SN32 BUILD 4.0.0010 (PSX) compiled by CC.` · ok
  - psyq4.5 · `75f28034f6844f0f7633e3f17443727865c8955da1cd19147db2c760b40f14f7` · `GNU C version egcs-2.91.66 19990314 (egcs-1.1.2 release) (PSX) compiled by CC.` · ok
  - psyq4.6 · `635603e09a452c9c2923492fea8b8eb051959dd94f240933b88e94866814b894` · `GNU C version 2.95.2 19991024 BUILD 4.0.0030 (PSX) compiled by CC.` · ok
- maspsx: `mkst/maspsx` commit `7686f845a181700534c83c0419183e38aeb3e49c` (HEAD 2026-10-01), `https://github.com/mkst/maspsx/archive/<commit>.tar.gz`, sha256 `604f5422662aaa7cebb0d7a183c4fd3152a27e261e168808eba29b1c6ad5cdd2`; run as `python3 /opt/cc/maspsx/maspsx.py` (image python3 3.12, no extra deps).
- wibo 1.2.0 (latest release 2026-10-01), `https://github.com/decompals/wibo/releases/download/1.2.0/wibo-i686`, sha256 `2575d3b0a2f408b2c2b0850db56f1af5d005a138394a6774eba77b6708ecc304`; `file`: `ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), statically linked, … with debug_info, not stripped`; `wibo --version` → `wibo 1.2.0 (Linux i686)`.
  - The `wibo-x86_64` asset of the same release fails under this host's emulation on every CC1PSX.EXE: `rosetta error: invalid gdt selector index 4` + `Trace/breakpoint trap` (also `-debug`). The i686 build runs. Rosetta is thus the emulation backend in use (see Host).
