# How we work — dino-crisis-2-decomp

<!-- The card: standing facts for every agent. Never appended to; the cap is
     card.max_chars in .claude/pa.json (default 7000), enforced by the archive.
     Each role prints its slice with `tools/card.py slice <role>`.
     Written at install (interview) and edited in the same task as whatever changed it (H7). -->

## Project <!-- roles: expert coder router planner review critic discuss auditor curator -->
dino-crisis-2-decomp — A matching decompilation of Dino Crisis 2 (PlayStation, USA). Constitution `PROJECT_CONTEXT.md`; roadmap `GENERATION_PLAN.md`;
rules `rules/INDEX.md`; techniques `cookbook/INDEX.md`; ops detail `docs/ops/INDEX.md`.

## Developer <!-- roles: router planner review discuss auditor curator -->
Who: anzaldoivan, solo. Experience: advanced; shipped BFM-decomp (PSX, PA3) to 100%. Plan: Claude Pro (tight budget:
free local work before paid). Models: Opus 5.5 for experts/coders; never Fable; deep judgments via `/discuss max`.
Preferences: recommendations, not questions. Plain-English recaps at phase end. A notification
whenever anything waits on them. Autonomy: full inside an approved plan; stop only at the two gates. Notification channel: toast.
The developer pushes; agents never do. They ratify rules at the next planner session.

## Rhythm <!-- roles: router planner review discuss auditor curator -->
autonomous  <!-- autonomous: the router runs the phase end to end, stopping only at the two gates.
                  review: tasks marked `review: yes` end the turn with REVIEW.md and wait. -->
Gates: plan approval (planner session) and the milestone (verified by the closer expert).

## Tools <!-- roles: expert coder router planner review critic discuss auditor curator -->
PY = /opt/homebrew/opt/python@3.14/bin/python3.14

| Name | Command | Purpose |
|---|---|---|
| launch | `PY tools/launch.py --seed-only` | pa-session state detector → `.run/seed.md` |
| plan_edit | `PY tools/plan_edit.py` | the only writer of `PHASE_PLAN.md` (status, Changes, add/reopen) |
| status | `PY tools/status.py` | `.run/status.json` for the statusline; INBOX consume; waiting flags |
| task_log | `PY tools/task_log.py` | lint and finish a task summary (`Verified:` required) |
| research_add | `PY tools/research_add.py` | allocate a report id, write the index line |
| rules_add | `PY tools/rules_add.py` | add, supersede, promote, retire a rule |
| skill_add | `PY tools/skill_add.py` | turn a `workflow:` gotcha into `.claude/skills/<name>/SKILL.md` |
| cookbook_add | `bash tools/cookbook_add.sh` | add a cookbook entry and its index line |
| phaseend_index | `PY tools/phaseend_index.py` | assemble, lint, verify, archive a PhaseEnd; verify binds `K of K` letters + bounds |
| genend_index | `PY tools/genend_index.py` | assemble and lint a GenerationEnd |
| commit_task | `bash tools/commit_task.sh` | the only commit path; explicit paths, no trailers, never pushes |
| run | `bash tools/run.sh` | any command that may print >40 lines; `--bg` / `--wait` for long compute |
| dc | `bash tools/docker/dc.sh` | amd64 build host: `build`, `sync` (tree → volume `dc2-work`), `run <cmd>` in `/work` |
| extract | `make extract [OUT=…]` | own disc extractor → files/ + manifest |
| census | `dc.sh run python3 tools/census.py --check` | function census → `.run/census/functions.tsv` |
| oracle_diff | `dc.sh run python3 tools/oracle_diff.py` | census vs Ghidra cache `config/ghidra/` |
| dup_census | `dc.sh run python3 tools/dup_census.py --check` | dup tiers, reach → `.run/census/dup.tsv` |
| progress | `dc.sh run make progress` | game functions in C, denominators from build |
| harness | `dc.sh run python3 tools/harness.py [--scanners]` | 6 pairs (fleet_check runs it); `config/scanners.tsv` |

## Skills <!-- roles: expert planner -->
<!-- one line per captured workflow; the SKILL.md is the canonical text -->
- docker-vm-no-privileged — Never probe the Docker VM with --privileged or --pid=host; the classifier treats it as containment escape
- pipe-exit-status — A check piped into tail reports tail's exit; read FAIL lines or run without a pipe (zsh has pipestatus, not PIPESTATUS)
- ghidra-mcp-scratch-copy — Run MCP smoke tests on a scratch copy of the Ghidra project so a live headless job never hits the project lock

## Paths <!-- roles: expert coder router planner review critic discuss auditor curator -->
- Oracles (X3): the byte gate; Ghidra 12.1.3 project `ghidra/dc2` (ignored; MCP `127.0.0.1:8080/sse`, docs/ops/disassembler-mcp.md); PCSX-Redux API `127.0.0.1:8081`
- Data: the dump, machine-local (`docs/ops/decomp-environment.md`), never in git; extraction gitignored
- Generated (never hand-edited, H1): `asm/`, `assets/`, linker scripts, progress reports
- Hand-edited: `src/`, `include/`, `config/`, `tools/`, `docs/`
- Scratch: `.run/` (gitignored; never the system temp); cap 25 GB, warn at 20 GB; pruning is the developer's call

## Build / run / test <!-- roles: expert coder router planner review critic discuss auditor curator -->
- Build: `dc.sh sync && dc.sh run make -j build [ONLY="<alias>…"]` (amd64, pinned: `docs/ops/docker-host.md`)
- Run: `bash tools/emu/emu.sh start|stop|status` (headless PCSX-Redux; `DC2_BREAK`, `prove_load.sh`: docs/ops/decomp-environment.md)
- Test / the gate: `dc.sh run bash tools/fleet_check.sh` (clean+extract+build+harness) — green = `83 of 83 byte-identical` + `harness: 6 of 6 pairs agree, 0 disagreements`
- Modes: CI = ROM audit + compile every unit (no game); local = byte-identity with the dump
Long gates go through `tools/run.sh` with a raised timeout, never a poll loop.

## Conventions & house style <!-- roles: expert coder router planner review critic discuss auditor curator -->
- Repo is public: no game-derived byte or proprietary code in any tracked file; docs cite commits by date + subject
<!-- project-specific only; the general house style is in the project-architect skill -->

## Environment <!-- roles: expert coder router planner review critic discuss auditor curator -->
- OS / shells: Darwin / bash, zsh
- Python: /opt/homebrew/opt/python@3.14/bin/python3.14 (3.14.7)
- Pins: triple psyq4.6/a2.86/G0/O2; candidates `/opt/cc` per `config/toolchains.tsv` (`fetch_toolchain.sh --verify`); image digest-pinned amd64 — `docs/ops/docker-host.md`
- Harness gotchas that bite here: arm64 Mac, so every build runs in the amd64 container (source in a named volume, not a bind mount)

## Docs map <!-- roles: expert coder router planner review critic discuss auditor curator -->
- `docs/ops/INDEX.md` — setup, env, pins, per-topic ops notes
- `cookbook/INDEX.md` — techniques, grep by tag
- `rules/INDEX.md` — full rule texts
- `phase-ends/TASK_INDEX.md`, `phase-ends/RESEARCH_INDEX.md` — what was done and what was learned
- `docs/research-archive/` — migrated legacy reports
- `docs/retired/` — everything moved out of the load order
- `docs/decomp-architect.md` — the decomp method; `docs/README.md` maps the decomp docs
