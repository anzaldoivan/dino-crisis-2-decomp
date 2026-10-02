# PhaseEnd — Phase 1.6: The multipliers (implements Gen 1.6)
Approved: 2026-10-01 | Closed: 2026-10-01 | Planner: claude-opus-5-5/medium | Tasks: 9 done, 0 superseded

## Milestone
Milestone: propagation dry run gates every member of one hand-matched family, fail-closed; twin band reproduces every known exact pair; a standalone match banks via the reconcile ladder without redraft; carves build; draw filter prints what it refuses; canonical-type check refuses a planted duplicate and a raw address cast; clean fleet check N of N — verified by: `bash tools/docker/dc.sh sync && bash tools/docker/dc.sh run bash tools/fleet_check.sh` → exit 0, prints `83 of 83 byte-identical` and `harness: P of P pairs agree, 0 disagreements` with P ≥ 6; `bash tools/docker/dc.sh run python3 tools/harness.py --scanners` → exit 0, prints `scanners: S of S denominator+control ok` with S ≥ 16; `bash tools/docker/dc.sh run python3 tools/propagate.py --dry-run --family psx_bin_st6:0x800d5c40` → exit 0, prints `propagation dry run: M of M members gated` with M ≥ 2 and `fail-closed control: ok`; `bash tools/docker/dc.sh run python3 tools/twins.py --check` → exit 0, prints `exact pairs reproduced: X of X` with X ≥ 1 and `random-pair control: ok`; `bash tools/docker/dc.sh run python3 tools/reconcile.py --check` → exit 0, prints `reconcile: K of K banked without redraft` with K ≥ 1 and `directory gate: ok`; `bash tools/docker/dc.sh run python3 tools/carve.py --check` → exit 0, prints `carves: C of C build hash-equal` with C ≥ 5 and `control: ok`; `bash tools/docker/dc.sh run python3 tools/draw_filter.py --all` → exit 0, prints `draw: A of D accepted` with D ≥ 1 and `control: ok`; `bash tools/docker/dc.sh run python3 tools/types_check.py` → exit 0, prints `types: 0 duplicates, 0 raw address casts` and `control: planted duplicate refused, planted raw cast refused`; `bash tools/docker/dc.sh run python3 tools/banked.py` → exit 0, prints `banked: B of 3426` with B ≥ 7; `PY tools/audit_public.py` → exit 0
Verified: run.sh t9-verify-host `bash tools/docker/dc.sh sync && PY tools/phaseend_index.py verify --verbose` → exit 0, `VERIFY: GREEN (10/10)`, GREEN 7 with `draw: 769 of 3426 accepted`. The literal plan line in the container (run.sh t9-verify) → exit 1, all RED, docker absent in the container. `PY .run/t9/fixtures.py` → FIXTURES: GREEN. `PY tools/plan_edit.py lint` → WARN on `A`, OK.

