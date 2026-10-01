# PhaseEnd — Phase 1.4: The compiler pinned by evidence (implements Gen 1.4)
Approved: 2026-10-01 | Closed: 2026-10-01 | Planner: claude-opus-5-5/medium | Tasks: 10 done, 0 superseded

## Milestone
Milestone: probe harness: 3–5 probes byte-identical under exactly one triple (cc1 build, assembler shim version explicit, binutils, flags); per-module variation recorded in `docs/ops/decomp-environment.md`; first functions matched end to end in their real units; clean fleet check N of N; `no-rom.yml` compile-only job enabled — verified by: `bash tools/docker/dc.sh sync && bash tools/docker/dc.sh run python3 tools/probe.py --pinned` → exit 0, prints `probes identical: K of K under the pinned triple` with 3 ≤ K ≤ 5 and `ladder: exactly 1 triple matches all K`; `bash tools/docker/dc.sh run bash tools/fleet_check.sh` → exit 0, prints `83 of 83 byte-identical`; `bash tools/docker/dc.sh run python3 tools/banked.py` → exit 0, prints `banked: n` with n ≥ 3 and `verbatim: 0`; `bash tools/docker/dc.sh run bash tools/compile_only.sh` (asm/ and extracted data absent) → exit 0; `grep -c "compile-only" .github/workflows/no-rom.yml` ≥ 1 and the job is not commented; `PY tools/audit_public.py` → exit 0; `bash tools/firewall_control.sh` → exit 0
Verified: `.run/t10/fixtures.py` → ALL PASS (5 of 5 bound 3..5 GREEN; 4 of 5, 6 of 6, ladder all 4, banked: 2 RED; banked: 5 + verbatim: 0 GREEN; verbatim: 1 RED; 83 of 83 GREEN, 82 of 83 RED); after `docker volume rm dc2-work`, run.sh t10-verify `PY tools/phaseend_index.py verify --verbose` → `VERIFY: GREEN (7/7)`

## Tasks
- T1 — candidate toolchains fetched and pinned in the image | Done: image `dc2-build` carries 12 candidate cc1 builds + maspsx + wibo in `/opt/cc/<name>/`, sha256-pinned in `config/toolchains.tsv`; 11 of 12 smoke-compile `cpp → cc1 → maspsx → as` to a MIPS `.o`; psyq3.6 dropped. | commit: cc7f70b | tasks/T1.md | logs/T1.md
- T2 — compiler fingerprint survey across the fleet | Done: `tools/cc_fingerprint.py` scans split asm of all 83 aliases, lib-object ranges (183) scored apart as control; writes `.run/fingerprint/<alias>.tsv` + `summary.txt`; deterministic. | commit: 81196b0 | tasks/T2.md | logs/T2.md
- T3 — standalone probe harness with controls | Done: `tools/probe.py` compiles our C probes under a narrowable triple matrix, masks relocs, compares with target bytes read at run time; known-true/known-false controls run every time; `--selftest` green. | commit: 8427714 | tasks/T3.md | logs/T3.md
- T4 — probes run down the ladder; the fingerprint verdict | Done: 5 game probes (exe + 3 overlays) in C match byte-identical masked; full ladder 5 × 528 triples → exactly 1 output class matching all 5: {gcc-2.95.2-psx, psyq4.6} × aspsx 2.56–2.86 × G0 × O2 (12 triples); pinned psyq4.6 a2.86/G0/O2 after review. | commit: b7d3556 | tasks/T4.md | logs/T4.md
- T5 — the pin wired into the build | Done: Makefile compiles `src/<a>/…c` through the pinned triple (psyq4.6 cc1 via wibo → maspsx --aspsx-version=2.86 -G0 → as 2.42) into `build/<a>/…c.o`; INCLUDE_ASM header in place; no C unit yet, gate unchanged. | commit: e1c632a | tasks/T5.md | logs/T5.md
- T6 — c subsegments from a tracked unit list, zero C bodies | Done: `splat_gen.py` cuts `c` subsegments from `config/c_units.tsv` (range = start to next existing cut), `--check` asserts them both ways; 4 INCLUDE_ASM-only units (exe, st2, st6, st9) hold all 5 T4 probes and build through the T5 C branch; clean fleet check 83 of 83. | commit: f875300 | tasks/T6.md | logs/T6.md
- T7 — first functions banked in their real units | Done: all 5 T4 probes banked as C in their T6 units (exe 2, st2 1, st6 1, st9 1); whole fleet 83 of 83 byte-identical from clean fleet_check (G10 third tier); `tools/banked.py` reports banked 5, verbatim 0. | commit: d6488d6 | tasks/T7.md | logs/T7.md
- T8 — no-ROM compile-only job | Done: `tools/compile_only.sh` compiles every `src/**/*.c` with cpp → cc1 → maspsx using the Makefile's own flags. It needs no asm/, no extracted data and no disc, and prints `compiled: 4 of 4 units`. `no-rom.yml` now has a live `compile-only` job. | commit: d1cb015 | tasks/T8.md | logs/T8.md
- T9 — probe --pinned self-sufficient; ladder line in milestone wording | Done: `tools/probe.py` extracts the target itself (`make extract OUT=.run/extracted/retail`) when no exe is extracted and BASEDIR is unset, rc 2 with a clear error if that fails; ladder line reads `ladder: exactly 1 triple matches all K` (plural `triples match` for m != 1). | commit: - | tasks/T9.md | logs/T9.md

