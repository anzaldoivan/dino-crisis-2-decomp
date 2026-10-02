# PhaseEnd — Phase 1.3: The all-assembly byte-identical baseline (implements Gen 1.3)
Approved: 2026-10-01 | Closed: 2026-10-01 | Planner: claude-opus-5-5/medium | Tasks: 5 done, 0 superseded

## Milestone
Milestone: `make build` in the container produces every binary of the 1.2 fleet list hash-equal at 100% assembly with the hash check inside the build; `make expected` set; clean fleet check prints N of N, exits 0 — verified by: `bash tools/docker/dc.sh sync && bash tools/docker/dc.sh run bash tools/fleet_check.sh` → exit 0, prints `83 of 83 byte-identical`; `bash tools/docker/dc.sh run make expected` → exit 0; `PY tools/splat_gen.py --check` → exit 0; `PY tools/audit_public.py` → exit 0; `bash tools/firewall_control.sh` → exit 0
Verified: PY tools/audit_public.py → "OK — 0 offenders among 543 paths", 86 source(s), rc 0; bash tools/firewall_control.sh → "firewall_control: OK", rc 0; dc.sh sync && run.sh t5-verify -- dc.sh run bash tools/fleet_check.sh → "83 of 83 byte-identical", 54 s, rc 0 (expert run)

## Tasks
- T1 — splat pinned in the build image | Done: image `dc2-build` carries venv `/opt/splat` with splat64[mips]==0.41.0, spimdisasm==1.42.4, rabbitizer==1.16.2; container extract reproduces the host manifest (sha1 874e695a…597d both sides). | commit: fb67574 | tasks/T1.md | logs/T1.md
- T2 — splat config generator from the load map | Done: `tools/splat_gen.py` writes 83 `config/splat/<alias>.yaml` + 83 `config/check.<alias>.sha` from loadmap + boundaries; `--check` asserts count, alias uniqueness, every boundary edge, tiling, vram, odd tails, WEP_S00, sha = loadmap. | commit: 71b78ec | tasks/T2.md | logs/T2.md
- T3 — Makefile pipeline green on four pilots | Done: in the container `make -j build BASEDIR=.run/extracted/retail/files ONLY="…"` splits, assembles, links, objcopies and `sha1sum -c` checks per alias; four pilots (exe, PSX/BIN/ST1, BIN/OPTION, BIN/WEP01) byte-identical from clean. | commit: c361ecd | tasks/T3.md | logs/T3.md
- T4 — whole fleet green, make expected, clean fleet check | Done: `tools/fleet_check.sh` in the container (clean → extract → `make -j build`) prints `83 of 83 byte-identical`, rc 0, 52 s; `make expected` copies build/<alias>.bin → expected/ only after a green build; `make clean` keeps non-generated build/ entries. | commit: 912fa1a | tasks/T4.md | logs/T4.md

## Decisions that still bind
- T1 — splat runs as `bash tools/docker/dc.sh run /opt/splat/bin/python -m splat <cmd>`; pins in docs/ops/docker-host.md
- T2 — splat YAMLs are generated once and then hand-editable; `--check` asserts cuts, not file equality; overwrite needs `--force`
- T3 — assembler `mipsel-linux-gnu-as -march=r3000 -mabi=32 -G0 -no-pad-sections`; splat env `SPIMDISASM_SYMBOL_ALIGNMENT_REQUIRES_ALIGNED_SECTION=True`
- T3 — container base files via generated `build/<alias>.override.yaml` (target_path under BASEDIR); tracked YAMLs keep host path
- T4 — the gate is `dc.sh run bash tools/fleet_check.sh`; green = `<N> of <N> byte-identical`, N from loadmap, never a subset
- T4 — `make clean` removes only per-alias generated outputs; anything else under build/ (e.g. build/ghidra_rebuild/) survives
- splat runs as `dc.sh run /opt/splat/bin/python -m splat …`, pins splat64[mips] 0.41.0 / spimdisasm 1.42.4 / rabbitizer 1.16.2 → environment fact, in docs/ops/docker-host.md:33 and pin table (T1).
- splat YAMLs are generated once, then hand-editable; `splat_gen.py --check` asserts cuts, not file equality; overwrite only with `--force` → contract, docs/ops/docker-host.md:34 and `splat_gen.py --check` (T2).
- assembler `mipsel-linux-gnu-as -march=r3000 -mabi=32 -G0 -no-pad-sections`; env `SPIMDISASM_SYMBOL_ALIGNMENT_REQUIRES_ALIGNED_SECTION=True` → environment fact, docs/ops/docker-host.md:67 (T3).
- container base files come from a make-generated `build/<alias>.override.yaml`; tracked YAMLs keep the host path → contract, docs/ops/docker-host.md:65 (T3).
- the gate is `dc.sh run bash tools/fleet_check.sh`, green = `<N> of <N> byte-identical`, N from loadmap, never a subset → norm, already rule G61 + card Test line (T4).
- `make clean` removes only per-alias generated outputs; other `build/` entries survive → contract, docs/ops/docker-host.md:58, cookbook C0021 (T4).
- `config/check.*.sha` is a `required:` hash source (glob expanded in `audit_public.rom_hashes`) → contract, config/firewall.txt:43 + `audit_public.py` (T5).
- Next task needs: whether matched C must also regenerate the loose /BIN DAT type-7 segments is open for 1.6+ (plan Context); card is near the 7000-char cap, the next card edit must trim.

