# PhaseEnd — Phase 1.1: Deterministic extraction with a committed manifest (implements Gen 1.1)
Approved: 2026-09-30 | Closed: 2026-09-30 | Planner: claude-opus-5-5/medium | Tasks: 3 done, 0 superseded

## Milestone
Milestone: `make extract` extracts the whole disc (Track 1 mode 2, every ISO9660 entry incl. XA/STR) from the cue/bin; two runs (host, then container) give the same `extracted/retail/manifest.sha1`; extracted payloads 0 differences against the reference extractor (dumpsxiso, pinned); `config/medium.sha1` and `extracted/retail/manifest.jsonl` are `required:` in `config/firewall.txt` and `PY tools/audit_public.py` exits 0; nothing ROM-derived staged — verified by: `make extract OUT=.run/x1 && make extract OUT=.run/x2 && cmp .run/x1/manifest.sha1 .run/x2/manifest.sha1 && cmp .run/x1/manifest.sha1 extracted/retail/manifest.sha1`; `bash tools/docker/dc.sh run sh -c 'diff -rq /work/.run/ref/files /work/.run/ours'` → exit 0 (after T2's container extraction + `sha1sum` of the container manifest.sha1 equal to the host's); `grep -c '^required:' config/firewall.txt` ≥ 2 naming both files; `PY tools/audit_public.py` exit 0; `bash tools/firewall_control.sh` exit 0; `git ls-files extracted/` = exactly `manifest.jsonl`, `manifest.sha1`
Verified: `PY tools/audit_public.py && bash tools/firewall_control.sh && grep -E '^required:' config/firewall.txt && git ls-files extracted/ disks/` → exit 0, both required paths, only the two manifests listed (.run/logs/t3-verify.log)

## Tasks
- T1 — disc volume + pinned dumpsxiso reference | Done: disc lives in local volume `dc2-disc` (`dc.sh disc <dir>`, ro at `/disc` in `dc.sh run`); image builds dumpsxiso 2.30 pinned by sha; reference extraction at `/work/.run/ref/{files,layout.xml}`. | commit: - | tasks/T1.md | logs/T1.md
- T2 — the extractor and the manifest | Done: own stdlib extractor `tools/extract_disc.py` (`make extract [OUT=…]`) reproduces dumpsxiso 2.30's file tree byte-for-byte; deterministic manifest; medium verified against `config/medium.sha1`. | commit: - | tasks/T2.md | logs/T2.md

## Decisions that still bind
- T1 — dumpsxiso 2.30 (pinned sha) is the reference extractor; the reference command lives in docs/ops/docker-host.md
- T1 — `dc.sh sync` never wipes `/work/.run` (container scratch, holds the reference)
- T2 — the reference is normalised: `license_data.dat` and `ZNULL.WAV` move from `/work/.run/ref/files/` to `/work/.run/ref/` (not ISO files)
- T2 — CD-DA ISO entries (ZNULL.DAT, extent in Track 2) are not extracted and not manifest records; printed as `da-files`
- T3 — `allow:` is exact-path only and never exempts the hash, 50 MiB or disasm-run checks
- dumpsxiso 2.30 (pinned sha) is the reference extractor → environment fact, in `docs/ops/docker-host.md:22-38`.
- `dc.sh sync` never wipes `/work/.run` → environment fact, in `docs/ops/docker-host.md:30-32`.
- Reference normalised (`license_data.dat`, `ZNULL.WAV` moved out of `ref/files`) → contract, `docs/formats.md:31-32` + `docs/ops/decomp-environment.md:30`.
- CD-DA entries not extracted, printed as `da-files` → contract, `docs/formats.md:31`, checked by the T2 diff.
- `allow:` is exact-path only, never exempts hash/50 MiB/disasm checks → contract, `config/firewall.txt:11`, tested by `tools/firewall_control.sh`.
- Next task needs: the Milestone header line still reads `/work/.run/ours`; `phaseend_index.py verify` honours only the header, not Changes corrections (planner/harness: fold critic path fixes into the header or teach verify the Changes line).

