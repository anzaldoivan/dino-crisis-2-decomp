---
name: dc2-volume-runs
description: One fleet-check owner per volume; re-sync before checks; own scratch dir; host-written docs; bg long gates
---

# Runs on the shared dc2-work volume

Captured 2026-10-02 from .run/phase-end/skill.md#Shared volume runs.

## When to use
parallel coders, fleet checks, generated docs, long container gates

## Steps

Every `dc.sh run` uses the one `dc2-work` volume; `dc.sh sync` rewrites it from the checkout.
- A coder that runs `fleet_check.sh` owns the volume: never run two volume users in parallel (1.7 T6).
- Parallel coders that only compile: re-sync (`dc.sh sync`) right before their own check (1.7 T3), and each uses its
  own scratch dir `.run/t<n>c<k>/`; one coder's `rm -rf` wiped another's files (1.7 T4).
- A generated doc that must be tracked is written on the host with PY, never inside `dc.sh run`: the container writes
  to the volume, not the checkout (1.7 T2).
- Parallel coders appending to shared registries (`config/levers.tsv`, `cookbook/INDEX.md`) get swept into each
  other's commits (`commit_task.sh` stages whole files): serialise the appends or record the sweep (1.7 T5).
- A scanner harness run or the milestone verify takes > 285 s: `run.sh --bg <name>` then `run.sh --wait <name>`,
  never one foreground call (1.7 T1, T9).
