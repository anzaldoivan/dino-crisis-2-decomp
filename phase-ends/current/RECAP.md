MILESTONE: red

## Recap
Phase 1.6 built the "multipliers": tools that let one matched function turn into several, and that pick which functions to attempt next. A "matched" or "banked" function is one whose C source compiles to exactly the original game bytes. The phase added six tools: twins.py finds near-identical function pairs; types_check.py guards one shared header of data shapes; carve.py cuts new source files safely; propagate.py writes one matched body into every identical copy; reconcile.py fits a separately matched function into its real source file without rewriting it; draw_filter.py lists which functions are worth attempting and why the rest are refused. All seven tasks finished, the whole game still rebuilds byte-identical (83 of 83 binaries), all 16 self-checking scanners pass, and banked functions went from 5 to 7 of 3426. The milestone is judged red, though: the milestone checker ran its 10 checks and 4 of them failed. Three failures are wording mismatches between the plan and what the tools print. One is real: the propagation dry run checks 1 family member, and the milestone asks for at least 2.

## Gate runs
Command: `PY tools/phaseend_index.py verify --verbose` via `tools/run.sh phaseend-verify` (log `.run/logs/phaseend-verify.log`, per-clause `.run/logs/verify1..10.log`) → `VERIFY: RED (6/10)`.
- GREEN 1 fleet_check: `83 of 83 byte-identical`, `harness: 6 of 6 pairs agree, 0 disagreements`.
- GREEN 2 scanners: `scanners: 16 of 16 denominator+control ok`.
- RED 3 propagate dry run: prints `propagation dry run: 1 of 1 members gated` and `fail-closed control: ok`; the bound M ≥ 2 fails (M = 1). The tool counts only non-exemplar members; family e322 has exemplar st6 0x800d5c40 + member st8 0x800d6ab4. Substantive.
- RED 4 twins: prints `exact pairs reproduced: 26745 of 26745`; control line is `random-pair control: 71 of 10000 (<= 2%): ok`, not the literal `random-pair control: ok`. Form only.
- RED 5 reconcile: `reconcile: 2 of 2 banked without redraft`; gate line is `directory gate: banked+failed+no-verdict = drafts: ok`, not the literal `directory gate: ok`. Form only.
- GREEN 6 carve: `carves: C of C build hash-equal` (C ≥ 5), `control: ok`.
- RED 7 draw_filter: prints `draw: 769 of 3426 accepted`, `control: ok`; the clause's letter `A` is not a placeholder in the verifier (a/A/I are excluded), so it is matched literally. Plan wording only.
- GREEN 8 types_check, GREEN 9 `banked: 7 of 3426` (B ≥ 7), GREEN 10 audit_public.
What would make it pass: (3) propagate.py's dry run also gates the exemplar's instantiation of the shared body (st6 includes src/shared/func_800D5C40.inc.c, so it is a real second gate) and counts it, giving `2 of 2`; (4, 5) twins.py and reconcile.py each print one extra plain line `random-pair control: ok` / `directory gate: ok` (additive; existing lines and scanner regexes stay); (7) a critic-approved plan edit renaming the letter (`draw: N of D accepted`), or draw_filter output unchanged and the clause reworded. All four are one coder brief plus one critic decision for clause 7; then rerun the phase end.

## Checks
- NOTE from T7 confirmed: the `progress` scanner row is `progress.py && ! progress.py --fixture functions`; its control regex requires `fixture functions: dropped` then `denominator from build: FAIL`. In `.run/harness/scanner_progress.txt` (container) the real run prints `denominator from build: ok` (line 86), the planted fixture prints `fixture functions: dropped census row bin_e00 0x800d0198` (87) and `denominator from build: FAIL bin_e00: …` (175). The FAIL is the expected planted-control output. Same pattern for `compile_only` (`FAILED src/slus_012_79/zz_harness_plant.c` is the planted unit).
- H7: every summary naming tools/Makefile/scanners also lists docs/ops/decomp-environment.md (T1–T6) or HOW_WE_WORK.md (T7). No miss.
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
- Next task needs: a fix task for clauses 3, 4, 5 (one coder brief: exemplar gated in the dry run; plain `random-pair control: ok` and `directory gate: ok` lines) and a critic decision on clause 7's letter `A`; then rerun the phase end.
- Next task needs: task experts check their own milestone clause with `phaseend_index.py verify` semantics (literal backticked strings, a/A/I not placeholders, bounds applied) before closing; T1/T4/T5/T6 recorded green against looser reading.

## Promotions (done in this run; a rerun skips them)
- Cookbook C0038–C0043 from the six `generalizable:` lines (T1:24, T2:23, T4:28, T5:28, T5:29, T6:22). No `workflow:` lines this phase, so no skills.
- Rule G71.
