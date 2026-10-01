MILESTONE: red

## Verify
`PY tools/phaseend_index.py verify` → RED 5/6 (.run/logs/phaseend-verify.log).
- RED clause 2: `dc.sh run sh -c 'diff -rq /work/.run/ref/files /work/.run/ours'` exit 1 (.run/logs/verify2.log): `/work/.run/ours` is the extractor's OUT root (`files/`, `manifest.jsonl`, `manifest.sha1`), so the literal diff compares the ISO tree against the OUT root.
- Intended comparison is green (.run/logs/pe-diff-files.log): `diff -rq /work/.run/ref/files /work/.run/ours/files` exit 0; container `manifest.sha1` sha1 `874e695a…` = host `extracted/retail/manifest.sha1`. Same command T2 verified (`tasks/T2.md:30`) and `docs/ops/decomp-environment.md:36` documents.
- Not redefined here (§5): the clause text needs a critic-approved plan edit, path `/work/.run/ours` → `/work/.run/ours/files`; then re-run phase-end.
- Clauses 1, 3–6 GREEN.

## Recap
This phase built our own tool that unpacks the Dino Crisis 2 disc image into its individual files, including the video and audio streams stored in a special CD format. It exists so that later phases have a reproducible, checked starting point without ever putting game data into the public repository. Our extractor produces exactly the same files as a well-known reference tool (dumpsxiso), gives the same fingerprint list on the Mac and in the build container, and only the fingerprint lists are committed. The public-repo audit now requires those fingerprints and passes. The milestone is red only because one check in the plan points at the wrong folder; the same check aimed at the extracted-files folder passes.

## Decisions that still bind
- dumpsxiso 2.30 (pinned sha) is the reference extractor → environment fact, already in `docs/ops/docker-host.md:22-38`.
- `dc.sh sync` never wipes `/work/.run` → environment fact, already in `docs/ops/docker-host.md:30-32`.
- Reference normalised (`license_data.dat`, `ZNULL.WAV` moved out of `ref/files`) → contract, in `docs/formats.md:31-32` + `docs/ops/decomp-environment.md:30`.
- CD-DA entries not extracted, printed as `da-files` → contract, `docs/formats.md:31`, checked by the T2 diff.
- `allow:` is exact-path only, never exempts hash/50 MiB/disasm checks → contract, `config/firewall.txt:11`, tested by `tools/firewall_control.sh`.
- Next task needs: plan edit of milestone clause 2 to `/work/.run/ours/files` (critic), then phase-end re-run.

## Deviations
- H7: T1, T2, T3 all list `docs/ops/`; no miss.
- Gotcha promotion (3 `generalizable:`, 0 `workflow:`) deferred to the green phase-end re-run, to avoid duplicate cookbook entries.
