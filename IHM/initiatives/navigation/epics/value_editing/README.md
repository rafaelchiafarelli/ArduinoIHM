# Epic: value_editing

What RE2 does to the selected setting, per setting kind (the initiative's
editing rule: select it, turn RE2, it changes, live). The initiative
README's open questions 3 (live) and 4 (1 % duty step) are answered, so
nothing in this epic is blocked on them any more.

Until `nav_state_machine/3` lands, "selected" means the RE1 focus ring;
afterwards it means the navigator's L2 setting.

## Tasks

```
[done]    1-toggle-settings     Enabled / Inverting / Relay N set by RE2 (CW on, CCW off)   -- done 2026-09-26
[done]    2-pwm-duty-edit       duty ±1 % per RE2 detent on a selected duty bar,             -- done 2026-09-26
                                applied via applyPwmChannel
[partial] 3-pwm-frequency-edit  step through PWMFrequency's fixed entries                    (deps: nav_state_machine/3)
                                -- partly delivered by fixes/000006; only "RE2 follows the navigator" is left
[done]    4-pwm-row-labels      CH0/CH1: wider frequency label, "Duty:" caption              -- done 2026-09-27 as fixes/00000a
```

## Parked 2026-09-27

Merged up to `dev` with task 3 still partial (Rafael's call, see the
initiative README's "Parked" section). The acceptance gate below has NOT
been run as a whole. The build and native suite were green at the merge
(132 tests).

## Acceptance gate

- `platformio run` builds; `test_native/run_tests.ps1` passes.
- Bench: a toggle / duty / frequency edited from the knob (or from
  `sim_input.py`) changes the probed PWM output and the PWM tab. A
  `PWM_CHANNEL_CONFIG` sent afterwards from the companion still wins
  (last writer wins).
