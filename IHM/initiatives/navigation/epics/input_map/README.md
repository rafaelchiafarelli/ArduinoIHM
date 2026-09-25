# Epic: input_map

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
