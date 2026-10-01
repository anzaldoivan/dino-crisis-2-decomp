MILESTONE: green

## Recap
Phase 1.5 built the measuring instruments the decompilation will be judged by, before any large-scale matching work starts. A "census" now lists every function in all 83 game binaries (3911 functions covering every byte of program code, none invented and none cut short), and a second, independent tool (the Ghidra disassembler) agrees with that list on every binary. A duplication census groups functions that are byte-for-byte copies of each other across binaries, separates Sony's library code (485 functions) from the game's own code (3426 functions), and reports the 2094 functions that appear only once. Progress is now counted against a denominator taken from the build itself (currently 5 of 3426 game functions written in C), and a differential harness of six independent cross-checks, each with a planted failure it must catch, runs on every full rebuild and weekly in CI. The milestone was verified from a clean rebuild: 83 of 83 binaries byte-identical, 6 of 6 harness pairs agreeing, 10 of 10 scanners printing their denominator and passing their control, and the public-repository audit clean.

## Decisions that still bind
- Contract: census definitions (text region, function extent, phantom criteria, data-in-text spans, start rules f/g, R3 invalid words, kind lib|game, lib-object grouping) → docs/ops/decomp-environment.md `### Function census`; tested by `census.py --check` controls and fixtures.
- Contract: oracle judgement after n1/n2/n3 normalisation, Ghidra seeds from program bytes + Ghidra only, exception kinds re-checked every run, new rows only via `--propose-exceptions` → docs/ops `### Ghidra oracle`; tested by `oracle_diff.py` planted-start and stale-hash controls.
- Contract: dedup exact/near keys, masks only on asm-annotated lines, never on address values; relocation and near-FP controls → docs/ops `### Duplication census`; tested by `dup_census.py --check` (now also prints the combined `control: ok`).
- Contract: progress denominator = census game rows of each binary, alias set checked against Makefile and build/<alias>.ld → docs/ops `### Progress denominators`; tested by `progress.py` and its `--fixture alias|functions`.
- Contract: harness pairs and fleet_check green = `83 of 83 byte-identical` + `harness: 6 of 6 pairs agree, 0 disagreements` → card Build/run/test line and docs/ops `### Differential harness`; tested by `harness.py --selftest`.
- Norm: a scanner's control must be able to fail (every-run control line or chained planted negative, never an unconditional line) → rule G70.
- Environment: the card is at 6982 of 7000 chars; the next Tools row needs a trim elsewhere (repeated in T1–T6 summaries).
- Next task needs: the 14 census data-in-text spans may be promoted to `config/boundaries.tsv` data-island rows (needs a splat_gen recut and the fleet gate), per T1.

## Deviations
- Phase-end verify was RED on clause 4 at first: `dup_census.py --check` printed `relocation control: … ok` and `near control: … ok` but never the literal `control: ok` the milestone names. Fixed by printing the combined `control: ok|FAIL` line (tools/dup_census.py, one line, ops note updated; commit 863b284), then the whole verify re-run GREEN 7/7 from a clean rebuild. The closer edited two files (code line + one doc line, H7) instead of one.
- H7 check: every summary naming tools, settings or build commands (T1–T6) lists HOW_WE_WORK.md or docs/ops; no miss.

## Verification
- `PY tools/phaseend_index.py verify --verbose` (run.sh pe_verify) → RED 6/7 (clause 4).
- After the fix (run.sh pe_verify2) → GREEN 7/7; logs .run/logs/verify1.log … verify7.log. fleet_check elapsed 139 s (harness 69 s).
- Cookbook C0031–C0037 added from the seven `generalizable:` gotchas; rule G70 added; no `workflow:` gotchas this phase.
