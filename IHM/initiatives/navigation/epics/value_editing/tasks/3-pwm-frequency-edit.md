# Task 3: pwm-frequency-edit

**Status:** partly delivered -- the frequency logic, the focusable labels and
an RE2 shortcut shipped in `fixes/000006/pwm-frequency-edit` (merged to `dev`
f70bcca, 2026-09-26, scope-verified by Rafael on the old board revision).
What is left is the RE0 edit-level path (below). It is still blocked on
initiative open question 3.
**Branch:** `3-pwm-frequency-edit` (from `tasks`)
**Depends on:** `nav_state_machine/3-wire-into-main` (for `navEdit` and the
L3 level). It used to also depend on `value_editing/2` for
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

   These are the answers Rafael gave for the RE2 shortcut: apply live on
   each detent, clamp, no confirm step. **Whether they also answer open
   question 3 for the RE0 edit level is not decided.**

## Remaining contract

### In

Open question 3 answered for the L3 VALUE level (live vs. confirm; what B0
and the timeout do to an unconfirmed edit), and `nav_state_machine/3`
merged, which gives `navEdit` and removes RE1 focus.

### Delivers

1. **`navEdit` for `NAV_SET_FREQUENCY`**: each RE0 detent calls
   `pwmStepFrequency(f, ±1)` then `pwmSetFrequency(ch, ...)`, or buffers
   the value until pbRE0 confirms, per open question 3. Reuse the
   functions above; add no second stepping implementation.
2. **Fate of the RE2 shortcut.** `nav_state_machine/3` removes RE1 focus,
   and the RE2 branch in `main.cpp` resolves "which channel" from that
   focus. Pick one with Rafael before implementing:
   - **Keep:** RE2 steps the channel the navigator currently has selected
     (L1/L2), independent of RE0's L3.
   - **Remove:** delete the RE2 branch, `pwmFrequencyActionChannel()`, and
     the labels' `on_press` (plus a regen) if the labels shouldn't be
     focusable any more.
3. **`demo/HARDWARE_RUNBOOK.md`**'s "UI navigation" section updated to the
   final controls.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes. Bench: the
probed frequency on OC3A follows RE0 at the L3 level, and follows or stops
following RE2 per the decision in item 2. The task file is marked done in
the same commit.