## Decisions that still bind
- T1 — candidate compilers live at `/opt/cc/<name>/` in the image; `config/toolchains.tsv` is the pin record; integrity check = `dc.sh run bash tools/docker/fetch_toolchain.sh --verify /opt/cc`
- T3 — pinned triple = optional column 7 `pinned` of `config/toolchains.tsv` on exactly one cc row, value `a<aspsx>/G<g>/O<o>` (e.g. `a2.79/G0/O2`); absent → `--pinned` exit 2. Adding it changes the Dockerfile-COPYed file (image rebuild).
- T4 — "exactly one triple" = one output equivalence class over all probes; probe.py `ladder:` counts classes
- T4 — pin `psyq4.6` + aspsx 2.86, -G0 -O2 (developer review 2026-10-01) — tie rule: same compiler as gcc-2.95.2-psx (cc1 .s identical but CRLF), PsyQ build pinned era-native; aspsx 2.56–2.86 indistinguishable on these probes
- T5 — per-module triple variation goes through `CFLAGS_<alias>` / `CPPFLAGS_<alias>` / `MASPSXFLAGS_<alias>` on the make command line or in the Makefile, never a second rule
- T6 — C cuts live in `config/c_units.tsv`; YAML c lines come only from `splat_gen.py --force --only <alias>`
- T8 — C flags live only in the Makefile; tools read them with `make -s print-c A=<alias> CCDIR=<dir>`, never re-typed
- T10 — verify placeholder = standalone single ASCII letter (not a/A/I, not adjacent to `[\w./-]`) in an expected string → `\d+`; one value per letter across a clause's expectations; bounds `lo OP X OP hi`, `X OP n`, `n OP X` (≤ < ≥ > = and ASCII forms) from the prose outside backticks; unbounded placeholder only needs to bind; all else literal substring
- Rule G69 (norm, T5/T8): the compiler triple and its flags are defined once, in the Makefile; tools read them via `make -s print-c`; per-module variation only through `CFLAGS_<alias>` hooks.
- Pinned triple psyq4.6 + aspsx 2.86, -G0 -O2 (T4, developer review): environment fact, in `docs/ops/decomp-environment.md:10` and the card Pins line.
- "Exactly one triple" = one output equivalence class (T4): contract, in `tools/probes/README.md:30` and enforced by `tools/probe.py`.
- Pinned triple record = `config/toolchains.tsv` column 7 on one cc row (T3): environment fact, `tools/probes/README.md:21`, `docs/ops/decomp-environment.md:10`.
- Candidate compilers at `/opt/cc/<name>/`, pin record `config/toolchains.tsv` (T1): environment fact, card Pins line.
- C cuts live in `config/c_units.tsv`; YAML `c` lines only from `splat_gen.py --force --only` (T6): environment fact, `docs/ops/decomp-environment.md:64`.
- verify placeholder binding and bounds (T10): tool contract, in `phaseend_index.py verify --help` and the card Tools row.
- Next task needs: overlays E, KOF, WEP, WEP_S, RES, MISC are pinned by assumption only (no probe); probe one function per module before decompiling there.

## Rules proposed
- (none)

