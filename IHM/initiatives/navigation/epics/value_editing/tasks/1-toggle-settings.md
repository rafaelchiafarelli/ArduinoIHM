# Task 1: toggle-settings

**Status:** planned -- partly pre-delivered 2026-09-25 (see below)
**Branch:** `1-toggle-settings` (from `tasks`)
**Depends on:** `nav_state_machine/3-wire-into-main`

## Contract

### Delivers

1. **`src/main.cpp` `navEdit`** for `NAV_SET_TOGGLE`: an RE2 step in
   either direction (the editing rule, initiative README) flips the value by dispatching the widget's existing
   `on_press` action through `janus_handle_action`, e.g.
   `toggle_pwm_ch0_enabled` or `toggle_relay_3`. That keeps one code path
   for "flip this toggle". Live, with no confirm (open question 3, answered
   2026-09-26).
2. If the nav table exposes Inverting on the complex channels, the
   missing widgets and actions are **not** added here. They get their own
   task (flag it).

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; bench:
relay N and PWM CH0 Enabled flip from the knob; task file marked done in
the same commit.

## Pre-delivered on dev (2026-09-25, fixes/000004)

Requested by Rafael ahead of the navigator. It answers open question 3 for
switches: **a press applies immediately**.

- `src/janus_actions.cpp` handles all ten PWM toggle actions and calls
  `pwmToggleOutput(ch, out, inverting)` in main.cpp. That flips one bit of
  `pwmLast[ch]` and re-applies it through `applyPwmChannel()`, the same
  path `PWM_CHANNEL_CONFIG` uses.
- Boot defaults per channel: 62500 Hz, 50 % duty, all outputs off and
  non-inverting (Rafael's choice).
- Relay presses now also mirror into `relay_instance`, so the relay
  switch and its LED follow the real relay.

Still owed by this task: dispatching the same toggles from `navEdit` once
`nav_state_machine/3` replaces pbRE1's direct activate.
