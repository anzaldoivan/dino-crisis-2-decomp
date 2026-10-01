# Phase 1.1 — Deterministic extraction with a committed manifest        (implements GENERATION_PLAN.md phase 1.1)
Milestone: `make extract` extracts the whole disc (Track 1 mode 2, every ISO9660 entry incl. XA/STR) from the cue/bin; two runs (host, then container) give the same `extracted/retail/manifest.sha1`; extracted payloads 0 differences against the reference extractor (dumpsxiso, pinned); `config/medium.sha1` and `extracted/retail/manifest.jsonl` are `required:` in `config/firewall.txt` and `PY tools/audit_public.py` exits 0; nothing ROM-derived staged — verified by: `make extract OUT=.run/x1 && make extract OUT=.run/x2 && cmp .run/x1/manifest.sha1 .run/x2/manifest.sha1 && cmp .run/x1/manifest.sha1 extracted/retail/manifest.sha1`; `bash tools/docker/dc.sh run sh -c 'diff -rq /work/.run/ref/files /work/.run/ours'` → exit 0 (after T2's container extraction + `sha1sum` of the container manifest.sha1 equal to the host's); `grep -c '^required:' config/firewall.txt` ≥ 2 naming both files; `PY tools/audit_public.py` exit 0; `bash tools/firewall_control.sh` exit 0; `git ls-files extracted/` = exactly `manifest.jsonl`, `manifest.sha1`
Approved: 2026-09-30   Planner: claude-opus-5-5/medium   Plan-hash: 5c8ec0a2bb08769175c5db46b3b5b067acf54782b421bee79cf539211adb3a7d

## Context
Phase 1.0 left: the ROM audit + negative control, the amd64 build host, no extractor. Facts from retrievers (2026-09-30):
- Medium (`PROJECT_CONTEXT.md:60-64`): Track 1 ISO9660 mode 2 (MODE2/2352 raw) + Track 2 CD-DA; `SYSTEM.CNF` → `SLUS_012.79` (591,872 B); 685 directory entries (105 `.BIN`, 415 `.DAT`, 89 `.DBS`, plus `.TEX .PXL .XAS .STR .TRG`). Whether 685 counts directories is unknown; the extractor prints files/dirs separately and the reference listing arbitrates.
- Dump: `/Users/Shared/GameInputs/dino-crisis-2/usa/` (cue + 2 tracks, 511 MB), machine-local; `docs/ops/decomp-environment.md:19-24` says it is copied once under `disks/` (ignored). Cue/bin file names are not documented: T1 records them.
- `tools/audit_public.py` (project tool, stdlib, `#!/usr/bin/env python3`, hand-parsed flags): hashes TRACKED files against hash sources named in `config/firewall.txt`; `required:` source missing or empty → exit 1; `pending:` missing → warning. `.jsonl` source = one JSON object per line with key `sha1` (optional `path`); else sha1sum lines. Zero-length files exempt.
- `config/firewall.txt:36-37`: `pending: extracted/retail/manifest.jsonl` and `pending: config/medium.sha1`, both `TODO(phase-1): promote to required:`. `config/medium.sha1` does not exist.
- CONFLICT: `firewall.txt` has `purge: extracted/` and `glob: extracted/**` (:18, :24, :30-31) while `.gitignore:17-21` re-includes `extracted/retail/manifest.jsonl` and `manifest.sha1`. Committing the manifest today FAILs the audit. T3 resolves it with a path-rule exemption that never exempts the hash check.
- Build host: `tools/docker/Dockerfile:6-10` installs `bchunk p7zip-full python3` (3.12) among others; no `cmake`, no `dumpsxiso`, no `isoinfo`. `tools/docker/dc.sh`: `build`, `sync` (wipes `/work`, tars `git ls-files -co --exclude-standard`, so ignored `disks/`/`extracted/` never reach the volume), `run` (`-v dc2-work:/work`); "No bind mounts, ever" (:7). The dump is not visible in the container today.
- `Makefile`: 13 lines, `format`/`format-check` only. `.gitignore` already promises `make extract`.
- `.github/workflows/no-rom.yml:38`: runs `python tools/audit_public.py` only; once the two sources are `required:` they must be tracked or CI goes red.
- Generated (never hand-edited): `extracted/`. Hand-edited: `tools/`, `config/`, `docs/`, `Makefile`.
- Sector format (R pending, see Research): raw sector = 12 sync + 4 header + 8 subheader (file, channel, submode, coding ×2) + data. Submode bit `0x20` = Form 2 (2324 data + 4 EDC); Form 1 = 2048 data + EDC/ECC. ISO9660 size of an XA file is usually `sectors*2048`, not its real payload.
- Operational (seed P1.0-1): run `dc.sh sync` before any `dc.sh run` after host edits.

