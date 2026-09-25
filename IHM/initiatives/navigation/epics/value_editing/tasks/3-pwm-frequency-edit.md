# Task 3: pwm-frequency-edit

**Status:** planned -- blocked on initiative open question 3
**Branch:** `3-pwm-frequency-edit` (from `tasks`)
**Depends on:** `value_editing/2-pwm-duty-edit`

## Contract

### Delivers

1. **`navEdit`** for `NAV_SET_FREQUENCY`: steps through `PWMFrequency`'s
   13 fixed entries, clamped at both ends with no wrap, then
   `applyPwmChannel`. `frequency_variable` (raw TOP) is **not** reachable
   from the knob. It stays PC-only.
2. **`test_native`**: a pure `stepFrequency(sel, delta)` is unit-tested,
   including that `frequency_variable` is never produced.
3. The PWM tab's frequency label shows the new value. If that needs the
   placeholder `pwm_chN_freq` label to become bound (the yaml header
   comment says Janus live text only renders string-bound fields), that
   yaml change plus regeneration is part of this task.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; bench:
probed frequency on OC3A follows the knob; task file marked done in the
same commit.
