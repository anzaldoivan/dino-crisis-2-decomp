# tools/probes — probe harness (T3, Phase 1.4)

`tools/probe.py` compiles our C probes per triple and compares the function bytes with the target. It proves body shape only (G10). It runs in the image: `bash tools/docker/dc.sh sync && bash tools/docker/dc.sh run python3 tools/probe.py …`.

## Usage
- `probe.py [names…] [--cc C,…] [--aspsx V,…] [-G N,…] [-O N,…] [-j N]` prints a match table per probe. Each option is repeatable or takes a comma list.
- `probe.py --pinned [names…]` runs the game probes (alias ≠ `self`) under the pinned triple. It adds the ladder class count over the (narrowed) matrix.
- `probe.py --selftest` runs the controls only on the `self` row. The matrix is `gcc-2.8.1-psx,psyq4.4 × 2.79 × G0 × O1,O2` (narrowable). The tool prints `controls: true ok, false ok`.
- Exit 0 means ok. Exit 1 means that a control misbehaved or a pinned probe differs. Exit 2 means a usage/config error (no pinned value, no rows, unknown cc).

## Matrix
- The cc values are the runnable `oldgcc|psyq` rows of `config/toolchains.tsv` minus `psyq3.6` (dropped, a DOS exe).
- The aspsx values are the maspsx README behaviour-table versions ≥ 2.30: `2.30 2.34 2.56 2.67 2.77 2.79 2.81 2.86`.
- The remaining axes are `-G 0,4,8` × `-O 1,2`. A triple name is `<cc>/a<aspsx>/G<g>/O<o>`.
- The pipeline (docs/ops/docker-host.md "Candidate toolchains") is `mipsel-linux-gnu-cpp -P -undef -nostdinc -D__GNUC__=2` → cc1 `-quiet -O<o> -G<g> -mips1 -fno-builtin` (`/opt/cc/<cc>/cc1`, psyq via `/opt/cc/wibo/wibo /opt/cc/<cc>/CC1PSX.EXE`) → `maspsx.py --aspsx-version=<a> -G<g>` → `mipsel-linux-gnu-as -march=r3000 -mabi=32 -G0`.

## Rows: `config/probes.tsv`

The columns are `name alias start end c_path notes` (tab-separated, end exclusive, hex addresses). `name` is the C function symbol.

The target is `[start,end)` of the alias's binary under `$BASEDIR`. The default is the first of `extracted/retail/files`, `.run/extracted/retail/files` that holds the exe `SLUS_012.79`.

If neither holds it and `$BASEDIR` is unset, a non-selftest run with game rows self-extracts via `make extract OUT=.run/extracted/retail`. `$DC2_CUE` defaults to the `/disc` cue as in `tools/fleet_check.sh`, and the log is `.run/probe/extract.log`. If the extraction fails, the tool prints one stderr line and exits with 2.

The file offset comes from `splat_gen.off_of` + `config/loadmap.tsv`. The tool reads the target at run time only.

For the alias `self` (start/end `-`), the target is the probe's own object under the control triple. Tracked files hold only our C, addresses, counts (G12).

## Pinned

The optional column 7 `pinned` of `config/toolchains.tsv` holds `a<aspsx>/G<g>/O<o>` (e.g. `a2.79/G0/O2`) on exactly one cc row. Elsewhere, the column holds `-` or is absent. If none is present, the tool prints a message and exits with 2.

## Masking and compare

The function bytes are `.text[value, value+size)` from the `readelf -s` FUNC symbol. The relocations from `objdump -r -j .text` in that range mask the word on both sides. `R_MIPS_26` masks the low 26 bits. `HI16`/`LO16`/`GPREL16` mask the low 16. Each other type masks the whole word.

The compare is word-by-word. A length mismatch is no match.

## Controls (every run)

The control triple is the first matrix triple (with `--selftest`: every matrix triple). The known-true control recompiles the `self` probe and compares it with its own object. They must match. The known-false control compiles the same probe with the other `-O` and compares it with that object. They must differ. If either fails, the tool prints `control failed: …` and exits with 1.

## Outputs

For each probe, the tool prints one line per triple (`match` | `diff@<word>` | `error`), then `matching: n of N: <triples>`. With `--pinned`, it prints `probes identical: K of K under the pinned triple`.

Every run with game probes (alias ≠ `self`, K is their count) prints `ladder: exactly 1 triple matches all K` (m = 1) or `ladder: exactly <m> triples match all K`. The value m counts the output equivalence classes among the triples that match all K. The key of a class is, per probe, the raw unmasked function words + the reloc list `(offset, type, symbol)`. Then the run prints one line per class: `class <i>: <n> triple(s): <triples>`.

The last line is `controls: true ok|FAIL, false ok|FAIL`. The scratch files (`.i/.s/.o`) are under `.run/probe/<name>/`.
