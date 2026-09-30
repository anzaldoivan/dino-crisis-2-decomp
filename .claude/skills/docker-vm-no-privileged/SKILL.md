---
name: docker-vm-no-privileged
description: Never probe the Docker VM with --privileged or --pid=host; the classifier treats it as containment escape
---

# Docker Vm No Privileged

Captured 2026-09-30 from phase-ends/current/tasks/T2.md:20.

## When to use
inspecting Docker Desktop VM facts (mem, CPUs, backend)

## Steps
- Never run `docker run --privileged` or `--pid=host` (nsenter etc.) to inspect the Docker Desktop VM; the classifier treats it as containment escape and then denies related waits too.
- Use `docker info` (NCPU, MemTotal, ServerVersion) and `docker version` instead; a fact they do not expose is recorded as "not determinable, default assumed" (see docs/ops/docker-host.md).
