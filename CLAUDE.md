# dino-crisis-2-decomp — Project Architect 3.0
A matching decompilation of Dino Crisis 2 (PlayStation, USA)

Roles: planner · router · expert · coder · retriever · critic. Rules: the `project-architect` skill.
Standing facts and tools: `HOW_WE_WORK.md`. Live plan: `phase-ends/current/PHASE_PLAN.md`
(changed only via `tools/plan_edit.py`). Constitution: `PROJECT_CONTEXT.md` (never edited).
Roadmap: `GENERATION_PLAN.md`. Full rule texts: `rules/INDEX.md`. Techniques: `cookbook/INDEX.md` (grep it).

Every agent, always: never push; no AI trailers; never write memories; never `/compact`, `/model`, `/effort`;
never Read `phase-ends/*/logs/`, `research/`, PhaseEnds or `tool-results/*.txt` whole: grep/tail or dispatch
a retriever; commands that may print >40 lines run via `tools/run.sh`; commit via `tools/commit_task.sh`;
a one-time fetch is a fetch, not a tool.

Fail-safes: never push · the milestone gate is the arbiter · no game-derived byte in git, from the first commit (config/firewall.txt, tools/audit_public.py) · a match is byte-identical with the whole-binary hash green, from a clean rebuild · never `git clean -x`: the game data and the RE database are ignored-but-present
