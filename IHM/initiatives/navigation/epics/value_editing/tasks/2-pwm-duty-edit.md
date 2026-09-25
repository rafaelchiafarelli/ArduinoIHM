# Task 2: pwm-duty-edit

**Status:** planned -- blocked on initiative open questions 3, 4
**Branch:** `2-pwm-duty-edit` (from `tasks`)
**Depends on:** `nav_state_machine/3-wire-into-main`

## Contract

### Delivers

1. **One apply path.** The body of `main.cpp`'s `PWM_CHANNEL_CONFIG`
   loop (wire -> `PWMChannelConfig` -> `compute*CallArgs` ->
   `setupPWMChannelN` -> `mirrorPwmToUi`) is extracted into
   `applyPwmChannel(ch, const PwmWireConfig&)`. Both MAVLink and the knob
   call it.
2. The current per-channel config is kept as a `PwmWireConfig[4]`
   (last applied, from either source) so an edit starts from what's on
   the pin.
3. **`navEdit`** for `NAV_SET_DUTY`: ±step, clamped to 0-100, then
   `applyPwmChannel`, when open question 3 says to apply.
4. **`test_native`**: a pure `stepDuty(value, delta, step)` (clamping) is
   unit-tested.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; bench:
probed duty on OC3A follows the knob; task file marked done in the same
commit.
