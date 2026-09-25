# Task 1: toggle-settings

**Status:** planned -- blocked on initiative open question 3
**Branch:** `1-toggle-settings` (from `tasks`)
**Depends on:** `nav_state_machine/3-wire-into-main`

## Contract

### Delivers

1. **`src/main.cpp` `navEdit`** for `NAV_SET_TOGGLE`: a step in either
   direction flips the value by dispatching the widget's existing
   `on_press` action through `janus_handle_action`, e.g.
   `toggle_pwm_ch0_enabled` or `toggle_relay_3`. That keeps one code path
   for "flip this toggle". Confirm, cancel and timeout behave as open
   question 3 decides.
2. If the nav table exposes Inverting on the complex channels, the
   missing widgets and actions are **not** added here. They get their own
   task (flag it).

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; bench:
relay N and PWM CH0 Enabled flip from the knob; task file marked done in
the same commit.
