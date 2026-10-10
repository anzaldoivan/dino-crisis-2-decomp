#!/usr/bin/env python3
"""extract_disc.py — our own extractor of the retail disc (stdlib only, Python >= 3.12).

Reads the cue, verifies the medium against config/medium.sha1 (any mismatch or missing file -> exit 1),
walks the ISO9660 tree of Track 1 (MODE2/2352) from the PVD at LBA 16 and writes:
  <out>/files/<ISO path without ;1>   Form 1 files: user data truncated to the ISO size;
                                      Form 2 / mixed files: 2336 B per sector (8 subheader + 2324 + 4 EDC,
                                      the raw sector minus sync+header), as dumpsxiso 2.30 writes them
  <out>/manifest.jsonl                one record per written file, sorted by path
  <out>/manifest.sha1                 sha1sum lines `<sha1>  files/<path>`, checkable from <out>
CD-DA entries (extent outside Track 1, e.g. ZNULL.DAT) are counted, never written. See docs/formats.md.
Usage: extract_disc.py [--cue PATH] [--out DIR]   (--cue default: $DC2_CUE, else the single *.cue in disks/)
"""

import argparse
import hashlib
import json
import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MEDIUM_SHA1 = ROOT / "config" / "medium.sha1"
RAW = 2352
USER = 2048
SUBMODE_FORM2 = 0x20


def fail(msg: str) -> int:
    print(f"extract_disc: {msg}", file=sys.stderr)
    return 1


def default_cue() -> Path | None:
    if os.environ.get("DC2_CUE"):
        return Path(os.environ["DC2_CUE"])
    cues = sorted((ROOT / "disks").glob("*.cue"))
    return cues[0] if len(cues) == 1 else None


def parse_cue(cue: Path) -> list[tuple[str, str]]:
    """[(bin basename, track mode)] in cue order."""
    tracks, cur = [], None
    for line in cue.read_text(encoding="utf-8", errors="replace").splitlines():
        m = re.match(r'\s*FILE\s+"(.+)"\s+\S+', line)
        if m:
            cur = m.group(1)
            continue
        m = re.match(r"\s*TRACK\s+\d+\s+(\S+)", line)
        if m and cur is not None:
            tracks.append((cur, m.group(1)))
    return tracks


def sha1_file(p: Path) -> str:
    h = hashlib.sha1()
    with p.open("rb") as f:
        while chunk := f.read(1 << 20):
            h.update(chunk)
    return h.hexdigest()


def check_medium(cue_dir: Path) -> str | None:
    """None when every line of config/medium.sha1 matches; else the error naming the file."""
    if not MEDIUM_SHA1.is_file():
        return f"missing {MEDIUM_SHA1}"
    for line in MEDIUM_SHA1.read_text(encoding="utf-8").splitlines():
        if not line.strip():
            continue
        want, name = line.split(None, 1)
        name = name.lstrip("*").strip()
        p = cue_dir / name
        if not p.is_file():
            return f"medium file missing: {p}"
        if sha1_file(p) != want.lower():
            return f"medium sha1 mismatch: {p}"
    return None


class Track:
    def __init__(self, path: Path):
        self.f = path.open("rb")
        self.sectors = path.stat().st_size // RAW

    def raw(self, lba: int) -> bytes:
        self.f.seek(lba * RAW)
        b = self.f.read(RAW)
        if len(b) != RAW:
            raise ValueError(f"short read at LBA {lba}")
        return b

    def user(self, lba: int) -> bytes:  # Mode 2 Form 1 user data
        return self.raw(lba)[24 : 24 + USER]


def walk(t: Track):
    """Yield (path, is_dir, lba, iso_size) for every entry below the root, depth-first."""
    pvd = t.user(16)
    if pvd[0:6] != b"\x01CD001":
        raise ValueError("no ISO9660 PVD at LBA 16")
    root = pvd[156 : 156 + 34]
    stack = [("", int.from_bytes(root[2:6], "little"), int.from_bytes(root[10:14], "little"))]
    seen = set()
    while stack:
        prefix, lba, size = stack.pop()
        if lba in seen:
            continue
        seen.add(lba)
        for s in range((size + USER - 1) // USER):
            sec = t.user(lba + s)
            off = 0
            while off < USER and sec[off]:
                rec = sec[off : off + sec[off]]
                off += sec[off]
                nlen = rec[32]
                name = rec[33 : 33 + nlen]
                if name in (b"\x00", b"\x01"):
                    continue
                name = name.decode("ascii").split(";")[0]
                elba = int.from_bytes(rec[2:6], "little")
                esize = int.from_bytes(rec[10:14], "little")
                path = f"{prefix}{name}"
                is_dir = bool(rec[25] & 0x02)
                yield path, is_dir, elba, esize
                if is_dir:
                    stack.append((path + "/", elba, esize))


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description="Extract the retail disc into <out>/files + manifest.")
    ap.add_argument("--cue", type=Path, default=None, help="cue sheet (default $DC2_CUE, else the single disks/*.cue)")
    ap.add_argument("--out", type=Path, default=ROOT / "extracted" / "retail", help="output dir (default extracted/retail)")
    a = ap.parse_args(argv)
    cue = a.cue or default_cue()
    if cue is None or not cue.is_file():
        return fail(f"no cue sheet ({cue or 'set --cue or $DC2_CUE, or put one *.cue in disks/'})")
    if err := check_medium(cue.parent):
        return fail(err)
    tracks = parse_cue(cue)
    if not tracks or tracks[0][1].upper() != "MODE2/2352":
        return fail(f"track 1 is not MODE2/2352 in {cue}")
    t = Track(cue.parent / tracks[0][0])

    entries = sorted(walk(t))
    if not entries:
        return fail("empty ISO walk")
    out = a.out
    files_dir = out / "files"
    records, dirs, da, total, form2 = [], 0, 0, 0, 0
    for path, is_dir, lba, iso_size in entries:
        if is_dir:
            dirs += 1
            (files_dir / path).mkdir(parents=True, exist_ok=True)
            continue
        nsec = (iso_size + USER - 1) // USER
        if lba + nsec > t.sectors:  # CD-DA: extent lies outside Track 1
            da += 1
            continue
        raws = [t.raw(lba + i) for i in range(nsec)]
        n2 = sum(1 for r in raws if r[18] & SUBMODE_FORM2)
        form = "1" if n2 == 0 else ("2" if n2 == nsec else "mixed")
        if form == "1":
            data = b"".join(r[24 : 24 + USER] for r in raws)[:iso_size]
        else:
            data = b"".join(r[16:] for r in raws)
            form2 += 1
        dst = files_dir / path
        dst.parent.mkdir(parents=True, exist_ok=True)
        dst.write_bytes(data)
        total += len(data)
        records.append({"path": path, "lba": lba, "sectors": nsec, "iso_size": iso_size, "size": len(data),
                        "form": form, "sha1": hashlib.sha1(data).hexdigest()})
    if not records:
        return fail("ISO walk produced no files")
    records.sort(key=lambda r: r["path"])
    with (out / "manifest.jsonl").open("w", encoding="utf-8", newline="\n") as f:
        for r in records:
            f.write(json.dumps(r, sort_keys=True, separators=(",", ":")) + "\n")
    with (out / "manifest.sha1").open("w", encoding="utf-8", newline="\n") as f:
        for r in records:
            f.write(f"{r['sha1']}  files/{r['path']}\n")
    print(f"entries {len(entries)} (files {len(records)}, dirs {dirs}), bytes {total}, "
          f"form2-files {form2}, da-files {da}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