## Rationale
- Reference first (T1), extractor second (T2): the Form 2 output convention is a choice; fixing it to the reference's evidenced convention before writing the extractor makes the payload comparison a byte comparison with no transform (rule 5: never redefine to make a failure pass).
- Reference = `dumpsxiso` (mkpsxiso), built from source at a pinned tag in the build host. Three lines: it extracts PSX discs incl. XA/STR Form 2 files and writes an LBA layout XML; the existing `p7zip-full`/`bchunk` read only the cooked 2048-byte view, wrong for Form 2; delta = an independent reader of every file incl. Form 2 plus an LBA listing to arbitrate the 685 count (G21). Fallback if it will not build at the pin: 7z for Form 1 payloads + LBA/size cross-check for Form 2, recorded as a deviation (critic decides).
- Full comparison, not sampling: diffing every file costs seconds; sampling would leave a denominator to argue about (G27).
- Extractor is stdlib Python ≥ 3.12, runs on the Mac host (`make extract`) and in the container; the second determinism run is in the container, which is stronger than two host runs (different Python, OS, filesystem). Host-first because the committed manifest is written on the host.
- Form 2 / mixed files are written in the reference's raw convention (expected 2336 B/sector: subheader + 2324 data + EDC, position-independent and what mkpsxiso rebuilds from; T1 confirms); Form 1 files are user data truncated to the ISO9660 size. CD-DA Track 2 is hashed in `config/medium.sha1`, not extracted (scope: Track 1).
- The extractor refuses a medium whose hashes differ from `config/medium.sha1` (G28, M10): contributors with a different dump fail loudly, not silently.
- Dump into the container through a second named volume `dc2-disc` mounted read-only at `/disc` by `dc.sh run`: no bind mount, and `sync` (which wipes `/work`) cannot delete it.
- Firewall carve-out as an exact-path `allow:` kind in `audit_public.py` that exempts only purge/glob path rules, never the hash, size or disasm checks; negative-controlled both ways (G25). Moving the manifest out of `extracted/` was rejected: the milestone, `.gitignore` and `firewall.txt` already name `extracted/retail/`.
- Three tasks, all `expert-opus55`/`coder: opus55`/`effort: medium`: preset has no hard rung, and every done-when is a byte or exit-code check.
- No separate gate script: the milestone is five existing commands; a wrapper would be a tool with one use.

## Interfaces
- `tools/audit_public.py:33` `REPO = pathlib.Path(__file__).resolve().parent.parent`
- `tools/audit_public.py:47` `read_config()` — per line strip `#`, `kind, _, value = ln.partition(":")`; kinds `purge glob required pending fixture`; unknown kind → `sys.exit`
- `tools/audit_public.py:75` `under_rules(...)` — purge prefix / glob (fnmatchcase) path test (T3 adds the `allow:` exemption here or at its caller)
- `tools/audit_public.py:85` `load_hash_source(path)` — `.jsonl`: `json` per line, key `sha1`, optional `path`; else 40-hex + name
- `tools/audit_public.py:102-124` `rom_hashes()` — required missing → exit (:114); zero hashes → exit (:119); pending missing → warning (:111-113); zero sources → exit (:122)
- `tools/audit_public.py:127` `tracked_files()`; `:132` `sha1_of`; `:157` `main(argv)`, `--paths a b …` (:158); offenders → exit 1 (:194)
- `config/firewall.txt:4-10` grammar header; `:18,:24,:30-31` extracted purge/glob; `:34` `fixture: config/firewall-fixture.sha1`; `:36-37` the two `pending:` lines
- `tools/firewall_control.sh:9-16` — planted fixture must FAIL with an `OFFENDER` line, tree must PASS
- `.gitignore:17-21` — `/extracted/*`, `!/extracted/retail/`, `/extracted/retail/*`, `!…/manifest.jsonl`, `!…/manifest.sha1`
- `tools/docker/dc.sh:2-7` header, `:20-36` case `build|sync|run`; `run` = `docker run --rm --platform linux/amd64 -v dc2-work:/work -w /work dc2-build <cmd>`
- `tools/docker/Dockerfile:6-10` apt list; base `ubuntu:24.04@sha256` pin
- `docs/ops/decomp-environment.md:19-24` dump; `:28-33` `TODO(phase-1)` extract/verify; `:83` tooling row `TODO(phase-1)`
- `Makefile` (13 lines, `format`, `format-check`, `.PHONY`)
- NEW `tools/extract_disc.py` — `main(argv) -> int`; `--cue PATH` (default `$DC2_CUE`, else the single `*.cue` under `disks/`), `--out DIR` (default `extracted/retail`), `--no-medium-check` absent by design. Writes `<out>/files/<ISO path without ;1>`, `<out>/manifest.jsonl`, `<out>/manifest.sha1`; prints `entries N (files F, dirs D), bytes B, form2-files X`.
- NEW record `manifest.jsonl` (one line per file, sorted by path, `json.dumps(..., sort_keys=True, separators=(",",":"))`): `{"path": str, "lba": int, "sectors": int, "iso_size": int, "size": int, "form": "1"|"2"|"mixed", "sha1": str}`; directories are NOT records (they carry no bytes) — their count is printed and recorded in `docs/formats.md`.
- NEW `manifest.sha1`: sha1sum lines `<sha1>  files/<path>` sorted by path, checkable with `sha1sum -c` from `<out>`.
- NEW `config/medium.sha1`: sha1sum lines for the cue and both track bins, by basename.