## Cookbook entries added
C0024 | Prefer i686 wibo under Apple-silicon Docker | psx,psyq,wibo,docker,apple-silicon,toolchain | 2026-10-01 | 1.4/T1 | T1 gotcha
C0025 | Count gp-relative accesses before picking -G | psx,gcc,gp,flags,fingerprint | 2026-10-01 | 1.4/T2 | T2 gotcha
C0026 | Pin a toolchain by output equivalence classes, not by triples | toolchain,probe,ladder,aspsx,pin | 2026-10-01 | 1.4/T4 | T4 gotcha
C0027 | make -n checks also see recipe comments and case branches | make,dry-run,check | 2026-10-01 | 1.4/T5 | T5 gotcha
C0028 | splat 0.41 c subsegments can emit stub C bodies | splat,c,include_asm,match | 2026-10-01 | 1.4/T6 | T6 gotcha
C0029 | INCLUDE_ASM through maspsx needs macro.inc in the assembled file | maspsx,include_asm,as,macro | 2026-10-01 | 1.4/T6 | T6 gotcha
C0030 | A verify clause that reads extracted data must extract it itself | milestone,verify,extract,fresh-volume | 2026-10-01 | 1.4/T9 | phase-end gotcha

## Research
R1.4-001 | candidate ladder of PsyQ-era GCC cc1 builds and how they are fetched; maspsx; flags; fingerprinting; Capcom evidence | PsyQ-era cc1 ladder, maspsx flags, fetch URLs (for PsyQ 4.7.0 PS1 decomp) | ps1, psyq, gcc, maspsx, old-gcc, decomp.me, capcom | retriever-web | 2026-10-01 | 37 lines

## Audit
- seed: median 14k, max 15k, n=13 (phase 1.4)
- previous phase 1.3: median 14k, growth 0.2%
- CLAUDE.md: 1202 bytes (unchanged)
- .claude/skills/docker-vm-no-privileged/SKILL.md: 702 bytes (unchanged)
- .claude/skills/ghidra-mcp-scratch-copy/SKILL.md: 3730 bytes (unchanged)
- .claude/skills/pipe-exit-status/SKILL.md: 2593 bytes (unchanged)
- .claude/skills/project-architect/SKILL.md: 13526 bytes (+78)
- .claude-state/memory/MEMORY.md: 214 bytes (unchanged)
- HOW_WE_WORK.md: 7021 bytes
- cookbook/INDEX.md: 4903 bytes
- rules/INDEX.md: 10309 bytes
### Carry audit — phase 1.4 (2026-10-01T12:41:43Z → open UTC, 1 sessions, 564 requests)
- beside 1.4: session b1ddad86 $3.25 (61 turns), not attributed

