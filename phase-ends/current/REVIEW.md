# REVIEW — Phase 1.4, T4

What ran: probe.py full matrix 11 cc × 8 aspsx × G{0,4,8} × O{1,2} = 528 triples × 5 probes in dc2-build (amd64 container), 3:14 wall
Results at: .run/logs/t4-matrix.log (ladder :2651, class :2652); config/probes.tsv; phase-ends/current/logs/T4.c1.md
What the expert saw: 5 probes from exe, st2, st6, st9 all byte-identical masked under one output class of 12 triples: {gcc-2.95.2-psx, psyq4.6} × aspsx 2.56–2.86 × G0 × O2; per probe matches 32/48/16/36/16 of 528

Decisions needed:
- which cc row carries the pin (gcc-2.95.2-psx and psyq4.6 produce identical cc1 output on all 5) — Recommended: psyq4.6 (era-native, plan tie rule)
- which aspsx version in 2.56–2.86 (indistinguishable here; 2.67 %hi/%lo and 2.77/2.81 gp tells unobservable at G0) — Recommended: a2.86, revisit if a later function splits the range
- accept equivalence-class counting as the meaning of "exactly 1 triple" — Recommended: accept (it is the done-when's own definition)

Plan edits proposed:
- none; pin applied by T4 completion after approval (config/toolchains.tsv col 7 on psyq4.6 = `a2.86/G0/O2`)