## Rules proposed
- (none)

## Cookbook entries added
C0021 | Keep non-generated artifacts out of build/; make clean removes only generated outputs | make,clean,build,hygiene | 2026-10-01 | 1.3/T3 | PhaseEnd 1.3
C0022 | splat: carve an overlay's pre-code data head as rodata (MIPS II+ ops rejected by as -march=r3000) | splat,overlay,rodata,mips,psx | 2026-10-01 | 1.3/T4 | PhaseEnd 1.3
C0023 | splat 0.41 omits out-of-segment j targets from undefined_funcs_auto; generate them | splat,linker,undefined-symbols,overlay | 2026-10-01 | 1.3/T4 | PhaseEnd 1.3

## Research
- (none)

## Audit
- seed: median 14k, max 14k, n=6 (phase 1.3)
- previous phase 1.2: median 14k, growth 0.1%
- CLAUDE.md: 1202 bytes (unchanged)
- .claude/skills/docker-vm-no-privileged/SKILL.md: 702 bytes (unchanged)
- .claude/skills/ghidra-mcp-scratch-copy/SKILL.md: 3730 bytes (unchanged)
- .claude/skills/pipe-exit-status/SKILL.md: 2593 bytes (unchanged)
- .claude/skills/project-architect/SKILL.md: 13448 bytes (unchanged)
- .claude-state/memory/MEMORY.md: 214 bytes (unchanged)
- HOW_WE_WORK.md: 6963 bytes
- cookbook/INDEX.md: 3983 bytes
- rules/INDEX.md: 10182 bytes
### Carry audit — phase 1.3 (2026-10-01T05:07:59Z → open UTC, 1 sessions, 239 requests)

| file (read) | chars | n |
|---|---|---|
| splat_gen.py | 17.3k | 2 |
| docker-host.md | 8.7k | 2 |
| Makefile | 5.7k | 2 |
| audit_public.py | 2.2k | 1 |
| task.template.md | 1.8k | 1 |
| Dockerfile | 1.8k | 1 |
| HOW_WE_WORK.md | 712 | 1 |
| file (write) | chars | n |
|---|---|---|
| splat_gen.py | 16.9k | 4 |
| Makefile | 5.7k | 6 |
| docker-host.md | 4.9k | 4 |
| T3.c1.md | 4.1k | 1 |
| T4.c1.md | 3.9k | 1 |
| RECAP.md | 3.3k | 1 |
| T2.c1.md | 2.9k | 1 |
| decomp-environment.md | 2.8k | 2 |
| result kind | chars | n |
|---|---|---|
| bash other | 123.5k | 69 |
| tools/card.py | 106.7k | 11 |
| Read | 38.2k | 10 |
| tools/splat_gen.py | 35.3k | 16 |
| run.sh | 28.5k | 33 |
| tools/plan_edit.py | 27.5k | 9 |
| Agent | 12.0k | 11 |
| tools/task_log.py | 8.1k | 6 |
| tools/audit_public.py | 8.0k | 7 |
| tools/fleet_check.sh | 7.6k | 3 |
- spilled: 1 results, 46.7k chars on disk, 0 read whole
- whole reads over 20.0k: 0
- seed floor: retriever-code 5,575 (n 2, prev 5,717)
- outline credit: 2 outlines · 0 followed by a ranged read · 0 chars credited
- warm pings: 4 pings over 2 runs, 2 warmed waits over the TTL, rewrites across warmed waits 0, waits past the cap 0, pings cost $0.0278 vs rewrites replaced $0.3469, cheaper than one rewrite: yes
- toasts by cause (waiting): question 0, review 0, replan 0, permission 0, input 0, discussion 0, crash 0, idle 0, stop 0, subagent-stop 0, model 0, other 0
- router turns by cause: loop 6, relay 4, re-arm 2, other 0, relay cost $0.0788
- retriever re-asks: 0 of 0 retriever briefs repeat a lookup of the same run
- router: pa-session 4a58baf9 · requests 31 · ctx at end 50.2k · growth T1 +1.1k, T2 +502, T3 +1.5k, T4 +2.0k, T5 +546 · top: tools/plan_edit.py 8.2k/6, Agent 6.6k/6, tools/status.py 2.1k/7, other 1.5k/7
- noise: 22 lines 1.4k chars — usage: : 13 lines/934, No such file or directory: 9 lines/495
- price: read $0.20/Mtok · 1h write $7.49/Mtok · output $18.72/Mtok (phase model mix)
- median requests after a read: 20
- carry/request: 1755 chars, 239 requests (prev 1906 chars, 616 requests, growth -7.9%)
- flag: 4 tool-source reads by coder-opus55, expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/audit_public.py  2.2k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/splat_gen.py  15.8k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/splat_gen.py  1.5k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/docker/Dockerfile  1.8k chars  coder-opus55