| file (read) | chars | n |
|---|---|---|
| probe.py | 47.7k | 8 |
| Makefile | 34.6k | 6 |
| docker-host.md | 19.2k | 3 |
| splat_gen.py | 17.1k | 1 |
| decomp-environment.md | 14.9k | 5 |
| phaseend_index.py | 14.8k | 3 |
| seed.md | 10.1k | 1 |
| PHASE_PLAN.md | 9.0k | 6 |
| README.md | 6.3k | 2 |
| task.template.md | 5.5k | 3 |
| file (write) | chars | n |
|---|---|---|
| probe.py | 14.4k | 4 |
| cc_fingerprint.py | 8.5k | 3 |
| decomp-environment.md | 7.8k | 10 |
| phaseend_index.py | 5.2k | 4 |
| T4.md | 4.3k | 1 |
| T4.c1.md | 4.3k | 1 |
| README.md | 4.2k | 4 |
| T6.c1.md | 3.4k | 1 |
| result kind | chars | n |
|---|---|---|
| bash other | 268.4k | 154 |
| tools/card.py | 204.9k | 20 |
| Read | 195.9k | 47 |
| run.sh | 75.2k | 66 |
| tools/probe.py | 55.4k | 22 |
| tools/splat_gen.py | 49.6k | 13 |
| tools/plan_edit.py | 34.2k | 18 |
| Agent | 29.6k | 27 |
| tools/phaseend_index.py | 8.5k | 7 |
| Write | 7.9k | 43 |
- whole reads over 20.0k: 0
- seed floor: retriever-code 5,704 (n 1, prev 5,575) · retriever-web 4,230 (n 1, prev —)
- outline credit: 2 outlines · 2 followed by a ranged read · 63.7k chars credited
- warm pings: 17 pings over 10 runs, 8 warmed waits over the TTL, rewrites across warmed waits 0, waits past the cap 0, pings cost $0.1259 vs rewrites replaced $1.6258, cheaper than one rewrite: yes
- toasts by cause (waiting): question 0, review 0, replan 0, permission 0, input 0, discussion 0, crash 0, idle 0, stop 0, subagent-stop 0, model 0, other 0
- router turns by cause: loop 24, relay 17, re-arm 5, other 0, relay cost $0.5090
- retriever re-asks: 0 of 0 retriever briefs repeat a lookup of the same run
- router: pa-session 167d8b13 · requests 110 · ctx at end 99.9k · growth T1 +1.9k, T2 +1.0k, T3 +1.0k, T4 +2.6k, T4 +1.0k, T5 +993, T6 +1.5k, T7 +460, T8 +523, PHASE-END +12.0k, T9 +979, T10 +1.5k · top: Agent 17.5k/16, Read 10.1k/1, other 6.1k/28, tools/status.py 4.9k/14, tools/plan_edit.py 4.0k/12
- noise: 48 lines 3.2k chars — usage: : 38 lines/2.6k, No such file or directory: 10 lines/565
- price: read $0.20/Mtok · 1h write $6.81/Mtok · output $17.03/Mtok (phase model mix)
- median requests after a read: 6
- carry/request: 1768 chars, 564 requests (prev 1722 chars, 242 requests, growth 2.6%)
- candidate: audit_public_help (proposed)
- flag: 17 tool-source reads by coder-opus55, expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/phaseend_index.py  6.2k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/phaseend_index.py  2.7k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probes/README.md  3.2k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/splat_gen.py  17.1k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/docker/Dockerfile  2.0k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  1.8k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  1.2k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  1.4k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  14.5k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/docker/Dockerfile  2.0k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/docker/dc.sh  2.8k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  2.9k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  8.1k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probes/README.md  3.2k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  2.2k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/phaseend_index.py  5.9k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/probe.py  15.5k chars  expert-opus55

## Agent runs
- - | auditor | other | claude-opus-5-5 | medium | ctx 25k | $0.143 | saved - | completed | parent 167d8b13
- T1 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 52k | $0.370 | saved $0.06 | completed | parent 167d8b13
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 55k | $0.500 | saved $0.02 | completed | parent a1b375e5
- T2 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 36k | $0.183 | saved $0.34 | completed | parent 167d8b13
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 63k | $0.534 | saved $0.28 | completed | parent a95cc7fc
- T3 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 49k | $0.368 | saved - | completed | parent 167d8b13
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 58k | $0.520 | saved - | completed | parent a15d1c0d
- T4 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 50k | $0.417 | saved - | completed | parent 167d8b13
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 102k | $1.312 | saved $0.56 | completed | parent aa2dc2af
- T4 | review | review | claude-opus-5-5 | medium | ctx 23k | $0.127 | saved - | completed | parent 167d8b13
- T4 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 43k | $0.240 | saved - | completed | parent 167d8b13
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 33k | $0.230 | saved $0.56 | completed | parent aea5f244
- T5 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 49k | $0.277 | saved - | completed | parent 167d8b13
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 44k | $0.325 | saved - | completed | parent a660ec3f
- T6 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 46k | $0.299 | saved - | completed | parent 167d8b13
- T6 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 67k | $0.728 | saved $0.03 | completed | parent a7d25393
- T7 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 37k | $0.246 | saved - | completed | parent 167d8b13
- T7 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 32k | $0.180 | saved - | completed | parent a56778dc
- T8 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 47k | $0.336 | saved - | completed | parent 167d8b13
- T8 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 40k | $0.287 | saved - | completed | parent a445615d
- PHASE-END | expert-opus55 | expert | claude-opus-5-5 | high | ctx 45k | $0.414 | saved - | completed | parent 167d8b13
- T1 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 45k | $0.261 | saved - | completed | parent 2424e7a8
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 46k | $0.367 | saved - | completed | parent af01685e
- T2 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 63k | $0.492 | saved - | completed | parent 2424e7a8
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 67k | $0.660 | saved - | completed | parent a2101a61
- T3 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 53k | $0.379 | saved - | completed | parent 2424e7a8
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 55k | $0.646 | saved - | completed | parent a7a6ad75
- T4 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 48k | $0.448 | saved - | completed | parent 2424e7a8
- T4 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 85k | $1.195 | saved - | completed | parent a06de12b
- T5 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 60k | $0.671 | saved - | completed | parent 2424e7a8
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 37k | $0.406 | saved $0.12 | completed | parent a857480c
- T5 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 37k | $0.430 | saved - | completed | parent a857480c
- PHASE-END | critic | critic | claude-opus-5-5 | medium | ctx 19k | $0.167 | saved - | completed | parent 167d8b13
- T9 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 52k | $0.399 | saved - | completed | parent 167d8b13
- T9 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 36k | $0.292 | saved - | completed | parent af2e4197
- T10 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 50k | $0.327 | saved $0.17 | completed | parent 167d8b13
- T5 | critic | critic | claude-opus-5-5 | medium | ctx 23k | $0.134 | saved - | completed | parent 2424e7a8
- T6 | expert-opus55 | expert | claude-opus-5-5 | high | ctx 69k | $0.533 | saved $0.05 | completed | parent 2424e7a8
- T10 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 31k | $0.292 | saved - | completed | parent ac4d026b
- … 2 more: PY ~/.claude/pa3/pa_ledger.py report

