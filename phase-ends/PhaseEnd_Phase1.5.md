# PhaseEnd — Phase 1.5: The honest census and the differential harness at 0% (implements Gen 1.5)
Approved: 2026-10-01 | Closed: 2026-10-01 | Planner: claude-opus-5-5/medium | Tasks: 6 done, 0 superseded

## Milestone
Milestone: every scanner prints its denominator and passes its known-true control; 0 phantoms, 0 truncations; second oracle 0 disagreements; differential harness ≥ 5 question pairs on a schedule, 0 disagreements; census of duplication, families, reach × size, unique tail and Sony library objects; progress denominators read from the build — verified by: `bash tools/docker/dc.sh sync && bash tools/docker/dc.sh run bash tools/fleet_check.sh` → exit 0, prints `83 of 83 byte-identical` and `harness: P of P pairs agree, 0 disagreements` with P ≥ 5; `bash tools/docker/dc.sh run python3 tools/census.py --check` → exit 0, prints `binaries: 83 of 83`, `phantoms: 0`, `truncations: 0`, `control: ok`; `bash tools/docker/dc.sh run python3 tools/oracle_diff.py` → exit 0, prints `oracle: 83 of 83 binaries compared, 0 disagreements`; `bash tools/docker/dc.sh run python3 tools/dup_census.py --check` → exit 0, prints `control: ok` and `unique tail:`; `bash tools/docker/dc.sh run python3 tools/harness.py --scanners` → exit 0, prints `scanners: S of S denominator+control ok` with S ≥ 6; `bash tools/docker/dc.sh run python3 tools/progress.py` → exit 0, prints `denominator from build: ok`; `PY tools/audit_public.py` → exit 0
Verified: bash tools/docker/dc.sh sync && dc.sh run bash tools/fleet_check.sh → `83 of 83 byte-identical`, `harness: 6 of 6 pairs agree, 0 disagreements`, elapsed 111 s, history `full 6 6 0 54 0` (run.sh t6-verify-fleet); dc.sh run python3 tools/harness.py --scanners → rc 0, `scanners: 10 of 10 denominator+control ok` (t6-verify-scanners); --selftest → `selftest: 6 of 6 planted disagreements caught` (t6-selftest)

## Tasks
- T1 — function census with phantoms, truncations and control | Done: `tools/census.py` censuses all 83 binaries from split asm (self-provisions extract/split), writes `.run/census/functions.tsv`; fleet 3885 functions, text 1228888 of 1228888 B, phantoms 0, truncations 0, controls ok. | commit: - | tasks/T1.md | logs/T1.md
- T2 — Ghidra function list as the second oracle, 0 disagreements | Done: all 83 programs in Ghidra project `ghidra/dc2`; function lists cached as `config/ghidra/<prog>.functions.tsv`; `tools/oracle_diff.py` prints `oracle: 83 of 83 binaries compared, 0 disagreements` with planted-start and stale-hash controls every run. | commit: - | tasks/T2.md | logs/T2.md
- T3 — duplication census: tiers, families, reach, unique tail | Done: `tools/dup_census.py` reads the 3911-function census + split asm, writes `.run/census/dup.tsv`, prints exact/near tiers, per-family counts, top-20 reach and the unique tail with denominators; relocation and near-FP controls green, `--check` rc 1 on either failure. | commit: - | tasks/T3.md | logs/T3.md
- T4 — Sony library objects census across the fleet | Done: every census function carries kind lib|game (unknown 0); census prints lib objects/functions/bytes, game functions, unknown, lib exact classes, and lib + game controls; `--check` fails on unknown > 0 or either control. | commit: - | tasks/T4.md | logs/T4.md
- T5 — progress denominators read from the build; banked.py honest | Done: `make progress` / `tools/progress.py` prints per-binary and fleet `progress: C of G game functions in C, c of b bytes`, a lib line and `denominator from build: ok|FAIL`; banked.py prints `banked: n of G` with an every-run fixture control (1 of 2). | commit: - | tasks/T5.md | logs/T5.md

