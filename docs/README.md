# `docs/` — what goes where

*Installed by decomp-architect (install.py S3) on 2026-09-30 for dino-crisis-2-decomp. This folder holds the records and references the
project's wiki summarises and links; knowledge has a home by KIND, not by the session that produced it. A note written for
one session is scratch (`.run/`); the durable result it produced goes into one of the files below, and the note is archived.*

| Kind of knowledge | Where it goes | The rule behind it |
|---|---|---|
| A compiler idiom proven on the bytes (the residual, the mechanism, the lever, the byte proof) | one `cookbook/C<nnnn>.md` entry written by `tools/cookbook_add.sh`, tagged by symptom in `cookbook/INDEX.md` (grepped, never read whole) | the flywheel: consult before a match, feed back after |
| A norm of conduct the project adopts (a "never X" / "always Y when Z") | one `rules/<id>.md` written by `tools/rules_add.py`, proposed as a `Rule candidate:` and adjudicated by the phase planner | rules accumulate; never added unilaterally |
| A strategic pivot: what was believed, what failed, the measurement, the hindsight | an entry in `docs/decision-log.md`, written while fresh | capture the why while it hurts |
| A late discovery that would have sped up an earlier phase — what it is, when it was found, when it *could* have been found, what it would have saved | `docs/accelerators.md` | the ledger a future project starts from |
| A procedure people run (the wave, the publication) | a runbook under `docs/`; a superseded runbook carries a banner at the top and is then archived | one runbook is the procedure |
| An environment or tool fact (a version, a flag, a hook, a row per tool) | a topic under `docs/ops/` (the decomp facts: `docs/ops/decomp-environment.md`), in the same change as the tool; a standing decision goes to `HOW_WE_WORK.md` only through PA3's routing | keep the ops reference current |
| An address, with its source and its verification status | `docs/memory-map.md` | address provenance and region tags |
| A file or container format | `docs/formats.md` (the medium's layout: Track 1 ISO9660 mode 2 + Track 2 CD-DA; boot exe SLUS_012.79; 105 presumed code overlays (.BIN under /BIN and /PSX/BIN); data in .DAT/.DBS/.TEX/.PXL archives; XA audio and STR movies) | — |
| The state of the open phase; then the phase's synthesis | `phase-ends/current/` (the plan, written only by `tools/plan_edit.py`; the task summaries and logs; a `TASK_PROGRESS.md` at a hand-off) → `phase-ends/PhaseEnd_Phase<N>.md`, assembled by PA3's scripts | the hand-off is written to be replayed (G59) |
| How the project is used: building, verifying, contributing, its layout and conventions | the wiki (`docs/wiki/`), the source of truth for documentation | — |
| A design note, a frontier analysis, a triage ladder, a worklist for one session | `.run/<session>/` while live; once its result is in the record above, the note moves to the archive — it is never a reference | see the archive |

**Authored versus generated.** Numbers are generated, never typed: every progress figure, badge, timeline row and census
in a published document is produced by a tool from the tree, and the tool asserts the published copy is fresh; a
generated file says so on its first line; edits go to the generator. A number that has to appear in prose is a dated
snapshot with the command that produced it. Snapshots of a state that no longer exists are frozen, not regenerated.

**Placeholders.** A tracked document never pastes a literal double-brace placeholder token; it names the placeholder in prose.
The install-time placeholder audit cannot tell a quotation from an unfilled placeholder, and it must stay a true signal.

**Links.** A document links the wiki page for a topic, not the `docs/` file behind it; a wiki page links into `docs/` only
through its Reference index; nothing links into the archive (`docs/sunset/`), whose index names files as backticked paths
with the version they were archived at. A backticked path is a citation, not a link; a link checker classifies cited
paths as TRACKED or UNTRACKED by asking git, never the disk.

**The archive.** A document leaves `docs/` when its purpose is fulfilled and its information lives elsewhere: `git mv` into
`docs/sunset/` under the same relative path, one row in the archive index (what it was, what came of it, where it lives
now), and a review row for the owner; deletion is the owner's decision, never part of the move; every referrer is
re-pointed first (a `git grep` over the tree must return only the two index files).