## Tasks
- T1 — twin band reproduces every exact pair | Done: `tools/twins.py` builds the near band over the 3426 game fns, writes `.run/twins/twins.tsv`, reproduces all 26745 exact pairs at distance 0, passes the random-pair control, ranks open fns by banked twin; scanner row `twins` live (11 of 11). | commit: - | tasks/T1.md | logs/T1.md
- T2 — canonical type layer and its check | Done: include/dc2.h holds the shared shapes with byte-proven fields; st6 and exe units include it, no local copies; tools/types_check.py reports `0 duplicates, 0 raw address casts` over 4 units with both planted controls refused; 83 of 83 byte-identical; scanner row `types` live (12 of 12). | commit: - | tasks/T2.md | logs/T2.md
- T3 — carve chain with closing cuts, revert on fail | Done: config/c_units.tsv takes an optional `end` (closing cut back to asm); `tools/carve.py` adds/narrows a row, regenerates one alias, writes an INCLUDE_ASM skeleton, builds + hash-checks, reverts on fail; st8 unit game_800D6AB4 carved as one fn; scanner `carve` live (13 of 13). | commit: - | tasks/T3.md | logs/T3.md
- T4 — propagation registry, fail-closed dry run, e322 banked | Done: the func_800D5C40 body is now written once in src/shared/func_800D5C40.inc.c, and both st6 and st8 instantiate it. Family psx_bin_st6:0x800d5c40 is registered with member psx_bin_st8:0x800d6ab4. `tools/propagate.py` dry run gates every member in scratch with a fail-closed control, and `--apply` banked st8. Result: banked 6 of 3426, 83 of 83, scanner `propagate` live (14 of 14). | commit: - | tasks/T4.md | logs/T4.md
- T5 — reconcile ladder banks a standalone match | Done: `tools/reconcile.py` climbs the reconcile ladder without editing the draft body and writes verdicts plus a directory gate. Its planted control banks via decl-sync. func_80052634 (exe, 0x20 B) matched standalone and banked in its real unit at rung self-decl. Result: banked 7 of 3426, 83 of 83, scanner `reconcile` live (15 of 15). | commit: - | tasks/T5.md | logs/T5.md
- T6 — draw filter prints every refusal | Done: `tools/draw_filter.py` derives every refusal from evidence each run. It prints `draw: A of D accepted` and one `refused <reason>: n` line for each of the 10 reasons, zeros included, and writes `.run/draw/{accepted,refused}.tsv` (`alias start reason evidence`). A planted control runs every time. Scanner `draw_filter` is live (16 of 16). | commit: - | tasks/T6.md | logs/T6.md
- T7 — wiring, card row, clean fleet and scanners gate | Done: docs/ops/decomp-environment.md has `## The multipliers (Phase 1.6)`. It covers the six tools, their records and the working order twins → draw_filter → probe → reconcile/propagate → fleet. The card has one `multipliers` Tools row and is 6995 of 7000 chars. From a fresh sync the full verify chain is green. | commit: - | tasks/T7.md | logs/T7.md
- T8 — milestone clauses 3,4,5: exemplar gated in dry run, plain control lines | Done: the propagate dry run now builds and byte-checks the exemplar (st6) as well as each member, so the gate reads 2 of 2. twins.py and reconcile.py print plain `random-pair control: ok` / `directory gate: ok` lines when their controls pass. Milestone clauses 3, 4 and 5 are GREEN under `phaseend_index.py verify`. | commit: - | tasks/T8.md | logs/T8.md

