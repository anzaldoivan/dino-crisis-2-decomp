MILESTONE: green

# Phase 1.0 — RECAP

Verification (2026-09-30, closing expert):
- `PY tools/phaseend_index.py verify` → `VERIFY: GREEN (4/4)` (.run/logs/pe-verify.log)
- Clauses: firewall control rc 0 (tree PASS + planted FAIL); rules count 67; amd64 `docker build` exit 0; `gh run list --workflow no-rom.yml -L 1` → run 36783706738, completed, conclusion success, head 7a8441a (T0 merge).
- Caveat: the green CI run is on 7a8441a, the last pushed commit; T1/T2 commits (6336631..eacd3a5) are not pushed yet, so CI has not exercised them. The clause as written (latest run green after first push) holds.
- H7 check: T1 (tools/firewall_control.sh) lists docs/ops/decomp-environment.md; T2 (tools/docker/*, pins) lists docs/ops/docker-host.md and HOW_WE_WORK.md. No miss.
- Gotchas: 1 workflow → skill `docker-vm-no-privileged`; 0 generalizable; 4 harness (T1.md:20-21, T2.md:18-19) left for the auditor.

## Recap
Phase 1.0 set up the safety rails and the build machine that every later phase of the Dino Crisis 2 decompilation depends on. The first task added a negative control for the public-repo firewall: a script that deliberately plants a known forbidden file and confirms the audit tool catches it, then confirms the real tree is clean, so we know the check that keeps game data out of git actually works. The second task built a pinned Linux x86-64 container (the "build host") on this ARM Mac, with the MIPS cross-compiler toolchain inside and the source copied into a Docker named volume rather than shared from the Mac, and documented its exact versions in docs/ops/docker-host.md. All four milestone checks passed, including the no-ROM continuous-integration run on GitHub.

## Decisions that still bind
- The firewall milestone clause is checked by `bash tools/firewall_control.sh` (exit 0) — environment fact, already a Tooling-inventory row in docs/ops/decomp-environment.md:81.
- Builds run only in the amd64 container via `tools/docker/dc.sh` on volume `dc2-work`, base `ubuntu:24.04` pinned by digest — environment fact, card Build/Pins lines and docs/ops/docker-host.md.
- Next task needs: after the developer pushes T1/T2, confirm the no-rom CI run on the new head is green; run `dc.sh sync` before any `dc.sh run` after host edits.
