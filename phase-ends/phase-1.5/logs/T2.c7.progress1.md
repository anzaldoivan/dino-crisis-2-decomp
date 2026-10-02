# TASK_PROGRESS — T2.c7 (coder, handoff at context threshold)

## Done (committed with this file)
- `tools/census.py`: R2 transfer split (criterion g, `(g) transfer split: n`, cross-check `+ transfer splits`); `never_emitted` R3 (`R3 words: n`); scan_asm keeps `wv` (raw words), `jtw` (`.word .L` per vram); Bin.jt (jr, s, e) from boundaries jtbl basis; leading R3/invalid words trimmed from a function start when not a cf target (data head -> invalid-insn span).
- R2 narrowing: block A when any inner target t has `transfer+4 <= t <= A` (branch into delay slot: exe SquareRoot0 0x80078e5c, InvSquareRoot, SquareRoot12, 0x8003889c) — removed all end-ghidra-longer rows.
- `DumpFunctions.java`: `fallReachable` (s3/s4; s2 when all refs non-flow), `neverEmitted`, `wordAt`, bodies mode (`bodies <outdir> <seeds>`: full seeding, rolled back, writes `<prog>.bodies.tsv`); header documented.
- `xprog_targets.py`: optional 3rd arg bodies dir; jal word kept only inside a body of its source program (bisect).
- `dump_functions.sh`: xprog_unfiltered -> bodies pass -> filtered xprog -> main pass (~4.5 min; use `--bg` + two `--wait`).
- `oracle_diff.py`: `pred_unreferenced` skips zero padding; new kind `data-head` (`pred_data_head`), KINDS/KINDS_OF/instrument; docstring.
- `docs/ops/decomp-environment.md`: census R2/R3, seeding T2.c7, oracle kinds T2.c7 (fleet numbers in census "Fleet" bullet NOT yet updated).
- 83 caches re-dumped (dump4).

## Current numbers (dump4, last run)
- census --check rc 0: functions 3911, phantoms 0, truncations 0, `(f) jal target split: 2`, `(g) transfer split: 24`, `R3 words: 47`, data in text 19 spans (14 invalid-insn), cross-check ok.
- exe `DC2DUMPFUNCS … s2=515/639 s3=12/254 s4=95/103 merged=457`.
- oracle: 29 disagreements (census-only 19, ghidra-only 9, end-ghidra-shorter 1, longer 0); C0020 10 of 10; proposals 62, predicate true 14 (saved `.run/oracle/t2c7_new_exc.tsv`, 5 cols alias start kind basis instrument).

## Rejected (evidence)
- jal filter on saved-project bodies only: lost exe s4 seeds (0x80049900 etc., 79 disagreements) — overlay bodies are mostly txn-made.
- jal filter on bodies after s1/s2/sw: lost 0x800263a8 (src WEP05 0x8017fe1c, body made by s4) and 0x8004104c (WEP_S06).
- fall-through rule without "prev word not a Ghidra instruction": lost first functions after rodata (bin_e00 0x800d0198, kof/wep heads).
- refs guard on R3 trim: st9 0x800d5a58 is %hi/%lo-loaded data (func_800DCBB8), guard blocked it; dropped.

## Next 5 steps
1. Append `.run/oracle/t2c7_new_exc.tsv` rows to `config/oracle_exceptions.tsv`; update its header comment (data-head kind, T2.c7 padding skip); refresh existing unreferenced rows' instrument to the new `pred_unreferenced` line (`grep -n "def pred_unreferenced" tools/oracle_diff.py`).
2. `bash .run/t2c7_cycle.sh` → expect 29-14 = 15 disagreements; `dc.sh run python3 tools/oracle_diff.py --plant-start slus_012_79:0x8001b45c` → rc 1.
3. Write `.run/oracle/t2c7_residuals.txt` (rows + 4 disassembled words each; c6 used the same format).
4. Update census Fleet bullet numbers in docs/ops/decomp-environment.md; audit_public.py; commit; log `phase-ends/current/logs/T2.c7.md`.
5. Return partial/blocked with residuals (expected ~15).

## Residual analysis so far (predicate false)
- wep0a 0x8017e500 `syscall 1` (handwritten) — not an R3 word by brief definition -> census-only + ghidra-only 0x8017e504.
- wep_s07 0x8017c504: Ghidra cache has 0 functions; no data-head parent.
- res01 0x80150034: Ghidra 0x80150000 size 4 does not contain it -> data-head false.
- psx_bin_logo 0x800d5920 (census-only, no Ghidra fn) + ghidra-only 0x800d638c (s2 mid-function).
- wep05 0x80180000 (s2, after jal+nop) still seeded despite s2 non-flow rule (has a flow ref or rule not hit) + end-shorter 0x8017fea0.
- exe library asm: 0x8007ae60/0x8007ae90 PATCHGTE_OBJ_DC, 0x8007e408/e420/e448 PATCH_OBJ_*, 0x8007e478, 0x8007e48c (new R2 split after `jr $v0; nop; nop` in patch blob; Ghidra has no fn at either; unreferenced false: ghidra refs 1).

## Gotchas
- `.run/oracle/*` outputs live in the container volume; `.run/t2c7_cycle.sh` copies disagreements/proposals to host `.run/oracle/t2c7_{dis,prop}.tsv`.
- dump_functions.sh now exceeds one 285 s tool call: `--bg` then `--wait --max 270` twice.
