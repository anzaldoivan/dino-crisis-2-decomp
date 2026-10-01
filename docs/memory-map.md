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
