# PhaseEnd — Phase 1.0: Governance and the firewall (implements Gen 1.0)
Approved: 2026-09-30 | Closed: 2026-09-30 | Planner: claude-opus-5-5/medium | Tasks: 2 done, 0 superseded

## Milestone
Milestone: `PY tools/audit_public.py` exits 0 on the tree and 1 on the planted fixture (`config/firewall-fixture.sha1`); `grep -c '| decomp-architect$' rules/INDEX.md` = 67; `docker build --platform linux/amd64` of `tools/docker/Dockerfile` exits 0; `gh run list --workflow no-rom.yml -L 1` green after the developer's first push — verified by: `bash tools/firewall_control.sh` (exit 0 = tree PASS + planted FAIL); `grep -c '| decomp-architect$' rules/INDEX.md`; `docker build --platform linux/amd64 -t dc2-build -f tools/docker/Dockerfile tools/docker` exit 0; `gh run list --workflow no-rom.yml -L 1` conclusion `success`
Verified: `bash tools/docker/dc.sh build && … sync && … run sh -c 'uname -m && make format-check'` → x86_64, rc=0; `bash tools/firewall_control.sh` → OK rc 0 (.run/logs/t2-verify.log)

## Tasks
- T1 — firewall negative control, both ways | Done: `bash tools/firewall_control.sh` plants the fixture blob, proves the ROM audit FAILs naming it, removes it, proves the tree PASSes; exit 0 iff all hold, exit 1 naming the failed step otherwise. | commit: - | tasks/T1.md | logs/T1.md

## Decisions that still bind
- T1 — the Phase 1.0 firewall milestone clause is checked by `bash tools/firewall_control.sh` (exit 0); interpreter `$PY`, else card PY, else `python3`
- The firewall milestone clause is checked by `bash tools/firewall_control.sh` (exit 0) — environment fact, already a Tooling-inventory row in docs/ops/decomp-environment.md:81.
- Builds run only in the amd64 container via `tools/docker/dc.sh` on volume `dc2-work`, base `ubuntu:24.04` pinned by digest — environment fact, card Build/Pins lines and docs/ops/docker-host.md.
- Next task needs: after the developer pushes T1/T2, confirm the no-rom CI run on the new head is green; run `dc.sh sync` before any `dc.sh run` after host edits.

## Rules proposed
- (none)

## Cookbook entries added
- (none)

## Research
- (none)

## Audit
- seed: median 14k, max 15k, n=4 (phase 1.0)
- CLAUDE.md: 1202 bytes
- .claude/skills/docker-vm-no-privileged/SKILL.md: 702 bytes
- .claude/skills/project-architect/SKILL.md: 13448 bytes
- .claude-state/memory/MEMORY.md: 214 bytes
- HOW_WE_WORK.md: 6060 bytes
- cookbook/INDEX.md: 1367 bytes
- rules/INDEX.md: 10057 bytes
### Carry audit — phase 1.0 (2026-09-30T22:15:45Z → open UTC, 1 sessions, 94 requests)

| file (read) | chars | n |
|---|---|---|
| seed.md | 8.3k | 1 |
| SKILL.md | 2.4k | 1 |
| Dockerfile | 1.9k | 2 |
| dc.sh | 1.6k | 1 |
| INDEX.md | 214 | 1 |
| file (write) | chars | n |
|---|---|---|
| docker-host.md | 2.5k | 1 |
| RECAP.md | 2.3k | 1 |
| T1.md | 2.2k | 1 |
| T2.md | 2.0k | 1 |
| T2.c2.md | 1.8k | 1 |
| T1.md | 1.6k | 1 |
| dc.sh | 1.5k | 1 |
| firewall_control.sh | 1.3k | 2 |
| result kind | chars | n |
|---|---|---|
| tools/card.py | 48.1k | 4 |
| bash other | 24.9k | 25 |
| Read | 14.5k | 6 |
| run.sh | 11.6k | 11 |
| Agent | 6.6k | 6 |
| tools/status.py | 6.2k | 4 |
| tools/plan_edit.py | 3.1k | 5 |
| other | 2.2k | 11 |
| Write | 2.0k | 10 |
| tools/firewall_control.sh | 1.2k | 3 |
- whole reads over 20.0k: 0
- seed floor: (no retriever runs)
- outline credit: 0 outlines · 0 followed by a ranged read · 0 chars credited
- warm pings: 0 pings over 0 runs, 0 warmed waits over the TTL, rewrites across warmed waits 0, waits past the cap 0, pings cost $0.0000 vs rewrites replaced $0.0000, cheaper than one rewrite: n/a
- toasts by cause (waiting): question 0, review 0, replan 0, permission 0, input 0, discussion 0, crash 0, idle 0, stop 0, subagent-stop 0, model 0, other 0
- router turns by cause: loop 4, relay 0, re-arm 1, other 4, relay cost $0.0000
- retriever re-asks: 0 of 0 retriever briefs repeat a lookup of the same run
- router: pa-session d585a8a6 · requests 23 · ctx at end 53.9k · growth T1 +1.8k, T2 +914, T2 +614 · top: Read 8.3k/1, tools/status.py 6.2k/4, Agent 4.4k/4, other 1.8k/5, tools/plan_edit.py 761/3
- noise: 5 lines 319 chars — usage: : 4 lines/267, No such file or directory: 1 lines/52
- price: read $0.20/Mtok · 1h write $6.51/Mtok · output $16.26/Mtok (phase model mix)
- median requests after a read: 8
- carry/request: 1329 chars, 94 requests
- flag: 4 tool-source reads by coder-opus55, expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/docker/Dockerfile  938 chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/.claude/skills/docker-vm-no-privileged/SKILL.md  2.4k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/docker/dc.sh  1.6k chars  coder-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/docker/Dockerfile  1.0k chars  coder-opus55

## Agent runs
- T1 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 33k | $0.241 | saved - | completed | parent d585a8a6
- T2 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 33k | $0.170 | saved - | completed | parent d585a8a6
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 30k | $0.201 | saved - | completed | parent a23a4a66
- T2 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 34k | $0.187 | saved - | completed | parent d585a8a6
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 27k | $0.236 | saved - | completed | parent afdd60fb
- T2 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 29k | $0.175 | saved - | completed | parent d585a8a6

## Discussions
- (none)

## Deferred
- from T2: nothing from T2; run `dc.sh sync` before any `dc.sh run` after host edits. (unconsumed)

## Changes
- 2026-09-30 router: T1 next -> done
- 2026-09-30 router: T2 next -> done

## Plain-English Recap
Phase 1.0 set up the safety rails and the build machine that every later phase of the Dino Crisis 2 decompilation depends on. The first task added a negative control for the public-repo firewall: a script that deliberately plants a known forbidden file and confirms the audit tool catches it, then confirms the real tree is clean, so we know the check that keeps game data out of git actually works. The second task built a pinned Linux x86-64 container (the "build host") on this ARM Mac, with the MIPS cross-compiler toolchain inside and the source copied into a Docker named volume rather than shared from the Mac, and documented its exact versions in docs/ops/docker-host.md. All four milestone checks passed, including the no-ROM continuous-integration run on GitHub.
