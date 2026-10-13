# Security policy

## Reporting

Report privately through GitHub's **private vulnerability reporting**. Go to the repository's *Security* tab, then to
*Report a vulnerability*. Do not open a public issue. Never attach game files to any report.

## What counts here

Two threats matter for this project:

1. **Leaked game bytes (a firewall bypass).** This threat is anything in the tracked tree or its history that was
   derived from the original medium. It includes executable or data bytes, extracted assets, disassembly pasted into
   notes and build output. The `.gitignore` firewall and `tools/audit_public.py` (run in CI on every push) are meant
   to stop this. A report that something got past them is a security report.
2. **CI and supply chain.** This threat is a workflow that can be made to run untrusted code with write access. It is
   also an action that is not pinned to a commit SHA, or a dependency that could be swapped.

## How a leaked-bytes report is handled

The offending content is purged from history (not just deleted in a new commit). Its hash is promoted into
`tools/audit_public.py`'s refusal set so that the audit refuses the same bytes from then on. Affected branches and
forks are told to rebase on the rewritten history.

Game files exist outside the developer's machine only in the private `dino-crisis-2-decomp-private`, which CI reads
through one read-only credential. The response depends on where a leak lands:

- **This repository** (a commit, a pull request, a CI log or artifact): It is made private at once. The runs and
  artifacts are deleted. The history is rewritten after a rehearsal on a copy (G14). GitHub Support drops cached views
  and `refs/pull/*`. It becomes public again only once a probe of the old object ids comes back empty (G17).
- **`-dev`**: It gets the same rewrite. The next release is checked against it.
- **The toolchain image**: The version is deleted, and a corrected one is released under a new digest.
- **The credential**: The deploy key is deleted and replaced with its `PRIVATE_DEPLOY_KEY` secret, or the token is
  revoked and replaced with its `PRIVATE_REPO_TOKEN` secret.
- **`-private` made public or forked**: It is made private, the credential is rotated and GitHub Support is contacted.
  Its history counts as published.