## Rules proposed
- (none)

## Cookbook entries added
C0007 | dumpsxiso writes XA/STR at 2336 B/sector; compare by sectors, not ISO size | psx,iso9660,xa,str,dumpsxiso,extract | 2026-09-30 | 1.1/T1 | T1 gotcha
C0008 | dumpsxiso -x dir holds license_data.dat and DA .WAV; move out before diffing | psx,dumpsxiso,extract,diff,reference | 2026-09-30 | 1.1/T2 | T2 gotcha
C0009 | PSX ISO da entries point into the CD-DA track; skip when extent >= Track 1 sectors | psx,iso9660,cdda,extract | 2026-09-30 | 1.1/T2 | T2 gotcha

## Research
R1.1-001 | compare reference extractors for PS1 MODE2/2352 BIN/CUE | PSX BIN/CUE extractors and Form 2 handling | psx, dumpsxiso, jpsxdec, xa, form2, manifest | retriever-web | 2026-09-30 | 32 lines

## Audit
- seed: median 14k, max 15k, n=6 (phase 1.1)
- previous phase 1.0: median 14k, growth -0.6%
- CLAUDE.md: 1202 bytes (unchanged)
- .claude/skills/docker-vm-no-privileged/SKILL.md: 702 bytes (unchanged)
- .claude/skills/project-architect/SKILL.md: 13448 bytes (unchanged)
- .claude-state/memory/MEMORY.md: 214 bytes (unchanged)
- HOW_WE_WORK.md: 6142 bytes
- cookbook/INDEX.md: 1833 bytes
- rules/INDEX.md: 10057 bytes
### Carry audit — phase 1.1 (2026-10-01T01:09:17Z → open UTC, 3 sessions, 143 requests)
- beside 1.1: session 0a7a3870 $0.53 (14 turns), not attributed

| file (read) | chars | n |
|---|---|---|
| PHASE_PLAN.md | 16.8k | 1 |
| decomp-environment.md | 9.9k | 2 |
| T2.md | 2.9k | 1 |
| dc.sh | 2.6k | 1 |
| docker-host.md | 2.6k | 1 |
| Dockerfile | 1.8k | 1 |
| file (write) | chars | n |
|---|---|---|
| extract_disc.py | 7.3k | 1 |
| generate-an-small-hands-velvet-wind.md | 5.8k | 2 |
| RECAP.md | 5.6k | 2 |
| T1.md | 2.9k | 1 |
| T2.md | 2.8k | 1 |
| T1.md | 2.8k | 1 |
| T2.c1.md | 2.7k | 1 |
| formats.md | 2.7k | 1 |
| result kind | chars | n |
|---|---|---|
| tools/card.py | 69.6k | 6 |
| bash other | 51.8k | 34 |
| Read | 36.6k | 7 |
| tools/audit_public.py | 24.3k | 9 |
| run.sh | 19.9k | 18 |
| tools/plan_edit.py | 12.2k | 11 |
| Agent | 10.9k | 10 |
| tools/launch.py | 8.5k | 1 |
| tools/status.py | 7.5k | 6 |
| Grep | 4.4k | 2 |
- whole reads over 20.0k: 0
- seed floor: retriever-code 5,547 (n 1, prev —) · retriever-web 3,909 (n 1, prev —)
- outline credit: 0 outlines · 0 followed by a ranged read · 0 chars credited
- warm pings: 1 pings over 1 runs, 1 warmed waits over the TTL, rewrites across warmed waits 0, waits past the cap 0, pings cost $0.0152 vs rewrites replaced $0.1777, cheaper than one rewrite: yes
- toasts by cause (waiting): question 0, review 0, replan 0, permission 0, input 0, discussion 0, crash 0, idle 0, stop 0, subagent-stop 0, model 0, other 0
- router turns by cause: loop 6, relay 0, re-arm 0, other 2, relay cost $0.0000
- retriever re-asks: 0 of 0 retriever briefs repeat a lookup of the same run
- router: pa-session 6dd5ff3c · requests 20 · ctx at end 47.1k · growth T1 +506, T2 +547, T3 +477, T3 +830 · top: Agent 6.6k/6, tools/status.py 5.9k/4, tools/plan_edit.py 1.8k/5, other 218/1
- router: pa-session a2d6fe63 · requests 7 · ctx at end 0 · growth (none) · top: tools/launch.py 8.5k/1, tools/status.py 1.5k/2, bash other 1.2k/1, Agent 1.1k/1, other 302/1
- noise: 12 lines 843 chars — usage: : 6 lines/450, No such file or directory: 5 lines/302, warning: in the working copy of: 1 lines/91
- price: read $0.20/Mtok · 1h write $7.11/Mtok · output $17.79/Mtok (phase model mix)
- median requests after a read: 2
- carry/request: 1806 chars, 143 requests (prev 2259 chars, 97 requests, growth -20.0%)
- flag: 2 tool-source reads by expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/docker/dc.sh  2.6k chars  expert-opus55
  /Users/ThinkPad/orca/workspaces/dino-crisis-2-decomp/bootstrap/tools/docker/Dockerfile  1.8k chars  expert-opus55

