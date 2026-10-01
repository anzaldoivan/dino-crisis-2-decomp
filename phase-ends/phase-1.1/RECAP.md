MILESTONE: green

## Verify
- `PY tools/phaseend_index.py verify` → RED 5/6 (.run/logs/phaseend-verify2.log): it runs the Milestone header text literally; clause 2 header reads `/work/.run/ours` (the OUT root) and fails with Only-in `files`, `manifest.jsonl`, `manifest.sha1`. Clauses 1, 3–6 GREEN.
- Clause 2 as corrected by the critic (Changes 2026-09-30, commit a2a02ff): `dc.sh run sh -c 'diff -rq /work/.run/ref/files /work/.run/ours/files'` → exit 0, 678 files each side (.run/logs/pe2-diff-files.log). GREEN.
- Container `/work/.run/ours/manifest.sha1` sha1 `874e695a0f9d2cbcba61ea0146966f851232597d` = host `extracted/retail/manifest.sha1`. GREEN.
- Reference normalisation confirmed: `license_data.dat` and `ZNULL.WAV` sit at `/work/.run/ref/`, not under `ref/files/`.

## Recap
This phase built our own tool that unpacks the Dino Crisis 2 disc image into its individual files, including the video and audio streams stored in a special CD format (XA/STR). It exists so that later phases have a reproducible, checked starting point without ever putting game data into the public repository. Our extractor produces exactly the same 678 files as a well-known reference tool (dumpsxiso), gives the same fingerprint list on the Mac and in the build container, and only the fingerprint lists are committed; the public-repo audit now requires those fingerprints and passes. One milestone check originally pointed at the extractor's output root rather than its extracted-files folder; the plan reviewer corrected the path to `/work/.run/ours/files`, and the corrected check passes. Two files the reference tool writes are not files of the disc's data track, so they were moved out of the reference folder before comparing: `license_data.dat` (the disc's system area) and `ZNULL.WAV` (the audio track behind the `ZNULL.DAT` entry).

## Decisions that still bind
- dumpsxiso 2.30 (pinned sha) is the reference extractor → environment fact, in `docs/ops/docker-host.md:22-38`.
- `dc.sh sync` never wipes `/work/.run` → environment fact, in `docs/ops/docker-host.md:30-32`.
- Reference normalised (`license_data.dat`, `ZNULL.WAV` moved out of `ref/files`) → contract, `docs/formats.md:31-32` + `docs/ops/decomp-environment.md:30`.
- CD-DA entries not extracted, printed as `da-files` → contract, `docs/formats.md:31`, checked by the T2 diff.
- `allow:` is exact-path only, never exempts hash/50 MiB/disasm checks → contract, `config/firewall.txt:11`, tested by `tools/firewall_control.sh`.
- Next task needs: the Milestone header line still reads `/work/.run/ours`; `phaseend_index.py verify` honours only the header, not Changes corrections (planner/harness: fold critic path fixes into the header or teach verify the Changes line).

## Deviations
- H7: T1, T2, T3 all list `docs/ops/`; no miss.
- Milestone judged green on the critic-corrected clause 2, not on the tool's literal-header RED; the claim (0 payload differences) is unchanged.
- Promoted 3 `generalizable:` gotchas → cookbook C0007, C0008, C0009; 0 `workflow:`.
