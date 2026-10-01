<!-- Cookbook index. One line per entry, appended by tools/cookbook_add.sh. Grep this file by tag or title; never read it whole; never read an entry you did not find here. -->
<!-- C<nnnn> | <title> | <tags> | <date> | <phase>/<task> | origin: <source section or report id> -->
C0001 | The decomp idiom entry shape: residual, mechanism, lever, byte proof | cookbook-format,idiom,residual,mechanism,lever,byte-proof | 2026-09-30 | - | decomp-architect
C0002 | The triage table: diff tell to mechanism to lever family | triage,symptom,tell,spill,register-pressure,aliasing,callee-saved,branch-polarity,cross-jump | 2026-09-30 | - | decomp-architect
C0003 | Offline tooling first: every computable step becomes a zero-token deterministic tool | strategy,deterministic,recovery,permuter,economics,tooling | 2026-09-30 | - | decomp-architect
C0004 | A slow gate is a bug: parallel worktrees, a per-binary lock, the parallel flag on every build | gate,parallel,worktree,lock,build,throughput | 2026-09-30 | - | decomp-architect
C0005 | Breadth is isolated agents: per-item agents cost about linearly, one serial loop about quadratically | breadth,agents,drafting,fan-out,cost | 2026-09-30 | - | decomp-architect
C0006 | Route drafters by a difficulty cliff measured on your own corpus | routing,model-tier,drafting,cliff,escalation,cost | 2026-09-30 | - | decomp-architect
C0007 | dumpsxiso writes XA/STR at 2336 B/sector; compare by sectors, not ISO size | psx,iso9660,xa,str,dumpsxiso,extract | 2026-09-30 | 1.1/T1 | T1 gotcha
C0008 | dumpsxiso -x dir holds license_data.dat and DA .WAV; move out before diffing | psx,dumpsxiso,extract,diff,reference | 2026-09-30 | 1.1/T2 | T2 gotcha
C0009 | PSX ISO da entries point into the CD-DA track; skip when extent >= Track 1 sectors | psx,iso9660,cdda,extract | 2026-09-30 | 1.1/T2 | T2 gotcha
C0010 | Ghidra on macOS arm64 needs locally built decompiler natives | ghidra,macos,psx_ldr | 2026-10-01 | 1.2/T1 | phase-ends/current/tasks/T1.md
C0011 | Score PsyQ SDK versions with psx_ldr SigApplier in a rolled-back txn | ghidra,psyq,psx_ldr,sdk-version | 2026-10-01 | 1.2/T1 | phase-ends/current/tasks/T1.md
C0012 | Ghidra annotation deltas must drop ANALYSIS/DEFAULT-sourced labels | ghidra,annotations,determinism | 2026-10-01 | 1.2/T2 | phase-ends/current/tasks/T2.md
C0013 | GhidrAssistMCP 12.1 zips run on Ghidra 12.1.x headless | ghidra,mcp | 2026-10-01 | 1.2/T2 | phase-ends/current/tasks/T2.md
C0014 | PsyQ libcard _patch_card self-modifies code at runtime | psyq,emulator,ram-proof,smc | 2026-10-01 | 1.2/T3 | phase-ends/current/tasks/T3.md
C0015 | PCSX-Redux arm64 dynarec SIGILLs: use -interpreter for oracle runs | pcsx-redux,emulator,arm64 | 2026-10-01 | 1.2/T3 | phase-ends/current/tasks/T3.md
C0016 | Find a PS1 game's file table by scanning the exe for disc LBAs | ps1,iso,file-table,loader | 2026-10-01 | 1.2/T4 | phase-ends/current/tasks/T4.md
C0017 | Headless Ghidra -readOnly -noanalysis for throwaway listings | ghidra,headless | 2026-10-01 | 1.2/T4 | phase-ends/current/tasks/T4.md
C0018 | Snapshot RAM before writing a decompressor to prove an overlay base | emulator,overlay,compression,ram-proof | 2026-10-01 | 1.2/T5 | phase-ends/current/tasks/T5.md
C0019 | PCSX-Redux Lua pad scripting | pcsx-redux,lua,input | 2026-10-01 | 1.2/T5 | phase-ends/current/tasks/T5.md
C0020 | Ghidra misses whole switch-bearing functions on PSX exes | ghidra,switch,jump-table,mips | 2026-10-01 | 1.2/T7 | phase-ends/current/tasks/T7.md
C0021 | Keep non-generated artifacts out of build/; make clean removes only generated outputs | make,clean,build,hygiene | 2026-10-01 | 1.3/T3 | PhaseEnd 1.3
C0022 | splat: carve an overlay's pre-code data head as rodata (MIPS II+ ops rejected by as -march=r3000) | splat,overlay,rodata,mips,psx | 2026-10-01 | 1.3/T4 | PhaseEnd 1.3
C0023 | splat 0.41 omits out-of-segment j targets from undefined_funcs_auto; generate them | splat,linker,undefined-symbols,overlay | 2026-10-01 | 1.3/T4 | PhaseEnd 1.3
C0024 | Prefer i686 wibo under Apple-silicon Docker | psx,psyq,wibo,docker,apple-silicon,toolchain | 2026-10-01 | 1.4/T1 | T1 gotcha
C0025 | Count gp-relative accesses before picking -G | psx,gcc,gp,flags,fingerprint | 2026-10-01 | 1.4/T2 | T2 gotcha
C0026 | Pin a toolchain by output equivalence classes, not by triples | toolchain,probe,ladder,aspsx,pin | 2026-10-01 | 1.4/T4 | T4 gotcha
C0027 | make -n checks also see recipe comments and case branches | make,dry-run,check | 2026-10-01 | 1.4/T5 | T5 gotcha
C0028 | splat 0.41 c subsegments can emit stub C bodies | splat,c,include_asm,match | 2026-10-01 | 1.4/T6 | T6 gotcha
C0029 | INCLUDE_ASM through maspsx needs macro.inc in the assembled file | maspsx,include_asm,as,macro | 2026-10-01 | 1.4/T6 | T6 gotcha
C0030 | A verify clause that reads extracted data must extract it itself | milestone,verify,extract,fresh-volume | 2026-10-01 | 1.4/T9 | phase-end gotcha
