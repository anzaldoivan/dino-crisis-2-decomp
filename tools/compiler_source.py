#!/usr/bin/env python3
"""compiler_source.py -- the vanilla compiler source tree, staged under .run/ (T1, Phase 1.7).

  --fetch  download each config/compiler_source.tsv row (stdlib only), sha256 fail-closed, keep the tarball next to
           dest (.run/compiler-src/<file>), extract to dest. On the host it then fetches the same into the container's
           /work/.run/compiler-src/ (dc.sh sync never carries .run/): `dc.sh run sh -c 'curl … && sha256sum -c && tar'`.
  --check  rehash the tarball -> `compiler source: gcc 2.95.2 sha256 ok`; compare three version sources (gcc/version.c
           string, pinned cc1 banner read live, config/toolchains.tsv pinned row gcc_version) ->
           `version sources: 3 of 3 agree`; a planted wrong version.c copy under .run/ must be refused ->
           `version control: ok`. rc 1 on any failure.
Banner: the Makefile's CC1 (`make print-c`, G69) run with `-version`. Host: via `bash tools/docker/dc.sh run`; inside the
container (/opt/cc exists) directly, never dc.sh (C0044). The tree and dumps stay under .run/ (G12).
"""
import argparse
import hashlib
import os
import re
import shutil
import subprocess
import sys
import tarfile
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TSV = ROOT / "config/compiler_source.tsv"
TOOLCHAINS = ROOT / "config/toolchains.tsv"
IN_CONTAINER = Path("/opt/cc").is_dir()
ALIAS = "slus_012_79"  # any alias: print-c's CC1 is the pinned cc1 for every alias
BANNER_SH = ('eval "$(make -s --no-print-directory print-c A=%s)" && echo "int g;" > /tmp/cs_banner.i '
             '&& $CC1 -version /tmp/cs_banner.i -o /dev/null' % ALIAS)


def rows():
    out = []
    for line in TSV.read_text().splitlines():
        if line.strip() and not line.startswith("#"):
            name, version, url, sha, dest = line.split("\t")
            out.append(dict(name=name, version=version, url=url, sha=sha, dest=ROOT / dest))
    return out


def tarball(r):
    return r["dest"].parent / r["url"].rsplit("/", 1)[1]


def sha256(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def fetch(r):
    tb = tarball(r)
    tb.parent.mkdir(parents=True, exist_ok=True)
    if not (tb.exists() and sha256(tb) == r["sha"]):
        part = tb.with_name(tb.name + ".part")
        with urllib.request.urlopen(r["url"], timeout=120) as resp, open(part, "wb") as f:
            shutil.copyfileobj(resp, f)
        got = sha256(part)
        if got != r["sha"]:
            part.unlink()
            print(f"sha256 mismatch for {r['name']} {r['version']}: got {got}", file=sys.stderr)
            return False
        part.replace(tb)
    if not r["dest"].is_dir():
        tmp = r["dest"].parent / (r["dest"].name + ".extract")
        shutil.rmtree(tmp, ignore_errors=True)
        with tarfile.open(tb) as t:
            try:
                t.extractall(tmp, filter="data")
            except TypeError:  # python without extraction filters
                t.extractall(tmp)
        (tmp / r["dest"].name).replace(r["dest"])
        shutil.rmtree(tmp)
    print(f"fetched: {r['name']} {r['version']} -> {r['dest'].relative_to(ROOT)}")
    if IN_CONTAINER:
        return True
    rel_tb, rel_dest = tarball(r).relative_to(ROOT), r["dest"].relative_to(ROOT)
    sh = (f'set -e; mkdir -p {rel_tb.parent}; '
          f'[ -f {rel_tb} ] && echo "{r["sha"]}  {rel_tb}" | sha256sum -c --quiet || '
          f'{{ curl -fsSL --retry 3 -o {rel_tb}.part {r["url"]} && echo "{r["sha"]}  {rel_tb}.part" | sha256sum -c --quiet '
          f'&& mv {rel_tb}.part {rel_tb}; }}; '
          f'[ -d {rel_dest} ] || tar -xzf {rel_tb} -C {rel_dest.parent}; echo "container: {rel_dest} ok"')
    rc = subprocess.run(["bash", str(ROOT / "tools/docker/dc.sh"), "run", "sh", "-c", sh]).returncode
    if rc:
        print(f"container fetch failed for {r['name']} (rc {rc})", file=sys.stderr)
    return rc == 0


def source_version(version_c):
    m = re.search(r'version_string\s*=\s*"([^"]*)"', Path(version_c).read_text(errors="replace"))
    return " ".join(m.group(1).split()[:2]) if m else None


def banner_version():
    cmd = ["sh", "-c", BANNER_SH] if IN_CONTAINER else ["bash", str(ROOT / "tools/docker/dc.sh"), "run", "sh", "-c", BANNER_SH]
    p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    m = re.search(r"^GNU C version (\S+ \S+)", p.stdout + p.stderr, re.M)
    if not m:
        print(f"banner: none (rc {p.returncode})", file=sys.stderr)
    return m.group(1) if m else None


def pinned_gcc_version():
    for line in TOOLCHAINS.read_text().splitlines():
        f = line.split("\t")
        if not line.startswith("#") and len(f) >= 7 and f[6] not in ("", "-"):
            return f[4]
    return None


def agree(ref, src, ban, tc):
    """Count of the three sources agreeing with ref (version) and with each other (src and ban: version + date)."""
    ok_src = bool(src) and src.split()[0] == ref
    ok_ban = bool(ban) and ban.split()[0] == ref and ban == src
    ok_tc = tc == ref
    return int(ok_src) + int(ok_ban) + int(ok_tc)


def check(r):
    ok = True
    tb = tarball(r)
    if tb.exists() and sha256(tb) == r["sha"]:
        print(f"compiler source: {r['name']} {r['version']} sha256 ok")
    else:
        print(f"compiler source: {r['name']} {r['version']} sha256 FAIL ({tb.relative_to(ROOT)})")
        ok = False
    vc = r["dest"] / "gcc/version.c"
    if not vc.is_file():
        print(f"version sources: missing {vc.relative_to(ROOT)}")
        return False
    src, ban, tc = source_version(vc), banner_version(), pinned_gcc_version()
    print(f"version.c: {src!r}; cc1 banner: {ban!r}; toolchains.tsv gcc_version: {tc!r}")
    n = agree(r["version"], src, ban, tc)
    print(f"version sources: {n} of 3 agree")
    ok &= n == 3
    # control: a scratch version.c with a planted wrong version string must be refused
    plant = ROOT / ".run/compiler-src/plant/version.c"
    plant.parent.mkdir(parents=True, exist_ok=True)
    plant.write_text(vc.read_text(errors="replace").replace(r["version"], "2.95.3", 1))
    psrc = source_version(plant)
    refused = psrc != src and agree(r["version"], psrc, ban, tc) < 3
    print(f"planted version.c: {psrc!r} {'refused' if refused else 'accepted'}")
    print(f"version control: {'ok' if refused else 'FAIL'}")
    return ok and refused


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--fetch", action="store_true")
    g.add_argument("--check", action="store_true")
    a = ap.parse_args()
    os.chdir(ROOT)
    good = all([fetch(r) if a.fetch else check(r) for r in rows()])
    return 0 if good else 1


if __name__ == "__main__":
    sys.exit(main())
