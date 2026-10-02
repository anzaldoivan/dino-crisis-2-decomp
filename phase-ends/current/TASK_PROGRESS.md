# TASK_PROGRESS — T7 (attempt 2 → handoff)

## Done so far
- attempt 1 progress archived: logs/T7.progress1.md (038cd66). T7.c1 (family draw kind + cost line) committed 54aca6e (coder returned after attempt 1 ended).
- Wave F1 drawn (run.sh t7-f1draw): `--kind family --weight 2500 --id F1` → `family classes: 40 eligible of 40`, 25 targets, 744 insns, twin-skipped 15, `cost: 529.4k`, validated 25 of 25; cards 25 of 25, card control ok. Open row committed 5123e1f.
- Usage: `wave.py usage F1 --before --pct 4 --resets 2026-10-02T19:00Z` at 14:38:58Z; `--after --pct 10` at 14:46:47Z (same resets); drafter_k 612.3 hand-filled in config/usage.tsv (tool has no drafter_k flag). Committed e380c3a. Row: `F1 4 2026-10-02T19:00Z 10 2026-10-02T19:00Z 612.3 manual`.
- Drafters c2..c26 (coder-opus55, one per card) all returned; logs/T7.c2..c26.md committed in e380c3a. Claims: 19 MATCH; c6 0x80032908 LENGTH-DRIFT (8 runs), c8 0x800785a4 LENGTH-DRIFT (8), c9 0x8002d820 OPCODE-MIXED (8), c17 0x800787b4 SIZE-MISMATCH (5), c21 0x80074794 SIZE-MISMATCH (5), c25 0x8007e234 SCHEDULE-REORDER (2).
- Per-draft tokens: .run/t7-ledger.tsv (c, start, tokens, runs, label); sum 612286 over 25, mean 24.5k (T6 guard predicted 529.4k: 1.16x).
- d023195: router's uncommitted PHASE_PLAN.md status flip committed so the gate saw a clean tree (deviation).
- Detailed timeline + drafter findings for logs/T7.md: .run/t7-notes.md (append it to logs/T7.md).

## In flight
- `wave.py gate F1` on the HOST, detached: `run.sh --bg t7-f1gate` pid 88955, started ~14:48Z; at 15:46Z still running (container `python3 tools/wave.…` 58 min; host log empty: inner output buffered, as in T5). It commits one `bank …` per bank, then prints `gate: banked B + failed F + no-verdict V = D drafted`. Wait: `bash tools/run.sh --wait t7-f1gate --max 3500` (run in a background Bash so the notification wakes you). Do not commit or sync while it runs (it pulls ledgers and commits).

## Hypotheses rejected
- Drafters all at once: harness caps concurrent subagents at 20 → launch the rest as slots free.

## Current hypothesis / measured window
- 5h pct rose 6 points (4 → 10, integer granularity, so 5-7) for 612.3k drafter tokens (+ expert/router usage in the same minutes). Window ≥ 612.3k / 0.06 ≈ 10.2M drafter tokens (range 8.7M-12.2M), ~10x T6's 1000k Pro estimate (Max 5x tier). F1 = 0.06 windows.
- Re-projection draft: T6 set exe drafter cost 40.37M (incl. above-cliff) → 40.37M / 10.2M ≈ 4.0 windows (below cliff 23.53M → 2.3); far under 2x 40.4 → no `question`. Redo with F1's realised cost per bank once the gate gives B (cost per bank = 612.3k / B).

## Next five steps
1. Gate done → `python3 tools/wave.py recover F1` (host; G50, runs even if gate banked all) → note banked B, failed F; read `propagation candidate: A:S (n members) unregistered` lines.
2. `run.sh --bg t7-f1fleet -- python3 tools/wave.py fleet F1` → `harvest F1 --fn A:S --note "strip:none …"` per banked fn (no lever credited unless a drafter named one: c10 if/else duplicated store, c18 ternary abs — still `strip:none` unless strip test) → `close F1`.
3. Brief coder T7.c27: propagation registration tool (e.g. `propagate.py --register slus_012_79:<start>`): exemplar banked body → src/shared/func_<S>.inc.c with SHARED_ defines; families.tsv rows for every kind=game member of the dup.tsv exact class; batched carve per alias (carve.py one start at a time is slow: e8 class has 205 copies); then `--apply --family` per banked exemplar (fail-closed); fleet; commit. tools/ outside plan file list → deviation (T5 precedent). Interfaces: ## Interfaces propagate.py, dup_census.py, census.py; docs/ops/decomp-environment.md "Propagation" :83-103 and "Carve chain" :70-81.
4. Wave F2: `draw --kind family --weight 2500 --dry-run` (the 15 twin-skipped; failed ones are at budget attempts 1) → draw → cards → usage before/after (cache absent → `--pct` from `PY ~/.claude/pa3/pa_ledger.py sql "select ts,pct,resets_at from utilization where window='five_hour' order by ts desc" --limit 1`) → drafters (brief template = logs/T7.c2.md's brief shape: verdict.json first, work file d_<start>.c, dc.sh push + plateau.py, ≤ 8 runs, no #include/asm) → gate → recover → fleet → harvest → close → propagate. Repeat until `family classes:` shows 0 eligible.
5. tasks/T7.md: per-wave windows, re-projection arithmetic, census `--set exe` before (1313 of 1332) and after ≥ banked + propagated; verify command; `task_log.py finish T7`; commit.

## Gotchas
- zsh: `echo =====` fails; use other separators.
- `.run/status.json` inline python and `~/.claude/usage-ledger/running.json` reads are hook-denied.
- Drafter hand-backs carry no token count; it comes in the task-notification `<usage><subagent_tokens>`.
- SIZE-MISMATCH targets c17/c21: unsplit lui per access (assembler-expanded `lw sym`), likely a different toolchain object (neighbours 0x800787d4, 0x800787f4 same shape); c21: plateau n_target counts 3 trailing pad nops. c25: epilogue delay-slot fill differs (assembler). Candidates for a toolchain/handasm lane, not redraft.
- The gate prints nothing to the host log for ~1 h.

## State to carry verbatim
- 5h window resets 2026-10-02T19:00Z (epoch 1790967600). F1 before 4 (14:36:30Z sample) after 10 (14:46:13Z sample).
- census `--set exe` baseline: `stubs: 1313 of 1332 game functions not in C (set exe)`.
- Coder numbering: c2..c26 used (F1 drafters); next coder T7.c27.