## Decisions that still bind
- T1 — twin ratio = 1 - edit_distance / max(len_a, len_b) over relocation-normalised word tokens (dup_census masking imported, G33); kept ≥ 0.3; prefilter length ±25% + opcode-histogram lower bound.
- T1 — the twin band never falls back to binary words when split asm is absent; it builds or refuses (binary fallback lost 104 exact pairs: 26641 of 26745).
- T2 — a struct shape used by more than one unit, or needed by a propagated body, lives once in include/dc2.h; single-unit shapes (Pair, Anim, Stage) stay local until a second unit needs them.
- T2 — fixed hardware addresses (scratchpad 0x1F800000) are named once in include/dc2.h as accessor macros; src/**/*.c never casts an integer literal to a pointer. Headers are the canonical layer and are not scanned.
- T3 — a unit is narrowed or created only through `tools/carve.py`, which builds and hash-checks the alias and restores row, yaml and unit on failure; generated yaml never hand-edited.
- T3 — a carve whose start lies in a lib-object span or outside game text is refused (splat_gen check) with the tree unchanged.
- T4 — a shared body lives in src/shared/<exemplar fn>.inc.c and is included inside the unit's function braces. Each unit keeps its own `void func_<ADDR>(…)` signature line and binds data and callees through one-line defines; *.inc.c is never compiled as a unit.
- T4 — a member unit is written only by `propagate.py --apply`, after a green dry run. Every run re-checks each member's exact key against the exemplar's and fails closed on a mismatch (C0036).
- T5 — a standalone-matched draft enters its real unit only through `tools/reconcile.py --apply`. The ladder edits declarations only. The fn body hash (whitespace-normalised, ladder call-site casts stripped) must be equal before and after, else the verdict is `failed <rung> body-changed`. The ladder never redrafts.
- T5 — every draft dir under .run/reconcile/ holds exactly one verdict (`banked <rung>|failed <rung> <reason>|no-verdict`); the directory gate is drafts = banked + failed + no-verdict.
- T5 — in census, a C-defined predecessor confirms its successor's start (fallthrough rule). gcc emits a complete function, so the boundary holds by construction. Census totals were unchanged (3911 / 3426 / 485).
- T6 — one reason per candidate, the first match in the order not-a-census-start, lib, banked, out-of-range, data-in-text, no-asm, switch, pin-unproven, has-banked-twin, open-twin-sibling. So accepted + all refusals = candidates.
- T6 — open-twin-sibling means the fn is a direct twin (any twins.tsv row) of a candidate already accepted in the same draw. Candidates are processed sorted by alias, start. Transitive clusters of the 0.3 band were not used because they are too coarse.
- T8 — a propagation dry-run count covers every family fn, the exemplar included, and each one counted needs a real scratch build that hash-checks equal. The exemplar unit must include the shared body, or its line is FAIL.
- T8 — a plain pass line (`<control>: ok`) is printed next to the detailed line, and only when the control actually passed. The detailed line stays.
- T9 — in a verify expectation, a single capital letter in a `<X> of <Y>` slot (both sides standalone capitals) is a numeric placeholder, `A` and `I` included. Everywhere else `a`, `A` and `I` stay literal, and the other single letters keep the T10 (1.4) rule. Planners should still avoid `A`/`I` as placeholders; lint warns about them.
- Norm → rule G71 (added): source units change only through carve.py, propagate.py --apply, reconcile.py --apply (T3:14, T4:17, T5:15).
- Contract → docs/ops/decomp-environment.md `### Twin band` + `twins.py --check`: twin ratio and band definition; no fallback to binary words (T1:10, T1:11).
- Contract → docs/ops `### Canonical types` + `types_check.py`: shared shapes once in include/dc2.h; hardware addresses as accessor macros, no raw casts (T2:13, T2:14).
- Contract → docs/ops carve chain note + `carve.py --check`: lib-span/out-of-text carves refused, tree unchanged (T3:15).
- Contract → docs/ops `### Propagation` + `propagate.py --dry-run`: shared body in src/shared/<fn>.inc.c, per-unit signature line and one-line binding defines (T4:16).
- Contract → docs/ops `### Reconcile ladder` + `reconcile.py --check`: one verdict per draft dir, directory gate drafts = banked + failed + no-verdict (T5:16).
- Contract → `census.py --check`: a C-defined predecessor confirms its successor's start (T5:17).
- Contract → docs/ops `### Draw filter` + `draw_filter.py --all`: fixed first-match reason order; open-twin-sibling = direct twin of an already-accepted candidate (T6:10, T6:11).
- Environment fact → card (already): card headroom 5 chars (T7:20).
- Contract → docs/ops `### Propagation` + `propagate.py --dry-run`: the dry-run count covers every family fn, exemplar included, each a real scratch build hash-checked (T8:12).
- Contract → twins.py/reconcile.py + config/scanners.tsv:20,24: a plain `<control>: ok` line prints beside the detailed one, only on pass (T8:13).
- Contract → docs/ops/decomp-environment.md:146-148 + `phaseend_index.py verify` + `plan_edit.py lint`: a capital letter in a `<X> of <Y>` slot is a placeholder, `A`/`I` included; lint warns on `A`/`I` (T9:10).
- Norm → rule G72 (added): a task runs its own milestone clause with the verifier's matching rules before recording it green.
- Environment fact → cookbook C0044: verifiers that call dc.sh run on the host, never through `dc.sh run` (T9:19).
- Next task needs: planners write task `verify:` lines that run `phaseend_index.py verify` on the host, not inside the container (T9 deviation).

## Rules proposed
- (none)

