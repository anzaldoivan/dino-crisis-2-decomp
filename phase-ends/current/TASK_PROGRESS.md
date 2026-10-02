# TASK_PROGRESS — T5 attempt 1

Expert: expert-opus55 (attempt 1). Handoff at context threshold, mid-drafting of wave M1.

## Done so far
- Retriever (no report id; inline) on wave.py/plateau.py internals:
  - routing.tsv parse: RCOLS 11 cols (wave.py:436); `routing: M of B` counts a row iff float(rate) parses, int(drafted) > 0, basis == id of a waves.tsv row with kind `manual` and closed != `-` (wave.py:1013-1038). No route vocabulary check; banked/cost not examined.
  - `waves: C of W`: closed set and harvest `^done:\d+ notes$`. `fleet: F of W`: col11 `^83/83@<hex>$`.
  - gate never called plateau.py before c1; label was the agent's claim (wave.py:563-585).
  - plateau.py scratch `.run/plateau/draft/<draft stem>/func_<START>/` (stem always `draft` → keyed by start; concurrent runs on distinct starts safe). cpp `-P -undef -nostdinc`, NO -I: drafts must be self-contained.
  - reconcile parse_draft rejects ≠ 1 fn definition (static helper defs count).
- T5.c1 (opus55) done, commits 40ef454 (code) + 5cf1d28 (log): wave.py draw `--per-bucket N`, `--family F[,F]`, `--id ID`; gate runs plateau.py on every non-banked draft and journals its label (UNCOMPILED if no compile); config/routing.tsv seeded (6 rows b1..b6: 1-15, 16-50, 51-100, 101-200, 201-400, 401-open; route draft, card_cap 8, attempts 1, rest `-`); env.md waves docs. Verified: dry draw 30/30 5-per-bucket, cards 30/30, selftest 4/4, --check rc0 `routing: 0 of 6`, scanners 22/22, audit ok.
- Real draw: `dc.sh run python3 tools/wave.py draw --kind manual --weight 1000000 --per-bucket 5 --family exe,LOGO+ST --seed 1 --id M1` (run.sh t5-draw) → pool 721, targets 30 (insns 5668), 5 per bucket ×6, twin-skipped 0, validated 30/30. `cards M1` (run.sh t5-cards) → 30 of 30, card control ok. Pulled to host. Commit 9371ec1 (waves.tsv open M1 row + router's PHASE_PLAN.md/DISCUSSION_INDEX.md edits, which were dirty and would block gate's clean-tree check).
- Card cap definition (identical for all 30): card_cap = 8 plateau.py compile runs per drafting coder; attempts = 1 coder per target. Every drafter got the identical brief (only TARGET/DIR/k differ); brief text = any of the T5.c<k> briefs (see logs/T5.c<k>.md for each coder's own log).
- Drafters c2..c31 = targets.tsv data rows in order (c<k> = line k of .run/waves/M1/targets.tsv). 26 of 30 returned (see State).

## In flight
- Background drafters still running when attempt 1 ended: c13 slus_012_79:0x8003d58c (401-inf), c24 slus_012_79:0x8006c78c (201-400), c25 psx_bin_st9:0x800da16c (401-inf), c31 slus_012_79:0x80054c58 (401-inf).
- Their hand-backs/notifications go to attempt 1, which is gone. The respawn sees completion only through the files: a target is finished when its `.run/waves/M1/<alias>_<start>/verdict.json` status != `no-verdict` AND `phase-ends/current/logs/T5.c<k>.md` exists. Their token cost (subagent_tokens) is lost unless the router relays the notifications; if lost, record cost for those 4 as `n/a` and compute per-bucket cost_ctx_k from the known coders only, saying so in basis/Deviations (never invent numbers).

## Hypotheses rejected
- "wave id must be w<NN>": rejected, wave.py treats ids opaquely (WAVE_ID regex) except allocation; `--id M1` added.
- "weight alone can give 5 per bucket": rejected, greedy round-robin consumes misfits; `--per-bucket` added.

## Current hypothesis
- Small/mid buckets bank near 100% at the plateau level; drop appears in 201-400 and 401+ (2 of 3 known 201-400 are plateaus, one GTE). The gate (real-unit reconcile + hash) is the only arbiter; plateau MATCH is a claim until gate.

## Next 5 steps
1. Wait (idle; do not sync, do not touch .run/waves/M1 or the volume) until all 8 in-flight dirs have a final verdict.json + log. If a drafter is dead (no progress for a long time, router confirms), its target is no-verdict per G50 (draft.c, if any, still gets scored) — do NOT redraft.
2. Commit drafter logs (`bash tools/commit_task.sh T5 "drafter logs c2-c31"`; sweeps phase-ends/current/logs) so the tree is clean; `git status --short` must be empty (gate requires it).
3. Gate: `bash tools/run.sh --bg t5-gate -- /opt/homebrew/opt/python@3.14/bin/python3.14 tools/wave.py gate M1` then `bash tools/run.sh --wait t5-gate --max 280` (repeat waits). Expect `gate: banked B + failed F + no-verdict V = 30 drafted`, per-unit bank commits. Read propagation candidate lines. Then G50 recovery: triage failures body vs plumbing; plumbing ones bank through reconcile without redraft.
4. Fleet: `bash tools/run.sh --bg t5-fleetw -- PY tools/wave.py fleet M1` + waits → `83/83@<HEAD>`. Commit. Harvest each banked fn: `PY tools/wave.py harvest M1 --fn A:S --note "strip:none ..."` (no lever credited unless one was used and strip-tested). Then `PY tools/wave.py close M1`.
5. Write config/routing.tsv measured columns (drafted, banked, rate = banked/drafted, cost_ctx_k = sum coder tokens in bucket / banked / 1000, basis M1) and route by the cliff rule (cliff = smallest bucket whose rate < half the best bucket's rate: buckets below → `draft`; cliff bucket and above → `draft+permute` where labels are permuter-bucket, `hard` where rate 0 / structural). Then coder (one) for: scanner `wave` row retightened to the routing line; fix reconcile-scanner breakage (gate leaves `.run/reconcile/<alias>_<start>/draft.c` per scored draft, `reconcile.py --check` globs them → scanners 21/22; c1 log line 19); env.md routing numbers (H7). Then the task verify command, log, summary, `task_log.py finish T5`.

## Gotchas
- harness: plateau.py prints no diff; drafters read our own compiled .s from `.run/plateau/draft/draft/func_<START>/` (read-only) to see the residual — fine, our output.
- generalizable: GTE-using fns (inline cop2 ctc2/lwc2/rtpt/swc2) cannot be drafted under a no-asm() brief, and gate's G11 verbatim grep (`asm(`, `__asm__`) would refuse PsyQ inline_c.h-style GTE macros anyway → a GTE lane/whitelisted macro header is needed (c18: LENGTH-DRIFT d=-25 calling extern gte_* fns). T2 RES probe already uses a gte_ldrgb inline-asm macro.
- generalizable: spimdisasm's "Handwritten function" header marker + a non-GCC `addi` (c9 slus_012_79:0x8005e160) and BIOS A-table trampolines that set $t1 and jump to 0xA0 (c2 slus_012_79:0x8008124c) are handasm, not C: draw_filter should refuse them; ledger them `handasm` with basis (later task; config/ledger.tsv not in T5 files).
- c1 deviation: gate's reconcile probes leave `.run/reconcile/<target>/draft.c` dirs that break the reconcile scanner denominator; fix after the gate (do not delete them before gate's recovery step reads them).
- c11 restored its run-5 draft (OPCODE-MIXED) after a worse run 8; gate relabels via plateau.py anyway.
- harness: plateau scratch tag is `draft/<stem>` → every drafter's draft.c shares `.run/plateau/draft/draft/`; per-fn subdir func_<START> keeps them apart, but c7 used a uniquely named copy to be safe. A future brief may name drafts uniquely.
- c17 left a `run.sh` file inside its dir in the volume (file, not dir; gate's stray check is for dirs).
- Never `dc.sh sync` while drafters run (wipes /work asm/ they read; plateau target split).

## State to carry verbatim
- Drafter ledger (also `.run/t5-ledger.tsv`, helper `.run/t5-led.sh <ck> <tokens> <label> <runs>`):
  c2 slus_012_79:0x8008124c 1-15 19203 SIZE-MISMATCH runs=2 (BIOS trampoline, handasm)
  c3 psx_bin_st8:0x800da040 16-50 19880 MATCH runs=1
  c4 psx_bin_st9:0x800dc094 51-100 39847 REGALLOC-PERM runs=8
  c5 slus_012_79:0x8002219c 101-200 25081 MATCH runs=1
  c6 psx_bin_st9:0x800d740c 201-400 71828 LENGTH-DRIFT runs=8 (d=-2, two load-delay fills)
  c8 slus_012_79:0x80063cc4 1-15 19334 MATCH runs=2
  c9 slus_012_79:0x8005e160 16-50 21891 OPCODE-MIXED runs=2 (handwritten marker)
  c10 slus_012_79:0x8002204c 51-100 29495 MATCH runs=2
  c11 slus_012_79:0x80051998 101-200 32557 OPCODE-MIXED runs=8
  c12 slus_012_79:0x80039654 201-400 29170 MATCH runs=3
  c14 slus_012_79:0x80065004 1-15 18666 MATCH runs=1
  c15 slus_012_79:0x80034d14 16-50 18726 MATCH runs=1
  c16 slus_012_79:0x8003de8c 51-100 24633 MATCH runs=2
  c17 slus_012_79:0x8003d2d4 101-200 31080 MATCH runs=4
  c18 slus_012_79:0x8005c608 201-400 39461 LENGTH-DRIFT runs=2 (GTE)
  c20 slus_012_79:0x80064998 1-15 18105 MATCH runs=1
  c21 slus_012_79:0x8004587c 16-50 19523 MATCH runs=1
  c22 slus_012_79:0x8002be20 51-100 23607 MATCH runs=1
  c26 slus_012_79:0x8005f088 1-15 18813 MATCH runs=1
  c27 psx_bin_st1:0x800d9000 16-50 21284 MATCH runs=2
  c28 slus_012_79:0x8004b7b8 51-100 22133 MATCH runs=1
  c29 slus_012_79:0x80036e94 101-200 23499 MATCH runs=1
  c7 slus_012_79:0x80053eac 401-inf 78986 OPCODE-MIXED runs=8 (sched1 hoists 3 items above a call at the head)
  c19 slus_012_79:0x80068ca0 401-inf 49554 LENGTH-DRIFT runs=1 (GTE)
  c23 psx_bin_st1:0x800d5990 101-200 31936 MATCH runs=5
  c30 slus_012_79:0x80057a8c 201-400 26935 MATCH runs=1
- Commits: 40ef454, 5cf1d28 (c1), 9371ec1 (M1 open). waves.tsv M1 row: `M1 manual 2026-10-02 - 30 - … - open`.
- Drafter brief invariant: self-contained draft.c, one fn `func_<START>`, no asm()/INCLUDE_ASM/#include, ≤ 8 plateau runs, verdict.json first.

## Coder runs so far
- T5.c1 opus55 done (40ef454, 5cf1d28). T5.c2..c31 drafters (no commits; logs phase-ends/current/logs/T5.c<k>.md, uncommitted).

## Reports commissioned
- retriever-code: wave.py/plateau.py/reconcile.py internals — answer inline, no report written (no id).
