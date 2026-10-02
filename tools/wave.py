#!/usr/bin/env python3
"""wave.py -- draw a wave of targets and write their cards (T3, Phase 1.8); stdlib only, run in the container:
    dc.sh run python3 tools/wave.py draw --kind {manual,family,exe,overlay,hard} --weight INSNS
                                         [--binary ALIAS] [--seed S] [--dry-run]
    dc.sh run python3 tools/wave.py cards [WAVE] [--dry-run]

draw: runs tools/draw_filter.py --all (rc != 0 -> rc 2), pool = .run/draw/accepted.tsv ∩ census kind=game (--binary:
one alias; unknown -> rc 2); insns = size/4. Buckets = config/routing.tsv rows, else DEFAULT_BUCKETS. Key within a
bucket: sha256("seed:alias:start") hex, alias, int(start). manual: round-robin over buckets ascending, each bucket's
next candidate that fits; other kinds: one merged list in key order. Greedy: add iff sum + insns <= weight. Never a
twins.tsv twin (either direction) of a drawn target (`twin-skipped`). G49 validation of every target (census game
start+end, asm on disk, not banked, accepted) -> failures listed, rc 1; empty draw -> `REFUSED: empty draw` rc 2.
Open guard (dry-run too): a config/waves.tsv row with harvest `open` or fleet `-`/empty -> `REFUSED: wave <id> ...`
rc 2; an in-memory control plants one of each and must refuse both (`open control: ok|FAIL`, FAIL rc 1).
Writes .run/waves/<wNN>/targets.tsv + appends an open config/waves.tsv row (refused rc 2 when the dir exists);
--dry-run: .run/waves/_dry/targets.tsv only.
cards: re-validates .run/waves/<WAVE>/targets.tsv (WAVE defaults to _dry with --dry-run), writes
<alias>_<start>/card.md per target; G44: every lever id resolves in config/levers.tsv and every C-id in
cookbook/INDEX.md, else the card fails (not written, listed, rc 1); a control card with a planted C9999 must fail
(`card control: ok|FAIL`). Deterministic; no game bytes (G12): addresses, symbol names, counts, paths, our C only.
Exit 0 ok / 1 fail / 2 refused. Notes: docs/ops/decomp-environment.md "Waves: draw and cards".
"""
import argparse
import datetime
import hashlib
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import census  # noqa: E402  (rows, load_fleet)
import draw_filter  # noqa: E402  (ensure: build-or-refuse; Evidence: census/asm/banked/twins)
import dup_census  # noqa: E402  (LINE: the asm line shape)

ROOT = census.ROOT
WAVES = ROOT / "config/waves.tsv"
JOURNAL = ROOT / "config/journal.tsv"
ROUTING = ROOT / "config/routing.tsv"
C_UNITS = ROOT / "config/c_units.tsv"
LEVERS = ROOT / "config/levers.tsv"
COOKBOOK = ROOT / "cookbook/INDEX.md"
ACCEPTED = draw_filter.OUT / "accepted.tsv"
RUN = ROOT / ".run/waves"
DEFAULT_BUCKETS = ((1, 16), (17, 32), (33, 64), (65, 128), (129, 256), (257, None))  # insns, inclusive; None = open
KINDS = ("manual", "family", "exe", "overlay", "hard")
FUNC_TOK = re.compile(r"\bfunc_[0-9A-Fa-f]{8}\b")
CALL_NAME = re.compile(r"([A-Za-z_]\w*)\s*\(")
CID = re.compile(r"^(C\d{4}) \|")


def rows(path):
    return census.rows(path) if path.exists() else []


def buckets():
    """-> [(lo, hi|None, label, routing row|None)] ascending."""
    out = []
    for r in rows(ROUTING):
        if len(r) >= 6:
            hi = int(r[2]) if r[2].strip() not in ("", "-") else None
            out.append((int(r[1]), hi, r))
    if not out:
        out = [(lo, hi, None) for lo, hi in DEFAULT_BUCKETS]
    out.sort(key=lambda b: b[0])
    return [(lo, hi, "%d-%s" % (lo, hi if hi is not None else "inf"), r) for lo, hi, r in out]


def bucket_of(bks, insns):
    return next((b for b in bks if b[0] <= insns and (b[1] is None or insns <= b[1])), None)


def guard(wrows):
    """-> REFUSED line when a wave is open, else None."""
    for r in wrows:
        r = r + [""] * (13 - len(r))
        if r[12] == "open":
            return "REFUSED: wave %s open (harvest open); harvest and close it first" % r[0]
        if r[11] in ("", "-"):
            return "REFUSED: wave %s open (fleet %s); run the fleet check and close it first" % (r[0], r[11] or "empty")
    return None


