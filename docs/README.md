# `docs/` — what goes where

*Installed by decomp-architect (install.py S3) on 2026-09-30 for dino-crisis-2-decomp. This folder holds the records and
references that the project's wiki summarises and links. Knowledge has a home by KIND, not by the session that produced it.
A note written for one session is scratch (`.run/`). The durable result that the note produced goes into one of the files
below. Then the note goes to the archive.*

| Kind of knowledge | Where it goes | The rule behind it |
|---|---|---|
| A compiler idiom proven on the bytes (the residual, the mechanism, the lever, the byte proof) | one `cookbook/C<nnnn>.md` entry written by `tools/cookbook_add.sh`, tagged by symptom in `cookbook/INDEX.md` (grepped, never read whole) | the flywheel: consult before a match, feed back after |
| A norm of conduct the project adopts (a "never X" / "always Y when Z") | one `rules/<id>.md` written by `tools/rules_add.py`, proposed as a `Rule candidate:` and adjudicated by the phase planner | rules accumulate. Never add a rule unilaterally |
| A strategic pivot: what was believed, what failed, the measurement, the hindsight | an entry in `docs/decision-log.md`, written while fresh | capture the why while it hurts |
| A late discovery that would have sped up an earlier phase. The entry gives what it is, when it was found, when it *could* have been found, and what it would have saved | `docs/accelerators.md` | the ledger a future project starts from |
| A procedure people run (the wave, the publication) | a runbook under `docs/`. A superseded runbook carries a banner at the top and then goes to the archive | one runbook is the procedure |
| An environment or tool fact (a version, a flag, a hook, a row per tool) | a topic under `docs/ops/` (the decomp facts: `docs/ops/decomp-environment.md`), in the same change as the tool. A standing decision goes to `HOW_WE_WORK.md` only through PA3's routing | keep the ops reference current |
| An address, with its source and its verification status | `docs/memory-map.md` | address provenance and region tags |
| A file or container format | `docs/formats.md` (the medium's layout). Track 1 is ISO9660 mode 2 and Track 2 is CD-DA. The boot exe is SLUS_012.79. The 105 presumed code overlays are .BIN under /BIN and /PSX/BIN. The data is in .DAT/.DBS/.TEX/.PXL archives, and there are XA audio and STR movies | — |
| The state of the open phase, then the phase's synthesis | `phase-ends/current/` holds the plan (only `tools/plan_edit.py` writes it), the task summaries and logs, and a `TASK_PROGRESS.md` at a hand-off. PA3's scripts then assemble `phase-ends/PhaseEnd_Phase<N>.md` | the hand-off is written to be replayed (G59) |
| How the project is used: building, verifying, contributing, its layout and conventions | the wiki (`docs/wiki/`), the source of truth for documentation | — |
| A design note, a frontier analysis, a triage ladder, a worklist for one session | `.run/<session>/` while live. Once its result is in the record above, the note moves to the archive. It is never a reference | see the archive |

**Not created here (2026-10-12).** This repository has no `docs/wiki/`, no `docs/decision-log.md`, no
`docs/accelerators.md` and no `docs/sunset/`. A row or a rule below that names one says where that knowledge goes when
the file exists. Until then, link the `docs/` file itself. The archive in use is `docs/retired/`.

**Authored versus generated.** Numbers are generated, never typed. A tool produces every progress figure, badge, timeline
row and census in a published document from the tree. The tool asserts that the published copy is fresh. A generated file
says so on its first line, and edits go to the generator.

A number that has to appear in prose is a dated snapshot with the command that produced it. Snapshots of a state that no
longer exists are frozen, not regenerated.

**Placeholders.** A tracked document never pastes a literal double-brace placeholder token. It names the placeholder in
prose. The install-time placeholder audit cannot tell a quotation from an unfilled placeholder, and it must stay a true
signal.

**Links.** A document links the wiki page for a topic, not the `docs/` file behind it. A wiki page links into `docs/` only
through its Reference index. Nothing links into the archive (`docs/sunset/`), and its index names files as backticked paths
with the version they were archived at. A backticked path is a citation, not a link. A link checker classifies cited
paths as TRACKED or UNTRACKED by asking git, never the disk.

**The archive.** A document leaves `docs/` when its purpose is fulfilled and its information lives elsewhere. The move is a
`git mv` into `docs/sunset/` under the same relative path. It adds one archive index row (what it was, what came of it,
where it lives now) and a review row for the owner. Deletion is the owner's decision, never part of the move. Re-point
every referrer first (a `git grep` over the tree must return only the two index files).
