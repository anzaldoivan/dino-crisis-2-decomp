# PhaseEnd — Phase 1.7: The codegen map and the permuter (implements Gen 1.7)
Approved: 2026-10-01 | Closed: 2026-10-02 | Planner: claude-opus-5-5/medium | Tasks: 9 done, 0 superseded

## Milestone
Milestone: ≥ 1 byte-proven lever per pass group with a symptom-keyed triage table; compiler source staged at the pinned version; permuter stored-draft run proves iteration; plateau classifier labels the planted plateau; cookbook index check passes with its coverage assertion — verified by: `PY tools/compiler_source.py --check` → exit 0, prints `compiler source: gcc 2.95.2 sha256 ok` and `version control: ok`; `bash tools/docker/dc.sh sync && bash tools/docker/dc.sh run python3 tools/codegen_map.py --check` → exit 0, prints `levers: L of L byte-proven` with L ≥ 5, `pass groups: 5 of 5 covered` and `inert control: ok`; `bash tools/docker/dc.sh run python3 tools/permute.py --check` → exit 0, prints `permuter: stored draft iterated W times, D distinct candidates` with W ≥ 100 and D ≥ 2, and `scorer control: ok`; `bash tools/docker/dc.sh run python3 tools/plateau.py --check` → exit 0, prints `plateau: P of P planted labelled` with P ≥ 1 and `match control: ok`; `PY tools/cookbook_index.py --check` → exit 0, prints `cookbook-index: N of N entries indexed` with N ≥ 49, `refs: R of R resolve` with R ≥ 5 and `control: ok`; `bash tools/docker/dc.sh run python3 tools/harness.py --scanners` → exit 0, prints `scanners: S of S denominator+control ok` with S ≥ 20; `bash tools/docker/dc.sh run bash tools/fleet_check.sh` → exit 0, prints `83 of 83 byte-identical` and `harness: P of P pairs agree, 0 disagreements` with P ≥ 6; `PY tools/audit_public.py` → exit 0
Verified: `bash tools/docker/dc.sh sync && PY tools/phaseend_index.py verify --verbose` (run.sh t9-verify) → exit 0, `VERIFY: GREEN (8/8)`; card.py check chars=6999 cap=7000

