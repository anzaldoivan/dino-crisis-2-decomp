# Phase 1.0 — Governance and the firewall        (implements GENERATION_PLAN.md phase 1.0)
Milestone: `PY tools/audit_public.py` exits 0 on the tree and 1 on the planted fixture (`config/firewall-fixture.sha1`); `grep -c '| decomp-architect$' rules/INDEX.md` = 67; `docker build --platform linux/amd64` of `tools/docker/Dockerfile` exits 0; `gh run list --workflow no-rom.yml -L 1` green after the developer's first push — verified by: `bash tools/firewall_control.sh` (exit 0 = tree PASS + planted FAIL); `grep -c '| decomp-architect$' rules/INDEX.md`; `docker build --platform linux/amd64 -t dc2-build -f tools/docker/Dockerfile tools/docker` exit 0; `gh run list --workflow no-rom.yml -L 1` conclusion `success`
Approved: 2026-09-30   Planner: claude-opus-5-5/medium   Plan-hash: 0cc7aa50de98da033c8417957083b47ffeb9a278e9186189eca7deecf92c7327

## Context
- Phase 1.0 = kit ladder Phase 0. No game data touched in this phase: nothing under `disks/`, `extracted/`, `/Users/Shared/GameInputs/` is read or written.
- State at planning (probed 2026-09-30, planner session, all diagnostic-grade until the task re-runs them):
  - `PY tools/audit_public.py` on the tree → `OK — 0 offenders among 220 paths`, rc 0; 3 `pending:` warnings (manifest, `config/medium.sha1`, `config/check.*.sha`) are expected until 1.1/1.3 promote them.
  - Planted control: `printf 'DECOMP-FIXTURE!!'` → sha1 `d4bc7b5d67878461ceac039b458b0f9db0e6adaa` = `config/firewall-fixture.sha1:1`; `audit_public.py --paths .run/firewall-control/planted.bin` → `OFFENDER … sha1 == config/firewall-fixture.sha1:blob.bin`, `FAIL — 1 offender(s)`, rc 1. Recipe source: kit `/Users/Shared/kits/decomp-architect/templates/firewall-fixture/README.md` (plant under `.run/firewall-control/planted.bin`, audit that path alone, assert FAIL naming it, remove, assert tree PASS). The blob is never tracked; only its sha1 is.
  - `grep -c '| decomp-architect$' rules/INDEX.md` → 67 (already green; closer re-checks).
  - CI: `gh run list --workflow no-rom.yml -L 1` → `completed success`, branch `anzaldoivan/bootstrap`, event `pull_request`, run 36783706738 (2026-09-30T22:07Z). `origin/main` = `f076478 Initial commit`. The developer has pushed once; the clause is green now and must still be green at close.
  - Docker: `/opt/homebrew/bin/docker`, Docker Desktop, server arch arm64, VM 8 GiB / 8 CPUs; amd64 images run under emulation. Existing volumes: one unrelated (`bfm-bench-…`, another project's; never touch it). No `dc2-*` image or volume yet.
- Files:
  - `tools/audit_public.py` (decomp-architect overlay, 200 lines): harness-owned; USE, never edit. CLI: no args = `git ls-files`; `--paths a b …` = explicit list. Forbidden set derived from `config/firewall.txt`.
  - `config/firewall.txt`: 17 purge rules, `fixture: config/firewall-fixture.sha1` (:34), 3 `pending:` (:35-37). Not edited in 1.0 (1.1 promotes).
  - `tools/docker/Dockerfile` (14 lines): `FROM ubuntu:24.04` (:4, floating tag), apt toolchain (:6-10), `WORKDIR /work` (:12), `TODO(phase-4)` (:14, keep). Project build host; hand-edited.
  - `.github/workflows/no-rom.yml`: job `audits` (:26), step `run: python tools/audit_public.py` (:38). Not edited in 1.0.
  - `Makefile`: `format-check` (:11) = clang-format dry run over `src/` (no game bytes) — the in-container smoke target.
  - `docs/ops/INDEX.md` (`<file> | <heading> | <line count>` lines), `docs/ops/decomp-environment.md` (build-host row in Version pins; Tooling inventory :78-82), `HOW_WE_WORK.md` (card, 5710 chars, cap 7000; Build :53, Pins :66, gotcha :67).
- Constraints: G12, G13, G16 (a probe/guard never writes into the repo it guards: scratch only under `.run/`, gitignored), G18 (never `git clean -x`). Named volume, never a bind mount, for any build (card :67). The repo is public: host facts recorded in docs are versions/digests/sizes only, no game bytes, no user paths beyond what the card already holds.

## Rationale
- Two tasks, split by subject: T1 firewall proof (tiny, expert-only), T2 build host (a coder loop with long emulated builds). The rules count and CI clause need no work; the closing expert checks them at the milestone. A third "verify" task would duplicate the closer.
- T1 keeps the negative control as a tracked script `tools/firewall_control.sh`, not a scratch check: G13 says the audit is trusted only after it fails on the fixture, and 1.1 (promotes manifest + medium hash) and 1.3 (per-binary contracts) edit `config/firewall.txt`; each must re-run the control. One ~20-line script beats re-deriving the recipe three times. It is a new project tool beside the harness (PROJECT_CONTEXT layout: `tools/` = PA3 tools + project tools); `tools/audit_public.py` itself is not edited.
- The control is NOT added to `no-rom.yml` this phase: the audit already refuses vacuous passes (missing fixture source = FAIL), so a CI copy adds little, and a workflow change would make the CI clause depend on a new developer push before close. Revisit when 1.1 touches the firewall.
- T2 pins the base image by digest (`FROM ubuntu:24.04@sha256:<digest>`): the tag floats, and M10 wants pinning at every boundary; the apt package versions still float, so T2 records them (`dpkg-query`) in `docs/ops/docker-host.md` as the evidence baseline for 1.4. Full apt pinning waits for the compiler pin (1.4) — premature now.
- T2 adds one wrapper `tools/docker/dc.sh` (`build`, `sync`, `run <cmd>`): every later phase (extraction 1.1, split 1.3, gates) runs in this container through the named volume; one entrypoint prevents each expert inventing its own sync. Sync recommendation: stream the working tree (`git ls-files -co --exclude-standard -z`) as a tar over stdin into `docker run -i -v dc2-work:/work` — no bind mount, carries uncommitted edits, never carries ignored data. The expert may pick another mechanism that meets the same constraints; note it under Deviations.
- Names: image `dc2-build`, volume `dc2-work` (short, project-prefixed, distinct from the other project's volume).
- Coder tier: opus55 (only tier). No `effort: high`: preset `pro` has no hard rung, and nothing here needs an unarbitrated judgment.

## Interfaces
- `tools/audit_public.py:6` — `tools/audit_public.py --paths a b …` explicit list; `:157` `main(argv)`; `:194` `return 1` on any offender; rc 0 prints `audit_public: OK — 0 offenders among <n> paths`; rc 1 prints `OFFENDER <path>: …` then `audit_public: FAIL — <k> offender(s) among <n> paths`; `sys.exit(<msg>)` (rc 1, no OFFENDER line) on missing/empty required source or zero purge rules (`:50,:71,:114,:119,:123`).
- `config/firewall-fixture.sha1:1` — `d4bc7b5d67878461ceac039b458b0f9db0e6adaa  blob.bin` (sha1sum format). Blob bytes: the 16 ASCII bytes `DECOMP-FIXTURE!!` (no newline).
- `config/firewall.txt:34` — `fixture:  config/firewall-fixture.sha1`.
- New `tools/firewall_control.sh` (T1): `bash tools/firewall_control.sh` → exit 0 iff (a) the planted blob's sha1 equals `config/firewall-fixture.sha1` field 1, (b) `audit_public.py --paths <planted>` exits 1 AND its output names `<planted>` on an `OFFENDER` line, (c) the planted copy is removed (trap), (d) `audit_public.py` on the tree exits 0. Prints one line per step with its result; exit 1 naming the failed step otherwise. Planted path `.run/firewall-control/planted.bin`; interpreter from `$PY` or `/opt/homebrew/opt/python@3.14/bin/python3.14` (card), falling back to `python3` for CI/container portability.
- `tools/docker/Dockerfile:4` — `FROM ubuntu:24.04` → `FROM ubuntu:24.04@sha256:<linux/amd64 index digest>` (T2); `:12` `WORKDIR /work` unchanged.
- New `tools/docker/dc.sh` (T2): `bash tools/docker/dc.sh build` (= `docker build --platform linux/amd64 -t dc2-build -f tools/docker/Dockerfile tools/docker`); `bash tools/docker/dc.sh sync` (create volume `dc2-work` if absent; replace `/work` contents with the tracked + untracked-unignored working tree); `bash tools/docker/dc.sh run <cmd…>` (`docker run --rm --platform linux/amd64 -v dc2-work:/work -w /work dc2-build <cmd…>`; exit code passed through). No bind mounts of the repo. Never touches other volumes.
- `Makefile:11` — `format-check` target (smoke target inside the container).
- `docs/ops/INDEX.md` line shape: `<file> | <heading> | <line count>`.

## Cookbook
- C0004 (a slow gate is a bug: parallel worktrees, per-binary lock) — T2 keeps the wrapper compatible with a later `-j`/per-worktree volume (volume name overridable by env `DC2_VOLUME`, default `dc2-work`); no parallelism built now.
- C0003 (offline tooling first) — background for T1's reusable control.

## Research
- (none; no prior reports. Planner probes recorded in Context.)

## Developer decides
- (none)

## Triage
- (none)

## Tasks

- T1 | next | expert-opus55 | title: firewall negative control, both ways | coder: none | effort: medium | files: tools/firewall_control.sh, docs/ops/decomp-environment.md | done-when: `tools/firewall_control.sh` exists per ## Interfaces, reports planted FAIL (rc 1, OFFENDER names the planted path) and tree PASS (rc 0), leaves no file under `.run/firewall-control/`, and a deliberately broken run (fixture hash mismatch simulated by planting different bytes, scratch only, not committed) makes the script exit 1 naming step (a) or (b); Tooling inventory in `docs/ops/decomp-environment.md` gains its row | verify: `bash tools/firewall_control.sh; echo rc=$?; test -e .run/firewall-control/planted.bin && echo LEFTOVER` → `rc=0`, no `LEFTOVER` | reads: — | deps: — | est-ctx: 30k | review: no | wait-for: —
  Expert writes the script itself (≤ ~20 lines, one edit + one verification run allowed); the broken-run check is a scratch invocation (e.g. an env override of the planted bytes, or a copy of the script under `.run/`), never a tracked change to the fixture or `config/firewall.txt`.
  Record in the summary the exact audit output lines for both runs (counts with denominators). Never edit `tools/audit_public.py`; a defect there is a `harness:` gotcha.
  Commit: `bash tools/commit_task.sh T1 "T1: Add the firewall negative control script" tools/firewall_control.sh docs/ops/decomp-environment.md`.
- T2 | queued | expert-opus55 | title: amd64 build host on a named volume | coder: opus55 | effort: medium | files: tools/docker/Dockerfile, tools/docker/dc.sh, docs/ops/docker-host.md, docs/ops/INDEX.md, docs/ops/decomp-environment.md, HOW_WE_WORK.md | done-when: base image pinned by digest; `dc.sh build` exits 0 from a clean cache for the tag (`--no-cache` once, time recorded); `dc.sh sync` populates volume `dc2-work` with no bind mount; `dc.sh run` executes in `/work` on the volume: `uname -m` = `x86_64`, `mipsel-linux-gnu-gcc --version`, `mipsel-linux-gnu-as --version`, `python3 --version`, `make format-check` exit 0; `docs/ops/docker-host.md` records host facts (Docker Desktop version, emulation backend, VM mem/CPUs, image id + base digest, `dpkg-query -W` versions of the mipsel gcc/binutils/cpp, python3, make, clang-format; build wall time; volume name; sync procedure + its limits); INDEX line added; card Build/Pins lines point at `dc.sh` and `docs/ops/docker-host.md` (card ≤ 7000 chars); audit still rc 0 | verify: `bash tools/docker/dc.sh build && bash tools/docker/dc.sh sync && bash tools/docker/dc.sh run sh -c 'uname -m && make format-check' ; echo rc=$?` → `x86_64`, rc=0; then `bash tools/firewall_control.sh` rc 0 | reads: tasks/T1.md | deps: T1 | est-ctx: 60k | review: no | wait-for: —
  Long builds: `bash tools/run.sh --bg dc2-build -- bash tools/docker/dc.sh build --no-cache` then `bash tools/run.sh --wait dc2-build --max 280` (repeat the wait call; never a sleep loop). Emulated apt installs can take minutes.
  Digest: resolve the `linux/amd64`-capable index digest of `ubuntu:24.04` (e.g. `docker buildx imagetools inspect ubuntu:24.04`), record source + date in `docker-host.md`.
  Sync: see ## Rationale (tar stream over stdin, recommended). Document the known limit (files deleted on the host linger until a full resync; `sync` must wipe `/work` first, or document why not). Ignored data (`disks/`, `extracted/`) is out of scope until 1.1.
  The container never mounts `/Users/Shared/GameInputs` or the repo; no game data enters the volume in 1.0.
  Coder commits green runs as `T2.c<k>`; expert commits docs/summary as `T2`. HOW_WE_WORK.md edit is surgical (lines :53, :66 only, plus Tools table row for `dc.sh` if it fits the cap).

## Risks
- Emulated amd64 build under Docker Desktop fails or is very slow (qemu segfault on apt/`dpkg` triggers, Rosetta disabled). Detection: `dc.sh build` rc ≠ 0 or wall time > 20 min. Response: check Docker Desktop "Use Rosetta for x86/amd64 emulation" setting (a developer toggle — T2 returns `blocked` with the exact setting, never flips it) and retry once.
- Docker Desktop VM disk fills (8 GiB mem, disk unknown). Detection: `docker system df` in T2's host facts; a build failing with ENOSPC. Response: report; pruning another project's images/volumes is the developer's call.
- CI clause goes red before close (e.g. a pushed change breaks `audits`, or GitHub Actions setup-python drift). Detection: closer's `gh run list --workflow no-rom.yml -L 1`. Response: the closer returns `blocked` with the run URL; the fix is a task, the push is the developer's.
- Digest pinning to a manifest list vs a platform manifest mis-resolves under `--platform linux/amd64`. Detection: `dc.sh build` pull error. Response: pin the index (manifest-list) digest, which resolves per platform.
- Sync leaks ignored or scratch files into the volume (`.run/`, `disks/`). Detection: `dc.sh run sh -c 'ls -a /work'` shows no `.run`, `disks`. Low impact in 1.0 (no game data present on the host tree yet besides empty dirs).

## Changes
