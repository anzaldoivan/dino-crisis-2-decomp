# Audit — 2026-10-01, phase 1.3 (previous —)
## Verdicts
- tools/splat_gen.py (coder-opus55, 15.8k + 1.5k) | leave | the coder read the source it was editing (splat_gen.py written 4 times in the phase, commits T2.c1/T3.c1/T4.c1); the cost is the work itself.
- tools/docker/Dockerfile (coder-opus55, 1.8k) | leave | one 1.8k read, made while the coder was changing the build host and docker-host.md (written 4 times); one occurrence, nothing recurring to tool.
- tools/audit_public.py (expert-opus55, 2.2k) | script-fix | `audit_public.py --help` ignores the flag and runs the full audit (verified 2026-10-01), so the expert had no help text and read the source; packaged by the decomp overlay (a0de544), so a candidate: 0.55k tokens × $0.20/Mtok × 20 + write = $0.0063 per occurrence.
## Tool candidates
- Tool candidate: audit_public_help | does: give tools/audit_public.py an argparse `--help` (purpose, allow/purge config files, exit codes) instead of running the audit | replaces: expert reading the 2.2k source to learn its use | occurrences: 1 in 1.3 | saving: $0.0063 (0.55k tokens on vanilla-net-v4) | build: S
## Rule candidates