## Agent runs
- T1 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 34k | $0.181 | saved - | completed | parent 4a58baf9
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 31k | $0.264 | saved $0.11 | completed | parent a8189a5b
- T2 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 48k | $0.283 | saved - | completed | parent 4a58baf9
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 51k | $0.479 | saved $0.01 | completed | parent af1a0f43
- T3 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 38k | $0.212 | saved - | completed | parent 4a58baf9
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 85k | $1.090 | saved - | completed | parent a2d3fbd9
- T4 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 42k | $0.324 | saved - | completed | parent 4a58baf9
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 81k | $0.898 | saved - | completed | parent a4b8b248
- T5 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 43k | $0.329 | saved - | completed | parent 4a58baf9
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 41k | $0.355 | saved - | completed | parent a24f53e7
- PHASE-END | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 42k | $0.308 | saved - | completed | parent 4a58baf9

## Discussions
- (none)

## Deferred
- from T5: nothing pending from T5 (unconsumed)

## Changes
- 2026-10-01 router: T1 next -> done
- 2026-10-01 router: T2 next -> done
- 2026-10-01 router: T3 next -> done
- 2026-10-01 router: T4 next -> done
- 2026-10-01 router: T5 next -> done

## Plain-English Recap
Phase 1.3 turned the game's 83 executable files (the main program and its 82 overlays, the code modules the game loads into memory on demand) into a project that rebuilds each of them from disassembled source, byte for byte. A splitter tool (splat) cuts each binary into assembly files using the load map and code boundaries found in phase 1.2; a generator writes one splitter config and one expected fingerprint (SHA-1 hash) per binary, and the Makefile assembles, links and checks every fingerprint inside the build, so a single wrong byte fails it. Everything runs in a pinned Linux build container, and one command, the fleet check, wipes the outputs, re-extracts the disc and rebuilds all 83, printing `83 of 83 byte-identical`. The fingerprints are now a required part of the public-repo firewall, and the pipeline and the per-binary build units are documented. This is the baseline every later phase must keep green while assembly is replaced by C.

Milestone evidence (`PY tools/phaseend_index.py verify --verbose`, .run/logs/phaseend-verify-v.log, GREEN 5/5):
- clause 1 fleet check after sync → `83 of 83 byte-identical`, 52 s, rc 0 (.run/logs/verify1.log)
- clause 2 `make expected` → `expected: 83 binaries`, rc 0 (verify2.log)
- clause 3 `splat_gen.py --check` → OK 83 binaries (verify3.log)
- clause 4 `audit_public.py` → 0 offenders / 545 paths, 86 hash sources (verify4.log)
- clause 5 `firewall_control.sh` → OK (verify5.log)

H7 check: T1-T5 each list `docs/ops/` or `HOW_WE_WORK.md` beside their tool/build changes: no miss.
Deviations:
- the card Build/Test line edits claimed by T3/T4 (`HOW_WE_WORK.md`) were left uncommitted in the tree; committed with this phase end.
- gotchas carried 15 tagged lines; 3 `generalizable:` → cookbook C0021-C0023; 0 `workflow:`; 6 `harness:` stay in the summaries.
