# Decomp environment

*Installed by decomp-architect on 2026-09-30. The environment, build and oracle facts of a matching decompilation;
the rules behind them are the G group in `rules/`.*

## Version pins (decomp)

| Component | Version | Notes / why pinned |
|---|---|---|
| The pinned toolchain triple (compiler → assembler shim → binutils, with flags) | TODO(phase-4) | TODO(phase-4): pinned by fingerprint evidence down the candidate ladder; the assembler's compatibility version is always passed explicitly — a shim's default is not "latest" |
| Candidate compiler family (from the SDK evidence) | PsyQ-era GCC 2.7.2 cc1 builds (candidate set; pinned in Phase 4) | the candidate set the pin phase runs down; never a sibling project's triple |
| The splitter / disassembler and its config | TODO(phase-3) | version pinned in the bootstrap script |
| The build host | x86-64 Linux, Ubuntu 24.04, ext4 | On another host, use the container `tools/docker/Dockerfile` (`--platform linux/amd64`). Keep the tree in a named volume. The vintage 32-bit compiler runs under the container's emulation. |
| The disassembler database and its agent server | TODO(phase-2) | the static oracle; the database is tracked as a TEXT export with a rebuild script |
| The emulator and its scripting bridge | TODO(phase-2) | the runtime oracle |

## The game and the medium

- **Title / platform / serial:** Dino Crisis 2 · Sony PlayStation (MIPS R3000A) · USA, SLUS-01279
- **The main executable on the medium:** `SLUS_012.79` (its hash is the first per-binary contract, Phase 3)
- **Container layout:** Track 1 ISO9660 mode 2 + Track 2 CD-DA; boot exe SLUS_012.79; 105 presumed code overlays (.BIN under /BIN and /PSX/BIN); data in .DAT/.DBS/.TEX/.PXL archives; XA audio and STR movies
- **SDK / compiler-era evidence:** "Library Programs (c) 1993-1997 Sony Computer Entertainment Inc." in SLUS_012.79 (PsyQ 4.x era); library version to be detected in Phase 2
- **The dump (machine-local, never committed):** `/Users/Shared/GameInputs/dino-crisis-2/usa` — copied once onto a fast local filesystem under `disks/`
  (ignored); the extractor reads it, nothing builds against it.

## Build / extract / verify (decomp)

```
# extract the medium and verify against the committed manifest
TODO(phase-1)
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
- **Emulator bridge:** TODO(phase-2) — a live-memory finding is verified only with three or more consistent datapoints
  or a controlled before/after diff.

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
| `make format` | `Makefile` | clang-format over `src/` with the tracked `.clang-format` (the community style) |
| TODO(phase-1): the extractor, the manifest | `tools/` | — |

## The three dictionaries (the kit master copy — consulted, never copied into this repository)

| Corpus | Where | How to use it |
|---|---|---|
| The tool dictionary — the source project's tools, verbatim, by ladder phase, keyed by the need each answers | `<kit master copy>/corpus/tools/INDEX.md` (installed summary: `docs/tools-manifest.md`) | before designing or debugging a tool, grep the index by the need; the matching file is the jumping-off point, its Adapt column the list of what differs here |
| The inherited knowledge base — the cookbook, its symptom index and the codegen map, verbatim | `<kit master copy>/corpus/cookbook/` (installed front page: `docs/knowledge-corpus.md`) | same compiler family: look the symptom up, apply, re-prove on your bytes; another compiler: read the same pass in your compiler's source and find your own lever |
| The inherited record — the source project's distilled records (the how-to, the decision log, the accelerators, the retrospective, the story, the playbook, the effort doctrine, the readability charter) and every phase-end, verbatim | `<kit master copy>/corpus/record/` (installed front page: `docs/inherited-record.md`) | when a rule or kernel cites a source, open it here; the digest first, a phase-end on demand, the how-to in order |
| Kit master copy location (machine-local) | `/Users/Shared/kits/decomp-architect` — where the kit was installed from; the dictionaries above live under it | — |
