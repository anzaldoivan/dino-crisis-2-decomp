# Decomp environment

*Installed by decomp-architect on 2026-09-30. The environment, build and oracle facts of a matching decompilation;
the rules behind them are the G group in `rules/`.*

## Version pins (decomp)

| Component | Version | Notes / why pinned |
|---|---|---|
| The pinned toolchain triple (compiler → assembler shim → binutils, with flags) | TODO(phase-4) | TODO(phase-4): pinned by fingerprint evidence down the candidate ladder; the assembler's compatibility version is always passed explicitly — a shim's default is not "latest" |
| Candidate compiler family (from the SDK evidence) | PsyQ-era GCC 2.7.2 cc1 builds (candidate set; pinned in Phase 4) | the candidate set the pin phase runs down; never a sibling project's triple |
| The splitter / disassembler and its config | TODO(phase-3) | version pinned in the bootstrap script |
| The build host | x86-64 Linux, Ubuntu 24.04, ext4 | On another host, use the container `tools/docker/Dockerfile` (`--platform linux/amd64`). Keep the tree in a named volume. The vintage 32-bit compiler runs under the container's emulation. Entry point `tools/docker/dc.sh`; host facts `docs/ops/docker-host.md`. |
| The disassembler database and its agent server | Ghidra 12.1.3 (`ghidra_12.1.3_PUBLIC_20260817.zip`, https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_12.1.3_build/ghidra_12.1.3_PUBLIC_20260817.zip, sha256 `93a5d11a9ad510622acaaf908c556a7b9b764d338e78a7567f3689bf5081fd54`; JDK Temurin 21) + psx_ldr 2026.09.03 (https://github.com/lab313ru/ghidra_psx_ldr/releases/download/2026.09.03/ghidra_12.1.3_PUBLIC_20260903_ghidra_psx_ldr.zip, sha256 `04ddface00dd141f41924effa93a5dadb5630a24cdde36400bc703d25fcdec27`) in `$GHIDRA_INSTALL_DIR/Ghidra/Extensions`; agent server GhidrAssistMCP 2.11.0 (https://github.com/symgraph/GhidrAssistMCP/releases/download/2.11.0/ghidra_12.1_PUBLIC_20260802_GhidrAssistMCP.zip, sha256 `baba204a9fe839921a1487be9dfb15526faea787e5e4312c8404281d82b3a1a7`; classes are Java 21 (major 65), no rebuild) installed as `$GHIDRA_INSTALL_DIR/Ghidra/Extensions/GhidrAssistMCP` (zip's `GhidrAssistMCP/` minus `.claude/`, `CLAUDE.md`, `*.db`, `lucene/`, `res/`; `extension.properties` `version=12.1` → `12.1.3`), served by `tools/ghidra/mcp_start.sh` on `http://127.0.0.1:8080/sse` (docs/ops/disassembler-mcp.md) | the static oracle; project `ghidra/dc2` (ignored), built by `tools/ghidra/import.sh`; the database is tracked as a TEXT export with a rebuild script. The release zip has no `mac_arm_64` decompiler natives: on Apple silicon build them once from `Ghidra/Features/Decompiler/src/decompile/cpp` (`make ghidra_opt "ARCH_TYPE=-arch arm64"`, then `make sleigh_opt …`, one goal per call) and copy to `os/mac_arm_64/{decompile,sleigh}` — psx_ldr's analyzer needs the decompiler (LibgpuMacroDetector) |
| The emulator and its scripting bridge | PCSX-Redux dev build 279 (`f7b388cc`, changeset `f7b388cc1e6555e2caf3ad78ed431126a546214a`), macOS arm64 `PCSX-Redux-f7b388cc-Arm.dmg` from https://distrib.app/storage/assets/bdd/5bd/80d/f23a1096406516ac7b2d9a0a1d1fb02e4cd634a972ef44833d9cbaf/PCSX-Redux-f7b388cc-Arm.dmg (catalog https://distrib.app/storage/manifests/pcsx-redux/dev-macos-arm/manifest.json), sha256 `f4adc63fd218dbcbda860956c2227a09278bb7b2b5efd95e810664962aa41561`, installed `$HOME/Applications/PCSX-Redux.app`; BIOS: bundled OpenBIOS (boots the game; `$DC2_BIOS` → `-bios` optional) | the runtime oracle; `tools/emu/emu.sh start|stop|status` runs it headless (`-no-ui -interpreter`, web API `127.0.0.1:8081`); the arm64 dynarec of this build dies with SIGILL ~25 s into the game, so the interpreter is used |

## The game and the medium

- **Title / platform / serial:** Dino Crisis 2 · Sony PlayStation (MIPS R3000A) · USA, SLUS-01279
- **The main executable on the medium:** `SLUS_012.79` (its hash is the first per-binary contract, Phase 3)
- **Container layout:** Track 1 ISO9660 mode 2 + Track 2 CD-DA; boot exe SLUS_012.79; 105 presumed code overlays (.BIN under /BIN and /PSX/BIN); data in .DAT/.DBS/.TEX/.PXL archives; XA audio and STR movies
- **SDK / compiler-era evidence:** "Library Programs (c) 1993-1997 Sony Computer Entertainment Inc." in SLUS_012.79 (PsyQ 4.x era); library version to be detected in Phase 2
- **The dump (machine-local, never committed):** `/Users/Shared/GameInputs/dino-crisis-2/usa` — copied once onto a fast local filesystem under `disks/`
  (ignored); the extractor reads it, nothing builds against it.

## Build / extract / verify (decomp)

```
# reference extract of the medium (dumpsxiso 2.30 in the build image; disc volume dc2-disc at /disc, see docker-host.md)
bash tools/run.sh t1-ref -- bash tools/docker/dc.sh run sh -c 'rm -rf /work/.run/ref && mkdir -p /work/.run/ref && dumpsxiso -x /work/.run/ref/files -s /work/.run/ref/layout.xml "/disc/Dino Crisis 2 (USA) (Track 1).bin" && mv /work/.run/ref/files/license_data.dat /work/.run/ref/files/ZNULL.WAV /work/.run/ref/'
# our own extract (tools/extract_disc.py; host python3 >= 3.12, disc in disks/ or $DC2_CUE; see docs/formats.md)
make extract                       # → extracted/retail/files + manifest.jsonl + manifest.sha1
(cd extracted/retail && sha1sum -c --quiet manifest.sha1)   # verify the files against the manifest
# the two manifests are tracked via config/firewall.txt `allow:` (exact path); manifest.jsonl + config/medium.sha1 are `required:` hash sources
# against the reference: dc.sh sync, then in the container
#   python3 tools/extract_disc.py --cue "/disc/Dino Crisis 2 (USA).cue" --out /work/.run/ours && diff -rq /work/.run/ref/files /work/.run/ours/files
# the clean fleet verification — every binary from clean → extract → build, exit code read
TODO(phase-3)
```

- **The gate:** a binary is green only when its hash check inside `make build` passes; a match is verified from a CLEAN
  rebuild, never incremental; the executable is gated only by a clean rebuild; a build is verified by its exit code. The
  clean fleet verification is the natural `verified by:` clause of every matching phase's milestone.

## The oracles (decomp)

- **Disassembler MCP:** TODO(phase-2) — verify with one cheap call before any reverse-engineering task; after a
  restart or a program switch, pause and ask the developer to reconnect the client (rule G2). Every configured MCP server
  adds its instructions to every session, so the entry is enabled only once the server exists (Phase 2).
- **PsyQ SDK version (T1, Phase 1.2):** SLUS_012.79 → **4.7.0**. psx_ldr's `DetectPsyQ` (the `Ps` lib-version stamp)
  returns `470` on the loaded image (not the analyzer's "if not found" fallback, which is also 4.7.0). Cross-check
  `tools/ghidra/scripts/ScorePsyqVersions.java` (each version's lib JSONs applied via `psyq.SigApplier` in a rolled-back
  transaction, read-only run; objs = matched OBJs/total, funcs = function labels of matched non-low-entropy OBJs):
  `400 objs=103/1238 funcs=111 libs=11/26` · `410 159/1339 379 14/25` · `420 165/1471 403 14/21` ·
  `430 169/1672 421 16/26` · `440 174/1766 441 16/28` · `450 176/1771 451 16/29` · `460 237/2187 736 18/30` ·
  `470 244/2190 892 18/31`. Verdict: 4.7.0 scores highest on every axis; the detector agrees. Sig sets: psx_ldr's
  bundled `data/psyq/` is identical to lab313ru/psx_psyq_signatures @ e9e46e7e (`diff -rq`, excluding `.git`).
  `import.sh --info SLUS_012.79`: `PSX:LE:32:default`, 1501 functions, 874 sig-hit functions. psx_ldr sets the
  image base to the RAM base `80000000` (by design), not the header t_addr `80018000`; the gate checks instead that the
  initialized block at t_addr starts exactly at t_addr (`BASE_CHECK`).
- **Emulator bridge:** PCSX-Redux web API on `127.0.0.1:8081` (`tools/emu/emu.sh`), verified with curl on build 279:
  `GET /api/v1/cpu/ram/raw` (2 MiB, offset i = `0x80000000+i`; `tools/emu/ram_probe.py`); `GET /api/v1/execution-flow`
  (JSON `running`, `debugger`, `isDynarec`, `8mb`); `POST /api/v1/execution-flow?function=pause|resume` (200);
  `GET /api/v1/gpu/vram/raw` (200). No built-in frame/vsync route: `tools/emu/lua/vsync.lua` (`-dofile`) counts
  `GPU::Vsync` events and serves `GET /api/v1/lua/vsync` (text count); `PCSX.WebServer` exists only after the first
  `/api/v1/lua/` request (404), so `emu.sh start` pokes it once. `DC2_BREAK=<addr>` adds `-debugger` and a pausing
  Exec breakpoint (empty = unset). `DC2_LUA="<file> ..."` (absolute or repo-relative) runs extra Lua after vsync.lua
  and adds `-debugger`; `tools/emu/lua/loadtrace.lua` (loader trace), `tools/emu/lua/pad.lua` (scripted pad).
  Pad API (H7, build 279): `PCSX.SIO0.slots[1].pads[1].setOverride(PCSX.CONSTS.PAD.BUTTON.<NAME>)` /
  `.clearOverride(btn)`; pad.lua takes `DC2_PAD="<vsync>:<BUTTON>[:<hold>] ..."` (hold default 6), logs
  `.run/emu/pad.log`. Screenshot (scratch use): Lua `PCSX.GPU.takeScreenShot()` → 320x240 16-bpp `data`. Other routes in the binary (unprobed): `/api/v1/assembly/symbols`, `cd/`, `cpu/cache`, `screen/`,
  `state/`. A live-memory finding is verified only with three or more consistent datapoints or a controlled
  before/after diff.
- **Load proof:** `tools/emu/prove_load.sh <exe>` boots fresh with `DC2_BREAK=pc0`, compares the full
  `[t_addr,t_addr+t_size)` at pc0, then only `[t_addr,.text end)` at +300/+600 vsyncs with two hard-coded libcard
  self-modifying exclusions (`_patch_card_info`, `_patch_card2`); stops the emulator at the end. Ranges, offsets and
  datapoints: `docs/memory-map.md#slus_01279`. `prove_load.sh <path>` (path as in `config/loadmap.evidence.tsv`)
  also proves a fixed overlay table (ST1, WEP01: 3 fixed attract-demo vsyncs, full file compare; OPTION: pad script
  to the title OPTION entry, full compare at the entry breakpoint, `[base,text_end)` at +300/+600); unknown path →
  exit 2. All proven rows: `for p in $(PY tools/loadmap_evidence.py --proven-paths); do bash tools/emu/prove_load.sh
  "$p" || exit 1; done` (~6 min; run via `tools/run.sh --bg`). Per overlay: `docs/memory-map.md#ovl-<name>`.

## Models and effort (decomp, on PA3)

PA3 pins model and effort per agent; no agent changes either. The judgments whose silent error would poison everything
downstream get the deepest reasoning through PA3's own practices, in this order: **split** the task until each piece is
routine (on the max5 and pro tiers the planner must — no plan task may carry `effort: high`); take the judgment to a
**`/discuss max`** session, whose record folds into the PhaseEnd; or **override a rung** in `.claude/pa.json` `"ladder"`.
Breadth — the same analysis over many independent items — is fan-out, not depth.

| Phase | The judgments for `/discuss max` | Breadth (fan-out) |
|---|---|---|
| 1 extraction + manifest | the container/compression semantics when they are ambiguous | a fleet-wide format audit |
| 2 oracles + load map | every load-address derivation; the segmentation decision (the forced boundaries) | a survey of every payload's loader route |
| 3 the all-assembly baseline | the linker-script/layout diagnosis when the first link is red | — |
| 4 the compiler pinned | **the fingerprint verdict** (the triple, the flags, per-module variation) | the candidate ladder run as parallel probes |
| 5 census + harness | the census's shape reading (what the strategy will be built on) | the fleet-wide census; the differential harness's pairs |
| 6 the multipliers | the reconcile ladder's design; any "class is dead" verdict | mass propagation and remaps |
| 7 the map + the permuter | **reading the compiler's source into the map**; the plateau classifier's classes | probes across many constructs |
| 8 the campaign | the routing cliff; **every wall verdict**; the harvest distillation's vocabulary | bulk drafting; the harvest over a wave's reports |
| 9 publish | the contract run's design; any irreversible repository operation (rehearsed) | the fresh-clone proof on a second machine |
| 10 readability | struct unification decisions; every name that asserts meaning | family-batched pin removal, gated |

## Git posture (decomp)

- **Visibility at day one:** public from the first commit — the ROM firewall applies either way (`config/firewall.txt`,
  `tools/audit_public.py`, the CI workflow). If ever private, a later flip is gated on the host's object store, never on
  a clean tree.
- Never `git clean -x` in this tree (the game-derived data is ignored-but-present); the backup of the reverse-engineering
  work is the text export + the checksum files + a private archive repository, not the ignored directories.

## Tooling inventory (decomp)

| Tool | Location | Purpose |
|---|---|---|
| `tools/audit_public.py` | `tools/` | the ROM audit (purge paths, the derived hash set, the size cap, the pasted-disassembly check); the first-push gate and the CI job; its sources are `config/firewall.txt` |
| `tools/firewall_control.sh` | `tools/` | the firewall negative control: plants the `config/firewall-fixture.sha1` blob under `.run/firewall-control/`, asserts the audit FAILs naming it, removes it, asserts the tree PASSes; exit 0 iff both (the Phase 1.0 milestone check) |
| `tools/loadmap.py` | `tools/` | regenerates `config/loadmap.tsv` from the manifest + `config/loadmap.evidence.tsv` (prints `rows= N=`); `--check` fails on a stale file, wrong row set, unclassified rows or a control row (exe + proven) disagreeing with `docs/memory-map.md`, with a built-in negative control; `--ledger PATH` / `--out PATH` override inputs; see `docs/memory-map.md#load-map` |
| `make format` | `Makefile` | clang-format over `src/` with the tracked `.clang-format` (the community style) |
| `make extract [OUT=…]` | `tools/extract_disc.py` | our own disc extractor: medium check vs `config/medium.sha1`, ISO9660 walk of Track 1 → `<OUT>/files/` + `manifest.jsonl` + `manifest.sha1` (Form 2 as 2336 B/sector, CD-DA skipped); see `docs/formats.md` |

## The three dictionaries (the kit master copy — consulted, never copied into this repository)

| Corpus | Where | How to use it |
|---|---|---|
| The tool dictionary — the source project's tools, verbatim, by ladder phase, keyed by the need each answers | `<kit master copy>/corpus/tools/INDEX.md` (installed summary: `docs/tools-manifest.md`) | before designing or debugging a tool, grep the index by the need; the matching file is the jumping-off point, its Adapt column the list of what differs here |
| The inherited knowledge base — the cookbook, its symptom index and the codegen map, verbatim | `<kit master copy>/corpus/cookbook/` (installed front page: `docs/knowledge-corpus.md`) | same compiler family: look the symptom up, apply, re-prove on your bytes; another compiler: read the same pass in your compiler's source and find your own lever |
| The inherited record — the source project's distilled records (the how-to, the decision log, the accelerators, the retrospective, the story, the playbook, the effort doctrine, the readability charter) and every phase-end, verbatim | `<kit master copy>/corpus/record/` (installed front page: `docs/inherited-record.md`) | when a rule or kernel cites a source, open it here; the digest first, a phase-end on demand, the how-to in order |
| Kit master copy location (machine-local) | `/Users/Shared/kits/decomp-architect` — where the kit was installed from; the dictionaries above live under it | — |
