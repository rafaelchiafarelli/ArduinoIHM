# Task 3: pwm-frequency-edit

**Status:** partly delivered -- the frequency logic, the focusable labels and
an RE2 shortcut shipped in `fixes/000006/pwm-frequency-edit` (merged to `dev`
f70bcca, 2026-09-26, scope-verified by Rafael on the old board revision).
What is left is making RE2 follow the navigator's selection instead of
RE1 focus (below). Open question 3 is answered (live).
**Branch:** `3-pwm-frequency-edit` (from `tasks`)
**Depends on:** `nav_state_machine/3-wire-into-main` (for `navEdit` and the
navigator's L2 selection). It used to also depend on `value_editing/2` for
`applyPwmChannel` and the last-applied config, but those already exist on
`dev` (`fixes/000004`: `applyPwmChannel()`, `pwmLast[4]`).

## Already delivered (fixes/000006 -- don't redo)

Built outside this task on a fix branch, a deviation from the workflow
skill recorded here so this task stays the one place for the frequency-edit
contract.

1. **Stepping logic**, pure and unit-tested (`lib/MultiOutput/src/PWMTiming.h`,
   `test_native/test_pwm_timing.cpp`, 5 tests). This is the `stepFrequency`
   the original contract asked for.
   - `pwmStepFrequency(f, steps)`: `steps > 0` = higher frequency. Clamped
     at 62500 Hz and 15 Hz, no wrap. Never returns `frequency_variable`.
     From `frequency_variable` (set by the PC) it lands on the end it was
     turned toward.
   - `pwmCycleFrequency(f)`: one step lower, wrapping 15 Hz -> 62500 Hz.
2. **Apply path** (`src/main.cpp`): `pwmSetFrequency(ch, f)` re-applies the
   channel through `applyPwmChannel()` with only `f_selector` changed, so
   enable/inverting/duty are kept and the PWM tab plus `IHM_PWM_STATE`
   follow.
3. **Focusable frequency labels** (`lib/GUI/pwm.screen.yaml`, regenerated
   with Janus dev 36adb5a). Each `pwm_chN_state` label (already bound to
   `chN_state_label`, which `formatFrequencyLabel` fills) sits in its own
   `focus_ring` row with `on_press: cycle_pwm_chN_frequency`. This covers
   original item 3.
4. **Temporary RE1/RE2 controls**, to be replaced by this task:
   - RE1 focuses a frequency label.
   - RE2 steps it (`pwmStepFrequency`, live, clamped). `main.cpp` resolves
     the focused widget with `janus_focus_activate()` and maps the action
     through `pwmFrequencyActionChannel()`.
   - pbRE1 cycles it (`pwmCycleChannelFrequency`, via `janus_actions.cpp`).

   Rafael later (2026-09-26) made RE2 the editing knob for the whole
   initiative (open question 3: live), so this is the final behaviour, not
   a shortcut. Only the selection source changes.

## Remaining contract

### In

`nav_state_machine/3` merged: the navigator owns selection, and RE1 focus
is gone.

### Delivers

1. **RE2 follows the navigator.** When the navigator's L2 setting is a
   channel's Frequency, RE2 calls `pwmStepFrequency(f, ±1)` then
   `pwmSetFrequency(ch, ...)`. This replaces the `janus_focus_activate()`
   plus `pwmFrequencyActionChannel()` lookup in `main.cpp`'s RE2 branch.
   Reuse the functions above; add no second stepping implementation.
2. Whether the labels keep their `on_press: cycle_pwm_chN_frequency`
   depends on open question 7 (what pbRE0 does at L2). Remove it, and
   regenerate, only if that answer makes the press unused.
3. **`demo/HARDWARE_RUNBOOK.md`**'s "UI navigation" section updated to the
   final controls.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes. Bench: with
CH0's Frequency selected through the navigator, the probed frequency on
OC3A follows RE2. The task file is marked done in
the same commit.