## Cookbook
- C0003 (offline tooling first: the extractor and the comparison are zero-token deterministic tools)
- No cookbook entry covers disc extraction yet; T2 adds one via `bash tools/cookbook_add.sh` if a generalizable gotcha appears.

## Research
- `phase-ends/current/research/pending/retriever-web-psx-bincue-extractors-form2.md` (PSX BIN/CUE extractors and Form 2 handling; dumpsxiso v2.30 flags `-x outdir -s layout.xml`; Form 2 detection; manifest conventions; several points marked unverified). T1's expert adopts it with `PY tools/research_add.py adopt` and cites the id.

## Developer decides
(none)

## Triage
- P1.0-1: T1 -- T1 is the first `dc.sh run` after host edits this phase; T1 adds the sync-before-run line to `docs/ops/docker-host.md`

## Tasks

- T1 | done   | expert-opus55 | title: disc volume + pinned dumpsxiso reference | coder: opus55 | effort: medium | files: tools/docker/Dockerfile, tools/docker/dc.sh, docs/ops/docker-host.md, docs/ops/decomp-environment.md | done-when: `dc.sh disc <dir>` loads the cue+bins into named volume `dc2-disc` (tar-pipe, no bind mount) and `dc.sh run` mounts it read-only at `/disc`; the image builds `dumpsxiso` from mkpsxiso source at a pinned tag + commit sha (plus `cmake` in apt); `dumpsxiso` extracts Track 1 to `/work/.run/ref/files` with layout XML `/work/.run/ref/layout.xml`; the expert records in `logs/T1.md`: cue/bin file names, the reference's file count and dir count (vs 685), its output size convention for Form 1 and Form 2 files (bytes/sector, with one Form 2 file's size ÷ sectors as evidence), and how it names paths (`;1` stripped, case) | verify: `bash tools/docker/dc.sh build && bash tools/docker/dc.sh sync && bash tools/docker/dc.sh run sh -c 'ls /disc && command -v dumpsxiso && test -s /work/.run/ref/layout.xml && find /work/.run/ref/files -type f -print -quit'` exit 0, printing a file path | reads: — | deps: — | est-ctx: 70k | review: no | wait-for: —
  Note: `sync` wipes `/work`, so the reference run is repeated after any sync before T2's diff; keep the exact reference command in `docs/ops/docker-host.md`. Never `--privileged` (skill docker-vm-no-privileged). Docs update decomp-environment `:28-33` (reference half) and the Dockerfile pin in docker-host.md (H7).
