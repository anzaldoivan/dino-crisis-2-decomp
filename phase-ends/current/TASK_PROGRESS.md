# TASK_PROGRESS — T7 attempt 4

Task: T7 exe-headed exact-dup classes, first wave measures usage
Attempt: 4   Agent: expert-opus55   Ctx at handoff: n/a   Commit: 2ee5fff (base)

## Done so far
- Attempts 1-2 (logs/T7.progress1.md, progress2.md): wave F1 drawn (--kind family --weight 2500, 25 targets, 744 insns, guard 529.4k), drafted c2..c26, gate a388f37, recover 222a97e, closed 089a802: banked 19, failed 6, fleet 83/83@222a97e. usage.tsv F1 row e380c3a: 4 -> 10 pct, resets 2026-10-02T19:00Z, drafter_k 612.3 (manual).
- Attempt 3 (no progress file, stalled): spawned coder T7.c27 = propagation registration tool (propagate.py, uncommitted, mtime 16:08) + `--apply` for F1's exemplars. Its .run/logs: t7c27-register, t7c27-apply (exit 0), t7c27-apply2, t7c27-census-before/after, t7c27-dryrun (running 16:12+).
- census-before: `stubs: 1293 of 1331 (set exe)`, 3911 fns; census-after: `stubs: 942 of 1331 (set exe)`, 3912 fns (+1 fn: to check), `check: OK`.

## In flight
- c27 dry-run ended 16:54 green (`367 of 367 members gated`, `fail-closed control: ok`); no c27 activity after (idle 16:54-17:01) → judged dead, no log/commit.
- c28 (attempt 4, spawned ~17:02): verify c27's tree fail-closed (fleet, census --set exe, wave --check, scanners, audit), explain census 3911→3912, apply any missing F1 exemplar, commit as T7.c28, log logs/T7.c28.md. Its files are off-limits until it returns.
- c27 apply: 15 exemplars rc 0 (8001b530 8001ea54 800264ac 80026a50 8002b3e0 8002d78c 8002e18c 8002e920 800436c8 80047958 8005a34c 8005ecb4 800610a0 800661fc 80066b80); 800786b4 not registered (no kind=game copy).

## Hypotheses rejected
- (none yet)

## Current hypothesis
Uncommitted tree = c27's registration tool + applied propagation of F1's 19 exemplars to overlay copies (~351 stubs). Needs: commit by c27, or verification by a follow-up coder (clean rebuild, fleet 83/83, census --set exe), then wave F2.

## Next 5 steps
1. Wait for c27 to finish (log/commit) or for its dry-run to end with no further activity.
2. If c27 did not commit: brief c28 to verify fail-closed (fleet_check clean, census 3912 vs 3911 explained) and commit, else revert.
3. Wave F2: `wave.py draw --kind family --dry-run`, guard <= 1000k, draw, cards, drafters (cap 20 concurrent), gate, recover, fleet, harvest, close, propagate.
4. Repeat until `family classes:` 0 eligible.
5. Write tasks/T7.md (windows per wave, re-projection), verify, finish, commit.

## Gotchas
- Never `dc.sh sync` while drafters or c27 run (wipes asm/).
- Another project's container (tools/mmx6) shares the docker host: slower runs.

## State to carry verbatim
- F1 window: 6 pct for 612.3k drafter -> window >= 10.2M drafter tokens (8.7M-12.2M).

## Coder runs so far
- c1 wave.py --kind family (54aca6e); c2..c26 F1 drafters; c27 propagation tool (attempt 3, uncommitted at 16:30)

## Reports commissioned
- none (digest of T7 logs returned inline, no report)
