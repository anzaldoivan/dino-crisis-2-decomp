#!/usr/bin/env python3
"""delta.py — the HAND-AUTHORED part of a Ghidra program, as a byte-stable file (adapted from the kit's
P2 ghidra_annotations_delta.py).

    delta.py <live.jsonl> <baseline.jsonl> <out.jsonl>   [--census]

`live.jsonl` is ExportAnnotations.java's dump of the program as it is; `baseline.jsonl` is the same
dump of a FRESH rebuild of that program (import + psx_ldr auto-analysis + PsyQ sigs/gdt, no annotations
imported). Every row auto-analysis produces on its own appears in both and subtracts itself out — minus
the counted analysis-drift classes below; what remains — types, retyped/renamed signatures and locals,
typed data, comments, bookmarks, equates, labels — is the RE work worth committing: config/ghidra/<program>.jsonl.
The container rows (`program`, `block`, `archive`) are always kept from `live` so an image-base or
memory-map drift shows as a diff rather than a silent skip. Lines starting with '#' are comments (skipped).

Only hand-authored and signature-applied rows survive (G12: no game text): every ANALYSIS/DEFAULT-sourced
label (switch `caseD_*`/`switchD_*` labels drift between analysis runs) and every auto-derived data label
name (`s_<text>_<addr>`, `DAT_<addr>`, ...) is dropped, as are Ghidra's Error/Analysis bookmarks.

The proof (tools/ghidra/rebuild.sh --proof) rebuilds, imports config/ghidra/<program>.jsonl, exports,
takes the delta against the SAME rebuild's pre-import baseline, and `cmp`s it against the committed file.

`--census` prints per-kind counts of the delta.
"""
import collections
import json
import re
import sys

KEEP_ALWAYS = ("program", "block", "archive")
ANALYSIS_BOOKMARKS = ("Error", "Analysis")
AUTO_NAME = ("FUN_", "func_", "thunk_FUN_")
# Ghidra's analysis-derived data labels: <prefix>_<anything>_<8 hex addr> (s_Hello_80012345, DAT_80012345, ...)
AUTO_LABEL = re.compile(r"^(s|u|ds|DAT|PTR|LAB|BYTE|WORD|DWORD|QWORD|FLOAT|DOUBLE|UNK|OFF|SUB|EXT|switchD|caseD)_"
                        r"(.*_)?[0-9a-fA-F]{8}$")


def is_function_set_drift(o):
    if o.get("sigsrc") != "DEFAULT" or o.get("comment") or o.get("custom"):
        return False
    if not str(o.get("name", "")).startswith(AUTO_NAME):
        return False
    return all(p.get("src") == "DEFAULT" for p in o.get("params", []) + o.get("locals", []))


def is_auto_label(o):
    return o.get("src") in ("ANALYSIS", "DEFAULT") or bool(AUTO_LABEL.match(str(o.get("name", ""))))


def rows(path):
    with open(path, encoding="utf-8") as f:
        return [ln.rstrip("\n") for ln in f if ln.strip() and not ln.startswith("#")]


def kind(line):
    return json.loads(line)["k"]


def main(argv):
    census = "--census" in argv
    argv = [a for a in argv if a != "--census"]
    if len(argv) != 4:
        sys.exit(__doc__)
    live, base, out = rows(argv[1]), set(rows(argv[2])), argv[3]
    base_funcs = {json.loads(ln)["addr"] for ln in base if kind(ln) == "func"}
    dropped = collections.Counter()
    kept = []
    for ln in live:
        k = kind(ln)
        if k in KEEP_ALWAYS or ln not in base:
            o = json.loads(ln)
            if k == "bookmark" and o.get("type") in ANALYSIS_BOOKMARKS:
                dropped["bookmark:analysis"] += 1        # Ghidra's own Error/Analysis bookmarks — never hand-authored
                continue
            if k == "func" and o["addr"] not in base_funcs and is_function_set_drift(o):
                dropped["func:set-drift"] += 1           # a boundary the live analysis found and the rebuild did not
                continue
            if k == "label" and is_auto_label(o):
                dropped["label:auto"] += 1               # analysis-derived label (string labels embed game text, G12)
                continue
            kept.append(ln)
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        for ln in kept:
            f.write(ln + "\n")
    c = collections.Counter(kind(ln) for ln in kept)
    hand = sum(v for k, v in c.items() if k not in KEEP_ALWAYS)
    drop = ", ".join(f"{k} {v}" for k, v in sorted(dropped.items())) or "none"
    print(f"delta: {len(live)} live - {len(base)} baseline -> {len(kept)} rows ({hand} hand-authored; "
          f"analysis drift dropped: {drop}) -> {out}", file=sys.stderr)
    if census:
        for k in sorted(c):
            print(f"  {k:9} {c[k]}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