## Cookbook entries added
C0038 | Near-band edit distance fast in stdlib via bit-parallel Myers | twins,dedup,performance | 2026-10-01 | 1.6/T1 | phase-ends/current/tasks/T1.md
C0039 | Field proofs for banked fns come from objdump of the built object | types,headers,objdump | 2026-10-01 | 1.6/T2 | phase-ends/current/tasks/T2.md
C0040 | Exclude shared *.inc.c bodies from every per-unit scanner | propagation,harness,build | 2026-10-01 | 1.6/T4 | phase-ends/current/tasks/T4.md
C0041 | Boundary analysis treats a C-defined predecessor as a complete fn | census,boundaries,banking | 2026-10-01 | 1.6/T5 | phase-ends/current/tasks/T5.md
C0042 | Derive check counts from the config, never hard-code them | harness,census,config | 2026-10-01 | 1.6/T5 | phase-ends/current/tasks/T5.md
C0043 | Overlapping refusal reasons need a fixed first-match order | draw,filter,counts | 2026-10-01 | 1.6/T6 | phase-ends/current/tasks/T6.md
C0044 | Run container-orchestrating verifiers on the host | verify,docker,phase-end | 2026-10-01 | 1.6/T9 | phase-ends/current/tasks/T9.md

## Research
R1.6-001 | extract definitions of the 7 multiplier subjects + rules | Phase 1.6 multipliers: definitions and mechanics | phase-1.6, multipliers, propagation, twins, reconcile, carve, draw-filter, types | retriever-digest | 2026-10-01 | 33 lines

## Audit
- seed: median 14k, max 15k, n=11 (phase 1.6)
- previous phase 1.5: median 15k, growth -2.1%
- CLAUDE.md: 1202 bytes (unchanged)
- .claude/skills/docker-vm-no-privileged/SKILL.md: 702 bytes (unchanged)
- .claude/skills/ghidra-mcp-scratch-copy/SKILL.md: 3730 bytes (unchanged)
- .claude/skills/pipe-exit-status/SKILL.md: 2593 bytes (unchanged)
- .claude/skills/project-architect/SKILL.md: 13526 bytes (unchanged)
- .claude-state/memory/MEMORY.md: 214 bytes (unchanged)
- HOW_WE_WORK.md: 7037 bytes
- cookbook/INDEX.md: 7064 bytes
- rules/INDEX.md: 10700 bytes
### Carry audit — phase 1.6 (2026-10-01T20:27:46Z → open UTC, 1 sessions, 535 requests)