## Tasks
- T1 — gcc 2.95.2 source staged; per-pass dumps from a real TU | Done: vanilla gcc-2.95.2 source fetched, sha256-pinned and version-checked against the live pinned cc1 banner; per-pass dumps from the pinned cc1 on a banked TU show all five pass groups run at -O2 -mips1; scanner `compiler_source` live. | commit: 3fd906e | tasks/T1.md | logs/T1.md
- T2 — lever registry, byte-proof checker, generated triage table | Done: `tools/codegen_map.py` byte-proves rows of `config/levers.tsv` under the pinned triple through tools/probe.py; controls (inert every run; not-matching and proven in `--selftest`) pass; docs/codegen-map.md generated (zero rows); scanner row `codegen_map` added. | commit: dbdcddf | tasks/T2.md | logs/T2.md
- T3 — levers for regalloc and combine, byte-proven on game fns | Done: one byte-proven lever each for regalloc (`reuse-user-var`, slus_012_79:0x8005ED34, C0046) and combine (`mask-then-shift`, slus_012_79:0x8001B538, C0045); reproducer batteries with dump attribution; scanner scoped. | commit: - | tasks/T3.md | logs/T3.md
- T4 — levers for sched-reorg, byte-proven on game fns | Done: two byte-proven sched-reorg levers: `multi-set-param` (sched1 birthing boost, slus_012_79:0x8003E164, C0048) and `swap-returning-arm` (jump.c arm order/polarity, slus_012_79:0x8005EE58, C0047); batteries with dump attribution; scanner extended. | commit: - | tasks/T4.md | logs/T4.md
- T5 — levers for expand-cse and loop, byte-proven on game fns | Done: one byte-proven lever each for expand-cse (`shift-scaled-index`, slus_012_79:0x800604F4, C0050) and loop (`while-postdec`, slus_012_79:0x800474B4, C0049); batteries with dump attribution; C0002 tells mapped to groups in the generated codegen map; scanner `codegen_map` now plain `--check` (all five groups covered). | commit: - | tasks/T5.md | logs/T5.md
- T6 — permuter pinned in the image, stored-draft run proves iteration | Done: decomp-permuter pinned in the image (venv /opt/permuter); `tools/permute.py` runs it on a stored lever draft with a relocation-masked scorer in a wrapper; `--check` iterates 120 times on `reuse-user-var`, scorer control ok; scanner `permute`. | commit: - | tasks/T6.md | logs/T6.md
- T7 — plateau classifier labels the planted plateaus | Done: `tools/plateau.py` labels a draft vs a game fn (or a permute dir's best candidate) with one of 12 fixed labels, a bucket and the lever ids of the label's groups; `--check` labels all 6 planted plateaus correctly; scanner `plateau`. | commit: - | tasks/T7.md | logs/T7.md
- T8 — cookbook symptom index with coverage assertion | Done: `tools/cookbook_index.py` (stdlib, host or container) generates docs/cookbook-symptoms.md and `--check` asserts cookbook/INDEX.md and file coverage, lever/C0002 ref resolution and freshness, with a planted control; scanner `cookbook_index`. | commit: - | tasks/T8.md | logs/T8.md

## Decisions that still bind
- T1 — dumps come from the pinned CC1PSX.EXE itself (`-da` under wibo works); no fallback cc1 for dumps
- T1 — source cites say "vanilla 2.95.2"; the pinned cc1 is Sony-patched (`BUILD 4.0.0030`), so a lever's byte proof is the authority (DK-52)
- T2 — draft symbol is `func_%08X` of the row's start; drafts live in tools/probes/levers/ and need no `-Iinclude`
- T2 — classification order, first hit wins: compile-error, identical-drafts (cpp output byte-equal), not-matching (after vs game), before-matches (inert), else proven
- T2 — docs/codegen-map.md is derived from levers.tsv text only; regenerate on the host with `PY tools/codegen_map.py --write`, then `dc.sh sync` before `--check` (files written inside `dc.sh run` stay in the volume)
- T3 — the `codegen_map` scanner command carries `--groups <groups proven so far>`; each lever task extends it;
- T4 — the pinned cc1's sched1/sched2 is the old list scheduler `gcc/sched.c`, not haifa-sched.c (vanilla 2.95.2 enables
- T5 — C0002 tell → group mapping lives in config/inherited_tells.tsv (tell text verbatim from C0002 col 1);
- T6 — permuter scorer = Levenshtein (cost 1) over probe.py `extract` words with per-index mask = OR of both sides'
- T6 — permuter determinism = threads 1, `random.seed(S)` (default 1), PYTHONHASHSEED=0; never the permuter's `--seed`
- T6 — target.o = the game fn's split `func_<START>.s` assembled with Makefile AS/ASFLAGS; `make split ONLY=<alias>` when absent after sync
- T6 — a score-0 permuter candidate is a waypoint; banked only through reconcile + clean fleet check (G61)
- T7 — plateau label order (first match, C0043): MATCH, SIZE-MISMATCH, LENGTH-DRIFT, REGALLOC-PERM, SCHEDULE-REORDER,
- T7 — bucket is policy derived at print time (kit _ROUTE), never stored: MATCH integration; SIZE-MISMATCH redraft;
- T8 — docs/cookbook-symptoms.md is generated by `tools/cookbook_index.py --write`; rerun it after any
- T9 — G12 ruling on plateau.py evidence lines: mnemonic names at diff indices plus counts (no operands, immediates,
- norm: plateau.py evidence lines (mnemonic names and counts at diff indices, no operands/encodings) are within G12 → rule G73
- norm: a lever's byte proof outranks any source cite of vanilla 2.95.2 (Sony-patched cc1, DK-52) → existing G67; docs/ops/decomp-environment.md:626
- env: dumps come from the pinned CC1PSX.EXE with `-da` under wibo, no fallback cc1 → docs/ops/decomp-environment.md:610-613
- env: sched/sched2 in the pinned cc1 are gcc/sched.c, not haifa-sched.c; cite sched.c → docs/ops/decomp-environment.md:619-621 (added here), C0048, C0051
- contract: draft symbol `func_<START>`, drafts in tools/probes/levers/ without `-Iinclude` → tools/probes/levers/README.md:6, `codegen_map.py --check`
- contract: lever classification order compile-error · identical-drafts · not-matching · before-matches · proven → docs/ops/decomp-environment.md:634-636, `codegen_map.py --selftest`
- contract: docs/codegen-map.md and docs/cookbook-symptoms.md are generated on the host (`--write`), then `dc.sh sync`; `--check` fails stale → docs/ops/decomp-environment.md:638, :680-685
- contract: C0002 tell → pass group mapping lives in config/inherited_tells.tsv → docs/ops/decomp-environment.md:641
- contract: permuter scorer (masked Levenshtein), determinism (threads 1, random.seed, PYTHONHASHSEED=0), target.o from split asm → docs/ops/decomp-environment.md:648-653, C0052, C0053
- norm: a score-0 permuter candidate is a waypoint, banked only through reconcile plus a clean fleet check → existing G61; docs/ops/decomp-environment.md:588
- contract: plateau label order and print-time buckets → docs/ops/decomp-environment.md:666-672, `plateau.py --check`
- scope: Next task needs: the codegen_map scanner now runs plain (5 of 5 groups); a new lever row or cookbook entry reruns `codegen_map.py --write` / `cookbook_index.py --write` before `--check`
- scope: Next task needs: card headroom is 7 chars (6993 of 7000); any new card line is paid for by trimming
- scope: Next task needs: matches found while proving levers are standalone, not banked; banking rate is phase 1.8

## Rules proposed
- (none)

## Cookbook entries added
C0045 | Byte-field mask/shift order survives combine only when the mask fits andi | codegen-map,combine,mask,shift,andi,extraction,insn-shape | 2026-10-01 | 1.7/T3 | -
C0046 | A late temp's v0/v1 choice follows local vs global ownership; reuse a user var to move it | codegen-map,regalloc,local-alloc,global-alloc,v0,v1,temp-reuse,reg-substitution | 2026-10-01 | 1.7/T3 | -
C0047 | If/else arm order: jump.c swaps an early-return then-arm last; invert the test to lay it | codegen-map,jump,block-order,branch-polarity,delay-slot,dbr,sched-reorg | 2026-10-02 | 1.7/T4 | -
C0048 | Param def sinks to its use in sched1 (birthing boost): give the pseudo a 2nd SET (++n) to keep it on top | codegen-map,sched,sched1,sched2,birthing,insn-order,param,sched-reorg | 2026-10-02 | 1.7/T4 | -
C0049 | Counted for vs while (n--): check_dbra_loop reverses a counting-only for to 0; n-- tests vs a hoisted -1 | codegen-map,loop,check_dbra_loop,move_movables,reversal,hoist,biv,count-down | 2026-10-02 | 1.7/T5 | -
C0050 | Scaled index: x*4 / p[i] puts the index first in the addu (expand MULT-first swap); i<<2 keeps base first | codegen-map,expand-cse,expand,fold,operand-order,address,mult,shift | 2026-10-02 | 1.7/T5 | -
C0051 | Check which scheduler file a gcc target builds before citing it | gcc,sched,haifa,sched-reorg,codegen-map | 2026-10-02 | 1.7/T4 | phase-ends/current/tasks/T4.md
C0052 | decomp-permuter --seed is a replay, not a search seed; seed random in a wrapper | permuter,determinism,seed | 2026-10-02 | 1.7/T6 | phase-ends/current/tasks/T6.md
C0053 | Split asm is wiped by sync; regenerate it with make split ONLY=<alias> | build,split,docker,permuter | 2026-10-02 | 1.7/T6 | phase-ends/current/tasks/T6.md
C0054 | Plateau classifier: test length drift before index-wise labels | plateau,classifier,diff,codegen-map | 2026-10-02 | 1.7/T7 | phase-ends/current/tasks/T7.md

## Research
R1.7-001 | extract planning facts (a)-(f) from the decomp-architect corpus and phase-1.4 tasks | Kit codegen map, permuter, plateau classifier, symptom index: shapes for DC2 phase 1.7 | codegen-map, permuter, plateau-classifier, cookbook-index, gcc-version, DK-6, DK-15, DK-44, DK-45, DK-47, DK-52 | retriever-digest | 2026-10-01 | 35 lines
R1.7-002 | per-rule detail of kit regalloc.md RC-1..15, combine/regmove levers, local/global-alloc priority formula | Kit gcc-2.7.2 map: RC-1..15 per-rule table, combine/regmove levers, priority formula | gcc-2.7.2, regalloc, combine, kit, levers, T3 | retriever-digest | 2026-10-01 | 47 lines
R1.7-003 | T3 | gcc 2.95.2 producer census: regalloc and combine | gcc-2.95.2,regalloc,combine,producer-census | retriever-code | 2026-10-01 | 26 lines
R1.7-004 | producer census for sched/delay-slot/branch tells, cc1 vs maspsx | gcc 2.95.2 mips sched/reorg producer census + maspsx | gcc-2.95.2, mips, haifa-sched, reorg, maspsx | retriever-code | 2026-10-01 | 22 lines
R1.7-005 | digest kit sched.md (+README:29-36) and locate 2.95.2 counterparts for T4 | Kit sched map S1-S13/D1-D5 vs gcc 2.95.2 | sched, dbr, haifa-sched, reorg, mips, S1-S13, D1-D5, gcc-2.95.2 | retriever-digest | 2026-10-01 | 46 lines
R1.7-006 | T5 phase 1.7, map kit rules to 2.95.2 producers | expand-cse pass group: kit cse_expr.md translated to gcc 2.95.2 | gcc-2.95.2, cse, gcse, expr, synth_mult, switch, dumps | retriever-digest | 2026-10-02 | 54 lines
R1.7-007 | T5 phase 1.7 loop-group lever translation | gcc 2.95.2 loop pass group vs 2.7.2 kit loop.md | gcc-2.95.2, loop.c, unroll.c, jump.c, stmt.c, strength-reduce, dbra | retriever-digest | 2026-10-02 | 28 lines

## Audit
- seed: median 14k, max 14k, n=10 (phase 1.7)
- previous phase 1.6: median 14k, growth -0.3%
- CLAUDE.md: 1202 bytes (unchanged)
- .claude/skills/dc2-volume-runs/SKILL.md: 1294 bytes (new)
- .claude/skills/docker-vm-no-privileged/SKILL.md: 702 bytes (unchanged)
- .claude/skills/ghidra-mcp-scratch-copy/SKILL.md: 3730 bytes (unchanged)
- .claude/skills/pipe-exit-status/SKILL.md: 2593 bytes (unchanged)
- .claude/skills/project-architect/SKILL.md: 13526 bytes (unchanged)
- .claude-state/memory/MEMORY.md: 214 bytes (unchanged)
- HOW_WE_WORK.md: 7041 bytes
- cookbook/INDEX.md: 8942 bytes
- rules/INDEX.md: 10848 bytes
### Carry audit — phase 1.7 (2026-10-02T02:04:03Z → open UTC, 1 sessions, 874 requests)

| file (read) | chars | n |
|---|---|---|
| probe.py | 63.4k | 7 |
| residual_class.py | 51.8k | 4 |
| regalloc.md | 44.0k | 2 |
| sched.md | 35.7k | 2 |
| br0uipd7b.txt | 34.0k | 1 |
| cse_expr.md | 30.4k | 1 |
| loop.md | 26.2k | 1 |
| local-alloc.c | 24.8k | 7 |
| loop.c | 21.4k | 8 |
| codegen_map.py | 20.7k | 7 |
| file (write) | chars | n |
|---|---|---|
| plateau.py | 15.9k | 2 |
| decomp-environment.md | 11.2k | 8 |
| codegen_map.py | 10.1k | 1 |
| retriever-digest-kit-regalloc-rc1-15-combine-levers.md | 9.9k | 1 |
| cookbook_index.py | 9.5k | 1 |
| retriever-digest-expand-cse-gcc295-map.md | 8.7k | 1 |
| permute.py | 7.7k | 1 |
| retriever-digest-kit-sched-s1-s13-d1-d5-to-2952.md | 7.2k | 1 |
| result kind | chars | n |
|---|---|---|
| Read | 566.8k | 124 |
| bash other | 564.6k | 311 |
| tools/card.py | 161.0k | 22 |
| Grep | 153.7k | 99 |
| tools/cc1_dumps.sh | 92.3k | 36 |
| run.sh | 49.8k | 66 |
| tools/codegen_map.py | 48.2k | 35 |
| tools/probe.py | 41.9k | 14 |
| Agent | 40.5k | 37 |
| tools/plan_edit.py | 31.7k | 15 |
- spilled: 1 results, 32.9k chars on disk, 1 read whole
- whole reads over 20.0k: 4
- seed floor: retriever-code 5,660 (n 8, prev 5,561) · retriever-digest 5,720 (n 7, prev 5,574)
- outline credit: 3 outlines · 1 followed by a ranged read · 0 chars credited
- warm pings: 15 pings over 8 runs, 10 warmed waits over the TTL, rewrites across warmed waits 0, waits past the cap 0, pings cost $0.1756 vs rewrites replaced $3.0155, cheaper than one rewrite: yes
- toasts by cause (waiting): question 0, review 0, replan 0, permission 0, input 0, discussion 0, crash 0, idle 0, stop 0, subagent-stop 0, model 0, other 0
- router turns by cause: loop 13, relay 15, re-arm 6, other 0, relay cost $0.3456
- retriever re-asks: 5 of 13 retriever briefs repeat a lookup of the same run
- router: pa-session 4218494a · requests 77 · ctx at end 69.9k · growth T1 +3.4k, T2 +1.4k, T3 +1.9k, T4 +2.6k, T5 +2.7k, T6 +3.3k, T7 +2.2k, T8 +1.8k, T9 +1.1k · top: Agent 10.9k/10, tools/status.py 7.1k/12, other 4.8k/23, tools/plan_edit.py 2.9k/10
- noise: 120 lines 9.4k chars — No such file or directory: 62 lines/5.2k, usage: : 58 lines/4.1k
- price: read $0.20/Mtok · 1h write $7.45/Mtok · output $18.62/Mtok (phase model mix)
- median requests after a read: 2
- carry/request: 2188 chars, 874 requests (prev 1492 chars, 540 requests, growth 46.6%)
- candidate: audit_public_help (proposed)
- flag: carry per request grew 47% vs 1.6
- flag: 1 spilled results read whole
  command: for t in 1 2 3 4 5 6 7 8; do echo "=== T$t"; cat phase-ends/current/tasks/T$t.md  reader: expert-opus55
- flag: 32 tool-source reads by coder-opus55, expert-opus55, retriever-code, retriever-digest
  /Users/ThinkPad/.claude/projects/-Users-ThinkPad-orca-workspaces-dino-crisis-2-decomp-bootstrap/4218494a-44a6-414a-8060-9ed1ef341a21/tool-results/br0uipd7b.txt  34.0k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  3.3k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probes/levers/README.md  804 chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  9.4k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/permute.py  5.8k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/permuter/compile.sh  1.4k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/codegen_map.py  2.2k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/permuter/permuter_run.py  4.3k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/permuter/permuter_run.py  326 chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/docker/Dockerfile  2.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  15.5k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/splat_gen.py  821 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probes/levers/mask-then-shift.before.c  498 chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probes/levers/mask-then-shift.after.c  501 chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  16.8k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probes/func_80052634.c  406 chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/codegen_map.py  2.1k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/compile_only.sh  3.2k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  9.6k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/splat_gen.py  151 chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/codegen_map.py  481 chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/codegen_map.py  11.0k chars  coder-opus55
  /Users/Shared/kits/decomp-architect/corpus/tools/P7/residual_class.py  16.6k chars  coder-opus55
  /Users/Shared/kits/decomp-architect/corpus/tools/P7/residual_class.py  4.2k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/codegen_map.py  1.3k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/codegen_map.py  547 chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/codegen_map.py  3.1k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  6.1k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  2.6k chars  retriever-code
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/plateau.py  2.6k chars  expert-opus55
  /Users/Shared/kits/decomp-architect/corpus/tools/P7/residual_class.py  24.0k chars  retriever-digest
  /Users/Shared/kits/decomp-architect/corpus/tools/P7/residual_class.py  6.9k chars  retriever-digest
- flag: 4 whole reads over 20.0k by expert-opus55, retriever-digest
  file: /Users/ThinkPad/.claude/projects/-Users-ThinkPad-orca-workspaces-dino-crisis-2-decomp-bootstrap/4218494a-44a6-414a-8060-9ed1ef341a21/tool-results/br0uipd7b.txt  role: expert-opus55  chars: 32.7k
  file: /Users/Shared/kits/decomp-architect/corpus/cookbook/gcc-2.7.2-map/cse_expr.md  role: retriever-digest  chars: 29.0k
  file: /Users/Shared/kits/decomp-architect/corpus/cookbook/gcc-2.7.2-map/sched.md  role: retriever-digest  chars: 33.5k
  file: /Users/Shared/kits/decomp-architect/corpus/cookbook/gcc-2.7.2-map/loop.md  role: retriever-digest  chars: 24.9k

## Agent runs
- T1 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 83k | $0.664 | saved - | completed | parent 832f9bc5
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 56k | $0.569 | saved - | completed | parent a584978a
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 39k | $0.328 | saved - | completed | parent a584978a
- T1 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 54k | $0.520 | saved - | completed | parent 4218494a
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 61k | $0.873 | saved - | completed | parent ae8de235
- T2 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 75k | $0.643 | saved - | completed | parent 832f9bc5
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 60k | $0.645 | saved - | completed | parent a6478909
- T3 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 81k | $0.798 | saved - | completed | parent 832f9bc5
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 69k | $0.702 | saved - | completed | parent a562a3c3
- T2 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 56k | $0.413 | saved - | completed | parent 4218494a
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 54k | $0.499 | saved - | completed | parent a86024ab
- T3 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 80k | $0.811 | saved $0.15 | completed | parent 4218494a
- T3 | retriever-code | retriever | claude-sonnet-5-5 | medium | ctx 56k | $0.289 | saved - | completed | parent a51efb79
- T3 | retriever-digest | retriever | claude-sonnet-5-5 | medium | ctx 41k | $0.188 | saved - | completed | parent a51efb79
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 72k | $0.702 | saved - | completed | parent a562a3c3
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 115k | $1.394 | saved - | completed | parent a51efb79
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 70k | $0.675 | saved $0.12 | completed | parent a51efb79
- T4 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 86k | $0.837 | saved - | completed | parent 832f9bc5
- T4 | retriever-digest | retriever | claude-sonnet-5-5 | medium | ctx 43k | $0.173 | saved - | completed | parent a49d5592
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 49k | $0.427 | saved - | completed | parent a49d5592
- T4 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 79k | $0.823 | saved $0.04 | completed | parent 4218494a
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 90k | $1.029 | saved - | completed | parent a49d5592
- T4 | retriever-digest | retriever | claude-sonnet-5-5 | medium | ctx 15k | $0.047 | saved - | completed | parent ab6dfd15
- T4 | retriever-digest | retriever | claude-sonnet-5-5 | medium | ctx 45k | $0.241 | saved - | completed | parent ab6dfd15
- T4 | retriever-code | retriever | claude-sonnet-5-5 | medium | ctx 44k | $0.255 | saved - | completed | parent ab6dfd15
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 125k | $1.700 | saved - | completed | parent ab6dfd15
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 81k | $0.887 | saved - | completed | parent ab6dfd15
- T4 | retriever-code | retriever | claude-sonnet-5-5 | medium | ctx 11k | $0.027 | saved - | completed | parent ac0be351
- T5 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 82k | $0.720 | saved - | completed | parent 832f9bc5
- T5 | retriever-digest | retriever | claude-sonnet-5-5 | medium | ctx 46k | $0.135 | saved - | completed | parent a21c0774
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 111k | $1.147 | saved - | completed | parent a21c0774
- T5 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 88k | $0.965 | saved - | completed | parent 4218494a
- T5 | retriever-digest | retriever | claude-sonnet-5-5 | medium | ctx 54k | $0.261 | saved - | completed | parent a1d05181
- T5 | retriever-digest | retriever | claude-sonnet-5-5 | medium | ctx 47k | $0.280 | saved - | completed | parent a1d05181
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 88k | $1.046 | saved - | completed | parent a1d05181
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 110k | $1.412 | saved - | completed | parent a1d05181
- T5 | retriever-code | retriever | claude-sonnet-5-5 | medium | ctx 13k | $0.048 | saved - | completed | parent a1946ad0
- T5 | retriever-code | retriever | claude-sonnet-5-5 | medium | ctx 11k | $0.025 | saved - | completed | parent ad008491
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 75k | $0.727 | saved - | completed | parent a21c0774
- … 24 more: PY ~/.claude/pa3/pa_ledger.py report

## Discussions
- (none)

## Deferred
- from T9: phase end: rule on the G12 binding above; `phaseend_index.py verify` is GREEN 8/8 as of this task (unconsumed)

## Changes
- 2026-10-01 router: T1 next -> done
- 2026-10-01 router: T2 next -> done
- 2026-10-01 router: T3 next -> done
- 2026-10-02 router: T4 next -> done
- 2026-10-02 router: T5 next -> done
- 2026-10-02 router: T6 next -> done
- 2026-10-02 router: T7 next -> done
- 2026-10-02 router: T8 next -> done
- 2026-10-02 router: T9 next -> done

## Plain-English Recap
Phase 1.7 built the toolkit for the moment a hand-written C function compiles to almost, but not exactly, the game's
machine code. The compiler (gcc 2.95.2, Sony's PlayStation build) turns C into code through a fixed series of passes;
for each of its five pass groups the phase found at least one "lever", a small change in how the C is written that
moves the output in a known way, and proved it by making a real game function match byte for byte (6 levers, 5 of 5
groups). It also staged the compiler's own source for reference, wired in a "permuter" (a program that tries many
small rewrites of a draft automatically and scores how close each gets), added a classifier that names why a stuck
draft differs, and generated a symptom index over the technique notebook (the cookbook) so a symptom leads to a lever.
The whole game still rebuilds to 83 of 83 byte-identical binaries.