## Agent runs
- T1 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 43k | $0.373 | saved - | completed | parent 6dd5ff3c
- T1 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 36k | $0.255 | saved $0.40 | completed | parent a412568b
- T1 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 31k | $0.187 | saved - | interrupted | parent a2d6fe63
- T2 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 39k | $0.214 | saved - | completed | parent 6dd5ff3c
- T2 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 44k | $0.437 | saved $0.01 | completed | parent ac99fdfe
- T3 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 32k | $0.160 | saved - | completed | parent 6dd5ff3c
- T3 | coder-opus55 | coder | claude-opus-5-5 | medium | ctx 33k | $0.224 | saved - | completed | parent ab3ea00a
- T3 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 29k | $0.174 | saved - | completed | parent 6dd5ff3c
- T3 | critic | critic | claude-opus-5-5 | medium | ctx 24k | $0.161 | saved - | completed | parent 6dd5ff3c
- T3 | expert-opus55 | expert | claude-opus-5-5 | medium | ctx 31k | $0.196 | saved - | completed | parent 6dd5ff3c

## Discussions
- (none)

## Deferred
- from T3: milestone run by the closing expert; `config/check.*.sha` still pending (phase 3) (unconsumed)

## Changes
- 2026-09-30 router: T1 next -> done
- 2026-09-30 router: T2 next -> done
- 2026-09-30 router: T3 next -> done
- 2026-09-30 critic: critic: milestone clause 2 reads `diff -rq /work/.run/ref/files /work/.run/ours/files`. The header's `/work/.run/ours` contradicts Interfaces (`<out>/files/<path>`, manifest.* beside files/) and T2's done-when and verify. The claim is unchanged: extracted payloads, 0 differences. Reference normalised per T2 binding (license_data.dat = system area, ZNULL.WAV = Track 2 audio, neither a Track 1 ISO file; DA entry ZNULL.DAT not extracted; scope Track 1 per Rationale:24). The recap states both. Closing expert reruns clause 2 as corrected and records the result.

## Plain-English Recap
This phase built our own tool that unpacks the Dino Crisis 2 disc image into its individual files, including the video and audio streams stored in a special CD format (XA/STR). It exists so that later phases have a reproducible, checked starting point without ever putting game data into the public repository. Our extractor produces exactly the same 678 files as a well-known reference tool (dumpsxiso), gives the same fingerprint list on the Mac and in the build container, and only the fingerprint lists are committed; the public-repo audit now requires those fingerprints and passes. One milestone check originally pointed at the extractor's output root rather than its extracted-files folder; the plan reviewer corrected the path to `/work/.run/ours/files`, and the corrected check passes. Two files the reference tool writes are not files of the disc's data track, so they were moved out of the reference folder before comparing: `license_data.dat` (the disc's system area) and `ZNULL.WAV` (the audio track behind the `ZNULL.DAT` entry).
