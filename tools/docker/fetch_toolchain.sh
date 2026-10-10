#!/usr/bin/env bash
# fetch_toolchain.sh — fetch the candidate compiler toolchains listed in config/toolchains.tsv (phase 1.4 T1).
#   fetch_toolchain.sh [--only <name>…] <dest>   download each row, check sha256 (mismatch → exit 1, nothing
#                                                unpacked), unpack to <dest>/<name>/, stamp <dest>/<name>/.sha256
#   fetch_toolchain.sh --verify <dest>           per row: stamp == tsv sha256 and main executable present;
#                                                prints `loader: <k> ok` (wibo) and `toolchains: <k> ok` (rest);
#                                                exit 0 iff all ok
# TSV: $TOOLCHAINS_TSV, else <repo>/config/toolchains.tsv beside this script. Columns: name kind url sha256 gcc_version notes.
# kind → unpack / main executable: oldgcc flat tar → cc1 · psyq, maspsx tar with one top dir (stripped) →
# CC1PSX.EXE, maspsx.py · wibo single binary (chmod +x) → wibo.
set -euo pipefail

TSV="${TOOLCHAINS_TSV:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)/config/toolchains.tsv}"
usage() { echo "usage: fetch_toolchain.sh [--only <name>…] <dest> | --verify <dest>" >&2; exit 2; }
[ -f "$TSV" ] || { echo "fetch_toolchain: no tsv at $TSV" >&2; exit 2; }

main_exe() {
  case "$1" in
    oldgcc) echo cc1 ;; psyq) echo CC1PSX.EXE ;; maspsx) echo maspsx.py ;; wibo) echo wibo ;;
    *) echo "fetch_toolchain: unknown kind $1" >&2; return 1 ;;
  esac
}

rows() { grep -v -e '^#' -e '^[[:space:]]*$' "$TSV"; }

if [ "${1:-}" = --verify ]; then
  [ $# -eq 2 ] || usage
  dest="$2"; ok=0; lok=0; bad=0
  while IFS=$'\t' read -r name kind _url sha _rest; do
    exe="$(main_exe "$kind")"
    stamp="$(cat "$dest/$name/.sha256" 2>/dev/null || true)"
    if [ "$stamp" = "$sha" ] && [ -f "$dest/$name/$exe" ]; then
      if [ "$kind" = wibo ]; then lok=$((lok + 1)); else ok=$((ok + 1)); fi
    else
      echo "FAIL $name: stamp='${stamp}' exe=$dest/$name/$exe" >&2; bad=$((bad + 1))
    fi
  done < <(rows)
  echo "loader: $lok ok"
  echo "toolchains: $ok ok"
  [ "$bad" -eq 0 ]
  exit
fi

only=()
if [ "${1:-}" = --only ]; then
  shift
  while [ $# -gt 1 ]; do only+=("$1"); shift; done
fi
[ $# -eq 1 ] || usage
dest="$1"; mkdir -p "$dest"

while IFS=$'\t' read -r name kind url sha _rest; do
  if [ ${#only[@]} -gt 0 ] && ! printf '%s\n' "${only[@]}" | grep -qxF "$name"; then continue; fi
  main_exe "$kind" >/dev/null
  tmp="$dest/.dl.$name"
  curl -fsSL --retry 3 -o "$tmp" "$url"
  got="$(sha256sum "$tmp" | cut -d' ' -f1)"
  if [ "$got" != "$sha" ]; then
    echo "fetch_toolchain: sha256 mismatch for $name: want $sha got $got" >&2
    rm -f "$tmp"; exit 1
  fi
  rm -rf "${dest:?}/$name"; mkdir -p "$dest/$name"
  case "$kind" in
    oldgcc) tar -xzf "$tmp" -C "$dest/$name" ;;
    psyq|maspsx) tar -xzf "$tmp" -C "$dest/$name" --strip-components=1 ;;
    wibo) mv "$tmp" "$dest/$name/wibo"; chmod +x "$dest/$name/wibo" ;;
  esac
  rm -f "$tmp"
  echo "$sha" > "$dest/$name/.sha256"
  echo "fetched $name"
done < <(rows)
