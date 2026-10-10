## What this changes

<!-- One paragraph, written by a person. -->

## Checklist

- [ ] **No game-derived bytes.** Nothing derived from the medium is added: no executable, data, assets, build output,
      or pasted disassembly. `python3 tools/audit_public.py` exits 0.
- [ ] **Matches are real.** Every function claimed as matched builds byte-identical with the **whole-binary hash green**
      from a clean rebuild. I state which compilation each claim survived (standalone / the real translation unit /
      the whole binary).
- [ ] **Names have evidence.** Every new or changed name cites its evidence (a string, a cross-reference chain, a debug
      menu, a live-memory datapoint, a community label with provenance). Unsure → left address-named.
- [ ] **I answer for every line** (CONTRIBUTING.md, *AI use — conduct*). <!-- A tool wrote a meaningful part? End
      this description with `Assisted-by: <model> [<tool> …]`. -->
