# Disc formats — Dino Crisis 2 (USA, SLUS-01279)

This file gives counts and names only. It holds no game bytes and no hashes of game files (G12). `make extract`
(`tools/extract_disc.py`) produces the counts and names. The reference is dumpsxiso 2.30 (`docs/ops/docker-host.md`,
`/work/.run/ref/layout.xml`).

## Medium
- Cue + two bins. `config/medium.sha1` pins the identity (sha1sum lines by basename). A check runs before every extract.
  On a mismatch or a missing file, the check exits 1 and names the file.
- Track 1: `MODE2/2352`, the ISO9660 filesystem. LBA n = byte offset n·2352 of the Track 1 bin.
- Track 2: `AUDIO` (CD-DA), INDEX 00 00:00:00, INDEX 01 00:02:00 (2 s pregap).

## Sectors
- Raw 2352 B: 12 sync + 4 header + 8 subheader (2×{file, channel, submode, coding}) + payload.
- If submode bit `0x20` is set, the sector is Form 2 (2324 user + 4 EDC). If the bit is clear, the sector is Form 1
  (2048 user + 4 EDC + 276 ECC).
- Filesystem: the PVD is at LBA 16. The tool walks the directory records from the root record (PVD offset 156), with
  multi-sector extents. It skips `.`/`..` and strips `;1`.

## Entries
- 685 entries = 678 files written + 1 CD-DA entry + 6 directories.
- Directories: `BIN`, `PSX`, `PSX/BIN`, `PSX/DATA`, `PSX/DATA_XA`, `PSX/MOVIE`.
- Files by directory: `/` 2 (`SLUS_012.79` and `SYSTEM.CNF`, plus DA `ZNULL.DAT`), `BIN` 89, `PSX/BIN` 15, `PSX/DATA` 550,
  `PSX/DATA_XA` 11, `PSX/MOVIE` 11.
- Files by extension: `DAT` 414, `BIN` 105, `DBS` 89, `TEX` 28, `PXL` 14, `STR` 11, `XAS` 11, `TRG` 4, `CNF` 1, `79` 1.
- Forms: 656 Form 1 (dumpsxiso `data`), 22 mixed (dumpsxiso `mixed`: `STR` in `PSX/MOVIE`, `XAS` in `PSX/DATA_XA`),
  0 pure Form 2.

## Output convention (matches dumpsxiso 2.30 byte for byte)
- Form 1 file: concatenated 2048 B user data, truncated to the ISO size.
- Form 2 or mixed file: 2336 B per sector, never truncated. The 2336 B are the raw sector minus sync+header = 8 subheader
  + 2324 + 4 EDC. Form 1 sectors inside carry their 2048 + EDC/ECC in the same 2336. Size = sectors·2336.
- CD-DA entry (dumpsxiso type `da`): not written, not a manifest record, printed as `da-files`. Its extent is outside
  Track 1: `ZNULL.DAT` → Track 2. The extra outputs of dumpsxiso (`license_data.dat` = system area, `ZNULL.WAV` = Track 2
  audio) are not ISO files. The reference command moves them out of `files/`.

## Manifest
- `<out>/manifest.jsonl`: one line per written file, sorted by path, `json.dumps(sort_keys=True, separators=(",",":"))`:
  `{"form": "1"|"2"|"mixed", "iso_size": int, "lba": int, "path": str, "sectors": int, "sha1": str, "size": int}`.
  Here `sectors` = ceil(iso_size/2048) and `size` = bytes written. Directories are not records.
- `<out>/manifest.sha1`: `<sha1>  files/<path>` sorted by path. To check it, run `cd <out> && sha1sum -c manifest.sha1`.
- Run summary line: `entries 685 (files 678, dirs 6), bytes 450178220, form2-files 22, da-files 1`.
