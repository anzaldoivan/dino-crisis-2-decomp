# The wave playbook — the operating procedure for the matching campaign

> ONE document is the procedure. Read it before every run. Keep it current in the same change as any tool it names.
> A superseded procedure gets a banner.
>
> The table pairs each guard below with the MEASUREMENT that earned it. Fill the measurement column from your own
> waves. The source project's measurements are in the kit's methodology (the calibration fences).
> Created by decomp-architect on 2026-09-30.

This game keeps the numbers of each wave in `config/waves.tsv`, one row for each wave. The last column of the table
names the column that holds each measurement (2026-10-12). That ledger is in the development repository. A release
does not publish it.

| # | Step | The guard | The measurement that earned it |
|---|---|---|---|
| 0 | Preconditions | The tree is clean. The last clean fleet run is green. | Not recorded (2026-10-12). |
| 1 | Draw | Exclude what cannot bank. Audit the exclude list first (it records what the TOOLING could not do). Rank by open templatable instructions. Know face mass vs delivered mass. | `config/waves.tsv`, column `pool`. |
| 2 | Cards | The banked twin found over the WHOLE world goes on the card. A card names only levers the base contains. | Not recorded (2026-10-12). |
| 3 | Packs | Make one for each target + the shared laws file, at the paths the drafting prompt reads. | Not recorded (2026-10-12). |
| 4 | Validate | Never hand-type a target. Assert that every target is still open at draw time. Send near-misses to the permuter first. | Not recorded (2026-10-12). |
| 5 | Draft | Use one agent for each target. Choose the model by measured size. Use streaming, not batched drafts. The wave's difficulty is a draw-time knob. | `config/waves.tsv`, columns `drafted`, `banked` and `failed`. |
| 6 | Gate | Gate everything in parallel (worktrees, and the executable by a clean rebuild). Gate the DIRECTORY, never the verdict list. Assert banked + failed + no-verdict == drafts. | `config/waves.tsv`, column `fleet`. |
| 6b | Reconcile | Run one deterministic pass over each gate group's slate BEFORE the rebuild. | Not recorded (2026-10-12). |
| 7 | After ANY bank | Run the twin rescan, then commit. | `config/waves.tsv`, column `propagated`. |
| 8 | Harvest — a hard gate | Put every idiom into the cookbook + index. Put every mechanical idiom into a sweep that banks the free functions. Do the widening review. | `config/waves.tsv`, column `harvest`. |
| 9 | Verify the fleet | Do a clean rebuild of everything. Read the exit code. | The gate is `tools/fleet_check.sh`. Green is `83 of 83 byte-identical` (`HOW_WE_WORK.md`). |
| 10 | Hand off | The task summary carries the exact state and the next step. At a context handoff, `TASK_PROGRESS.md` does. Stale is worse than absent. | Not recorded (2026-10-12). |

Then **recover** before re-drawing. Triage the gate's failures into body and plumbing. Bank the plumbing rejects
through the reconcile ladder without a redraft. The gate number is not the close rate until recovery has run.
