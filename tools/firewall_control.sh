#!/usr/bin/env bash
# Firewall negative control: the planted fixture must FAIL the ROM audit, the tree must PASS. Exit 0 iff both.
set -u; cd "$(dirname "$0")/.."
PY="${PY:-/opt/homebrew/opt/python@3.14/bin/python3.14}"; command -v "$PY" >/dev/null || PY=python3
P=.run/firewall-control/planted.bin
fail() { echo "firewall_control: FAIL at step ($1): $2"; exit 1; }
trap 'rm -f "$P"; rmdir .run/firewall-control 2>/dev/null' EXIT
mkdir -p .run/firewall-control; printf 'DECOMP-FIXTURE!!' > "$P"
want=$(cut -d' ' -f1 config/firewall-fixture.sha1 | head -1)
got=$( (sha1sum "$P" 2>/dev/null || shasum -a 1 "$P") | cut -d' ' -f1)
[ "$got" = "$want" ] || fail a "planted sha1 $got != fixture $want"; echo "(a) planted sha1 == fixture: ok"
out=$("$PY" tools/audit_public.py --paths "$P" 2>&1); rc=$?; echo "$out" | tail -1
[ $rc -eq 1 ] && echo "$out" | grep -q "^[[:space:]]*OFFENDER $P:" || fail b "planted audit rc=$rc, no OFFENDER $P"
echo "(b) planted audit FAIL naming $P: ok"
rm -f "$P"; [ ! -e "$P" ] || fail c "planted copy not removed"; echo "(c) planted copy removed: ok"
out=$("$PY" tools/audit_public.py 2>&1); rc=$?; echo "$out" | tail -1
[ $rc -eq 0 ] || fail d "tree audit rc=$rc"; echo "(d) tree audit PASS: ok"
echo "firewall_control: OK"
