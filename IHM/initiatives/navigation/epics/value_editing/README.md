# Epic: value_editing

What RE2 does to the selected setting, per setting kind (the initiative's
editing rule: select it, turn RE2, it changes, live). The initiative
README's open questions 3 (live) and 4 (1 % duty step) are answered, so
nothing in this epic is blocked on them any more.

"Selected" means the RE1 focus ring. That is final: `nav_state_machine`
was dropped on 2026-09-27 (see the initiative README).

## Tasks

```
[done]    1-toggle-settings     Enabled / Inverting / Relay N set by RE2 (CW on, CCW off)   -- done 2026-09-26
[done]    2-pwm-duty-edit       duty ±1 % per RE2 detent on a selected duty bar,             -- done 2026-09-26
                                applied via applyPwmChannel
[done]    3-pwm-frequency-edit  step through PWMFrequency's fixed entries                    -- done 2026-09-27
                                (delivered by fixes/000006; the navigator follow-up was dropped)
[done]    4-pwm-row-labels      CH0/CH1: wider frequency label, "Duty:" caption              -- done 2026-09-27 as fixes/00000a
```

## Parked, then closed, 2026-09-27

Merged up to `dev` with task 3 still partial (Rafael's call, see the
initiative README). Closed the same day: with `nav_state_machine` dropped,
task 3 has nothing left, so all four tasks are done. The build and native
suite were green at the merge (132 tests). The bench part of the gate
below was covered per task (tasks 1 and 3 on the bench board, task 2 is
Rafael's scope check), never as one run.

## Acceptance gate

- `platformio run` builds; `test_native/run_tests.ps1` passes.
- Bench: a toggle / duty / frequency edited from the knob (or from
  `sim_input.py`) changes the probed PWM output and the PWM tab. A
  `PWM_CHANNEL_CONFIG` sent afterwards from the companion still wins
  (last writer wins).