## Discussions
- (none)

## Deferred
- from T10: phase-end verify can be re-run; expect GREEN 7/7 (unconsumed)

## Changes
- 2026-10-01 router: T1 next -> done
- 2026-10-01 router: T2 next -> done
- 2026-10-01 router: T3 next -> done
- 2026-10-01 developer: T4 review: cc pin = psyq4.6 (identical to gcc-2.95.2-psx on all 5 probes; tie rule, era-native)
- 2026-10-01 developer: T4 review: aspsx pin = a2.86 (2.56-2.86 indistinguishable at G0); revisit if a later function splits the range
- 2026-10-01 developer: T4 review: 'exactly 1 triple' = one output equivalence class (12 members), per Rationale; accepted
- 2026-10-01 router: T4 next -> done
- 2026-10-01 router: T5 next -> done
- 2026-10-01 router: T6 next -> done
- 2026-10-01 router: T7 next -> done
- 2026-10-01 router: T8 next -> done
- 2026-10-01 critic: added T9 — probe --pinned self-sufficient; ladder line in milestone wording
- 2026-10-01 critic: added T10 — verify binds milestone placeholders and checks their bounds
- 2026-10-01 critic: critic: phase-end RED 5/7 was literal placeholder matching (K of K, banked: n) plus clause 1 depending on clause 2's extraction; T9 and T10 added, milestone text unchanged; respawn the closer after T10
- 2026-10-01 router: T9 next -> done
- 2026-10-01 router: T10 next -> done

## Plain-English Recap
Phase 1.4 found the exact compiler setup the original game was built with, so that C code we write can turn into the very same machine code. A "triple" here means one compiler build, one assembler-shim version and one set of flags; the phase built a probe harness that compiles five small functions we rewrote in C under every candidate triple and keeps only the triples whose output is byte-identical to the game. Exactly one group of equivalent triples survived, and the project pinned PsyQ 4.6 with assembler compatibility 2.86 and flags -G0 -O2. Those five functions now live as C in their real source files, the whole game (83 binaries) still rebuilds byte-identical, and a new CI job compiles all our C without any game data. The first closing attempt failed because the milestone checker read placeholders like "K of K" literally and one check needed data another check produced; tasks T9 and T10 fixed both, and the rerun passed 7 of 7 checks.

Verified: `PY tools/phaseend_index.py verify --verbose` (run.sh --bg pe3-verify) → VERIFY: GREEN (7/7); clause 1 `probes identical: 5 of 5 under the pinned triple` + `ladder: exactly 1 triple matches all 5` (.run/logs/verify1.log:2652-2653); clause 2 `83 of 83 byte-identical`; clause 3 `banked: 5`, `verbatim: 0`; clause 4 `compiled: 4 of 4 units`; clause 5 count 4, job `compile-only:` at no-rom.yml:42 uncommented; clauses 6-7 exit 0.
