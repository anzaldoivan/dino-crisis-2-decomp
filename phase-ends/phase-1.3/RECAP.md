MILESTONE: green

## Recap
Phase 1.3 turned the game's 83 executable files (the main program and its 82 overlays, the code modules the game loads into memory on demand) into a project that rebuilds each of them from disassembled source, byte for byte. A splitter tool (splat) cuts each binary into assembly files using the load map and code boundaries found in phase 1.2; a generator writes one splitter config and one expected fingerprint (SHA-1 hash) per binary, and the Makefile assembles, links and checks every fingerprint inside the build, so a single wrong byte fails it. Everything runs in a pinned Linux build container, and one command, the fleet check, wipes the outputs, re-extracts the disc and rebuilds all 83, printing `83 of 83 byte-identical`. The fingerprints are now a required part of the public-repo firewall, and the pipeline and the per-binary build units are documented. This is the baseline every later phase must keep green while assembly is replaced by C.

Milestone evidence (`PY tools/phaseend_index.py verify --verbose`, .run/logs/phaseend-verify-v.log, GREEN 5/5):
- clause 1 fleet check after sync → `83 of 83 byte-identical`, 52 s, rc 0 (.run/logs/verify1.log)
- clause 2 `make expected` → `expected: 83 binaries`, rc 0 (verify2.log)
- clause 3 `splat_gen.py --check` → OK 83 binaries (verify3.log)
- clause 4 `audit_public.py` → 0 offenders / 545 paths, 86 hash sources (verify4.log)
- clause 5 `firewall_control.sh` → OK (verify5.log)

H7 check: T1-T5 each list `docs/ops/` or `HOW_WE_WORK.md` beside their tool/build changes: no miss.
Deviations:
- the card Build/Test line edits claimed by T3/T4 (`HOW_WE_WORK.md`) were left uncommitted in the tree; committed with this phase end.
- gotchas carried 15 tagged lines; 3 `generalizable:` → cookbook C0021-C0023; 0 `workflow:`; 6 `harness:` stay in the summaries.

## Decisions that still bind
- splat runs as `dc.sh run /opt/splat/bin/python -m splat …`, pins splat64[mips] 0.41.0 / spimdisasm 1.42.4 / rabbitizer 1.16.2 → environment fact, in docs/ops/docker-host.md:33 and pin table (T1).
- splat YAMLs are generated once, then hand-editable; `splat_gen.py --check` asserts cuts, not file equality; overwrite only with `--force` → contract, docs/ops/docker-host.md:34 and `splat_gen.py --check` (T2).
- assembler `mipsel-linux-gnu-as -march=r3000 -mabi=32 -G0 -no-pad-sections`; env `SPIMDISASM_SYMBOL_ALIGNMENT_REQUIRES_ALIGNED_SECTION=True` → environment fact, docs/ops/docker-host.md:67 (T3).
- container base files come from a make-generated `build/<alias>.override.yaml`; tracked YAMLs keep the host path → contract, docs/ops/docker-host.md:65 (T3).
- the gate is `dc.sh run bash tools/fleet_check.sh`, green = `<N> of <N> byte-identical`, N from loadmap, never a subset → norm, already rule G61 + card Test line (T4).
- `make clean` removes only per-alias generated outputs; other `build/` entries survive → contract, docs/ops/docker-host.md:58, cookbook C0021 (T4).
- `config/check.*.sha` is a `required:` hash source (glob expanded in `audit_public.rom_hashes`) → contract, config/firewall.txt:43 + `audit_public.py` (T5).
- Next task needs: whether matched C must also regenerate the loose /BIN DAT type-7 segments is open for 1.6+ (plan Context); card is near the 7000-char cap, the next card edit must trim.
