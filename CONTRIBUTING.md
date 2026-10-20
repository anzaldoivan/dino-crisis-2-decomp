# Contributing to dino-crisis-2-decomp

## Bring your own copy of the game

Every contributor extracts from their own legally obtained copy. Commit nothing derived from it. That means no
executable, payloads, disassembly, assets, build output, memory images, database, vendor SDK, or notes that paste the
target's instructions. The `.gitignore` firewall stops the ordinary case. `tools/audit_public.py` refuses the rest and
runs in CI on every push. Before every commit, review `git status`.

## What "done" means

A function is matched when **both** of these conditions are true:

1. Its compiled output is instruction-identical to the original, register allocation included.
2. The whole binary's hash check still passes with the function linked.

"Functionally equivalent" is never done. Unmatched C lives under the non-matching guard with the assembly stub in the
default build.

A match is verified from a clean rebuild, never an incremental one. A pull request states which compilation each claim
survived (standalone / the real translation unit / the whole binary).

## Names, types and formatting

Rename a symbol only on recorded evidence. Such evidence is a string the symbol prints, a cross-reference chain, a
debug menu, a live-memory datapoint, or a community label with provenance. **If you are not sure what something does,
leave it unnamed rather than name it wrongly.** The address-named placeholder is honest and greppable. Keep one
definition for each structure. A duplicate type is a defect.

Before you commit, run `make format` (`.clang-format` is the community's style). Mark anything that exists only to
force a match with `// !FAKE:` and its reason. Mark an original bug with `//! @bug`.

## AI use — conduct

The README's [AI policy](README.md#description) applies to every contribution. Every change is justifiable from recorded evidence. A person reviews each phase gate. Names come only from evidence. A person writes outward text.

Whoever submits a change answers for all of it. They understand it, have tested it and can defend it in review. Maintainers may decline what they cannot.

These rules keep that honest, for every contributor and every agent:

1. **The byte gate is the only claim of success.** Never report a match the gate has not proven. The claims "it
   compiled" and "looks equivalent" are not results. Write "Banked" from a tool's printed success line.
2. **Names and types are evidence-based, never guessed.** A model may propose a name. A person with recorded evidence
   asserts one. Hallucinated meaning is the specific way a model damages a decompilation without any test catching it.
3. **Outward text is written by a person.** Their author writes issues, pull requests, forum posts and outreach to other
   projects the way a developer writes. That way is short, plain and from the facts. The text is never a model draft
   with the tells removed. Before you contribute to another project, read and follow its own AI-contribution policy.
4. **No automated traffic against community infrastructure.** A person uses shared services in a browser. Replicate
   anything repetitive locally.
5. **Agents assist. A person owns.** Every change is justifiable by a person from the record (the phase logs, the
   decision log, the cookbook's byte proofs).

## Submitting

Submit small, per-function or per-family pull requests. The hash check must be green on every binary the change
touches. Anything that touches a shared body, a shared header or the executable needs the clean fleet verification.
The commit message must name what was matched and how it was verified. Add no AI trailer of any kind
(no sign-off, co-author or `Assisted-by:` line).

Pull requests from forks run CI only after the maintainer approves them. After every push, they need the approval
again. The build runs with the game files present, so approval follows a review of the whole diff, including the
Makefile, headers, `tools/` and `.github/`.
