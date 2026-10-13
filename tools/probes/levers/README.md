# tools/probes/levers — lever drafts (T2, Phase 1.7)

This directory holds the draft pairs for the rows of `config/levers.tsv`. `tools/codegen_map.py --check` checks them under the pinned triple.
- A draft holds our C only: no game bytes, no asm, no pasted disassembly (G12). The row cites the game fn by `alias:0xADDR`.
- A draft is self-contained. `tools/probe.py` compiles it with `cpp -P -undef -nostdinc` and no `-Iinclude`. Declare the types you need locally.
- The draft defines the symbol `func_<START>` (8 upper-case hex digits of the row's `start`, e.g. `func_80052634`).
- The files are `<lever>.before.c` (shows the row's tell) and `<lever>.after.c`. The after draft differs from the before draft only by the lever and is byte-identical to the game fn.
- The classification is compile-error, identical-drafts, not-matching, before-matches (inert, G45), else proven.
