MILESTONE: green

## Recap
Phase 1.6 built the "multipliers": tools that let one matched function turn into several, and that pick which functions to attempt next. A "matched" or "banked" function is one whose C source compiles to exactly the original game bytes. The phase added six tools: twins.py finds near-identical function pairs; types_check.py guards one shared header of data shapes; carve.py cuts new source files safely; propagate.py writes one matched body into every identical copy; reconcile.py fits a separately matched function into its real source file without rewriting it; draw_filter.py lists which functions are worth attempting and why the rest are refused. The first phase-end check failed 4 of its 10 milestone checks, because several tasks had judged their own check more loosely than the milestone checker does; two follow-up tasks fixed that (T8 made the propagation dry run also test the original copy and added plain pass lines to two tools, T9 taught the checker that a capital letter in an "X of Y" count is a number placeholder). On the rerun all 10 checks pass: the whole game still rebuilds byte-identical (83 of 83 binaries), all 16 self-checking scanners pass, and banked functions went from 5 to 7 of 3426.

## Gate runs
Rerun (after T8, T9): `bash tools/docker/dc.sh sync && PY tools/phaseend_index.py verify --verbose` on the host via `tools/run.sh --bg phaseend-verify2` (log `.run/logs/phaseend-verify2.log`, per-clause `.run/logs/verify1..10.log`) → exit 0, `VERIFY: GREEN (10/10)`.
- GREEN 1 fleet_check: `83 of 83 byte-identical`, `harness: 6 of 6 pairs agree, 0 disagreements`.
- GREEN 2 scanners: `scanners: 16 of 16 denominator+control ok`.
- GREEN 3 propagate dry run: `propagation dry run: 2 of 2 members gated`, `fail-closed control: ok` (T8: exemplar st6 gated too).
- GREEN 4 twins: `exact pairs reproduced: 26745 of 26745`, `random-pair control: ok` (T8 plain line).
- GREEN 5 reconcile: `reconcile: 2 of 2 banked without redraft`, `directory gate: ok` (T8 plain line).
- GREEN 6 carve: `carves: 5 of 5 build hash-equal`, `control: ok`.
- GREEN 7 draw_filter: `draw: 769 of 3426 accepted`, `control: ok` (T9: count-slot capitals are placeholders).
- GREEN 8 types_check `types: 0 duplicates, 0 raw address casts`; GREEN 9 `banked: 7 of 3426` (B ≥ 7); GREEN 10 audit_public.
First run (before T8/T9): `VERIFY: RED (6/10)`, RED 3 (M = 1 < 2, substantive), RED 4/5 (detailed control lines, not the literal ones), RED 7 (letter `A` read literally).
Process lesson: T1, T4, T5, T6 recorded green against a looser reading of their clause than the verifier uses (literal backticked strings, `a`/`A`/`I` literal, bounds applied). Each task must run its own clause with the verifier's matching rules; promoted as rule G72.

## Checks
- NOTE from T7 confirmed: the `progress` scanner row is `progress.py && ! progress.py --fixture functions`; its control regex requires `fixture functions: dropped` then `denominator from build: FAIL`. In `.run/harness/scanner_progress.txt` (container) the real run prints `denominator from build: ok` (line 86), the planted fixture prints `fixture functions: dropped census row bin_e00 0x800d0198` (87) and `denominator from build: FAIL bin_e00: …` (175). The FAIL is the expected planted-control output. Same pattern for `compile_only` (`FAILED src/slus_012_79/zz_harness_plant.c` is the planted unit).
- H7: every summary naming tools/Makefile/scanners also lists docs/ops/decomp-environment.md (T1–T6, T8, T9) or HOW_WE_WORK.md (T7). No miss.
- Card headroom 5 of 7000 chars (T7): the next Tools row needs a trim in the same task.

## Decisions that still bind
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

## Promotions
- First run: cookbook C0038–C0043 from the six `generalizable:` lines (T1:24, T2:23, T4:28, T5:28, T5:29, T6:22); rule G71.
- Rerun: cookbook C0044 (T9:19); rule G72 (process lesson). No `workflow:` lines this phase, so no skills.
