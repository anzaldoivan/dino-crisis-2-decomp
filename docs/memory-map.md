# Memory map

Where each executable sits in PS1 RAM, how that was proven, and what the proof excludes. Addresses and offsets only;
no game bytes (G12). Proof tool: `tools/emu/prove_load.sh <exe>`.

<a id="slus_01279"></a>
## SLUS_012.79

- path `extracted/retail/files/SLUS_012.79` · class exe · base (t_addr) `0x80018000` · t_size `0x90000` ·
  end `0x800a8000` · pc0 `0x8001b3b8` · status **proven**
- .text end `0x80085f74` (binding, T3; `tools/ghidra` TextExtent.java). Data from `+0x6df74` mutates once `main` runs.
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
  (no `ec10`/`ecbc` with idx 587..675). Shipped form believed to be a DAT type-7 segment: **unverified**.
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
  Score 1178..9933; j 2..60, p 19..98, l 9..80. Shipped form: DAT type-7 segment, unverified.

<a id="fam-kof"></a>
### KOF_Pxyz (R3, 14 files)
- `*0P` (7) rank `0x80128000`; `*1P` (7) rank `0x800d0000`. Each agrees with a type-7 dest of its same-stem DAT
  (14/14). Score 1414..5482; j 0..6, p 27..98, l 37..97. Pairs share size and signal counts, distinct sha1 (relinked).
  Shipped form: DAT type-7 segment, unverified.

<a id="fam-wep"></a>
### WEP00..WEP13 (R3, 20 files)
- WEP00..WEP0C (13) rank `0x8017e500`; WEP0D..WEP13 (7) rank `0x80120000`. No same-stem DAT; both bases are type-7
  dests of WEP_P*.DAT archives (`0x8017e500` x15; `0x80120000` incl. WEP_PA0F/PC12/PI13). Score 564..5481; j 0..6,
  p 11..98, l 3..97. Shipped form: DAT type-7 segment, unverified.

<a id="fam-wep-s"></a>
### WEP_S00..WEP_S09 (R3, 10 files)
- WEP_S02..06, S08, S09 (7) rank `0x80180d00` (score 104: p 2, l 2); S01, S07 rank `0x8017c500` on lui pairs only
  (score 6..7, p 0): weak. Both bases are WEP_SUB*.DAT type-7 dests. No same-stem DAT.
- WEP_S00 (28 B; u32 + three empty functions): ranked none, no self-reference at any of 46 candidates; base `-`.

<a id="fam-res"></a>
### RES00..RES02 (R3)
- 3/3 rank `0x80150000`; agrees with a type-7 dest of each same-stem DAT (3/3). Score 814..2222; j 7..21, p 2, l 12..20.

<a id="fam-title"></a>
### M_RESULT, M_TITLE, TITLE2, OPENING, ENDING (R3)
- 5/5 rank `0x800d5800`; agrees with a type-7 dest of each same-stem DAT (5/5). Score -793..3726; j 0..33, p 4..18,
  l 3..25.
- OPENING (1592 B): only self-referencing candidate, score -793 (1 jal into range misses every start; p 4, l 3): weak.

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
