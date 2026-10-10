# tools/probes/levers — lever drafts (T2, Phase 1.7)

Draft pairs for the rows of `config/levers.tsv`, checked by `tools/codegen_map.py --check` under the pinned triple.
- Our C only: no game bytes, no asm, no pasted disassembly (G12). The game fn is cited by `alias:0xADDR` in the row.
- Self-contained: compiled by `tools/probe.py` with `cpp -P -undef -nostdinc` and no `-Iinclude`; declare the types you need locally.
- Symbol: the draft defines `func_<START>` (8 upper-case hex digits of the row's `start`, e.g. `func_80052634`).
- Files: `<lever>.before.c` (shows the row's tell) and `<lever>.after.c` (differs from before only by the lever; byte-identical to the game fn).
- Classification: compile-error, identical-drafts, not-matching, before-matches (inert, G45), else proven.