def open_control():
    a = ["w98", "manual", "2026-01-01", "2026-01-02", "1", "1", "1", "0", "0", "1", "0", "83/83@0000000", "open"]
    b = ["w99", "manual", "2026-01-01", "2026-01-02", "1", "1", "1", "0", "0", "1", "0", "-", "done:0 notes"]
    return guard([a]) is not None and guard([b]) is not None


def accepted():
    return {(r[0], int(r[1], 16)) for r in rows(ACCEPTED) if len(r) >= 3 and r[2] == "accepted"}


def validate(ev, acc, targets):
    """G49: -> [failure lines] for (alias, start, end) targets."""
    bad = []
    for al, st, en in targets:
        f = ev.fn.get((al, st))
        why = []
        if f is None or f[1] != "game":
            why.append("not a census game start")
        elif f[0] != en:
            why.append("census end 0x%08x != 0x%08x" % (f[0], en))
        if not ev.has_asm(al, st):
            why.append("no asm on disk")
        if (al, st) in ev.banked:
            why.append("banked (%s)" % ev.banked[(al, st)])
        if (al, st) not in acc:
            why.append("not in %s" % ACCEPTED.relative_to(ROOT))
        if why:
            bad.append("  %s:0x%08x:0x%08x %s" % (al, st, en, "; ".join(why)))
    return bad


def twin_map():
    tw = {}
    for r in rows(draw_filter.TWINS):
        if len(r) >= 7:
            x, y = (r[0], int(r[1], 16)), (r[2], int(r[3], 16))
            tw.setdefault(x, set()).add(y)
            tw.setdefault(y, set()).add(x)
    return tw


def cmd_draw(a):
    wrows = rows(WAVES)
    ctl = open_control()
    msg = guard(wrows)
    if msg:
        print(msg)
        print("open control: %s" % ("ok" if ctl else "FAIL"))
        return 2 if ctl else 1
    p = subprocess.run([sys.executable, str(ROOT / "tools/draw_filter.py"), "--all"], cwd=ROOT,
                       capture_output=True, text=True)
    if p.returncode:
        print("REFUSED: tools/draw_filter.py --all rc %d" % p.returncode)
        print("\n".join(p.stdout.splitlines()[-5:]))
        return 2
    ev = draw_filter.Evidence()
    acc = accepted()
    if a.binary and a.binary not in census.load_fleet():
        print("REFUSED: unknown binary %s" % a.binary)
        return 2
    pool = sorted(k for k in acc if k in ev.fn and ev.fn[k][1] == "game" and (not a.binary or k[0] == a.binary))
    bks = buckets()
    per = {b[2]: [] for b in bks}
    keyed = []
    for al, st in pool:
        ins = (ev.fn[(al, st)][0] - st) // 4
        b = bucket_of(bks, ins)
        if b is None:
            continue
        key = (hashlib.sha256(("%s:%s:0x%08x" % (a.seed, al, st)).encode()).hexdigest(), al, st)
        c = (key, al, st, ev.fn[(al, st)][0], ins, b[2])
        per[b[2]].append(c)
        keyed.append(c)
    tw = twin_map()
    drawn, total, skipped = [], 0, 0
    taken = set()

    def take(c):
        nonlocal total, skipped
        if total + c[4] > a.weight:
            return False
        if tw.get((c[1], c[2]), set()) & taken:
            skipped += 1
            return False
        drawn.append(c)
        taken.add((c[1], c[2]))
        total += c[4]
        return True

    if a.kind == "manual":
        qs = [sorted(per[b[2]]) for b in bks]
        idx = [0] * len(qs)
        progress = True
        while progress:
            progress = False
            for i, q in enumerate(qs):
                while idx[i] < len(q):
                    c = q[idx[i]]
                    idx[i] += 1
                    if take(c):
                        progress = True
                        break
    else:
        for c in sorted(keyed):
            take(c)
    wave = "_dry" if a.dry_run else "w%02d" % (1 + max([int(m.group(1)) for r in wrows
                                                          for m in [re.match(r"w(\d+)$", r[0])] if m] or [0]))
    print("wave: %s kind %s seed %s" % (wave, a.kind, a.seed))
    print("pool: %d accepted" % len(pool))
    if not drawn:
        print("REFUSED: empty draw")
        return 2
    print("targets: %d (insns %d of %d)" % (len(drawn), total, a.weight))
    for b in bks:
        print("bucket %s: %d" % (b[2], sum(1 for c in drawn if c[5] == b[2])))
    print("twin-skipped: %d" % skipped)
    bad = validate(ev, acc, [(c[1], c[2], c[3]) for c in drawn])
    print("validated: %d of %d" % (len(drawn) - len(bad), len(drawn)))
    for line in bad:
        print(line)
    print("open control: %s" % ("ok" if ctl else "FAIL"))
    if bad or not ctl:
        return 1
    d = RUN / wave
    if not a.dry_run and d.exists():
        print("REFUSED: %s already exists" % d.relative_to(ROOT))
        return 2
    d.mkdir(parents=True, exist_ok=True)
    with open(d / "targets.tsv", "w") as fh:
        fh.write("# alias\tstart\tend\tinsns\tbucket\n")
        for c in drawn:
            fh.write("%s\t0x%08x\t0x%08x\t%d\t%s\n" % (c[1], c[2], c[3], c[4], c[5]))
    if a.dry_run:
        print("dry-run: nothing tracked written")
        return 0
    today = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%d")
    with open(WAVES, "a") as fh:
        fh.write("\t".join([wave, a.kind, today, "-", str(len(drawn))] + ["-"] * 7 + ["open"]) + "\n")
    print("pull: bash tools/docker/dc.sh run tar -cf - .run/waves/%s config/waves.tsv | tar -xf -" % wave)
    return 0