## Decisions that still bind
- T1 — text region of a binary = [vram of first asm|c subsegment, text-end row end of config/boundaries.tsv); the text-end row's start is the base and includes the rodata head.
- T1 — function extent = glabel .. last instruction + 4; alignment nops after endlabel belong to the preceding function; C-defined (banked) functions = gaps in a c span partitioned by `func_<ADDR>` defs in src.
- T1 — phantom confirmation criteria (any one): (a) symbol referenced in asm (exe: fleet-wide), (b) previous fn ends in jr/j/b delay slot, (b') start = text_start or end of an evidenced data-in-text span, (c) lib-object start, (d) C def, (e) raw `.word` literal = start. Sole-confirmation counts printed per criterion.
- T1 — data-in-text span = uncovered span whose words are all data/invalid per spimdisasm, with no control-flow target into it, and holding an invalid insn or referenced as data from outside; leaves the denominator, reported as `data in text: D B (n spans …)`; anything else uncovered = truncation.
- T2 — oracle disagreement is judged after n1 (only starts inside census text region; Ghidra 0x2000xxxx loader stubs and out-of-text starts counted `not judged`) and n2 (ends equal across zero words) only.
- T2 — Ghidra seeds and merges use program bytes + Ghidra only, never census/splat/config (C0020 independence); n3 merges a Ghidra function whose every ref is internal flow from the preceding function (psx_ldr `<OBJ>_OBJ_<off>` labels on epilogues/loop heads).
- T2 — exception kinds are data-span, unreferenced, switch-case (boundaries.tsv jtbl), data-head, lib-patch-blob (PATCH objects copied to RAM); each row valid only while its predicate holds; a failed row counts as a disagreement. New rows only via `--propose-exceptions`.
- T2 — census function start rules now include (f) jal target inside a function, (g) first non-zero word after an unconditional transfer + delay slot unless an internal branch/jtbl target; R3 words (write $zero non-nop, base-$zero load/store; syscall/break in head trim) count as invalid for data spans.
- T3 — exact key = J/JAL target masked + imm16 masked only on asm-annotated lines (`%hi(`/`%lo(`, splat raw-pair `(0x… >> 16)`/`(0x… & 0xFFFF)`, and lo-uses of a reg last written by an annotated hi, addu-propagated); trailing zero words stripped. Masks are never keyed on address values (under-matching, DK-10).
- T3 — near key = op/rs/rt/rd/funct; tier exact if exact class ≥ 2, else near if near class ≥ 2, else unique.
- T3 — relocation control is independent of the masking: word-level data-flow relocator, delta 0x12340, over every function with a moved field (k = n); near control seed 1505, 10000 pairs from different exact classes, ≤ 1%.
- T4 — kind lib = exe function inside a boundaries.tsv lib-object extent, or overlay function in a dup exact class that is lib-pure in the exe (≥ 1 exe lib member, 0 exe non-lib members); everything else game. No size or address thresholds (DK-10).
- T4 — lib object = distinct lib-object extent; same-start rows (always same end) collapse to one object named `A=B[=…]`: 183 rows → 171 objects, 10 groups.
- T5 — progress denominator G/b = census kind game rows/bytes of the binary; lib reported separately (`not in G`); numerator = census game row whose start is a `func_<ADDR>` C body in a C unit the alias's `build/<alias>.ld` links.
- T5 — denominator check = census alias set (from census stdout) = Makefile ALIASES = aliases with build/<alias>.ld; per alias game+lib rows and bytes = census's own `functions:` / `text bytes covered`; fleet sum = census total; every C body has a census game row.
- T6 — pairs = matched (banked.py source set vs compiler-emitted C-object bytes, relocations masked, = retail at census extent), compiles (compile_only set vs src objects in build/<alias>.ld; no-game: config/c_units.tsv), fleet (clean_run.tsv vs incremental rebuild after touching src/**/*.c), coverage (census rows + data spans vs every text word derived from splat yaml + boundaries text-end), oracle (oracle_diff rc/0 disagreements + its controls), denominators (progress + banked G vs functions.tsv kind=game per alias).
- T6 — a scanner row's control is the tool's every-run control line or a chained planted negative (`cmd && ! cmd --fixture …`) whose failure line is matched; never an unconditional line; denominator last group ≥ 1.
- T6 — fleet_check green = `83 of 83 byte-identical` and `harness: 6 of 6 pairs agree, 0 disagreements`.
- Contract: census definitions (text region, function extent, phantom criteria, data-in-text spans, start rules f/g, R3 invalid words, kind lib|game, lib-object grouping) → docs/ops/decomp-environment.md `### Function census`; tested by `census.py --check` controls and fixtures.
- Contract: oracle judgement after n1/n2/n3 normalisation, Ghidra seeds from program bytes + Ghidra only, exception kinds re-checked every run, new rows only via `--propose-exceptions` → docs/ops `### Ghidra oracle`; tested by `oracle_diff.py` planted-start and stale-hash controls.
- Contract: dedup exact/near keys, masks only on asm-annotated lines, never on address values; relocation and near-FP controls → docs/ops `### Duplication census`; tested by `dup_census.py --check` (now also prints the combined `control: ok`).
- Contract: progress denominator = census game rows of each binary, alias set checked against Makefile and build/<alias>.ld → docs/ops `### Progress denominators`; tested by `progress.py` and its `--fixture alias|functions`.
- Contract: harness pairs and fleet_check green = `83 of 83 byte-identical` + `harness: 6 of 6 pairs agree, 0 disagreements` → card Build/run/test line and docs/ops `### Differential harness`; tested by `harness.py --selftest`.
- Norm: a scanner's control must be able to fail (every-run control line or chained planted negative, never an unconditional line) → rule G70.
- Environment: the card is at 6982 of 7000 chars; the next Tools row needs a trim elsewhere (repeated in T1–T6 summaries).
- Next task needs: the 14 census data-in-text spans may be promoted to `config/boundaries.tsv` data-island rows (needs a splat_gen recut and the fleet gate), per T1.

## Rules proposed
- (none)

## Cookbook entries added
C0031 | Classify uncovered text spans by evidence, not by trimming the range | census,splat,data-in-text | 2026-10-01 | 1.5/T1 | phase-ends/current/tasks/T1.md
C0032 | Ghidra BinaryLoader on PSX overlays: seed functions in a rolled-back txn | ghidra,overlay,switch,oracle | 2026-10-01 | 1.5/T2 | phase-ends/current/tasks/T2.md
C0033 | psx_ldr object-offset labels turn jumps into calls; check the opcode | ghidra,psx_ldr,oracle,mips | 2026-10-01 | 1.5/T2 | phase-ends/current/tasks/T2.md
C0034 | A prologue after an unconditional transfer is a function start splat merges | splat,census,mips,function-boundary | 2026-10-01 | 1.5/T2 | phase-ends/current/tasks/T2.md
C0035 | Relocation-masked dedup keys must mask raw hi/lo pairs and lo-reuses | dedup,splat,relocation,mips | 2026-10-01 | 1.5/T3 | phase-ends/current/tasks/T3.md
C0036 | Propagating a label through duplicate classes needs a purity rule | dedup,lib,census | 2026-10-01 | 1.5/T4 | phase-ends/current/tasks/T4.md
C0037 | census.py --check prints fixture totals after the real block | census,parsing,harness | 2026-10-01 | 1.5/T6 | phase-ends/current/tasks/T6.md

## Research
R1.5-001 | extract census-relevant facts from phase 1.2-1.4, memory-map, DK kernels, rules | Phase 1.5 census and differential harness: prior facts | census, differential-oracle, boundaries, psyq, DK-8, DK-9, G19, G21, G27, G32 | retriever-digest | 2026-10-01 | 54 lines

## Audit
- seed: median 15k, max 15k, n=7 (phase 1.5)
- previous phase 1.4: median 14k, growth 1.6%
- CLAUDE.md: 1202 bytes (unchanged)
- .claude/skills/docker-vm-no-privileged/SKILL.md: 702 bytes (unchanged)
- .claude/skills/ghidra-mcp-scratch-copy/SKILL.md: 3730 bytes (unchanged)
- .claude/skills/pipe-exit-status/SKILL.md: 2593 bytes (unchanged)
- .claude/skills/project-architect/SKILL.md: 13526 bytes (unchanged)
- .claude-state/memory/MEMORY.md: 214 bytes (unchanged)
- HOW_WE_WORK.md: 7024 bytes
- cookbook/INDEX.md: 6027 bytes
- rules/INDEX.md: 10411 bytes
### Carry audit — phase 1.5 (2026-10-01T15:31:42Z → open UTC, 1 sessions, 910 requests)

| file (read) | chars | n |
|---|---|---|
| census.py | 186.0k | 26 |
| oracle_diff.py | 124.8k | 13 |
| DumpFunctions.java | 91.3k | 13 |
| decomp-environment.md | 65.4k | 13 |
| t2c6_residuals.txt | 37.6k | 2 |
| dup_census.py | 33.1k | 4 |
| harness.py | 25.2k | 1 |
| DumpSwitchTables.java | 16.9k | 3 |
| T1.md | 9.0k | 2 |
| HOW_WE_WORK.md | 8.6k | 2 |
| file (write) | chars | n |
|---|---|---|
| census.py | 56.7k | 40 |
| harness.py | 23.1k | 1 |
| decomp-environment.md | 20.1k | 11 |
| dup_census.py | 12.9k | 2 |
| DumpFunctions.java | 11.0k | 6 |
| oracle_diff.py | 10.9k | 7 |
| progress.py | 8.7k | 3 |
| T2.c6.md | 6.3k | 1 |
| result kind | chars | n |
|---|---|---|
| Read | 715.8k | 109 |
| bash other | 424.6k | 265 |
| run.sh | 144.3k | 124 |
| tools/card.py | 110.0k | 18 |
| tools/census.py | 68.5k | 46 |
| tools/outline.py | 54.4k | 12 |
| tools/plan_edit.py | 46.2k | 17 |
| tools/banked.py | 41.9k | 8 |
| tools/oracle_diff.py | 32.2k | 40 |
| Agent | 27.4k | 25 |
- whole reads over 20.0k: 3
- seed floor: retriever-code 5,667 (n 2, prev 5,704) · retriever-digest 5,606 (n 1, prev —)
- outline credit: 21 outlines · 16 followed by a ranged read · 270.1k chars credited
- warm pings: 32 pings over 6 runs, 14 warmed waits over the TTL, rewrites across warmed waits 1, waits past the cap 0, pings cost $0.5410 vs rewrites replaced $5.4703, cheaper than one rewrite: yes
- toasts by cause (waiting): question 0, review 0, replan 0, permission 0, input 0, discussion 0, crash 0, idle 0, stop 0, subagent-stop 0, model 0, other 0
- router turns by cause: loop 7, relay 30, re-arm 9, other 0, relay cost $0.6689
- retriever re-asks: 0 of 1 retriever briefs repeat a lookup of the same run
- router: pa-session 0c26ff2d · requests 108 · ctx at end 70.0k · growth T1 +1.5k, T2 +12.4k, T3 +957, T4 +2.5k, T5 +986, T6 +4.3k · top: other 8.0k/41, Agent 7.7k/7, tools/status.py 6.0k/7, tools/plan_edit.py 2.1k/7, tools/managed.py 1.4k/1
- noise: 113 lines 8.0k chars — usage: : 58 lines/4.4k, No such file or directory: 55 lines/3.6k
- price: read $0.20/Mtok · 1h write $7.61/Mtok · output $19.04/Mtok (phase model mix)
- median requests after a read: 5
- carry/request: 1989 chars, 910 requests (prev 1334 chars, 568 requests, growth 49.1%)
- candidate: audit_public_help (proposed)
- flag: carry per request grew 49% vs 1.4
- flag: 74 tool-source reads by coder-opus55, expert-opus55, retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/dup_census.py  12.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/harness.py  25.2k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  5.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/boundaries.py  2.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/loadmap.py  2.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  7.4k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  6.0k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  3.9k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  330 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  24.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  3.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  3.8k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  10.5k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  8.4k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  20.0k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/dup_census.py  15.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/cc_fingerprint.py  8.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  8.1k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  13.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/dump_functions.sh  5.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/docker/dc.sh  2.9k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/dup_census.py  1.5k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/banked.py  2.1k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  11.5k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpSwitchTables.java  10.9k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  16.4k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  2.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  1.9k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  1.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  3.0k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpSwitchTables.java  2.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  23.3k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  8.1k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  3.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  16.8k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  18.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  1.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/dup_census.py  3.3k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/progress.py  8.2k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  4.9k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  3.9k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  11.9k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  3.3k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/import_raw.sh  3.1k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/import.sh  3.7k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/export.sh  2.2k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/boundaries.py  2.4k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  2.6k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  2.9k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpSwitchTables.java  3.4k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  3.0k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  4.0k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  14.8k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  6.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  8.1k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  10.5k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  13.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  21.4k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  20.8k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  358 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  3.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  4.4k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  14.9k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  9.9k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  3.8k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  1.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  1.8k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  965 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/oracle_diff.py  473 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  1.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  1.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  4.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/fleet_check.sh  1.5k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/compile_only.sh  3.1k chars  expert-opus55
- flag: 3 whole reads over 20.0k by coder-opus55
  file: /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/harness.py  role: coder-opus55  chars: 23.2k
  file: /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  role: coder-opus55  chars: 22.6k
  file: /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/ghidra/scripts/DumpFunctions.java  role: coder-opus55  chars: 20.1k

## Agent runs
- T1 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 83k | $0.948 | saved $0.46 | completed | parent 0c26ff2d
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 75k | $0.835 | saved $0.24 | completed | parent a95bb3c5
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 83k | $1.129 | saved $2.18 | completed | parent a95bb3c5
- T2 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 147k | $2.540 | saved - | completed | parent 0c26ff2d
- T2 | retriever-code | retriever | claude-sonnet-5-5 | medium | ctx 35k | $0.151 | saved - | completed | parent a0da69fd
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 65k | $0.763 | saved - | completed | parent a0da69fd
- T1 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 87k | $0.801 | saved $0.94 | completed | parent 657dd7d1
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 69k | $0.692 | saved - | completed | parent a0da69fd
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 62k | $0.538 | saved $2.30 | completed | parent a6cec3d5
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 61k | $0.564 | saved - | completed | parent a0da69fd
- T1 | critic | critic | claude-opus-5-5 | medium | ctx 23k | $0.129 | saved - | completed | parent 657dd7d1
- T2 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 72k | $0.730 | saved $1.28 | completed | parent 657dd7d1
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 100k | $0.981 | saved $0.53 | completed | parent a0da69fd
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 76k | $0.891 | saved $0.80 | completed | parent a99d2990
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 136k | $1.850 | saved $1.91 | completed | parent a0da69fd
- T2 | critic | critic | claude-opus-5-5 | medium | ctx 22k | $0.175 | saved - | completed | parent 657dd7d1
- T3 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 68k | $0.623 | saved $1.15 | completed | parent 657dd7d1
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 54k | $0.479 | saved $1.22 | completed | parent aef19d07
- T4 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 69k | $0.567 | saved $0.22 | completed | parent 657dd7d1
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 139k | $1.914 | saved - | completed | parent a0da69fd
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 56k | $0.570 | saved $1.19 | completed | parent a4b2dd34
- T5 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 116k | $1.998 | saved $5.16 | completed | parent 657dd7d1
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 172k | $4.377 | saved $0.96 | handoff | parent a0da69fd
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 41k | $0.266 | saved $0.24 | completed | parent ae6ba8f8
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 146k | $2.103 | saved $0.61 | completed | parent ae6ba8f8
- T5 | review | review | claude-opus-5-5 | medium | ctx 23k | $0.139 | saved - | completed | parent 657dd7d1
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 115k | $1.524 | saved $0.26 | completed | parent a0da69fd
- T6 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 62k | $0.450 | saved $0.03 | completed | parent 657dd7d1
- T6 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 65k | $0.610 | saved $0.39 | completed | parent a221cb04
- T7 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 58k | $0.471 | saved - | completed | parent 657dd7d1
- T7 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 52k | $0.612 | saved $0.01 | completed | parent a8de1abd
- T3 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 70k | $0.498 | saved - | completed | parent 0c26ff2d
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 63k | $0.559 | saved $0.24 | completed | parent a8e17b59
- T8 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 97k | $0.898 | saved $0.34 | completed | parent 657dd7d1
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 35k | $0.240 | saved - | completed | parent a8e17b59
- T8 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 96k | $0.960 | saved $0.17 | completed | parent a14accfb
- T4 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 60k | $0.485 | saved $0.12 | completed | parent 0c26ff2d
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 75k | $0.805 | saved $0.37 | completed | parent a35bce1e
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 44k | $0.382 | saved $0.64 | completed | parent a35bce1e
- … 7 more: PY ~/.claude/pa3/pa_ledger.py report

## Discussions
- (none)

## Deferred
- from T6: phase end — Milestone gate is fleet_check (now includes harness) + `harness.py --scanners`; history evidence in container `.run/harness/history.tsv`. (unconsumed)

## Changes
- 2026-10-01 router: T1 next -> done
- 2026-10-01 router: T2 next -> done
- 2026-10-01 router: T3 next -> done
- 2026-10-01 router: T4 next -> done
- 2026-10-01 router: T5 next -> done
- 2026-10-01 router: T6 next -> done

## Plain-English Recap
Phase 1.5 built the measuring instruments the decompilation will be judged by, before any large-scale matching work starts. A "census" now lists every function in all 83 game binaries (3911 functions covering every byte of program code, none invented and none cut short), and a second, independent tool (the Ghidra disassembler) agrees with that list on every binary. A duplication census groups functions that are byte-for-byte copies of each other across binaries, separates Sony's library code (485 functions) from the game's own code (3426 functions), and reports the 2094 functions that appear only once. Progress is now counted against a denominator taken from the build itself (currently 5 of 3426 game functions written in C), and a differential harness of six independent cross-checks, each with a planted failure it must catch, runs on every full rebuild and weekly in CI. The milestone was verified from a clean rebuild: 83 of 83 binaries byte-identical, 6 of 6 harness pairs agreeing, 10 of 10 scanners printing their denominator and passing their control, and the public-repository audit clean.