| file (read) | chars | n |
|---|---|---|
| game_80037824.c | 43.4k | 2 |
| PHASE_PLAN.md | 37.9k | 5 |
| decomp-environment.md | 29.6k | 8 |
| phaseend_index.py | 26.7k | 7 |
| twins.py | 16.2k | 4 |
| propagate.py | 15.5k | 2 |
| HOW_WE_WORK.md | 14.5k | 2 |
| plan_edit.py | 8.4k | 2 |
| T4.md | 8.1k | 2 |
| T3.md | 6.8k | 2 |
| file (write) | chars | n |
|---|---|---|
| reconcile.py | 17.0k | 3 |
| decomp-environment.md | 15.9k | 13 |
| propagate.py | 14.9k | 8 |
| draw_filter.py | 12.7k | 1 |
| twins.py | 10.5k | 7 |
| carve.py | 8.6k | 1 |
| RECAP.md | 5.9k | 1 |
| types_check.py | 5.4k | 1 |
| result kind | chars | n |
|---|---|---|
| Read | 262.9k | 58 |
| bash other | 254.0k | 119 |
| tools/card.py | 109.8k | 16 |
| tools/plan_edit.py | 99.9k | 32 |
| run.sh | 96.2k | 101 |
| tools/census.py | 38.6k | 15 |
| Agent | 28.5k | 26 |
| tools/task_log.py | 25.2k | 13 |
| tools/carve.py | 24.4k | 9 |
| tools/dup_census.py | 22.9k | 4 |
- whole reads over 20.0k: 2
- seed floor: retriever-code 5,561 (n 2, prev 5,667) · retriever-digest 5,574 (n 3, prev 5,606)
- outline credit: 3 outlines · 2 followed by a ranged read · 97.2k chars credited
- warm pings: 10 pings over 7 runs, 1 warmed waits over the TTL, rewrites across warmed waits 0, waits past the cap 0, pings cost $0.0215 vs rewrites replaced $0.4823, cheaper than one rewrite: yes
- toasts by cause (waiting): question 0, review 0, replan 0, permission 0, input 0, discussion 0, crash 0, idle 0, stop 0, subagent-stop 0, model 0, other 0
- router turns by cause: loop 17, relay 0, re-arm 0, other 2, relay cost $0.0000
- retriever re-asks: 0 of 1 retriever briefs repeat a lookup of the same run
- router: pa-session f11d66e5 · requests 48 · ctx at end 71.5k · growth T1 +460, T2 +470, T3 +1.5k, T4 +568, T5 +2.0k, T6 +458, T7 +555, T7 +1.4k, T8 +2.4k, T9 +1.7k · top: Agent 14.2k/13, tools/plan_edit.py 3.8k/12, tools/status.py 2.4k/9, other 1.9k/3, tools/managed.py 202/1
- noise: 60 lines 4.1k chars — usage: : 44 lines/2.9k, No such file or directory: 16 lines/1.2k
- price: read $0.20/Mtok · 1h write $7.57/Mtok · output $18.93/Mtok (phase model mix)
- median requests after a read: 1
- carry/request: 2094 chars, 535 requests (prev 1475 chars, 912 requests, growth 42.0%)
- candidate: audit_public_help (proposed)
- flag: carry per request grew 42% vs 1.5
- flag: 3 whole-plan reads by critic, expert-opus55
- flag: 19 tool-source reads by coder-opus55, critic, discuss, expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/phaseend_index.py  9.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/phaseend_index.py  993 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/plan_edit.py  6.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/phaseend_index.py  543 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/phaseend_index.py  2.0k chars  critic
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/plan_edit.py  1.8k chars  critic
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/propagate.py  2.3k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/propagate.py  13.2k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/twins.py  929 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/twins.py  528 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/reconcile.py  1.0k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/reconcile.py  665 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/twins.py  4.8k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/twins.py  9.9k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/banked.py  3.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/phaseend_index.py  2.9k chars  discuss
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/census.py  933 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/phaseend_index.py  9.7k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/phaseend_index.py  940 chars  expert-opus55
- flag: 2 whole reads over 20.0k by critic, expert-opus55
  file: /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/phase-ends/current/PHASE_PLAN.md  role: critic  chars: 27.3k
  file: /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/src/slus_012_79/game_80037824.c  role: expert-opus55  chars: 37.5k

