# Memory map

Where each executable sits in PS1 RAM, how that was proven, and what the proof excludes. Addresses and offsets only;
no game bytes (G12). Proof tool: `tools/emu/prove_load.sh <path>` (exe + [proven overlays](#proven-overlays)).

<a id="slus_01279"></a>
## SLUS_012.79

- path `extracted/retail/files/SLUS_012.79` · class exe · base (t_addr) `0x80018000` · t_size `0x90000` ·
  end `0x800a8000` · pc0 `0x8001b3b8` · status **proven**
- .text end `0x80085f74` (binding, T3; `tools/ghidra` TextExtent.java). Data from `+0x6df74` mutates once `main` runs.
- gp `0x800a79b0` (T2 Phase 1.3, `tools/splat_gen.py` scan of 64 insns from pc0): file `+0x3c34` `lui $gp,0x800a`,
  `+0x3c38` `addiu $gp,$gp,0x79b0`; set as `gp_value` in `config/splat/slus_012_79.yaml`.
- Compared ranges: moment 1 (pc0 breakpoint, load-complete) full `[0x80018000,0x800a8000)` vs file `[0x800..)`,
  no masking; moments 2-3 (two later distinct vsyncs, pc0 +~300 / +~600) `[0x80018000,0x80085f74)` word-wise.
- Exclusions (moments 2-3 only; self-modifying libcard code, names from the T1 PsyQ 4.7 signature hits on the Ghidra
  import of SLUS_012.79):
  - `_patch_card_info` `[0x8007e3c4,0x8007e408)`; observed differing words: `+0x663dc +0x663e0 +0x663e4 +0x663e8
    +0x663f0` from t_addr (`0x8007e3dc 0x8007e3e0 0x8007e3e4 0x8007e3e8 0x8007e3f0`).
  - `_patch_card2` `[0x8007e534,0x8007e5a4)`; observed differing words: `+0x66554 +0x6655c +0x66560`
    (`0x8007e554 0x8007e55c 0x8007e560`).
  - No differing word outside these two ranges in any run.
- Datapoints (2026-09-30, `bash tools/run.sh t3-prove[2] -- bash tools/emu/prove_load.sh SLUS_012.79`, exit 0 both):
  - moment 1: vsync 462 · EQUAL (both runs)
  - moment 2: vsync 773 / 772 · EQUAL with the 8 excluded words above
  - moment 3: vsync 1064 / 1081 · EQUAL with the 8 excluded words above

## Proven overlays

Proof: `tools/emu/prove_load.sh <path>` (fixed per-overlay table; compares file `[0,size)` to RAM `[base,base+size)`).
Runs: OpenBIOS, `-interpreter`, fresh boot each; datapoints from 2026-10-01 runs `t5c2-loop1` / `t5c2-loop2`
(`bash tools/run.sh --bg t5c2-loopN -- <loop over loadmap_evidence.py --proven-paths>`, exit 0 both). No exclusions.

<a id="ovl-st1"></a>
### PSX/BIN/ST1.BIN
- route R1 · base `0x800d5800` · size `0x3ec8` · end `0x800d96c8` · status **proven**
- State: no pad input, attract demo; ST1 resident vsync 5240-10717 in T5.c1 run A (R1 `ecbc(4,0x800d5800)` at 5201).
- Compared: 3 moments at fixed target vsyncs 6000 / 8000 / 10000, each FULL `[0x800d5800,0x800d96c8)`, no masking.
- Datapoints (vsync loop1 / loop2): 6003 / 6003 · 8019 / 8015 · 10020 / 10003 · all EQUAL.

<a id="ovl-wep01"></a>
### BIN/WEP01.BIN
- route R3 · base `0x8017e500` · size `0x2260` · end `0x80180760` · status **proven**
- State: no pad input, attract demo, ST1 span. T5.c1's 5399-19950 is first/last seen, not contiguous: WEP07 holds
  `0x8017e500` ~10951-16639 (a moment at vsync 12010 gave 2171 differing words, T5.c2), so targets sit in 5399-10717.
- Compared: 3 moments at fixed target vsyncs 6000 / 8000 / 10000, each FULL `[0x8017e500,0x80180760)`, no masking.
- Datapoints (vsync loop1 / loop2): 6005 / 6019 · 8019 / 8015 · 10005 / 10018 · all EQUAL.

<a id="ovl-option"></a>
### BIN/OPTION.BIN
- route R2 (`FUN_8004262c(0)`, archive idx 0xf0) · base `0x801c1500` · size `0x3538` · end `0x801c4a38` · entry
  `0x801c15f4` · status **proven**
- text_end `0x801c4858` (`+0x3358`). Rule: `TextExtent.java` MAX_FUNC_END on a raw import
  (`tools/ghidra/import_raw.sh BIN/OPTION.BIN 0x801c1500 OPTION_BIN`); MAX_INSN_END agrees (158 functions).
- State: title menu (NEW GAME / LOAD GAME / OPTION) is up by vsync 1800 with no input; `tools/emu/lua/pad.lua`
  `DC2_PAD="1900:DOWN 1920:DOWN 1950:CROSS"` (6-vsync holds) → r2 k=0 at 1962, exec `0x801c15f4` at 1988-1989
  (T5.c1 reached it in-game at 19156).
- Compared: moment 1 = pausing Exec breakpoint at the entry (load moment), FULL `[0x801c1500,0x801c4a38)`;
  moments 2-3 = entry hit +300 / +600 vsyncs, `[0x801c1500,0x801c4858)` word-wise. No exclusions.
- Data past text_end mutates after entry: moment-3 full compare differs from `+0x3380` (165 bytes, run t5c2-opt1),
  matching T5.c1's tail `+0x3380..+0x3538`.
- Datapoints (vsync loop1 / loop2): 1989 / 1989 EQUAL full · 2301 / 2303 EQUAL .text · 2594 / 2599 EQUAL .text.

## Loader routes

Static RE of SLUS_012.79 (T4). Route ids are the `route` column of `config/loadmap.evidence.tsv`.
- File table `0x80085fb0`: 676 records x 8 B `{u32 lba, u32 size}`, index = LBA order; every disc file except
  SYSTEM.CNF and SLUS_012.79; ends `0x800874d0`.
- Loader core: `FUN_8001ec10(u16 idx)` self-describing archive request (descriptor list `0x801e6d00`);
  `FUN_8001ecbc(idx, dest)` raw whole-file read; pump `FUN_8001fa70` (state `0x800af63d`); sync read `FUN_8001dbfc`;
  exec handoff `FUN_8001ced4(entry)`.

<a id="route-r0"></a>
### R0 `exe`
- BIOS loads SLUS_012.79 at header t_addr `0x80018000`. Proven: [SLUS_012.79](#slus_01279).

<a id="route-r1"></a>
### R1 `bin-raw`
- `FUN_8001ecbc` on a whole .BIN.
- LOGO.BIN: main `0x8001b8a0` -> `FUN_8001d0c4`: `ecbc(2, 0x800d5800)`, entry `0x800d5920` via `ced4`.
- ST0..ST9.BIN: stage table `0x80089b0c`, stride 0x18, `u16 file idx` at +0, 10 rows (idx 3..12); callers `0x800293a8`,
  `FUN_80041778`; dest = word at `0x800181d4` = `0x800d5800`.
- PSX/DATA/MAP.BIN: `FUN_80042758` -> `ecbc(137, 0x801c1500)`.

<a id="route-r2"></a>
### R2 `dat-module`
- `FUN_8004262c(k)`: table `0x8001945c`, 26 rows x 0xc `{u16 file idx, u32 base, u32 entry}`; `ec10(file)` then
  `ced4(entry)` when entry != 0. Dests: `0x801c1500`, `0x800d5800`, `0x80100000`.
- DAT header: 0x20-B records `{u32 type, u32 size, u32 a, u32 b, ...}`, terminated by u32 `0x6d6d7564`; payloads from
  0x800, 0x800-aligned. RAM types 0 (raw, dest `a`) and 7 (dest `a`; odd size -> believed compressed, unverified).
- BIN/OPTION, SAVE, LOAD, SUBSCR3, SUBSCR6 are byte-identical to the type-0 segment at `0x801c1500` of the same-named DAT.

<a id="route-r3"></a>
### R3 `dat-seg`
- Other DATs requested with `ec10` (index tables `0x80089b14` (+2), `0x80089ce4`, `0x80089d50`, `0x8008a02c`; E*, WEP_P*,
  KOF_*, ST* DATs) carry type-7 RAM segments.
- Loose /BIN copies (E*, WEP*, WEP_S*, KOF_*, RES*, M_*, TITLE2, OPENING, ENDING) are loaded by no route by file index
  (no `ec10`/`ecbc` with idx 587..675). Shipped form believed to be a DAT type-7 segment.
- RAM evidence (T5.c1 residency scans, OpenBIOS, attract demo run A / pad run B): E00 @`0x800d0000`, E20, E90,
  WEP01 @`0x8017e500`, WEP07 each fully equal to the loose copy at the ranked base at some snapshot; WEP01 proven
  ([below](#ovl-wep01)). KOF_*, RES*, M_TITLE, TITLE2 never seen resident; OPENING best 0.9975 @`0x800d5800`, never
  fully equal. Loose copy = RAM image holds for the E*/WEP* rows seen; the DAT decompression path stays unverified.
- R2 `k=0x10` (`FUN_8004262c`) runs at vsync 2026, before the OPENING exec (`0x800d5c64`, vsync 2044) (T5.c1 run B).
- 46 distinct type-0/7 dests in RAM over all PSX/DATA/*.DAT headers (tool candidate set).
- Unreferenced: `0x8001cf84`, `0x8001d028` (module helpers indexing `0x800181f0`, no callers).

## Fleet classification

Evidence: `tools/loadmap_evidence.py --controls --json .run/t4/ranks.json` (control: exe body ranks t_addr
`0x80018000` first, score 220599: self-jals on start 4116/4369, ptrs on start 1164, lui pairs 1919). Signals below:
`j` self-jals onto own starts, `p` abs pointers onto own starts, `l` lui/lo16 pairs into range; ranges over the family.
Status `ranked` = static evidence only; the byte gate is the arbiter. No sha1 twins among the 105 .BIN.
- N = 83 = exe 1 + code 82 (R1 12, R2 5, R3 65 incl. WEP_S00 ranked none); debug code 0 (SYS_DEB not code-shaped);
  stub 22; debug 1. Total rows 106.

<a id="fam-logo-st"></a>
### LOGO, ST0..ST9 (R1)
- 11/11 rank `0x800d5800` = R1 dest. Score 815..14585; j 2..65, p 10..156, l 4..124. No same-stem DAT.

<a id="fam-map"></a>
### PSX/DATA/MAP.BIN (R1)
- Ranks `0x801c1500` = R1 dest. Score 7761; j 67, p 20, l 41.

<a id="fam-r2-modules"></a>
### OPTION, SAVE, LOAD, SUBSCR3, SUBSCR6 (R2)
- 5/5 rank `0x801c1500`; agrees with the same-named DAT type-0 dest. Score 1158..15813; j 8..105, p 5..102, l 7..99.
- SUBSCR3: whole-file op density 0.68 (trailing data), 1.00 over its code span; tool density is span-based.

<a id="fam-e"></a>
### E00..EA0 (R3, 13 files)
- 13/13 rank `0x800d0000`; every same-stem DAT has a type-7 segment at `0x800d0000` (agrees 13/13).
  Score 1178..9933; j 2..60, p 19..98, l 9..80. Shipped form: DAT type-7 segment; RAM: E00, E20, E90 fully equal
  to the loose copy at `0x800d0000` (T5.c1 run A; [route-r3](#route-r3)); others not seen.

<a id="fam-kof"></a>
### KOF_Pxyz (R3, 14 files)
- `*0P` (7) rank `0x80128000`; `*1P` (7) rank `0x800d0000`. Each agrees with a type-7 dest of its same-stem DAT
  (14/14). Score 1414..5482; j 0..6, p 27..98, l 37..97. Pairs share size and signal counts, distinct sha1 (relinked).
  Shipped form: DAT type-7 segment, unverified.

<a id="fam-wep"></a>
### WEP00..WEP13 (R3, 20 files)
- WEP00..WEP0C (13) rank `0x8017e500`; WEP0D..WEP13 (7) rank `0x80120000`. No same-stem DAT; both bases are type-7
  dests of WEP_P*.DAT archives (`0x8017e500` x15; `0x80120000` incl. WEP_PA0F/PC12/PI13). Score 564..5481; j 0..6,
  p 11..98, l 3..97. Shipped form: DAT type-7 segment; RAM: WEP01, WEP07 fully equal to the loose copy at
  `0x8017e500` (T5.c1 run A); WEP01 proven ([ovl-wep01](#ovl-wep01)); others not seen.

<a id="fam-wep-s"></a>
### WEP_S00..WEP_S09 (R3, 10 files)
- WEP_S02..06, S08, S09 (7) rank `0x80180d00` (score 104: p 2, l 2); S01, S07 rank `0x8017c500` on lui pairs only
  (score 6..7, p 0): weak. Both bases are WEP_SUB*.DAT type-7 dests. No same-stem DAT.
- WEP_S01: T5.c1 found its head at `0x80180d00` (A 5479-19950, B 13921-19962) and the whole 1432 B equal there in the
  final run-B snapshot (one datapoint): TSV base `0x80180d00`, status ranked (basis tasks/T5.md).
- WEP_S00 (28 B; u32 + three empty functions): ranked none, no self-reference at any of 46 candidates; base `-`.

<a id="fam-res"></a>
### RES00..RES02 (R3)
- 3/3 rank `0x80150000`; agrees with a type-7 dest of each same-stem DAT (3/3). Score 814..2222; j 7..21, p 2, l 12..20.

<a id="fam-title"></a>
### M_RESULT, M_TITLE, TITLE2, OPENING, ENDING (R3)
- 5/5 rank `0x800d5800`; agrees with a type-7 dest of each same-stem DAT (5/5). Score -793..3726; j 0..33, p 4..18,
  l 3..25.
- OPENING (1592 B): only self-referencing candidate, score -793 (1 jal into range misses every start; p 4, l 3): weak.
  RAM (T5.c1 run B): exec `0x800d5c64` at vsync 2044, just after R2 `k=0x10` (2026); best 0.9975, never fully equal.

<a id="fam-sys-deb"></a>
### BIN/SYS_DEB.BIN (debug)
- Judged separately: no loader route found (idx 635). 0 `jr ra`, 0 prologues, op density 0.53 -> class hint data;
  no self-reference at any of 39 candidates -> ranked none, base `-`.

<a id="fam-stubs"></a>
### Stubs (22, <= 8 B)
- 21 x 4 B placeholder u32 (one small value, distinct per file): AT_EDIT, DM_BLK, DM_EMHIT, DM_EMPOS, DM_SCA, DM_SUB,
  MOT, SCR_LIT, TESTCONP, TESTDOOR, TESTESP2, TESTSND2, TEST_CAM, TEST_EM, TEST_ESP, TEST_OBJ, TEST_PRI (BIN/);
  AREA_JMP, DM_MES, TEST_SND, VRAMVIEW (PSX/BIN/).
- BIN/DBMODULE.BIN 8 B = `jr ra; nop`.
- Verdict: DBMODULE/DM_*/AT_EDIT/TEST* are placeholders, debug modules not shipped (same for MOT, SCR_LIT, AREA_JMP,
  VRAMVIEW). SYS_DEB judged separately ([above](#fam-sys-deb)).

<a id="load-map"></a>
## Load map

- `config/loadmap.tsv`: generated (never hand-edited) by `tools/loadmap.py` from `extracted/retail/manifest.jsonl` +
  `config/loadmap.evidence.tsv`; one row per manifest .BIN + SLUS_012.79, `path size sha1 class route base end status`.
  end = header t_addr + t_size (exe), base + size (others), `-` when base is `-`.
- Regenerate: `PY tools/loadmap.py`. Check: `PY tools/loadmap.py --check` (stale file, row set, classes, control rows =
  exe + `status=proven` vs their sections here, built-in negative control).
- N = rows with class exe or code.
- N = 83 · rows 106 (tool output `rows=106 N=83`).

<a id="boundaries"></a>
## Boundaries

- `config/boundaries.tsv`: generated (never hand-edited) by `tools/boundaries.py`; `binary kind start end basis`, end
  exclusive, sorted (binary, start, kind). Binaries: SLUS_012.79 + the 81 `class=code` loadmap rows with a base.
- Kinds: `jtbl` (own MIPS back-slice from `jr rY`, rY != ra; basis `jr=<addr> sltiu|run`), `lib-object` (exe only;
  PsyQ 4.7 objects matched by psx_ldr sigs, non-low-entropy; basis `psyq470 <LIB>/<OBJ>`), `text-end` (row
  `[base, text_end)`; exe = binding `0x80085f74`, asserted equal to the scan; overlays = after the last `jr ra` +
  delay slot, a `jr ra` adjacent to another is data), `data-island` (`[text_end, loadmap end)`).
- Ghidra caches (addresses + names only): `config/ghidra/SLUS_012.79.switch_tables.tsv` (DumpSwitchTables.java, 59:
  49 `src=auto` `switchdataD_` tables + 10 `src=txn`), `config/ghidra/SLUS_012.79.psyq_objects.tsv`
  (DumpPsyqObjects.java, 244 matches, 61 low).
- Commands: `PY tools/boundaries.py` (write) · `PY tools/boundaries.py --check` (two regenerations equal + equal to
  the tracked file; exe jtbl vs the Ghidra switch dump; control switch; exe text-end) · `PY tools/boundaries.py
  --ghidra` (refresh the caches; headless read-only; refuses while :8080 listens).
- Rows 481 (tool output `rows=481`): exe jtbl 59 · lib-object 183 · text-end 1 · data-island 1; code jtbl 77 ·
  text-end 81 · data-island 79.
- Overlay text-end cross-check: BIN/OPTION.BIN scan `0x801c4858` = the proven value ([above](#proven-overlays)).
- lib-object: 12 overlapping pairs at the same start (identical code in two libs, e.g. LIBCD/LIBDS); both rows kept.
- Ghidra comparison (exe, by jr): 59/59 equal ours (jr, start, end), 0 disagreements: 49 auto + 10 recovered-in-txn.
- recovered-in-txn (T7.c2): auto-analysis never disassembled jr `0x80026c94 0x80026e78 0x80026fe8 0x80027270
  0x80027b88 0x80027ce0 0x80027f08 0x80028038 0x800285a8 0x80056028`. DumpSwitchTables.java, in a rolled-back txn:
  candidates = `jr rs` (rs != ra) words with no Ghidra instruction in [first fn entry, last fn end] (11); entry =
  scan back to after `jr ra` + delay slot or an existing instruction; clear data, disassemble, create function,
  DecompInterface with `toggleJumpLoads(true)`; the JumpTable load table (addr, size 4, num) is the row. Nothing
  from boundaries.py seeds it. 11th candidate jr `0x8007e480` (4-insn function at `0x8007e478`): decompiler finds
  no jump table, ours none either (`SWITCH_UNRECOVERED` line in the log, no row).
- Control switch: jr `0x8003500c` table `[0x80018624,0x800186f8)` (53 entries), found by both.

<a id="build-units"></a>
## Build units

- One splat config per fleet row: `config/splat/<alias>.yaml` + `config/check.<alias>.sha`, generated by
  `tools/splat_gen.py` from loadmap + boundaries (never hand-edited); 83 binaries.
- Top-level segments 84: 82 `code` (one per binary with a base, vram = base; exe vram `0x80018000` at file 0x800, after its header)
  + 2 `bin` (exe PS-X EXE header; WEP_S00).
- Subsegments 653: asm 275 · rodata 281 (data heads, jtbls, `[base, text_start)`) · data 80 (after text-end) · bin 17
  (odd tails). Counts: scratch scan of config/splat/*.yaml (T5.c1).
- By family (binaries: asm / rodata / data / bin): exe 1: 194/92/1/0 · E 13: 13/59/13/0 · KOF 14: 14/19/14/10 ·
  WEP 20: 20/36/20/7 · WEP_S 10: 9/4/7/0 (+ WEP_S00 raw) · LOGO+ST 11: 11/36/11/0 · MAP 1: 1/1/1/0 · R2 5 (OPTION,
  SAVE, LOAD, SUBSCR3, SUBSCR6): 5/25/5/0 · RES 3: 3/2/3/0 · M_RESULT, M_TITLE, TITLE2, OPENING, ENDING 5: 5/7/5/0.
- Odd tails: 17 binaries with size % 4 != 0 (KOF_P10P..PA1P 10 at 2 B; WEP06, WEP0C 1 B; WEP0F..WEP13 5 at 2 B).
  splat_gen cuts at `size & ~3` and emits the last 1-3 B as subsegment `bin <alias>_trailing` (an edge inside the tail
  is an error); the Makefile objcopy output is shrink-trimmed to the target size when it exceeds it by <= 3 B
  (SUBALIGN(4) end pad). `--check` fails `tail` if the last piece is not that bin.
- WEP_S00 (`BIN/WEP_S00.BIN`, 28 B, class code, R3, base `-`, status n/a): no boundaries.tsv rows (no text-end).
  splat_gen emits one raw `bin` segment at vram 0 over `[0, 0x1c)`, never a guessed base; `--check` asserts that
  shape and that WEP_S00 is the only no-base row. It has its `check.bin_wep_s00.sha`, is built and hashed like any
  alias and counts in fleet_check's 83.
