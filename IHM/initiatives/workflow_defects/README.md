# Initiative: workflow_defects

Adds a **Defect** concept to the `workflow` skill
(`.claude/skills/workflow/SKILL.md`), so "found a gap while doing
something else, record it, don't work it now" has a defined place in the
skill instead of being improvised each time.

## Why

2026-09-22: explaining the PC companion app's PWM panel to Rafael
surfaced a real gap (no board -> PC PWM state telemetry -- see
`initiatives/pwm_state_telemetry/`). Rafael asked for it "in the form of
an initiative," which the skill already supports structurally, but two
things had to be improvised on the spot because the skill doesn't say
what to do:

1. **What counts as a defect vs. a normal initiative**, and whether it
   needs the full epic/task ladder immediately or can just be a planning
   record.
2. **Container branch name collisions.** The skill's `features`/`epics`/
   `tasks` container names are flat by design, one chain per clone -- but
   this clone already had an active `serial_commands` chain (`tasks`
   unmerged), so creating a second initiative's own `epics`/`tasks`
   branches wasn't possible under the same names. Asked Rafael; he chose
   "skip the epics/tasks containers for a docs-only initiative, commit
   planning docs straight on the initiative branch, merge that directly
   to `features`." That resolution isn't written down anywhere either.

Both got resolved ad hoc for `pwm_state_telemetry`. This initiative
writes the resolution into the skill so the next occurrence doesn't need
Rafael to re-decide it live.

## Scope

One epic, `skill_defect_vocabulary`, one task -- see
`epics/skill_defect_vocabulary/`. Edits only
`.claude/skills/workflow/SKILL.md`; no IHM firmware/app code.