## Agent runs
- T1 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 80k | $0.695 | saved $1.81 | completed | parent a0525f51
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 57k | $0.740 | saved $1.37 | completed | parent a2789770
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 59k | $0.545 | saved $1.71 | completed | parent a2789770
- T2 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 55k | $0.369 | saved $0.84 | completed | parent a0525f51
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 61k | $0.653 | saved $1.67 | completed | parent a59372dc
- T3 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 53k | $0.383 | saved $0.75 | completed | parent a0525f51
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 51k | $0.613 | saved $2.41 | completed | parent a5c28ae0
- T4 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 83k | $0.884 | saved $1.45 | completed | parent a0525f51
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 102k | $1.378 | saved $4.43 | completed | parent adb6e9ed
- T5 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 86k | $1.189 | saved $0.71 | completed | parent a0525f51
- T1 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 53k | $0.644 | saved - | completed | parent f11d66e5
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 68k | $0.719 | saved - | completed | parent ac1ab443
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 65k | $0.632 | saved $1.34 | completed | parent a8321060
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 145k | $2.682 | saved $2.24 | completed | parent a8321060
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 25k | $0.160 | saved - | completed | parent ac1ab443
- T2 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 46k | $0.440 | saved - | completed | parent f11d66e5
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 40k | $0.406 | saved $0.87 | completed | parent ab537db8
- T3 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 45k | $0.410 | saved - | completed | parent f11d66e5
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 71k | $0.727 | saved - | completed | parent a17e0608
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 135k | $2.165 | saved $3.24 | completed | parent a8321060
- T4 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 47k | $0.446 | saved - | completed | parent f11d66e5
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 96k | $1.468 | saved - | completed | parent a9084d30
- T5 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 99k | $2.057 | saved - | completed | parent f11d66e5
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 33k | $0.416 | saved $0.49 | completed | parent a560023a
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 82k | $0.967 | saved - | completed | parent a560023a
- T6 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 83k | $0.789 | saved $0.19 | completed | parent a0525f51
- T6 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 120k | $1.611 | saved $0.96 | completed | parent a858a4b8
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 25k | $0.272 | saved $0.02 | completed | parent a560023a
- T6 | critic | critic | claude-opus-5-5 | medium | ctx 37k | $0.257 | saved - | completed | parent a0525f51
- T6.1 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 66k | $0.435 | saved $0.21 | completed | parent a0525f51
- T6 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 59k | $0.695 | saved - | completed | parent f11d66e5
- T6 | retriever-digest | retriever | claude-sonnet-5-5 | medium | ctx 14k | $0.066 | saved - | completed | parent a7959bec
- T6.1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 61k | $0.502 | saved $0.87 | completed | parent a50e62ac
- T6 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 58k | $0.551 | saved - | completed | parent a7959bec
- T7 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 62k | $0.519 | saved $0.53 | completed | parent f11d66e5
- T7 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 32k | $0.271 | saved - | completed | parent aa3f5b8a
- T7 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 92k | $0.873 | saved $0.06 | completed | parent a0525f51
- T7 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 101k | $1.260 | saved $0.24 | completed | parent a1b1f7d4
- T7 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 58k | $0.504 | saved - | completed | parent f11d66e5
- … 13 more: PY ~/.claude/pa3/pa_ledger.py report

## Discussions
- (none)

## Deferred
- from T9: rerun the phase end (`PY tools/phaseend_index.py verify` on the host); all 10 clauses are GREEN as of this task. (unconsumed)

## Changes
- 2026-10-01 router: T1 next -> done
- 2026-10-01 router: T2 next -> done
- 2026-10-01 router: T3 next -> done
- 2026-10-01 router: T4 next -> done
- 2026-10-01 router: T5 next -> done
- 2026-10-01 router: T6 next -> done
- 2026-10-01 router: T7 next -> done
- 2026-10-01 developer: added T8 — milestone clauses 3,4,5: exemplar gated in dry run, plain control lines
- 2026-10-01 developer: added T9 — verifier: single capital letter in a count slot is a placeholder (clause 7)
- 2026-10-01 developer: milestone red 6/10; T8 (clauses 3,4,5 tool output) and T9 (verifier placeholder rule, clause 7) added by developer; milestone text unchanged; respawn the closer after T9
- 2026-10-01 router: T8 next -> done
- 2026-10-01 router: T9 next -> done

## Plain-English Recap
Phase 1.6 built the "multipliers": tools that let one matched function turn into several, and that pick which functions to attempt next. A "matched" or "banked" function is one whose C source compiles to exactly the original game bytes. The phase added six tools: twins.py finds near-identical function pairs; types_check.py guards one shared header of data shapes; carve.py cuts new source files safely; propagate.py writes one matched body into every identical copy; reconcile.py fits a separately matched function into its real source file without rewriting it; draw_filter.py lists which functions are worth attempting and why the rest are refused. The first phase-end check failed 4 of its 10 milestone checks, because several tasks had judged their own check more loosely than the milestone checker does; two follow-up tasks fixed that (T8 made the propagation dry run also test the original copy and added plain pass lines to two tools, T9 taught the checker that a capital letter in an "X of Y" count is a number placeholder). On the rerun all 10 checks pass: the whole game still rebuilds byte-identical (83 of 83 binaries), all 16 self-checking scanners pass, and banked functions went from 5 to 7 of 3426.
