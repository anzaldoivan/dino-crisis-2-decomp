MILESTONE: green

## Recap
Phase 1.7 built the toolkit for the moment a hand-written C function compiles to almost, but not exactly, the game's
machine code. The compiler (gcc 2.95.2, Sony's PlayStation build) turns C into code through a fixed series of passes;
for each of its five pass groups the phase found at least one "lever", a small change in how the C is written that
moves the output in a known way, and proved it by making a real game function match byte for byte (6 levers, 5 of 5
groups). It also staged the compiler's own source for reference, wired in a "permuter" (a program that tries many
small rewrites of a draft automatically and scores how close each gets), added a classifier that names why a stuck
draft differs, and generated a symptom index over the technique notebook (the cookbook) so a symptom leads to a lever.
The whole game still rebuilds to 83 of 83 byte-identical binaries.

## Milestone
`PY tools/phaseend_index.py verify --verbose` from a fresh `dc.sh sync`, after this phase end's commits: see
`Verified:` below; numbers per clause are in the verify log named there.

## Decisions that still bind
- norm: plateau.py evidence lines (mnemonic names and counts at diff indices, no operands/encodings) are within G12 → rule G73
- norm: a lever's byte proof outranks any source cite of vanilla 2.95.2 (Sony-patched cc1, DK-52) → existing G67; docs/ops/decomp-environment.md:626
- env: dumps come from the pinned CC1PSX.EXE with `-da` under wibo, no fallback cc1 → docs/ops/decomp-environment.md:610-613
- env: sched/sched2 in the pinned cc1 are gcc/sched.c, not haifa-sched.c; cite sched.c → docs/ops/decomp-environment.md:619-621 (added here), C0048, C0051
- contract: draft symbol `func_<START>`, drafts in tools/probes/levers/ without `-Iinclude` → tools/probes/levers/README.md:6, `codegen_map.py --check`
- contract: lever classification order compile-error · identical-drafts · not-matching · before-matches · proven → docs/ops/decomp-environment.md:634-636, `codegen_map.py --selftest`
- contract: docs/codegen-map.md and docs/cookbook-symptoms.md are generated on the host (`--write`), then `dc.sh sync`; `--check` fails stale → docs/ops/decomp-environment.md:638, :680-685
- contract: C0002 tell → pass group mapping lives in config/inherited_tells.tsv → docs/ops/decomp-environment.md:641
- contract: permuter scorer (masked Levenshtein), determinism (threads 1, random.seed, PYTHONHASHSEED=0), target.o from split asm → docs/ops/decomp-environment.md:648-653, C0052, C0053
- norm: a score-0 permuter candidate is a waypoint, banked only through reconcile plus a clean fleet check → existing G61; docs/ops/decomp-environment.md:588
- contract: plateau label order and print-time buckets → docs/ops/decomp-environment.md:666-672, `plateau.py --check`
- scope: Next task needs: the codegen_map scanner now runs plain (5 of 5 groups); a new lever row or cookbook entry reruns `codegen_map.py --write` / `cookbook_index.py --write` before `--check`
- scope: Next task needs: card headroom is 7 chars (6993 of 7000); any new card line is paid for by trimming
- scope: Next task needs: matches found while proving levers are standalone, not banked; banking rate is phase 1.8

## Gotchas routed
- generalizable: T3:36 → C0045, T3:38 → C0046, T4:43 → C0048, T4:45 → C0047, T5:46 → C0050, T5:48 → C0049 (captured
  in-phase); T4:49 → C0051, T6:31 → C0052, T6:32 → C0053, T7:35 → C0054 (promoted here)
- workflow: T1:28, T2:28, T3:40, T4:47, T5:50, T6:33 → skill `dc2-volume-runs` (one card line; Skills lines trimmed to pay)
- binding: as above; nothing appended to the card
- harness (for the auditor, not promoted): T1:27 and T5:52 `dc.sh run` has no stdin and `--help` does not say so;
  T3:41 Bash `cat` of tools/probes/levers/ drafts hook-denied; T9:27-28 card headroom, `wc -c` counts bytes not chars;
  phase end: reading a SKILL.md just written by skill_add.py via Bash `head` is hook-denied as tool-source reading

## H7 check
Every summary whose `Files:` names tools, config pins or build files (T1-T8) also lists docs/ops/ (T6 also
docs/ops/docker-host.md); T9 edits HOW_WE_WORK.md and docs/ops/. No miss.

## Deviations
- phase end made two small doc edits beyond RECAP: one sub-line in docs/ops/decomp-environment.md (sched.c binding had
  no docs/ops home) and the card's Skills lines trimmed so the new skill line fits the cap
