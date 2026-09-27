# Task 3: pwm-frequency-edit

**Status:** done 2026-09-27. Everything shipped in
`fixes/000006/pwm-frequency-edit` (merged to `dev` f70bcca, 2026-09-26,
scope-verified by Rafael on the old board revision). The remaining contract
below was about following a navigator, and it was closed without code
because `nav_state_machine` was dropped. Rafael, 2026-09-27: the PWM tab as
it works now is the standard, so RE1 focus is the final selection.
**Branch:** `3-pwm-frequency-edit` (from `tasks`)
**Depends on:** (dropped 2026-09-27) `nav_state_machine/3-wire-into-main` (for `navEdit` and the
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

## Remaining contract -- closed 2026-09-27, no code

Kept as planned, with how each item closed:

1. ~~**RE2 follows the navigator.**~~ There is no navigator. RE2 keeps
   resolving the RE1-focused widget through `janus_focus_activate()` plus
   `pwmFrequencyActionChannel()`, which was already the final behaviour.
2. **`on_press: cycle_pwm_chN_frequency` stays.** Open question 7 closed
   as "pbRE0 has no UI role; pbRE1 keeps today's press", so the press is
   still used. No regenerate.
3. **`demo/HARDWARE_RUNBOOK.md`** "UI navigation" already describes the
   final controls (RE0 tabs, RE1 focus, pbRE1 press, RE2 live edit).
   No change needed.

## Definition of done

Met by `fixes/000006` (build, native suite, scope check on OC3A with the
frequency label selected by RE1). Nothing further to run for the closing
commit, which is docs only.
