# Epic: value_editing

What RE2 does to the selected setting, per setting kind (the initiative's
editing rule: select it, turn RE2, it changes, live). The initiative
README's open questions 3 (live) and 4 (1 % duty step) are answered, so
nothing in this epic is blocked on them any more.

Until `nav_state_machine/3` lands, "selected" means the RE1 focus ring;
afterwards it means the navigator's L2 setting.

## Tasks

```
1-toggle-settings     Enabled / Inverting / Relay N flip          (deps: nav_state_machine/3)
2-pwm-duty-edit       duty ±1 % per RE2 detent on a selected      (no deps -- uses RE1 focus today)
                      duty bar, applied via applyPwmChannel
3-pwm-frequency-edit  step through PWMFrequency's fixed entries   (deps: nav_state_machine/3)
                      -- partly delivered by fixes/000006, see the task
4-pwm-row-labels      CH0/CH1: wider frequency label, "Duty:" caption  -- done as fixes/00000a
```

## Acceptance gate

- `platformio run` builds; `test_native/run_tests.ps1` passes.
- Bench: a toggle / duty / frequency edited from the knob (or from
  `sim_input.py`) changes the probed PWM output and the PWM tab. A
  `PWM_CHANNEL_CONFIG` sent afterwards from the companion still wins
  (last writer wins).
