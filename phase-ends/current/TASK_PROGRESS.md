# TASK_PROGRESS — T7 (attempt 1 → handoff)

## Done so far
- Context loaded; census `--set exe` baseline: `stubs: 1313 of 1332 (set exe)` (run.sh t7-census0).
- Scope measured (.run/t7/exe_classes2.tsv in the volume): 40 exe-headed exact classes with >= 1 slus_012_79 game member, all b1-b4; 103 slus game members; 460 game copies. Est. one card per class ≈ 805k drafter tokens (under the 1000k guard before twin-skips).
- draw_filter refuses most slus reps (`open-twin-sibling`: alphabetical rep is an overlay copy; `has-banked-twin`: e334, e337).
- Retriever findings: logs/T7.md (draw `--kind family` had no own pool; propagate members only from families.tsv; no registration tool; only 24 c_units rows, so overlay copies need carves). R1.8-002 adopted (T5 drafter invariants).
- Usage cache `usage_api.json` absent → `wave.py usage W --before --pct P --resets 2026-10-02T19:00Z` (basis manual); P from `PY ~/.claude/pa3/pa_ledger.py sql "select ts,pct,resets_at from utilization where window='five_hour' order by ts desc" --limit 1`.

## In flight
- T7.c1 (coder-opus55, background, log phase-ends/current/logs/T7.c1.md): `wave.py draw --kind family` (pool one slus rep per exe-headed exact class, honours refusals except open-twin-sibling/has-banked-twin, route `draft` only, attempts budget from journal, order copies×insns desc) + `cost:` line + docs. Its hand-back will NOT reach a respawn: read `git log --oneline -5` for a `T7.c1` commit and logs/T7.c1.md.

## Hypotheses rejected
- `--kind family` via existing flags (`--binary`, `--family`): rejected, draw ignores dup.tsv (wave.py:228-230).

## Next five steps
1. Confirm T7.c1 commit; run `dc.sh run python3 tools/wave.py draw --kind family --weight 2500 --dry-run`; check `cost:` ≤ 1000k.
2. Real draw + `wave.py cards W`; commit; `wave.py usage W --before --pct …` immediately before spawning drafters (one coder-opus55 per card; draft.c self-contained one fn, verdict.json `{"alias","start","status":"match|plateau|no-verdict","rung":"-","label"}` first, ≤ 8 plateau.py runs, only draft.c + verdict.json in the dir; never `dc.sh sync` while drafters run).
3. `--after` right after the last drafter; record drafter tokens per card; gate → recover → fleet → harvest → close (host order).
4. Brief T7.c2: `propagate.py --register slus_012_79:<start>` (batched carve of member units per alias, exemplar → src/shared/*.inc.c with SHARED_ defines, families.tsv rows) then `--apply --family` per banked exemplar; fleet; commit.
5. Next family waves (twin-skipped siblings) until none undrafted at budget; tasks/T7.md with measured windows and re-projection (> 80.8 windows → `question`).

## Gotchas
- zsh: `echo =====` fails (`=cmd` expansion); use other separators.
- Reading `.run/status.json` via inline python is hook-denied; use `status.py show`.

## State to carry verbatim
- 5h window: resets 2026-10-02T19:00Z (epoch 1790967600); 14:00:09Z five_hour pct 0.0.
