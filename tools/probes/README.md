# tools/probes — probe harness (T3, Phase 1.4)

`tools/probe.py` compiles our C probes per triple and compares the function bytes with the target. Proves body shape only (G10). Runs in the image: `bash tools/docker/dc.sh sync && bash tools/docker/dc.sh run python3 tools/probe.py …`.

## Usage
- `probe.py [names…] [--cc C,…] [--aspsx V,…] [-G N,…] [-O N,…] [-j N]` — match table per probe; options repeatable or comma lists.
- `probe.py --pinned [names…]` — game probes (alias ≠ `self`) under the pinned triple + ladder class count over the (narrowed) matrix.
- `probe.py --selftest` — controls only on the `self` row, matrix `gcc-2.8.1-psx,psyq4.4 × 2.79 × G0 × O1,O2` (narrowable); prints `controls: true ok, false ok`.
- Exit: 0 ok · 1 a control misbehaved or a pinned probe differs · 2 usage/config error (no pinned value, no rows, unknown cc).

## Matrix
- cc: runnable `oldgcc|psyq` rows of `config/toolchains.tsv` minus `psyq3.6` (dropped, DOS exe).
- aspsx: maspsx README behaviour-table versions ≥ 2.30: `2.30 2.34 2.56 2.67 2.77 2.79 2.81 2.86`.
- `-G 0,4,8` × `-O 1,2`. Triple name `<cc>/a<aspsx>/G<g>/O<o>`.
- Pipeline (docs/ops/docker-host.md "Candidate toolchains"): `mipsel-linux-gnu-cpp -P -undef -nostdinc -D__GNUC__=2` → cc1 `-quiet -O<o> -G<g> -mips1 -fno-builtin` (`/opt/cc/<cc>/cc1`, psyq via `/opt/cc/wibo/wibo /opt/cc/<cc>/CC1PSX.EXE`) → `maspsx.py --aspsx-version=<a> -G<g>` → `mipsel-linux-gnu-as -march=r3000 -mabi=32 -G0`.

## Rows: `config/probes.tsv`
`name alias start end c_path notes` (tab-separated, end exclusive, hex addresses). `name` = the C function symbol. Target = `[start,end)` of the alias's binary under `$BASEDIR` (default first existing of `extracted/retail/files`, `.run/extracted/retail/files`), file offset via `splat_gen.off_of` + `config/loadmap.tsv`; read at run time only. Alias `self` (start/end `-`): target = the probe's own object under the control triple. Tracked files hold only our C, addresses, counts (G12).

## Pinned
Optional column 7 `pinned` of `config/toolchains.tsv` on exactly one cc row: `a<aspsx>/G<g>/O<o>` (e.g. `a2.79/G0/O2`); `-` or absent elsewhere. None present → message, exit 2.

## Masking and compare
Function bytes = `.text[value, value+size)` from `readelf -s` FUNC symbol. Relocations from `objdump -r -j .text` in that range mask the word on both sides: `R_MIPS_26` low 26 bits; `HI16`/`LO16`/`GPREL16` low 16; other types the whole word. Word-by-word; length mismatch = no match.

## Controls (every run)
Control triple = first matrix triple (`--selftest`: every matrix triple). Known-true: the `self` probe recompiled vs its own object (must match). Known-false: the same probe with the other `-O` vs that object (must differ). Either failing → `control failed: …` and exit 1.

## Outputs
Per probe: one line per triple (`match` | `diff@<word>` | `error`), then `matching: n of N: <triples>`. `--pinned`: `probes identical: K of K under the pinned triple`. Every run with game probes (alias ≠ `self`; K = their count): `ladder: exactly <m> triple(s) match all K`, m = output equivalence classes among the triples matching all K (key = per probe the raw unmasked function words + reloc list `(offset, type, symbol)`), then one line per class `class <i>: <n> triple(s): <triples>`. Last line `controls: true ok|FAIL, false ok|FAIL`. Scratch (`.i/.s/.o`) under `.run/probe/<name>/`.
