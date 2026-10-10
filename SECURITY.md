# Security policy

## Reporting

Report privately through GitHub's **private vulnerability reporting**: the repository's *Security* tab →
*Report a vulnerability*. Do not open a public issue, and never attach game files to any report.

## What counts here

Two threats matter for this project:

1. **Leaked game bytes (a firewall bypass).** Anything in the tracked tree or its history that was derived from the
   original medium: executable or data bytes, extracted assets, disassembly pasted into notes, build output. The
   `.gitignore` firewall and `tools/audit_public.py` (run in CI on every push) are meant to stop this; a report that
   something got past them is a security report.
2. **CI and supply chain.** A workflow that can be made to run untrusted code with write access, an action that isn't
   pinned to a commit SHA, or a dependency that could be swapped.

## How a leaked-bytes report is handled

The offending content is purged from history (not just deleted in a new commit), and its hash is promoted into
`tools/audit_public.py`'s refusal set so that the audit refuses the same bytes from then on. Affected branches and
forks are told to rebase on the rewritten history.

Game files exist outside the developer's machine only in the private `dino-crisis-2-decomp-private`, which CI reads
through one read-only credential. By where a leak lands:

- **This repository** (a commit, a pull request, a CI log or artifact): made private at once, the runs and artifacts
  deleted, the history rewritten after a rehearsal on a copy (G14); GitHub Support drops cached views and
  `refs/pull/*`; public again only once a probe of the old object ids comes back empty (G17).
- **`-dev`**: the same rewrite; the next release is checked against it.
- **The toolchain image**: the version deleted, a corrected one released under a new digest.
- **The credential**: the deploy key deleted and replaced with its `PRIVATE_DEPLOY_KEY` secret, or the token
  revoked and replaced with its `PRIVATE_REPO_TOKEN` secret.
- **`-private` made public or forked**: made private, the credential rotated, GitHub Support contacted; its history
  counts as published.