class Asm:
    """Per alias: vram -> asm file (repo path) and jal callee names by vram, from `/* off vram word */` lines."""

    def __init__(self):
        self.cache = {}

    def load(self, alias):
        if alias not in self.cache:
            where, jal = {}, {}
            for f in sorted((ROOT / "asm" / alias).rglob("*.s")):
                rel = str(f.relative_to(ROOT))
                for line in f.read_text(errors="replace").splitlines():
                    m = dup_census.LINE.match(line)
                    if not m:
                        continue
                    v = int(m.group(1), 16)
                    where.setdefault(v, rel)
                    if m.group(3) == "jal":
                        n = FUNC_TOK.search(m.group(4))
                        if n:
                            jal[v] = n.group(0)
            self.cache[alias] = (where, jal)
        return self.cache[alias]

    def path(self, alias, start):
        return self.load(alias)[0].get(start)

    def callees(self, alias, start, end):
        jal = self.load(alias)[1]
        return sorted({n for v, n in jal.items() if start <= v < end})


def decl_index():
    """name -> Counter(whitespace-normalised top-level declaration lines) over our C and headers."""
    files = sorted(set((ROOT / "src").rglob("*.c")) | set((ROOT / "include").rglob("*.h")))
    idx = {}
    for f in files:
        for line in f.read_text(errors="replace").splitlines():
            if not line or line[0].isspace() or line.startswith(("#", "/", "*", "INCLUDE_")):  # INCLUDE_ASM: no decl
                continue
            s = " ".join(line.split())
            if not s.endswith(";"):
                continue
            m = CALL_NAME.search(s)
            if m:
                idx.setdefault(m.group(1), Counter())[s] += 1
    return idx


def unit_decls(path):
    out = []
    for line in path.read_text(errors="replace").splitlines():
        if not line or line[0].isspace():
            continue
        s = line.rstrip()
        if s.startswith(("#include", "typedef", "extern")) or (
                not s.startswith(("#", "/", "*", "INCLUDE_")) and "(" in s and s.endswith(");")):
            out.append(s)
    return out


class Ctx:
    """Inputs every card reads, loaded once."""

    def __init__(self):
        self.ev = draw_filter.Evidence()
        self.acc = accepted()
        self.asm = Asm()
        self.units = {}
        for r in rows(C_UNITS):
            if len(r) >= 3:
                self.units[(r[0], r[2])] = r
        self.twins = {}
        for r in rows(draw_filter.TWINS):
            if len(r) >= 7 and r[6] == "1":
                self.twins.setdefault((r[0], int(r[1], 16)), []).append(
                    (int(r[4]), -float(r[5]), r[2], int(r[3], 16), r[5]))
        self.decls = decl_index()
        self.journal = rows(JOURNAL)
        self.bks = buckets()
        self.levers = [(r[0], r[3], r[9]) for r in rows(LEVERS) if len(r) >= 10]
        self.lever_ids = {r[0] for r in rows(LEVERS) if r}
        self.cids = {m.group(1) for line in COOKBOOK.read_text().splitlines() for m in [CID.match(line)] if m}


def unit_of(ctx, alias, path):
    """c_units row of alias whose unit holds the fn: its asm lives under asm/<alias>/nonmatchings/<unit>/."""
    parts = Path(path).parts if path else ()
    if len(parts) > 4 and parts[2] == "nonmatchings":
        r = ctx.units.get((alias, parts[3]))
        if r:
            return r[2], "src/%s/%s.c" % (alias, r[2])
    return None


