#!/usr/bin/env python3
"""tools/permuter/permuter_run.py -- in-process decomp-permuter driver (T6, Phase 1.7); spawned by tools/permute.py.

Run under /opt/permuter/bin/python (the pinned venv; checkout /opt/permuter/decomp-permuter, never patched). Rebinds
the permuter's `Scorer` to MaskedScorer and its `Permuter` to a subclass that logs every candidate unbuffered to
<dir>/candidates.log and stops after exactly --iters iterations; single worker (threads=1); `random` seeded with
--seed (the permuter's own --seed is its reproduce mode: it always keeps one candidate, so it is not used).

Scorer (also imported by tools/permute.py for the scorer control, system python): words, masks from tools/probe.py
`extract`; per word index the mask is the OR of both sides' masks; score = Levenshtein distance (insert/delete/replace
cost 1) over the two masked word sequences; 0 iff same length and probe.compare is None. Compile or extract failure =
not judged: logged with its reason, scored PENALTY_INF to the permuter, never counted as a score.

candidates.log: `iter<TAB>status<TAB>score<TAB>sha1` (iter 0 = base; status judged | judged-cached |
not-judged:<reason>; score `-` when not judged; sha1 of the candidate source, `-` when there is none).
  permuter_run.py <dir> --fn <func_name> --iters N --seed S
"""
import argparse
import hashlib
import random
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import probe  # noqa: E402

PERMUTER = "/opt/permuter/decomp-permuter"
PENALTY_INF = 10**9


def masked(a, b):
    """Masked word sequences of extract results a, b (mask = OR of both sides' masks at the same index)."""
    (wa, ma), (wb, mb) = a[:2], b[:2]
    def one(w, i):
        return w[i] & ~(ma.get(i, 0) | mb.get(i, 0)) & 0xFFFFFFFF
    return [one(wa, i) for i in range(len(wa))], [one(wb, i) for i in range(len(wb))]


def distance(a, b):
    """Levenshtein distance over the masked words of extract results a (candidate), b (target)."""
    x, y = masked(a, b)
    prev = list(range(len(y) + 1))
    for i, xi in enumerate(x, 1):
        cur = [i]
        for j, yj in enumerate(y, 1):
            cur.append(min(prev[j] + 1, cur[j - 1] + 1, prev[j - 1] + (xi != yj)))
        prev = cur
    d = prev[-1]
    assert (d == 0) == (len(x) == len(y) and probe.compare(a, b) is None), "scorer disagrees with probe.compare"
    return d


def score_o(o, fn, target):
    """(score, None, hash of the candidate's words) or (None, reason, None) for object o against target extract."""
    try:
        cand = probe.extract(Path(o), fn)
    except RuntimeError as e:
        return None, "extract: " + str(e).splitlines()[0][:120], None
    finally:
        Path(o).with_suffix(".text.bin").unlink(missing_ok=True)
    return distance(cand, target), None, hashlib.sha256(repr(cand[:2]).encode()).hexdigest()


class _Done(BaseException):
    pass


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("dir")
    ap.add_argument("--fn", required=True)
    ap.add_argument("--iters", type=int, required=True)
    ap.add_argument("--seed", type=int, required=True)
    a = ap.parse_args()
    sys.path.insert(0, PERMUTER)
    import src.main as pm  # noqa: E402
    from src.permuter import EvalError, Permuter  # noqa: E402

    target = probe.extract(Path(a.dir) / "target.o", a.fn)
    log = open(Path(a.dir) / "candidates.log", "w", buffering=1)

    class MaskedScorer:
        PENALTY_INF = PENALTY_INF

        def __init__(self, target_o, **_kw):
            self.last = None

        def score(self, cand_o):
            if cand_o is None:
                self.last = ("not-judged:compile failed", None)
                return PENALTY_INF, ""
            s, why, h = score_o(cand_o, a.fn, target)
            if s is None:
                self.last = ("not-judged:" + why, None)
                return PENALTY_INF, ""
            self.last = ("judged", s)
            return s, h

    class LoggingPermuter(Permuter):
        def __init__(self, *args, **kw):
            self.n = 0
            self.status = {}
            super().__init__(*args, **kw)
            self.write(0, self.scorer.last, hashlib.sha1(self.base_source.encode()).hexdigest())

        def write(self, i, last, sha):
            status, s = last
            self.status[sha] = last
            log.write(f"{i}\t{status}\t{'-' if s is None else s}\t{sha}\n")
            log.flush()

        def try_eval_candidate(self, seed):
            if self.n >= a.iters:
                raise _Done()
            self.scorer.last = None
            r = super().try_eval_candidate(seed)
            self.n += 1
            if isinstance(r, EvalError):
                why = (r.exc_str or "compile failed").strip().splitlines()[-1][:120].replace("\t", " ")
                status = ("not-judged:eval " + why, None)
                log.write(f"{self.n}\t{status[0]}\t-\t-\n")
                log.flush()
                return r
            sha = hashlib.sha1(self._cur_cand.get_source().encode()).hexdigest()
            last = self.scorer.last
            if last is None:  # the permuter's per-source score cache: not recompiled
                prior = self.status.get(sha, ("not-judged:uncached repeat", None))
                last = ("judged-cached", prior[1]) if prior[0] == "judged" else prior
            self.write(self.n, last, sha)
            return r

    pm.Scorer, pm.Permuter = MaskedScorer, LoggingPermuter
    random.seed(a.seed)
    opts = pm.Options(directories=[a.dir], threads=1)
    try:
        pm.run_inner(opts, lambda: None)
    except _Done:
        pass
    log.close()


if __name__ == "__main__":
    main()
