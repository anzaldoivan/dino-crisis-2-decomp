# Phase-end attempt 1 (phase 1.4) — blocked on verifier

## Timeline
- run.sh pe-verify (`phaseend_index.py verify --verbose`) → RED 5/7: c1 `target binary absent: /work/extracted/retail/files/SLUS_012.79` (clause 1 runs before clause 2's fleet_check extracts; fresh volume had no extraction); c3 RED with `banked: 5`, `verbatim: 0` in .run/logs/verify3.log.
- run.sh pe-c1 (foreground `bash -c 'dc.sh sync && dc.sh run probe.py --pinned'`) exceeded the 285 s tool limit, the harness moved it to background; log 0 bytes, container `dc2-build` idle at 0 CPU ~30 min. Router: kill. `docker kill 0b585313ffd7` (only the dc2-build container; `pensive_clarke` mmx6-build is another project, untouched).
- Hang cause (diagnostic-grade, one observation): the hung run was a harness-backgrounded foreground call, not run.sh --bg; probe.py itself is not hung: pe-c1b (`timeout 200`) showed live maspsx/as/objcopy workers at 45 s, timed out at 200 s (full ladder 528 triples × 5 probes, T4: 3:14 wall uncontended; host shared with mmx6 `make -j8`); pe-c1c (`timeout 900`, run.sh --bg) → exit 0 in < 270 s, `probes identical: 5 of 5 under the pinned triple`, `ladder: exactly 1 triple(s) match all 5`, `controls: true ok, false ok`.
- run.sh pe-verify2 (full verify again, volume now extracted) → RED 5/7: clauses 2,4,5,6,7 GREEN; c1 RED with .run/logs/verify1.log:2652 `probes identical: 5 of 5 under the pinned triple`, :2653 `ladder: exactly 1 triple(s) match all 5`; c3 RED with verify3.log:1 `banked: 5`, :2 `verbatim: 0`; `dc.sh run python3 tools/banked.py` → exit 0 (direct).

## Cause of RED
- phaseend_index.py verify matches the clause's quoted text literally: placeholders `K of K` and `banked: n` never appear in real output; the values satisfy the milestone (K=5, 3≤K≤5; n=5≥3; verbatim 0). Also `triple(s)` vs milestone `triple`.
- Ordering: clause 1 depends on extraction that only clause 2 (fleet_check) performs; green on a used volume, RED on a fresh one.

## Not done
- RECAP.md, gotcha promotion, H7 check: withheld while the gate tool reads RED.

## Gotchas
- harness: phaseend_index.py verify matches placeholder text (`K of K`, `banked: n`) literally; a milestone with variables can never go GREEN; `--help` silent on it
- harness: a foreground Bash call over 285 s is moved to background by the harness and can stall at 0 CPU (docker run); long gates only via run.sh --bg + --wait ≤ 270 s
- generalizable: a verify clause that reads extracted data must extract itself or run after the clause that does
