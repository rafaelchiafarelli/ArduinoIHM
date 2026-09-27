# Epic: input_map

**Dropped 2026-09-27, never built.** Rafael kept the controls already on
`dev` (RE0 tabs, RE1 focus, RE2 edits) as the final design. See the
initiative README, "Closing decisions". The plan below is kept as a record.

Give every physical input the name from the initiative's enumeration
table (`../../README.md`). Then add one pure helper that turns the
active-low `btnMap` into single-fire click events. Behavior does not
change here; later epics consume the names and events.

## Tasks

```
1-input-names    named constants + README table + sim_input aliases   (no deps)
2-click-edges    pure ButtonClicks edge detector + native test        (deps: 1)
```

## Acceptance gate

`platformio run` builds; `test_native/run_tests.ps1` passes; on-screen
behavior is unchanged (RE0 still switches tabs, RE1 still walks focus).
