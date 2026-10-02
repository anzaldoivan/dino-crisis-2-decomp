#!/usr/bin/env python3
"""wave.py -- draw a wave of targets and write their cards (T3, Phase 1.8); stdlib only, run in the container:
    dc.sh run python3 tools/wave.py draw --kind {manual,family,exe,overlay,hard} --weight INSNS
                                         [--binary ALIAS] [--seed S] [--dry-run]
                                         [--per-bucket N] [--family F[,F...]] [--id ID]   (T5.c1)
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
--dry-run: .run/waves/_dry/targets.tsv only. --per-bucket N (manual): a bucket stops at N; --family: pool ∩ census
family; --id ID replaces wNN (refused rc 2 when in waves.tsv or its dir exists; checked on --dry-run too).
cards: re-validates .run/waves/<WAVE>/targets.tsv (WAVE defaults to _dry with --dry-run), writes
<alias>_<start>/card.md per target; G44: every lever id resolves in config/levers.tsv and every C-id in
cookbook/INDEX.md, else the card fails (not written, listed, rc 1); a control card with a planted C9999 must fail
(`card control: ok|FAIL`). Deterministic; no game bytes (G12): addresses, symbol names, counts, paths, our C only.
Exit 0 ok / 1 fail / 2 refused. Notes: docs/ops/decomp-environment.md "Waves: draw and cards".

T4 (Phase 1.8), spec .run/briefs/T4.c1.md; host drivers (commit / write tracked config; refused rc 2 in the container):
    wave.py gate WAVE            clean tree, unGated row -> sync, push the wave dir, inner gate, pull, commit banks + ledgers
    wave.py recover WAVE         (T5.c32) gated wave: as gate, re-scores only targets with no banked journal row; appends
                                 journal rows route `recover`, recounts the waves row from the journal; re-runnable
    wave.py fleet WAVE           sync + fleet_check.sh (tee .run/waves/WAVE/fleet.log); green -> fleet 83/83@<HEAD>
    wave.py harvest WAVE --fn A:S --note TEXT [--lever ID --stripped PATH]   note `strip:ok:<ID> TEXT` | `strip:none TEXT`
    wave.py close WAVE           refusals (gate, fleet ancestry, unharvested) -> closed + harvest done:<n> notes, commit
container (--in-volume, cwd /work):
    wave.py gate WAVE --in-volume       score every target dir (G50/G47/G11, reconcile probe), bank per destination unit
                                        (reconcile --apply + ONE scratch build + sha1), journal + waves row
    wave.py strip --fn A:S --stripped PATH --in-volume   `strip: differs` rc 0 | `strip: identical` rc 1 (G45)
anywhere: wave.py --check (ledger counts + close-refusal control); container: wave.py --selftest (scratch only).
Notes: docs/ops/decomp-environment.md "Waves: gate, bank, fleet, harvest, close".
"""
import argparse
import datetime
import hashlib
import re
import shutil
import subprocess
import sys
import types
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
    if a.id is not None:
        if not WAVE_ID.match(a.id):
            print("REFUSED: --id %s must match [A-Za-z0-9_]+" % a.id)
            return 2
        if any(r and r[0] == a.id for r in wrows) or (RUN / a.id).exists():
            print("REFUSED: wave id %s already in config/waves.tsv or .run/waves/%s/ exists" % (a.id, a.id))
            return 2
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
    fams = set(a.family.split(",")) if a.family else None
    if fams and fams - {v[2] for v in ev.fn.values()}:
        print("REFUSED: unknown family %s" % ",".join(sorted(fams - {v[2] for v in ev.fn.values()})))
        return 2
    pool = sorted(k for k in acc if k in ev.fn and ev.fn[k][1] == "game" and (not a.binary or k[0] == a.binary)
                  and (not fams or ev.fn[k][2] in fams))
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
        held = [0] * len(qs)
        progress = True
        while progress:
            progress = False
            for i, q in enumerate(qs):
                while idx[i] < len(q) and (a.per_bucket is None or held[i] < a.per_bucket):
                    c = q[idx[i]]
                    idx[i] += 1
                    if take(c):
                        held[i] += 1
                        progress = True
                        break
    else:
        for c in sorted(keyed):
            take(c)
    wave = "_dry" if a.dry_run else a.id or "w%02d" % (1 + max([int(m.group(1)) for r in wrows
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


# ---- T4: gate, bank chain, fleet, harvest, close, --check, --selftest ----

WCOLS = ("wave", "kind", "opened", "closed", "pool", "drafted", "banked", "failed", "no_verdict", "insns_banked",
         "propagated", "fleet", "harvest")
JCOLS = ("alias", "start", "wave", "route", "outcome", "label", "note")
BCOLS = ("k", "alias", "unit", "fns", "paths", "commit")
RCOLS = ("bucket", "min_insns", "max_insns", "route", "card_cap", "attempts", "drafted", "banked", "rate", "cost_ctx_k",
         "basis")
# a real compile/assemble/link failure in a make log; the Makefile's own `make: *** [...] Error 1` has no colon
BUILD_ERR = re.compile(r"\berror:|\bError:|undefined reference|Assembler messages|Segmentation fault|multiple definition")
VERBATIM = ("asm(", "__asm__", "INCLUDE_ASM", "glabel", ".word", ".set noreorder")
FLEET_N = census.FLEET_N
FLEET_RE = re.compile(r"^%d/%d@([0-9a-f]{7,40})$" % (FLEET_N, FLEET_N))
HARVEST_RE = re.compile(r"^done:\d+ notes$")
WAVE_ID = re.compile(r"^[A-Za-z0-9_]+$")
DC = ["bash", "tools/docker/dc.sh"]
WORK = Path("/work")


def ledger(path):
    """-> (comment lines, data rows) of a TSV ledger."""
    head, data = [], []
    for line in (path.read_text().splitlines() if path.exists() else []):
        if line.startswith("#"):
            head.append(line)
        elif line.strip():
            data.append(line.split("\t"))
    return head, data


def write_ledger(path, head, data):
    path.write_text("".join(x + "\n" for x in head + ["\t".join(r) for r in data]))


def header_ok(path, cols):
    head, data = ledger(path)
    return ("# " + "\t".join(cols)) in head and all(len(r) == len(cols) for r in data)


def build_outcome(rung, log):
    """Pure: -> (outcome, note) for a reconcile `failed <rung> build-error` from its make.<rung>.log text. Compiled but
    the sha1 check failed (Makefile renames to `.bin.bad`) -> plateau `hash-mismatch <rung>`; else compile-error."""
    if ".bin.bad" in log and not BUILD_ERR.search(log):
        return "plateau", "hash-mismatch %s" % rung
    return "compile-error", "failed %s build-error" % rung


def wave_row(data, wave):
    return next((i for i, r in enumerate(data) if r and r[0] == wave), None)


def tdir(alias, start):
    return "%s_0x%08x" % (alias, start)


def sh(cmd, capture=True):
    return subprocess.run(cmd, cwd=ROOT, stdout=subprocess.PIPE if capture else None,
                          stderr=subprocess.STDOUT if capture else None, text=True)


def host_refusal(name):
    """Host drivers commit or write tracked config: never inside the container (no docker, or cwd /work)."""
    if ROOT == WORK or not shutil.which("docker"):
        return "REFUSED: %s is a host driver (no docker / cwd /work here); run it on the host" % name
    return None


def volume_refusal(name):
    if ROOT != WORK:
        return "REFUSED: %s --in-volume runs only in the container (cwd /work)" % name
    return None


def clean_tree():
    return sh(["git", "status", "--porcelain"]).stdout.strip() == ""


def git_anc(a, b):
    return sh(["git", "merge-base", "--is-ancestor", a, b]).returncode == 0


def short_head():
    return sh(["git", "rev-parse", "--short=7", "HEAD"]).stdout.strip()


def pull(paths):
    return subprocess.run(["bash", "-c", 'set -o pipefail; bash tools/docker/dc.sh run tar -cf - "$@" | tar -xf -',
                           "_"] + paths, cwd=ROOT).returncode


def tree_snapshot():
    return {str(p.relative_to(ROOT)): p.read_bytes() for d in ("src", "config", "include")
            for p in sorted((ROOT / d).rglob("*")) if p.is_file() and not p.is_symlink()}


def read_targets(wave):
    tsv = RUN / wave / "targets.tsv"
    return [(r[0], int(r[1], 16), int(r[2], 16), int(r[3]), r[4]) for r in rows(tsv) if len(r) >= 5]


def route_of(bk):
    b = next((x for x in buckets() if x[2] == bk), None)
    return b[3][3] if b and b[3] and len(b[3]) > 3 else "-"


def word(s, n=120):
    return " ".join(str(s).split())[:n] or "-"


def score(wave, targets, keys=None):
    """1a: -> (refusal lines, {(alias, start): score dict}). Directory-gated (G50/G47) over every target; keys (recover):
    only those (alias, start) are scored."""
    import json
    wd = RUN / wave
    names = {tdir(t[0], t[1]) for t in targets}
    bad = []
    for t in targets:
        d = wd / tdir(t[0], t[1])
        if not (d / "draft.c").is_file() and not (d / "verdict.json").is_file():
            bad.append("missing verdict: %s" % d.name)
    for d in sorted(wd.iterdir()) if wd.is_dir() else []:
        if d.is_dir() and d.name not in names and not d.name.startswith("_") and d.name != "banks":
            bad.append("stray dir: %s" % d.name)
    if bad:
        return bad, {}
    out = {}
    for al, st, en, ins, bk in targets:
        if keys is not None and (al, st) not in keys:
            continue
        d = wd / tdir(al, st)
        claim = {}
        try:
            claim = json.loads((d / "verdict.json").read_text())
        except (OSError, ValueError):
            pass
        claim = claim if isinstance(claim, dict) else {}
        s = {"alias": al, "start": "0x%08x" % st, "outcome": "no-verdict", "rung": "-", "reason": "-",
             "label": word(claim.get("label") or "-", 40), "claim": str(claim.get("status", "")),
             "note": "no draft.c"}
        if (d / "draft.c").is_file():
            draft = (d / "draft.c").read_text(errors="replace")
            if any(v in draft for v in VERBATIM):
                s.update(outcome="plateau", reason="verbatim", label="verbatim", note="pasted asm (G11)")
            else:
                sh([sys.executable, "tools/reconcile.py", str(d / "draft.c"), "--target", "%s:0x%08x" % (al, st),
                    "--out", str(wd / "_reconcile")])  # T5.c33: the wave's own scratch, not the reconcile corpus
                vf = wd / "_reconcile" / tdir(al, st) / "verdict"
                v = vf.read_text().split() if vf.is_file() else []
                if len(v) == 2 and v[0] == "banked":
                    s.update(outcome="candidate", rung=v[1], note="rung %s" % v[1])
                elif len(v) == 3 and v[0] == "failed" and v[1] == "carve" and v[2] == "needs-apply":
                    s.update(outcome="candidate", rung="carve", note="rung carve")  # carve is --apply only
                elif len(v) >= 3 and v[0] == "failed" and v[2] == "build-error":
                    ml = vf.parent / ("make.%s.log" % v[1])
                    oc, nt = build_outcome(v[1], ml.read_text(errors="replace") if ml.is_file() else "")
                    s.update(outcome=oc, rung=v[1], reason=v[2], note=word(nt))
                elif len(v) >= 3 and v[0] == "failed":
                    s.update(outcome="plateau", rung=v[1], reason=v[2], note=word("failed %s %s" % (v[1], v[2])))
                else:
                    s.update(note="reconcile verdict missing or garbled")
        out[(al, st)] = s
    return [], out


def restore(al, snap):
    """Write back the tree_snapshot snap (changed paths rewritten, new ones removed); resplit-config when config/ moved."""
    now = tree_snapshot()
    ks = sorted(k for k in set(snap) | set(now) if snap.get(k) != now.get(k))
    for k in ks:
        if k in snap:
            (ROOT / k).parent.mkdir(parents=True, exist_ok=True)
            (ROOT / k).write_bytes(snap[k])
        else:
            (ROOT / k).unlink()
    if any(k.startswith("config/") for k in ks):
        sh([sys.executable, "tools/splat_gen.py", "--force", "--only", al])
    return ks


def apply_note(verdict, ladder):
    """Pure (T5.c32): journal note for a reconcile --apply that did not bank: the verdict and the ladder's last rung
    line, hashes and the make-log path dropped (the gate logged a bare `apply-failed` before)."""
    last = [x.strip() for x in ladder.splitlines() if x.strip().startswith("rung")]
    why = last[-1] if last else "no rung line"
    why = re.sub(r"sha1 \S+ != config/check\.\S+ [0-9a-f]{40} \(make rc \d+: |\b[0-9a-f]{40}\b|; \.run/\S+", "", why)
    return word("apply-failed %s; %s" % (verdict, why))


def bank(wave, targets, scores):
    """1b: per destination unit, splice every candidate (reconcile --apply) then ONE scratch build + sha1."""
    import propagate
    import tarfile
    propagate.SCRATCH = RUN / "_scratch"
    asm = Asm()
    units = types.SimpleNamespace(units={(r[0], r[2]): r for r in rows(C_UNITS) if len(r) >= 3})
    groups = {}
    for al, st, en, ins, bk in targets:
        if scores[(al, st)]["outcome"] != "candidate":
            continue
        u = unit_of(units, al, asm.path(al, st))
        groups.setdefault((al, u[0] if u else "~carve_%08x" % st), []).append(st)
    wd = RUN / wave
    bdir = wd / "banks"
    bpath = wd / "banks.tsv"
    head, brows = ledger(bpath)
    head = head or ["# " + "\t".join(BCOLS)]
    for (al, unit), all_sts in sorted(groups.items()):
        snap = tree_snapshot()
        note, sts = None, []
        for st in all_sts:  # T5.c32: a fn whose apply fails is restored and dropped alone, not its whole unit
            pre = tree_snapshot()
            sh([sys.executable, "tools/reconcile.py", str(wd / tdir(al, st) / "draft.c"), "--target",
                "%s:0x%08x" % (al, st), "--apply",
                "--out", str(wd / "_reconcile")])  # rc also reflects other dirs' verdicts (reconcile gate): unused
            rd = wd / "_reconcile" / tdir(al, st)
            v = (rd / "verdict").read_text().strip() if (rd / "verdict").is_file() else "no-verdict"
            if not v.startswith("banked"):
                restore(al, pre)
                lg = (rd / "ladder.log").read_text(errors="replace") if (rd / "ladder.log").is_file() else ""
                vv = v.split()
                scores[(al, st)].update(outcome="plateau", rung=vv[1] if len(vv) > 1 else "-",
                                        reason=" ".join(vv[2:]) or "-", note=apply_note(v, lg))
                print("  apply %s:0x%08x: %s" % (al, st, scores[(al, st)]["note"]))
                continue
            scores[(al, st)]["rung"] = v.split()[1]
            sts.append(st)
        if not sts:
            print("bank %s/%s: no fn applied (%d tried)" % (al, unit, len(all_sts)))
            continue
        now = tree_snapshot()
        changed = sorted(k for k in now if snap.get(k) != now[k])
        gone = sorted(k for k in snap if k not in now)
        if gone or not changed:
            note = "apply-failed: %s" % ("paths removed" if gone else "no tree change")
        if note is None:
            rel = next((k for k in changed if not k.startswith("include/")), None)
            try:
                if rel is None:
                    raise propagate.Fail("only include/ changed")
                propagate.scratch_build(al, rel, (ROOT / rel).read_text())
            except propagate.Fail as e:
                print("  bank %s/%s: %s" % (al, unit, word(e, 200)))
                note = "bank-hash %s" % al
        if note:
            restore(al, snap)
            for st in sts:
                scores[(al, st)].update(outcome="plateau", note=note)
            print("bank %s/%s: red (%s), %d fns restored" % (al, unit, note, len(sts)))
            continue
        k = len(brows) + 1
        bdir.mkdir(parents=True, exist_ok=True)
        with tarfile.open(bdir / ("%d.tar" % k), "w") as tf:
            for c in changed:
                tf.add(str(ROOT / c), arcname=c)
        brows.append([str(k), al, unit, ",".join("0x%08x" % s for s in sts), ",".join(changed), "-"])
        for st in sts:
            scores[(al, st)]["outcome"] = "banked"
        print("bank %s/%s: green, %d fns, %d paths" % (al, unit, len(sts), len(changed)))
    if brows:
        write_ledger(bpath, head, brows)


def after_banks(banked):
    """census refresh, twins rescan (G46), family propagation dry runs; -> members gated green."""
    for cmd, pre in (([sys.executable, "tools/census.py", "--check"], "check:"),
                     ([sys.executable, "tools/twins.py"], "twins:")):
        p = sh(cmd)
        line = [x for x in p.stdout.splitlines() if x.startswith(pre)]
        print(line[-1] if line else "%s rc %d (no %s line)" % (cmd[1], p.returncode, pre))
    fam = {}
    for r in rows(ROOT / "config/families.tsv"):
        if len(r) >= 4:
            fam[(r[2], int(r[3], 16))] = r[1]
    dup = {}
    for r in rows(ROOT / ".run/census/dup.tsv"):
        if len(r) >= 6 and r[3] == "exact" and r[5].isdigit() and int(r[5]) > 1:
            dup[(r[0], int(r[1], 16))] = int(r[5])
    prop = 0
    for al, st in sorted(banked):
        if fam.get((al, st)) == "exemplar":
            p = sh([sys.executable, "tools/propagate.py", "--dry-run", "--family", "%s:0x%08x" % (al, st)])
            m = re.search(r"(\d+) of (\d+) members gated", p.stdout)
            print("propagate %s:0x%08x: %s" % (al, st, m.group(0) if m else "rc %d" % p.returncode))
            prop += int(m.group(1)) if m and p.returncode == 0 else 0
        elif (al, st) not in fam and (al, st) in dup:
            print("propagation candidate: %s:0x%08x (%d members) unregistered" % (al, st, dup[(al, st)]))
    return prop


def plateau_labels(wave, targets, scores):
    """T5.c1: unbanked drafts (plateau / compile-error, not verbatim) get tools/plateau.py's `label:` value, replacing
    the agent's claim; no label line (draft does not compile) -> UNCOMPILED. Serial: plateau scratch is per draft stem."""
    for al, st, en, ins, bk in targets:
        s = scores[(al, st)]
        dc = RUN / wave / tdir(al, st) / "draft.c"
        if s["outcome"] not in ("plateau", "compile-error") or s["reason"] == "verbatim" or not dc.is_file():
            continue
        p = sh([sys.executable, "tools/plateau.py", str(dc), "--target", "%s:0x%08x:0x%08x" % (al, st, en)])
        lab = [x.split(":", 1)[1].strip() for x in p.stdout.splitlines() if x.startswith("label:")]
        s["label"] = word(lab[-1], 40) if p.returncode == 0 and lab else "UNCOMPILED"


def gate_inner(wave):
    """1a + 1b in the volume: score, bank per unit, journal + waves row. -> rc."""
    import json
    targets = read_targets(wave)
    if not targets:
        print("REFUSED: no targets in %s" % (RUN / wave / "targets.tsv"))
        return 2
    bad, scores = score(wave, targets)
    if bad:
        print("REFUSED: gate %s: %d dirs without an agent run" % (wave, len(bad)))
        print("\n".join(bad))
        return 2
    D = len(targets)
    if len(set((t[0], t[1]) for t in targets)) != D or len(scores) != D:
        print("FAIL: coverage: %d scores for %d targets" % (len(scores), D))
        return 1
    cls = {"candidate": "banked"}
    agree = sum(1 for s in scores.values() if s["claim"] == cls.get(s["outcome"], s["outcome"]))
    print("claims: %d of %d agree with the score" % (agree, D))
    if any(s["outcome"] == "candidate" for s in scores.values()):
        msg = draw_filter.ensure()  # dc.sh sync drops asm/: destination units need it
        if msg:
            print(msg)
            return 2
    bank(wave, targets, scores)
    plateau_labels(wave, targets, scores)
    out = Counter(s["outcome"] for s in scores.values())
    B, V = out["banked"], out["no-verdict"]
    F = D - B - V
    if B + F + V != D or out["candidate"]:
        print("FAIL: banked+failed+no-verdict != drafted")
        return 1
    print("gate: banked %d + failed %d + no-verdict %d = %d drafted" % (B, F, V, D))
    banked = [(t[0], t[1]) for t in targets if scores[(t[0], t[1])]["outcome"] == "banked"]
    prop = after_banks(banked) if banked else 0
    jh, jd = ledger(JOURNAL)
    for al, st, en, ins, bk in targets:
        s = scores[(al, st)]
        d = RUN / wave / tdir(al, st)
        d.mkdir(parents=True, exist_ok=True)
        (d / "gate.json").write_text(json.dumps({k: s[k] for k in ("alias", "start", "outcome", "rung", "reason",
                                                                    "label")}) + "\n")
        jd.append([al, "0x%08x" % st, wave, route_of(bk), s["outcome"], s["label"], word(s["note"])])
    write_ledger(JOURNAL, jh, jd)
    wh, wd = ledger(WAVES)
    i = wave_row(wd, wave)
    if i is not None:
        r = wd[i] + ["-"] * (13 - len(wd[i]))
        r[5:11] = [str(D), str(B), str(F), str(V), str(sum(t[3] for t in targets if (t[0], t[1]) in banked)),
                   str(prop)]
        wd[i] = r[:13]
        write_ledger(WAVES, wh, wd)
    return 0


def cmd_gate(a):
    if a.in_volume:
        msg = volume_refusal("gate")
        if msg:
            print(msg)
            return 2
        return gate_inner(a.wave)
    msg = host_refusal("gate")
    if msg:
        print(msg)
        return 2
    if not clean_tree():
        print("REFUSED: working tree not clean (git status --porcelain)")
        return 2
    i = wave_row(ledger(WAVES)[1], a.wave)
    if i is None:
        print("REFUSED: no config/waves.tsv row %s" % a.wave)
        return 2
    if ledger(WAVES)[1][i][5] != "-":
        print("REFUSED: wave %s already gated (drafted %s); no double journal" % (a.wave, ledger(WAVES)[1][i][5]))
        return 2
    rel = ".run/waves/%s" % a.wave
    for cmd in (DC + ["sync"], DC + ["push", rel]):
        rc = sh(cmd, capture=False).returncode
        if rc:
            print("FAIL: %s rc %d" % (" ".join(cmd[1:]), rc))
            return rc
    rc = sh(DC + ["run", "python3", "tools/wave.py", "gate", a.wave, "--in-volume"], capture=False).returncode
    if rc:
        return rc
    rc = pull([rel, "config/journal.tsv", "config/waves.tsv"])
    if rc:
        print("FAIL: pull rc %d" % rc)
        return 1
    rc = commit_banks(a.wave)
    if rc:
        return rc
    r = ledger(WAVES)[1][wave_row(ledger(WAVES)[1], a.wave)]
    msg = "gate: banked %s + failed %s + no-verdict %s = %s drafted" % (r[6], r[7], r[8], r[5])
    return sh(["bash", "tools/commit_task.sh", a.wave, msg, "config/journal.tsv", "config/waves.tsv"],
              capture=False).returncode


def wave_counts(wave, targets, journal):
    """Pure (T5.c32): (B, F, V, insns banked) of a wave from its journal rows: banked = any banked row; else the latest
    row's outcome (no-verdict -> V, rest -> F); a target without a row counts F."""
    last, bk = {}, set()
    for r in ledger(journal)[1]:
        if len(r) >= 5 and r[2] == wave and r[1].startswith("0x"):
            last[(r[0], int(r[1], 16))] = r[4]
            if r[4] == "banked":
                bk.add((r[0], int(r[1], 16)))
    keys = [(t[0], t[1]) for t in targets]
    B = sum(1 for k in keys if k in bk)
    V = sum(1 for k in keys if k not in bk and last.get(k) == "no-verdict")
    return B, len(keys) - B - V, V, sum(t[3] for t in targets if (t[0], t[1]) in bk)


def recover_inner(wave):
    """T5.c32, in the volume: re-score every target with no banked journal row (fixed scorer), bank per unit, append
    journal rows (route `recover`), recount the waves row from the journal. -> rc."""
    import json
    targets = read_targets(wave)
    if not targets:
        print("REFUSED: no targets in %s" % (RUN / wave / "targets.tsv"))
        return 2
    done = {(r[0], int(r[1], 16)) for r in ledger(JOURNAL)[1]
            if len(r) >= 5 and r[2] == wave and r[4] == "banked" and r[1].startswith("0x")}
    todo = [t for t in targets if (t[0], t[1]) not in done]
    print("recover %s: %d of %d targets not banked" % (wave, len(todo), len(targets)))
    bad, scores = score(wave, targets, {(t[0], t[1]) for t in todo})
    if bad:
        print("REFUSED: recover %s: %d dirs without an agent run" % (wave, len(bad)))
        print("\n".join(bad))
        return 2
    if any(s["outcome"] == "candidate" for s in scores.values()):
        msg = draw_filter.ensure()
        if msg:
            print(msg)
            return 2
    bank(wave, todo, scores)
    plateau_labels(wave, todo, scores)
    if any(s["outcome"] == "candidate" for s in scores.values()):
        print("FAIL: a candidate left unbanked and unscored")
        return 1
    banked = [(t[0], t[1]) for t in todo if scores[(t[0], t[1])]["outcome"] == "banked"]
    prop = after_banks(banked) if banked else 0
    jh, jd = ledger(JOURNAL)
    for al, st, en, ins, bk in todo:
        s = scores[(al, st)]
        (RUN / wave / tdir(al, st) / "recover.json").write_text(
            json.dumps({k: s[k] for k in ("alias", "start", "outcome", "rung", "reason", "label")}) + "\n")
        jd.append([al, "0x%08x" % st, wave, "recover", s["outcome"], s["label"], word(s["note"])])
    write_ledger(JOURNAL, jh, jd)
    B, F, V, ins = wave_counts(wave, targets, JOURNAL)
    wh, wd = ledger(WAVES)
    i = wave_row(wd, wave)
    if i is not None:
        r = wd[i] + ["-"] * (13 - len(wd[i]))
        p0 = int(r[10]) if r[10].isdigit() else 0
        r[5:11] = [str(len(targets)), str(B), str(F), str(V), str(ins), str(p0 + prop)]
        wd[i] = r[:13]
        write_ledger(WAVES, wh, wd)
    print("recover: banked %d of %d re-scored" % (len(banked), len(todo)))
    print("gate: banked %d + failed %d + no-verdict %d = %d drafted" % (B, F, V, len(targets)))
    return 0


def cmd_recover(a):
    """Host driver (T5.c32): as gate, on a gated wave: sync, push, recover --in-volume, pull, commit banks + ledgers."""
    if a.in_volume:
        msg = volume_refusal("recover")
        if msg:
            print(msg)
            return 2
        return recover_inner(a.wave)
    msg = host_refusal("recover") or (None if clean_tree() else "REFUSED: working tree not clean (git status --porcelain)")
    i = wave_row(ledger(WAVES)[1], a.wave)
    if msg is None and (i is None or ledger(WAVES)[1][i][5] == "-"):
        msg = "REFUSED: wave %s not gated (config/waves.tsv drafted -); run gate first" % a.wave
    if msg:
        print(msg)
        return 2
    rel = ".run/waves/%s" % a.wave
    for cmd in (DC + ["sync"], DC + ["push", rel]):
        rc = sh(cmd, capture=False).returncode
        if rc:
            print("FAIL: %s rc %d" % (" ".join(cmd[1:]), rc))
            return rc
    rc = sh(DC + ["run", "python3", "tools/wave.py", "recover", a.wave, "--in-volume"], capture=False).returncode
    if rc:
        return rc
    rc = pull([rel, "config/journal.tsv", "config/waves.tsv"])
    if rc:
        print("FAIL: pull rc %d" % rc)
        return 1
    rc = commit_banks(a.wave)
    if rc:
        return rc
    r = ledger(WAVES)[1][wave_row(ledger(WAVES)[1], a.wave)]
    msg = "recover: banked %s + failed %s + no-verdict %s = %s drafted" % (r[6], r[7], r[8], r[5])
    return sh(["bash", "tools/commit_task.sh", a.wave, msg, "config/journal.tsv", "config/waves.tsv"],
              capture=False).returncode


def commit_banks(wave):
    """1c: extract every uncommitted bank tar at the repo root and commit it; commit hash into banks.tsv."""
    import tarfile
    bpath = RUN / wave / "banks.tsv"
    head, brows = ledger(bpath)
    for j, r in enumerate(brows):
        if r[5] != "-":
            continue
        with tarfile.open(RUN / wave / "banks" / ("%s.tar" % r[0])) as tf:
            tf.extractall(ROOT, filter="data")
        n = len(r[3].split(","))
        rc = sh(["bash", "tools/commit_task.sh", wave, "bank %s/%s: %d fns, sha1 ok" % (r[1], r[2], n)]
                + r[4].split(","), capture=False).returncode
        if rc:
            print("FAIL: bank commit %s rc %d; remaining: %s" % (r[0], rc, " ".join(x[0] for x in brows[j:])))
            write_ledger(bpath, head, brows)
            return 1
        r[5] = short_head()
        write_ledger(bpath, head, brows)
    return 0


def banks_unreached(run, wave, anc, ref):
    """-> refusal lines for banks.tsv rows not committed or not ancestor-or-equal of ref."""
    out = []
    for r in ledger(run / wave / "banks.tsv")[1]:
        if len(r) < 6 or r[5] == "-":
            out.append("uncommitted bank: %s" % (r[0] if r else "?"))
        elif not anc(r[5], ref):
            out.append("bank %s commit %s not an ancestor of %s" % (r[0], r[5], ref))
    return out


def cmd_fleet(a):
    msg = host_refusal("fleet")
    if msg:
        print(msg)
        return 2
    wd = ledger(WAVES)[1]
    i = wave_row(wd, a.wave)
    bad = [] if i is not None and wd[i][5] != "-" else ["gate not done: %s" % a.wave]
    if not clean_tree():
        bad.append("working tree not clean")
    bad += banks_unreached(RUN, a.wave, git_anc, "HEAD")
    if bad:
        print("\n".join("REFUSED: " + b for b in bad))
        return 2
    head = short_head()
    rc = sh(DC + ["sync"], capture=False).returncode
    if rc:
        return rc
    log = RUN / a.wave / "fleet.log"
    lines = []
    p = subprocess.Popen(DC + ["run", "bash", "tools/fleet_check.sh"], cwd=ROOT, stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT, text=True)
    with open(log, "w") as fh:
        for line in p.stdout:
            fh.write(line)
            sys.stdout.write(line)
            lines.append(line.rstrip("\n"))
    rc = p.wait()
    text = "\n".join(lines)
    m = re.search(r"^(\d+) of (\d+) byte-identical$", text, re.M)
    h = re.search(r"^harness: (\d+) of \1 pairs agree, 0 disagreements$", text, re.M)
    if rc or not m or not (int(m.group(1)) == int(m.group(2)) == FLEET_N) or not h:
        print("fleet: red (rc %d); nothing written (%s)" % (rc, log.relative_to(ROOT)))
        return 1
    wh, wd = ledger(WAVES)
    wd[i][11] = "%d/%d@%s" % (FLEET_N, FLEET_N, head)
    write_ledger(WAVES, wh, wd)
    print("fleet: %s written to config/waves.tsv (uncommitted; close commits it)" % wd[i][11])
    return 0


def lever_ids():
    return {r[0] for r in rows(LEVERS) if r}


def harvest_refusal(wave, alias, start, text, lever, stripped, journal, levers):
    """-> (refusal|None, journal row index, note). Pure over the journal path and the lever id set."""
    jd = ledger(journal)[1]
    i = next((k for k, r in enumerate(jd) if len(r) >= 5 and r[0] == alias and r[2] == wave and r[4] == "banked"
              and r[1].startswith("0x") and int(r[1], 16) == start), None)
    note = ("strip:ok:%s %s" % (lever, text)) if lever else ("strip:none %s" % text)
    if i is None:
        return "REFUSED: notes of unbanked drafts refused (%s:0x%08x in %s)" % (alias, start, wave), None, note
    if "\t" in text or "\n" in text:
        return "REFUSED: note has a tab or newline", i, note
    if len(note) > 120:
        return "REFUSED: note %d chars > 120" % len(note), i, note
    if lever and lever not in levers and not (re.match(r"^C\d{4}$", lever) and (ROOT / "cookbook" /
                                                                                ("%s.md" % lever)).is_file()):
        return "REFUSED: lever %s does not resolve (config/levers.tsv, cookbook/C<nnnn>.md; G44)" % lever, i, note
    if lever and not stripped:
        return "REFUSED: --lever needs --stripped PATH", i, note
    return None, i, note


def parse_fn(s):
    al, st = s.split(":")
    return al, int(st, 16)


def cmd_harvest(a):
    msg = host_refusal("harvest")
    if msg:
        print(msg)
        return 2
    al, st = parse_fn(a.fn)
    msg, i, note = harvest_refusal(a.wave, al, st, a.note, a.lever, a.stripped, JOURNAL, lever_ids())
    if msg:
        print(msg)
        return 2
    if a.lever:
        rel = ".run/waves/%s/%s/stripped.c" % (a.wave, tdir(al, st))
        (ROOT / rel).parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(a.stripped, ROOT / rel)
        for cmd in (DC + ["sync"], DC + ["push", ".run/waves/%s" % a.wave]):
            rc = sh(cmd, capture=False).returncode
            if rc:
                return rc
        rc = sh(DC + ["run", "python3", "tools/wave.py", "strip", "--fn", a.fn, "--stripped", rel, "--in-volume"],
                capture=False).returncode
        if rc == 1:
            print("harvest: lever %s inert (G45); credit refused, nothing written" % a.lever)
            return 1
        if rc:
            return rc
    jh, jd = ledger(JOURNAL)
    jd[i][6] = note
    write_ledger(JOURNAL, jh, jd)
    print("harvest: %s note written (uncommitted; close commits it)" % a.fn)
    return 0


def cmd_strip(a):
    """Inner: the unit with func_<ADDR>'s definition replaced by the stripped one, scratch-built; differs = lever."""
    msg = volume_refusal("strip") if a.in_volume else "REFUSED: strip runs only with --in-volume (container)"
    if msg:
        print(msg)
        return 2
    import propagate
    import reconcile
    propagate.SCRATCH = RUN / "_scratch"
    al, st = parse_fn(a.fn)
    name = "func_%08X" % st
    stext = Path(a.stripped).read_text()
    sdef = next((stext[b:e] for n, b, e in reconcile.fn_spans(stext) if n == name), None)
    if sdef is None:
        print("REFUSED: %s defines no %s" % (a.stripped, name))
        return 2
    for p in sorted((ROOT / "src" / al).glob("*.c")):
        text = p.read_text()
        span = next(((b, e) for n, b, e in reconcile.fn_spans(text) if n == name), None)
        if span:
            break
    else:
        print("REFUSED: no unit in src/%s defines %s" % (al, name))
        return 2
    rel = str(p.relative_to(ROOT))
    try:
        propagate.scratch_build(al, rel, text[:span[0]] + sdef + text[span[1]:])
    except propagate.Fail as e:
        bad = propagate.SCRATCH / al / "build" / ("%s.bin.bad" % al)  # the Makefile's sha1 check renames a mismatch
        if "make rc 0" in str(e) or bad.is_file():
            print("strip: differs")
            return 0
        print("strip: build-error (%s)" % word(e, 200))
        return 2
    print("strip: identical")
    return 1


def close_refusals(wave, waves, journal, run, anc):
    """-> (refusals, row index, banked rows n, fleet commit). Pure over ledger paths and anc(a, b) (git ancestry;
    anc(c, c) = c resolves)."""
    wd = ledger(waves)[1]
    i = wave_row(wd, wave)
    if i is None or len(wd[i]) < 13 or wd[i][5] == "-":
        return ["gate not done: %s" % wave], i, 0, None
    r = wd[i]
    out = ["already closed: %s %s" % (wave, r[3])] if r[3] not in ("", "-") else []
    m = FLEET_RE.match(r[11])
    c = m.group(1) if m else None
    if c is None or not anc(c, c):
        out.append("fleet not %d/%d@<commit>: %s" % (FLEET_N, FLEET_N, r[11]))
    else:
        out += banks_unreached(run, wave, anc, c)
    jd = [x for x in ledger(journal)[1] if len(x) >= 7 and x[2] == wave and x[4] == "banked"]
    out += ["unharvested: %s:%s" % (x[0], x[1]) for x in jd if not x[6].startswith("strip:")]
    return out, i, len(jd), c


def cmd_close(a):
    msg = host_refusal("close")
    if msg:
        print(msg)
        return 2
    bad, i, n, c = close_refusals(a.wave, WAVES, JOURNAL, RUN, git_anc)
    if bad:
        print("\n".join("REFUSED: " + b for b in bad))
        return 2
    wh, wd = ledger(WAVES)
    wd[i][3] = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%d")
    wd[i][12] = "done:%d notes" % n
    write_ledger(WAVES, wh, wd)
    msg = "close: banked %s, harvest done:%d, fleet %d/%d@%s" % (wd[i][6], n, FLEET_N, FLEET_N, c)
    return sh(["bash", "tools/commit_task.sh", a.wave, msg, "config/waves.tsv", "config/journal.tsv"],
              capture=False).returncode


def plant(d, wave, note):
    """A gated wave with one banked row (journal note `note`), a valid fleet and one committed bank."""
    d.mkdir(parents=True, exist_ok=True)
    write_ledger(d / "waves.tsv", ["# " + "\t".join(WCOLS)],
                 [[wave, "manual", "2026-01-01", "-", "1", "1", "1", "0", "0", "4", "0", "83/83@abc1234", "open"]])
    write_ledger(d / "journal.tsv", ["# " + "\t".join(JCOLS)],
                 [["slus_012_79", "0x80000000", wave, "-", "banked", "-", note]])
    (d / wave).mkdir(exist_ok=True)
    write_ledger(d / wave / "banks.tsv", ["# " + "\t".join(BCOLS)],
                 [["1", "slus_012_79", "game_x", "0x80000000", "src/slus_012_79/game_x.c", "abc1234"]])


def control():
    d = RUN / "_control"
    plant(d, "c1", "rung as-is")
    bad = close_refusals("c1", d / "waves.tsv", d / "journal.tsv", d, lambda x, y: True)[0]
    return any(b.startswith("unharvested: ") for b in bad)


def cmd_check():
    led = [(WAVES, WCOLS), (JOURNAL, JCOLS)] + ([(ROUTING, RCOLS)] if ROUTING.exists() else [])
    k = sum(1 for p, c in led if header_ok(p, c))
    print("wave ledgers: %d of %d parse" % (k, len(led)))
    ok = k == len(led)
    wd = ledger(WAVES)[1]
    manual = {r[0] for r in wd if len(r) >= 13 and r[1] == "manual" and r[3] not in ("", "-")}
    rd = ledger(ROUTING)[1]
    m = 0
    for r in rd:
        r = r + [""] * (11 - len(r))
        try:
            float(r[8])
            m += int(r[6]) > 0 and r[10] in manual
        except ValueError:
            pass
    print("routing: %d of %d buckets measured on the manual wave" % (m, len(rd)))
    c = sum(1 for r in wd if len(r) >= 13 and r[3] not in ("", "-") and HARVEST_RE.match(r[12]))
    print("waves: %d of %d closed with harvest" % (c, len(wd)))
    f = sum(1 for r in wd if len(r) >= 13 and FLEET_RE.match(r[11]))
    print("fleet: %d of %d banked batches followed by a clean fleet check" % (f, len(wd)))
    ctl = control()
    print("wave control: %s" % ("ok" if ctl else "FAIL"))
    if not ok:
        print("FAIL: wave ledger header/row shape")
    return 0 if ok and ctl else 1


def cmd_selftest():
    import contextlib
    import io
    import json
    global WAVES, JOURNAL, RUN

    def cfg_hash():
        return hashlib.sha256(b"".join(p.read_bytes() for p in sorted((ROOT / "config").rglob("*"))
                                       if p.is_file())).hexdigest()

    before = cfg_hash()
    sroot = ROOT / ".run/waves/_selftest"
    shutil.rmtree(sroot, ignore_errors=True)
    saved = (WAVES, JOURNAL, RUN)
    RUN, WAVES, JOURNAL = sroot, sroot / "waves.tsv", sroot / "journal.tsv"
    ok = []
    try:
        # (1) unharvested bank refused; with a strip: note it passes (positive control)
        plant(sroot, "s1", "rung as-is")
        r1 = close_refusals("s1", WAVES, JOURNAL, RUN, lambda x, y: True)[0]
        plant(sroot, "s1", "strip:none positive control")
        r2 = close_refusals("s1", WAVES, JOURNAL, RUN, lambda x, y: True)[0]
        ok.append(any(b.startswith("unharvested: ") for b in r1) and not r2)
        if ok[-1]:
            print("selftest unharvested bank: refused ok")
        # (2) a dir without an agent run refuses the gate; dropped, the gate counts the no-verdict
        shutil.rmtree(sroot)
        sroot.mkdir(parents=True)
        write_ledger(WAVES, ["# " + "\t".join(WCOLS)],
                     [["s2", "manual", "2026-01-01", "-", "2"] + ["-"] * 7 + ["open"]])
        write_ledger(JOURNAL, ["# " + "\t".join(JCOLS)], [])
        A, B = ("slus_012_79", 0x80000000), ("slus_012_79", 0x80000010)
        wd = sroot / "s2"
        for t in (A, B):
            (wd / tdir(*t)).mkdir(parents=True)
        (wd / tdir(*A) / "verdict.json").write_text(json.dumps({"alias": A[0], "start": "0x%08x" % A[1],
                                                                "status": "no-verdict", "rung": "-", "label": ""}))
        (wd / tdir(*B) / "card.md").write_text("# card\n")
        line = "%s\t0x%08x\t0x%08x\t4\t1-16\n"
        (wd / "targets.tsv").write_text("# alias\tstart\tend\tinsns\tbucket\n" + line % (A + (A[1] + 16,))
                                        + line % (B + (B[1] + 16,)))
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            rc = gate_inner("s2")
        refused = rc == 2 and ("missing verdict: %s" % tdir(*B)) in buf.getvalue()
        shutil.rmtree(wd / tdir(*B))
        (wd / "targets.tsv").write_text("# alias\tstart\tend\tinsns\tbucket\n" + line % (A + (A[1] + 16,)))
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            rc = gate_inner("s2")
        counted = rc == 0 and "gate: banked 0 + failed 0 + no-verdict 1 = 1 drafted" in buf.getvalue() \
            and len(ledger(JOURNAL)[1]) == 1
        ok.append(refused and counted)
        if ok[-1]:
            print("selftest missing-verdict dir: refused ok")
        # (3) a harvest note on the unbanked A is refused
        msg = harvest_refusal("s2", A[0], A[1], "note", None, None, JOURNAL, lever_ids())[0]
        ok.append(bool(msg) and "unbanked" in msg)
        if ok[-1]:
            print("selftest unbanked note: refused ok")
        # (4) build-error outcome map over planted make logs (our words only)
        mism = ("cc1 -quiet -O2 ... src/x/y.c\nld -o build/x.elf\nFAILED sha1: x (build/x.bin.bad)\n"
                "make: *** [Makefile:122: build/x.bin] Error 1\n")
        cerr = "src/x/y.c:3: cc1: error: planted parse error before `}'\nmake: *** [build/x/y.o] Error 1\n"
        ok.append(build_outcome("as-is", mism) == ("plateau", "hash-mismatch as-is")
                  and build_outcome("as-is", cerr)[0] == "compile-error" and build_outcome("as-is", "")[0]
                  == "compile-error")
        if ok[-1]:
            print("selftest outcome map: ok")
        # (5) T5.c32: an apply that does not bank names its cause (M1 gate: bare `apply-failed`, cause logged nowhere);
        # a carve inside an enclosing unit prunes that unit's text to its range (M1: INCLUDE_ASM of moved .s files)
        lad = ("body hash before: 0123\nrung carve: refused make build ONLY=x rc 2: make: *** No rule to make target "
               "'asm/x/x_1f.s'\n")
        an = apply_note("failed carve refused", lad)
        import carve
        unit = ('#include "common.h"\nINCLUDE_ASM("asm/x/nonmatchings/u", func_80000000);\n\nvoid func_80000010(void)'
                ' {\n}\n\nINCLUDE_ASM("asm/x/nonmatchings/u", func_80000020);\n')
        lo_t, hi_t = carve.prune(unit, 0x80000000, 0x80000010), carve.prune(unit, 0x80000020, 0x80000030)
        ok.append(an.startswith("apply-failed failed carve refused; rung carve: refused") and "No rule" in an
                  and len(an) <= 120 and apply_note("no-verdict", "") == "apply-failed no-verdict; no rung line"
                  and "func_80000000" in lo_t and "func_80000010" not in lo_t and "func_80000020" not in lo_t
                  and "func_80000020" in hi_t and "func_80000010" not in hi_t and "common.h" in hi_t)
        if ok[-1]:
            print("selftest apply cause + enclosing-unit prune: ok")
        # (6) T5.c32: recover recounts the waves row from the journal (a later banked row wins over the gate's plateau)
        write_ledger(JOURNAL, ["# " + "\t".join(JCOLS)],
                     [[A[0], "0x%08x" % A[1], "s6", "draft", "plateau", "MATCH", "apply-failed"],
                      [B[0], "0x%08x" % B[1], "s6", "draft", "no-verdict", "-", "-"],
                      [A[0], "0x%08x" % A[1], "s6", "recover", "banked", "MATCH", "rung carve"]])
        tg = [A + (A[1] + 16, 4, "1-16"), B + (B[1] + 16, 4, "1-16"), (A[0], 0x80000020, 0x80000030, 4, "1-16")]
        ok.append(wave_counts("s6", tg, JOURNAL) == (1, 1, 1, 4))
        if ok[-1]:
            print("selftest recover counts: ok")
    finally:
        WAVES, JOURNAL, RUN = saved
    same = cfg_hash() == before
    if not same:
        print("FAIL: config/ changed during the selftest")
    n = sum(ok) if same else 0
    print("selftest: %d of 6 ok" % n)
    return 0 if n == 6 else 1


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--check", action="store_true", help="ledger counts + close-refusal control (tracked config only)")
    ap.add_argument("--selftest", action="store_true", help="scratch-only refusals under .run/waves/_selftest/")
    sub = ap.add_subparsers(dest="cmd")
    for n in ("gate", "recover", "fleet", "close"):
        s = sub.add_parser(n)
        s.add_argument("wave")
        if n in ("gate", "recover"):
            s.add_argument("--in-volume", action="store_true")
    h = sub.add_parser("harvest")
    h.add_argument("wave")
    h.add_argument("--fn", required=True)
    h.add_argument("--note", required=True)
    h.add_argument("--lever")
    h.add_argument("--stripped")
    s = sub.add_parser("strip")
    s.add_argument("wave", nargs="?", help="unused; accepted for symmetry")
    s.add_argument("--fn", required=True)
    s.add_argument("--stripped", required=True)
    s.add_argument("--in-volume", action="store_true")
    d = sub.add_parser("draw")
    d.add_argument("--kind", required=True, choices=KINDS)
    d.add_argument("--weight", required=True, type=int)
    d.add_argument("--binary")
    d.add_argument("--seed", default="0")
    d.add_argument("--dry-run", action="store_true")
    d.add_argument("--per-bucket", type=int, help="manual: a bucket stops taking once it holds N (weight stays a cap)")
    d.add_argument("--family", help="F[,F...]: pool restricted to these census family values (unknown -> rc 2)")
    d.add_argument("--id", help="wave id (default w<NN>); refused rc 2 if in config/waves.tsv or its dir exists")
    c = sub.add_parser("cards")
    c.add_argument("wave", nargs="?")
    c.add_argument("--dry-run", action="store_true")
    a = ap.parse_args(argv)
    if a.check:
        return cmd_check()
    if a.selftest:
        return cmd_selftest()
    if a.cmd is None:
        ap.error("a subcommand, --check or --selftest is required")
    if getattr(a, "wave", None) and not WAVE_ID.match(a.wave):
        ap.error("wave id must match [A-Za-z0-9_]+")
    return {"draw": cmd_draw, "cards": cmd_cards, "gate": cmd_gate, "recover": cmd_recover, "fleet": cmd_fleet,
            "harvest": cmd_harvest,
            "strip": cmd_strip, "close": cmd_close}[a.cmd](a)


if __name__ == "__main__":
    sys.exit(main())
