# Phase 1.6 multipliers: definitions and mechanics
task: extract definitions of the 7 multiplier subjects + rules
agent: retriever-digest
tags: phase-1.6, multipliers, propagation, twins, reconcile, carve, draw-filter, types

## Answer
See Findings. Sources are docs/decomp-architect.md section 6 (lines 208-240), docs/decomp-kernels.md DK-5/7/10/11/65, rules G38 G41 G43 G44 G46 G49 G50 G54 G62.

## Findings
- Propagation (DK-5): a body authored once, instantiated at every family member (shared header), byte-gated per member, registered fail-closed. Gate verifies one binary but writes N, so the clean fleet run follows every propagating gate.
- Families (DK-11): structural fingerprint (skeleton with relocations and immediates masked). Templates, not free dedup: crack one exemplar, remap siblings, gate each. Ceiling is TU type collisions.
- Twins band (DK-10): exact tier plus near tier (relocation-normalised, length and opcode-histogram prefilter, edit distance). Must reproduce every exact pair; control vs random pairs (base rate 1.17%); rank by work; filter lookalikes (ratio >= ~0.3). G46 rescan after every bank; G44 scan whole world, banked twin beats open one as seed.
- Reconcile ladder (DK-7, G50): declaration sync to file consensus, callee casts at use site, canonical-signature layer, self-declaration normalisation, carve chain, real-unit probe. Failures triaged body vs plumbing; plumbing banks without redraft. Gate number is not close rate until recovery has run.
- Carves (G43): split config and per-binary make fragments are carve state; gate commit carries only its binary's lines; after any blanket restore run interleave and pad audits on every touched binary.
- Draw filter (G38, G49): audited before every draw; refuses unbankable work (wrong opt level, uncarveable table, library region); stale exclude lists refused (88 of 107 stale next day). G49 target validity: real boundary, asm on disk, in range, not already banked; an empty tier ends the chain.
- Canonical types (G62 bank-time clause, DK-65): canonical type file from first bank, one proven field at a time; refuse draft with duplicate definition of an existing shape or a raw address cast; width/signedness proven by bytes. DK-65 correction: struct spelling is NOT byte-neutral in gcc 2.7.2 (90.6% neutral on 165 bodies); introduce under byte gate per access, never blanket rewrite.
- Known-true controls (BFM calibration): ~1232 struct defs + 143 raw casts at 100% (type check); exact hash 22/352 vs band 75/352 (twins); 299-insn head -> 4485 insns / 15 siblings (propagation); 5/8 at gate -> 8/8 after recovery (reconcile). Repo-local controls not found in scope.
- Other rules: G41 key scratch/ledgers by binary+address, agent never find/rm outside own dir (tidy-up swept 11 deliverables). G54 port banked sibling's spelling before dials; similarity score not shape oracle. G44 card names only levers in KB.
- Related tools (docs/tools-manifest.md:129-158): sync_tu_decls, decl_prior, conform_decls, lift_types, canon_sig_reconcile.
- Cookbook: C0036 propagate a label through duplicate classes only with purity rule (lib-pure: >=1 lib member, 0 non-lib). C0003 deterministic recovery separate from search; stage mutating shared state undoes by snapshot-restore. C0005/C0006 irrelevant to 1.6 mechanics.
- Failure modes: stale exclude lists; gating verdict list instead of directory (G50 coverage assert banked+failed+no-verdict==drafts); two open-cluster twins drafted same wave; lookalike twins; single-tier verdict ("families SPENT") wrongly generalised; broken probe 0% vs 89%.
- DK-8: scanners assert coverage; known-true case checked first.

## Dead ends
- No TODO(phase-6) markers anywhere in docs tools src include Makefile config; "Phase 6" appears only as source-project phase numbering (kernels, tools-manifest). PROJECT_CONTEXT.md:79 "banking layer (1.6) precedes the waves". Mapping: source Phase 6 = our 1.6.
- No repo-local twin/propagation/draw-filter tool definitions read; no dry-run spec found beyond "fail-closed registry".

sources: docs/decomp-architect.md:208-240; docs/decomp-kernels.md:68-163,803-832; rules/G*.md; cookbook/C0036.md; PROJECT_CONTEXT.md:79
