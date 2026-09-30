---
description: Request a planner-phase session at the next launch
allowed-tools: Bash(/opt/homebrew/opt/python@3.14/bin/python3.14 tools/launch.py:*)
disable-model-invocation: true
---
!`/opt/homebrew/opt/python@3.14/bin/python3.14 tools/launch.py --request planner-phase`
A planner-phase session is queued (`.run/next_mode`). Do not plan here. If an expert is running, let it finish or `TaskStop` it and set its task back to `next` via plan_edit.py. Then run `/opt/homebrew/opt/python@3.14/bin/python3.14 tools/launch.py --seed-only` and enter the planning mode it names, in this session.