- T2 | done   | expert-opus55 | title: the extractor and the manifest | coder: opus55 | effort: medium | files: tools/extract_disc.py, Makefile, config/medium.sha1, docs/formats.md, docs/ops/decomp-environment.md, HOW_WE_WORK.md | done-when: `tools/extract_disc.py` (stdlib, py≥3.12) parses the cue, verifies the medium against `config/medium.sha1` (refuses on mismatch, exit 1, naming the file), reads raw 2352-byte sectors of Track 1, walks ISO9660 from the PVD (LBA 16) over every directory record (extents may span several sectors; skip `.`/`..`; strip `;1`), classifies each file's sectors by submode bit 0x20, writes Form 1 files as user data truncated to `iso_size` and Form 2/mixed files in the convention T1 recorded, writes `manifest.jsonl` + `manifest.sha1` per the Interfaces shapes with stable sort, refuses an empty walk (G28) and prints its denominators; `make extract [OUT=…]` runs it (default `extracted/retail`); two host runs to fresh dirs give byte-identical `manifest.sha1`; a container run (`dc.sh sync`, then `python3 tools/extract_disc.py --cue /disc/<cue> --out /work/.run/ours`) gives the same `manifest.sha1` sha1; `diff -rq /work/.run/ref/files /work/.run/ours/files` exit 0 (every file, both forms); printed file count = reference file count; `docs/formats.md` documents the medium layout (tracks, sector forms, entry counts by extension and directory, Form 2 convention, manifest record shape) with no game bytes; card Tools gets an `extract` row and decomp-environment `:28-33`, `:83` TODOs are filled | verify: `make extract OUT=.run/x1 && make extract OUT=.run/x2 && cmp .run/x1/manifest.sha1 .run/x2/manifest.sha1 && (cd .run/x1 && sha1sum -c --quiet manifest.sha1)` exit 0, then `bash tools/docker/dc.sh run sh -c 'diff -rq /work/.run/ref/files /work/.run/ours/files && sha1sum /work/.run/ours/manifest.sha1'` exit 0 with the host's sha1 | reads: T1 | deps: T1 | est-ctx: 90k | review: no | wait-for: —
  Note: macOS has `shasum`, not `sha1sum`; use `shasum -c` on the host or run the check through `PY`. The container image's `/disc` volume must already hold the medium (T1). `extracted/retail/manifest.*` is written but NOT committed in T2 (the audit would fail until T3). A count mismatch vs 685 is recorded with the reference's number, never forced.
- T3 | done   | expert-opus55 | title: promote the manifest and medium to required | coder: opus55 | effort: medium | files: tools/audit_public.py, config/firewall.txt, tools/firewall_control.sh, extracted/retail/manifest.jsonl, extracted/retail/manifest.sha1, config/medium.sha1, docs/ops/decomp-environment.md | done-when: `audit_public.py` accepts `allow: <exact path>` that exempts that path from purge/glob rules only (hash, 50 MiB and disasm-run checks still apply); `firewall.txt` adds `allow:` for the two manifest files and turns both `pending:` lines (:36-37) into `required:`; `firewall_control.sh` gains a step proving a planted `extracted/retail/planted.bin` path is still an OFFENDER via `--paths` (the allow is exact, not a prefix) and the existing fixture/tree steps still hold; `make extract` writes `extracted/retail/manifest.*`, which are committed together with `config/medium.sha1`; the audit loads 2 + fixture sources with a non-zero hash count; `git ls-files extracted/` lists exactly the two manifest files; no extracted payload, bin or cue is staged | verify: `PY tools/audit_public.py` exit 0 && `bash tools/firewall_control.sh` exit 0 && `grep -E '^required:' config/firewall.txt` shows both paths && `git ls-files extracted/ disks/` prints only `extracted/retail/manifest.jsonl` and `extracted/retail/manifest.sha1` | reads: T2 | deps: T2 | est-ctx: 60k | review: no | wait-for: —
  Note: commit `config/medium.sha1` and the manifest in the same commit as the `required:` flip, or CI (`no-rom.yml:38`) goes red on the developer's push. Rule G25: the new exemption is negative-controlled before it is trusted. The milestone is then run whole by the closing expert.

## Risks
- `dumpsxiso` will not build at the pin, or misreads this disc → T1 returns `blocked` with the build log; fallback (Rationale) goes to the critic. Detection: T1 verify.
- The reference's Form 2 convention differs from what any downstream (Gen 2 repacking) wants → the convention is recorded in `docs/formats.md`; changing it later only regenerates `extracted/` and the manifest. Detection: T1 log.
- Reference and extractor disagree on files with interleaved/mixed sectors or on ISO sizes of XA files → `diff -rq` names them; T2 bug-checks its own reader first (G26) before suspecting the reference.
- 685 is not the file count (dirs included, or associated files) → recorded with the reference's numbers; not a failure of the milestone, which is the reference diff.
- The `allow:` exemption is over-broad (prefix match) → the planted-path control in T3 fails. Detection: `firewall_control.sh`.
- CI red after push because a `required:` source is untracked → T3 commits all three in one commit; verify includes `git ls-files`.
- `dc.sh sync` wipes `/work/.run/ref` between T1 and T2 → T2 reruns the reference command recorded in docker-host.md.

## Changes
- 2026-09-30 router: T1 next -> done
- 2026-09-30 router: T2 next -> done
- 2026-09-30 router: T3 next -> done
