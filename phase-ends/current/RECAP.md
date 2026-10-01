MILESTONE: green

## Recap
Phase 1.4 found the exact compiler setup the original game was built with, so that C code we write can turn into the very same machine code. A "triple" here means one compiler build, one assembler-shim version and one set of flags; the phase built a probe harness that compiles five small functions we rewrote in C under every candidate triple and keeps only the triples whose output is byte-identical to the game. Exactly one group of equivalent triples survived, and the project pinned PsyQ 4.6 with assembler compatibility 2.86 and flags -G0 -O2. Those five functions now live as C in their real source files, the whole game (83 binaries) still rebuilds byte-identical, and a new CI job compiles all our C without any game data. The first closing attempt failed because the milestone checker read placeholders like "K of K" literally and one check needed data another check produced; tasks T9 and T10 fixed both, and the rerun passed 7 of 7 checks.

Verified: `PY tools/phaseend_index.py verify --verbose` (run.sh --bg pe3-verify) → VERIFY: GREEN (7/7); clause 1 `probes identical: 5 of 5 under the pinned triple` + `ladder: exactly 1 triple matches all 5` (.run/logs/verify1.log:2652-2653); clause 2 `83 of 83 byte-identical`; clause 3 `banked: 5`, `verbatim: 0`; clause 4 `compiled: 4 of 4 units`; clause 5 count 4, job `compile-only:` at no-rom.yml:42 uncommented; clauses 6-7 exit 0.

## Decisions that still bind
- Rule G69 (norm, T5/T8): the compiler triple and its flags are defined once, in the Makefile; tools read them via `make -s print-c`; per-module variation only through `CFLAGS_<alias>` hooks.
- Pinned triple psyq4.6 + aspsx 2.86, -G0 -O2 (T4, developer review): environment fact, in `docs/ops/decomp-environment.md:10` and the card Pins line.
- "Exactly one triple" = one output equivalence class (T4): contract, in `tools/probes/README.md:30` and enforced by `tools/probe.py`.
- Pinned triple record = `config/toolchains.tsv` column 7 on one cc row (T3): environment fact, `tools/probes/README.md:21`, `docs/ops/decomp-environment.md:10`.
- Candidate compilers at `/opt/cc/<name>/`, pin record `config/toolchains.tsv` (T1): environment fact, card Pins line.
- C cuts live in `config/c_units.tsv`; YAML `c` lines only from `splat_gen.py --force --only` (T6): environment fact, `docs/ops/decomp-environment.md:64`.
- verify placeholder binding and bounds (T10): tool contract, in `phaseend_index.py verify --help` and the card Tools row.
- Next task needs: overlays E, KOF, WEP, WEP_S, RES, MISC are pinned by assumption only (no probe); probe one function per module before decompiling there.

## Deviations
- H7 miss: T7 added `tools/banked.py` without a `docs/ops/` or `HOW_WE_WORK.md` entry; the closer added its Tooling inventory row in `docs/ops/decomp-environment.md`.
- Harness gotchas (8 `harness:` lines in summaries, 2 in logs/phase-end.md; chiefly the tool-source-read hook denying Bash `cat`/`sed`/`ls` on tool inputs, the 285 s foreground limit, `fetch_toolchain.sh --help` failing on BSD mkdir) are not promoted by the closer; they stand in the summaries for the auditor.