def card(ctx, t, levers):
    """-> (text, [G44 failures]) for target t = (alias, start, end, insns, bucket)."""
    al, st, en, ins, bk = t
    unres = ["lever %s" % i for i, _, _ in levers if i not in ctx.lever_ids]
    unres += ["cookbook %s" % c for _, _, c in levers if c not in ctx.cids]
    asm = ctx.asm.path(al, st)
    L = ["# card %s:0x%08x:0x%08x" % (al, st, en), "", "- insns: %d" % ins, "- bucket: %s" % bk,
         "- asm: %s" % (asm or "-")]
    u = unit_of(ctx, al, asm)
    L.append("- destination unit: %s (%s)" % u if u else
             "- destination unit: none — carve: tools/carve.py %s 0x%08x" % (al, st))
    tw = sorted(ctx.twins.get((al, st), []))
    if tw:
        d, _, ta, ts, ratio = tw[0]
        src = ctx.ev.banked.get((ta, ts), "-")
        L.append("- banked twin: %s:0x%08x distance %d ratio %s unit %s (%s)" % (ta, ts, d, ratio, Path(src).stem, src))
    else:
        L.append("- banked twin: none")
    L += ["", "## Destination-unit declarations"]
    decls = unit_decls(ROOT / u[1]) if u and (ROOT / u[1]).is_file() else []
    L += decls or ["-"]
    L += ["", "## Callees"]
    cs = ctx.asm.callees(al, st, en)
    for n in cs:
        c = ctx.decls.get(n)
        if c:
            k = max(c.values())
            best = min(s for s, v in c.items() if v == k)
            L.append("- %s: %s  (%d of %d)" % (n, best, k, sum(c.values())))
        else:
            L.append("- %s: undeclared" % n)
    if not cs:
        L.append("-")
    L += ["", "## Journal history"]
    jr = [r for r in ctx.journal if len(r) >= 2 and r[0] == al and r[1].startswith("0x") and int(r[1], 16) == st]
    L += ["- " + " ".join(r) for r in jr] or ["none"]
    labels = {r[5] for r in jr if len(r) > 5}
    L += ["", "## Route"]
    b = next((x for x in ctx.bks if x[2] == bk), None) or bucket_of(ctx.bks, ins)
    if b and b[3]:
        r = b[3] + [""] * (6 - len(b[3]))
        L.append("- route: %s  card_cap: %s  attempts: %s" % (r[3], r[4], r[5]))
    else:
        L.append("- route: - (no config/routing.tsv)")
    L += ["", "## Lever pointers"]
    order = [x for x in levers if x[1] in labels] + [x for x in levers if x[1] not in labels]
    L += ["- %s  %s  %s" % x for x in order] or ["-"]
    return "\n".join(L) + "\n", unres


def cmd_cards(a):
    wave = a.wave or ("_dry" if a.dry_run else None)
    if wave is None:
        print("REFUSED: WAVE is required without --dry-run")
        return 2
    tsv = RUN / wave / "targets.tsv"
    if not tsv.exists():
        print("REFUSED: no %s" % tsv.relative_to(ROOT))
        return 2
    targets = [(r[0], int(r[1], 16), int(r[2], 16), int(r[3]), r[4]) for r in rows(tsv) if len(r) >= 5]
    if not targets:
        print("REFUSED: empty %s" % tsv.relative_to(ROOT))
        return 2
    msg = draw_filter.ensure()
    if msg:
        print(msg)
        return 2
    ctx = Ctx()
    bad = validate(ctx.ev, ctx.acc, [t[:3] for t in targets])
    if bad:
        print("validated: %d of %d" % (len(targets) - len(bad), len(targets)))
        print("\n".join(bad))
        return 1
    _, ures = card(ctx, targets[0], ctx.levers + [(ctx.levers[0][0] if ctx.levers else "-", "-", "C9999")])
    ctl = any("C9999" in x for x in ures)
    written, failed = 0, []
    for t in targets:
        text, unres = card(ctx, t, ctx.levers)
        if unres:
            failed.append("  %s:0x%08x unresolved %s" % (t[0], t[1], ", ".join(unres)))
            continue
        d = RUN / wave / ("%s_0x%08x" % (t[0], t[1]))
        d.mkdir(parents=True, exist_ok=True)
        (d / "card.md").write_text(text)
        written += 1
    print("cards: %d of %d written (%s)" % (written, len(targets), wave))
    for line in failed:
        print(line)
    print("card control: %s" % ("ok" if ctl else "FAIL"))
    if a.dry_run:
        print("dry-run: nothing tracked written")
    return 0 if ctl and not failed else 1


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    d = sub.add_parser("draw")
    d.add_argument("--kind", required=True, choices=KINDS)
    d.add_argument("--weight", required=True, type=int)
    d.add_argument("--binary")
    d.add_argument("--seed", default="0")
    d.add_argument("--dry-run", action="store_true")
    c = sub.add_parser("cards")
    c.add_argument("wave", nargs="?")
    c.add_argument("--dry-run", action="store_true")
    a = ap.parse_args(argv)
    return cmd_draw(a) if a.cmd == "draw" else cmd_cards(a)


if __name__ == "__main__":
    sys.exit(main())
