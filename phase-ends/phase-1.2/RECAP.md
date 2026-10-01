MILESTONE: green

## Recap
Phase 1.2 set up the instruments a matching decompilation is checked against, before any code is rewritten. The game's
main program (`SLUS_012.79`) now lives in a Ghidra disassembly database with the Sony PsyQ library signatures applied
(SDK 4.7.0, evidence in `docs/ops/decomp-environment.md`), and our hand annotations survive a full rebuild of that
database byte-for-byte. The PCSX-Redux emulator is driven from the Mac by scripts, and it proved where the main program
and one overlay per loader route (an overlay is a code file the game loads into memory on demand) sit in live memory.
All 105 `.BIN` files are classified, which fixes the fleet at N = 83 code units, and the jump-table and library-object
boundaries the later splitter must respect are recorded in `config/boundaries.tsv`.

## Milestone verification
- `PY tools/phaseend_index.py verify` (run 2026-10-01, `.run/logs/pe12-verify2.log`): 5/6 GREEN, clause 2 RED as reported
  by the verifier: `expected in the output: N=<n>`. The verifier matches the milestone's placeholder `N=<n>` literally.
- Clause 2 by hand: `PY tools/loadmap.py --check` → exit 0, `rows=106 N=83 unclassified: 0 controls: 4/4 pass
  negative-control: OK`; every element the clause names is present with the placeholder filled (n=83, k=4).
- Deviation (fixed at phase end): before this close `loadmap.py --check` printed `controls: OK`, not the milestone's
  `controls: k/k pass`; `tools/loadmap.py:254-256` now prints the control count; ops row updated
  (`docs/ops/decomp-environment.md:118`).
- Other clauses GREEN: rebuild.sh --proof, boundaries.py --check, prove_load.sh SLUS_012.79, audit_public.py,
  firewall_control.sh.
- H7 check: T4 `Files:` names `tools/loadmap_evidence.py` without `HOW_WE_WORK.md` or `docs/ops/`; recorded as a
  deviation, repaired at phase end with a Tooling row (`docs/ops/decomp-environment.md:119`).

## Decisions that still bind
- Rule G68 (new): oracle comparisons are symmetric, unseeded, with proof ranges fixed before the run (T3.1, T5, T7).
- Contract: exe .text = [0x8001b3b8, 0x80085f74), t_addr 0x80018000, t_size 0x90000 → `docs/memory-map.md#slus_01279`.
- Contract: loader routes R0..R3, file table 0x80085fb0 (676 × {lba,size}) → `docs/memory-map.md` `## Loader routes`.
- Contract: N = rows with class exe|code = 83 of 106; control rows = exe + proven → `docs/memory-map.md#load-map`,
  checked by `tools/loadmap.py --check`.
- Contract: Ghidra annotation file is a delta; ANALYSIS/DEFAULT labels never tracked → `tools/ghidra/delta.py`,
  checked by `rebuild.sh --proof`.
- Contract: lib-object rows from DumpPsyqObjects (psyq470), control switch at jr 0x8003500c → `docs/memory-map.md#boundaries`.
- Environment: Redux runs `-interpreter` (arm64 dynarec SIGILL); MCP = GhidrAssistMCP 2.11.0 on `/sse` →
  `docs/ops/decomp-environment.md`, `docs/ops/disassembler-mcp.md`.
- Environment: overlay windows are time-shared; proof moments stay inside one residency window → `tools/emu/prove_load.sh`.
- Next task needs: `phaseend_index.py verify` must treat `<n>`-style placeholders and `k/k` in Milestone clauses as
  patterns, or milestones must quote literal output; clause 2 here was verified by hand.
- Next task needs: the loose `/BIN` copies (E*, WEP*, KOF*, RES*, M_*, TITLE2, OPENING, ENDING) ship as DAT type-7
  segments believed compressed, unverified; decide before splitting them.
- Cookbook C0010..C0020 and skills `ghidra-mcp-scratch-copy`, `pipe-exit-status` promoted from this phase's gotchas.
