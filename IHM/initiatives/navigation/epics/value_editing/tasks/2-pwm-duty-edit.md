# Task 2: pwm-duty-edit

**Status:** done 2026-09-26. All 5 items delivered as written: 125 native
tests pass; RAM 58.9 %. Generated layout: CH3's C duty bar ends at x 310,
y 474, and its ring reaches exactly the panel's bottom edge (480). The
scope check is Rafael's bench step.
**Branch:** `2-pwm-duty-edit` (from `tasks`)
**Depends on:** nothing unmerged. It uses today's RE1 focus as "selected"
and the apply path from `fixes/000004`.

Re-planned from the RE0/L3 version: Rafael made RE2 the editing knob
("select it, turn RE2, it changes", live), answered open question 4 with
**1 % per detent**, and decided that **pbRE1 on a duty bar does nothing**.
The old items 1-2 (extract `applyPwmChannel`, keep a last-applied
`PwmWireConfig[4]`) were delivered by `fixes/000004` as `applyPwmChannel()`
and `pwmLast[4]`, so they're dropped here.

## Contract

### Delivers

1. **Selectable duty bars** (`lib/GUI/pwm.screen.yaml` + regenerate with
   the WSL Janus). Each of the 8 duty `progress` bars (`pwm_ch0_duty`,
   `pwm_ch1_duty`, `pwm_ch{2,3}_{a,b,c}_duty`) gets
   `on_press: edit_pwm_<id>_duty`, which makes it focusable, and sits in its
   own `focus_ring` row, so the ring shows exactly which one RE2 will
   change.
   - CH0/CH1: the bar alone in a ring row (column height 30 + 26 + 18 =
     74 of 80 px).
   - CH2/CH3: each A/B/C line is split. The switch gets its own ring row,
     then the LED and the letter, then the bar in its own ring row. Today
     one ring covers both focusable widgets, which would be ambiguous.
     Width 40 + 14 + 18 + 102 = 174 + 126 icon = 300 of 320 px.
2. **`pwmStepDuty(percent, steps)`** (`lib/MultiOutput/src/PWMWireConfig.h`),
   pure: `percent + steps`, clamped to 0-100. Unit-tested in
   `test_native/test_pwm_wire_config.cpp`, including both clamps and an
   out-of-range start (> 100, which `pwmLast` can hold because the PC's
   duty is only clamped when applied).
3. **RE2 edits the selected duty** (`src/main.cpp`). The RE2 branch maps
   the focused widget's action to either a channel frequency (existing) or
   a (channel, output) duty. For duty it calls
   `pwmSetDuty(ch, out, pwmStepDuty(current, ±1))`, which re-applies
   through `applyPwmChannel()` (tab and `IHM_PWM_STATE` follow). CW =
   higher duty.
4. **pbRE1 on a duty bar does nothing**: the 8 `edit_pwm_*_duty` actions
   are no-ops in `src/janus_actions.cpp`, there only so Janus can focus the
   bars.
5. **`demo/HARDWARE_RUNBOOK.md`** "UI navigation": duty bars listed as
   selectable, and RE2 changes duty by 1 %.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes. Bench (Rafael,
scope): with CH0's duty bar selected, the duty on OC3A follows RE2 in 1 %
steps and stops at 0 and 100. The task file is marked done in the same
commit.
