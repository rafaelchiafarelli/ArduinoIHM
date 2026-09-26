# Task 1: add-defect-concept

**Status:** done
**Branch:** `workflow_defects` (implemented directly on the initiative
branch — small enough to just do, no epics/tasks containers needed for a
single-task Defect-about-Defects; see the edit this task made to the
Branches section for why)
**Depends on:** nothing

## Contract

### In

Nothing. Pure documentation edit to one file.

### Delivers

`.claude/skills/workflow/SKILL.md`:

1. **Vocabulary section** — new "Defect" bullet: a gap/bug found
   incidentally, recorded as a normal-shaped initiative
   (`initiatives/<name>/README.md` + `epics/<epic>/tasks/<task>.md`)
   scoped to the size of the gap, distinguished from a regular initiative
   by intent (record now, work later) stated explicitly in its README.

2. **Branches section** — new paragraph right after "One initiative chain
   per working copy," covering the case this task itself hit: a Defect
   found mid-session inside a clone whose `epics`/`tasks` names are
   already taken by an active chain. Resolution: skip those container
   branches, commit the initiative's folder (still laid out with its
   normal epic/task subfolders) straight on the `<initiative>` branch,
   merge directly to `features`; create the real `epics`/`tasks` branches
   later only when implementation actually starts. Explicitly forbids
   silently reusing/moving the existing `epics`/`tasks` pointers without
   Rafael confirming first.

## Dependencies

None.

## Pre-work

None.

## Definition of done

- `SKILL.md` reads correctly top to bottom (no dangling cross-references
  — the new Branches paragraph is referenced by name from the new
  Vocabulary bullet).
- Task file marked done in the same commit as the `SKILL.md` edit.

## Notes

- Written retroactively, describing the exact ad hoc resolution this
  session already used for `initiatives/pwm_state_telemetry/` (skip
  containers, initiative branch straight to `features`) after confirming
  the approach with Rafael live — this task just makes that the
  documented default so the next occurrence doesn't need to ask again.
- Conventional-commit: `docs(workflow-skill): add Defect vocabulary +
  container-collision rule`.
