# Epic: value_editing

What L3 (VALUE) does for each `NavSettingKind` in the nav table. It fills
in the `navEdit(...)` stub from `nav_state_machine/3`.

The whole epic is blocked on initiative open question 3 (live apply vs.
confirm, and keep vs. discard on B0/timeout). Task 2 is also blocked on
open question 4 (duty step).

## Tasks

```
1-toggle-settings     Enabled / Inverting / Relay N flip          (deps: nav_state_machine/3)
2-pwm-duty-edit       duty ±step per RE0 detent, applied to the   (deps: nav_state_machine/3)
                      timer via the same path as PWM_CHANNEL_CONFIG
3-pwm-frequency-edit  step through PWMFrequency's fixed entries   (deps: nav_state_machine/3)
                      -- partly delivered by fixes/000006, see the task
```

## Acceptance gate

- `platformio run` builds; `test_native/run_tests.ps1` passes.
- Bench: a toggle / duty / frequency edited from the knob (or from
  `sim_input.py`) changes the probed PWM output and the PWM tab. A
  `PWM_CHANNEL_CONFIG` sent afterwards from the companion still wins
  (last writer wins).
