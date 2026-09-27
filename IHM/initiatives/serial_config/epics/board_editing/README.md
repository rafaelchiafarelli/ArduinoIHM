# Epic: board_editing

The SERIAL tab's compact rows (from `fixes/000010`) become editable with
the knobs, as on the PWM tab: RE1 selects a field (focus ring), RE2
changes it live.

## Tasks

```
1-screen-fields   yaml: the per-bus row(s) with focusable fields + new bindings   (blocked: Q1)
2-knob-editing    RE2 on a selected field changes it (actions + main.cpp)          (deps: 1, config_model/1; blocked: Q2)
3-persist         EEPROM save per the save policy; load at boot                    (deps: 2, config_model/2; blocked: Q3)
```

## Acceptance gate

Build + native suite. Bench: each field edited with the knobs shows on
the TFT, in the companion's readback and mirror, and survives a power
cycle (per the save policy).
