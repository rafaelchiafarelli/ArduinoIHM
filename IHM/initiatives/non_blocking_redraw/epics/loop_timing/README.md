# Epic: loop_timing

The instrument for the initiative's 20 ms gate. `IHM_BOARD_STATE`'s
`time_statistics` sums time spent in the Timer2 ISR. It says nothing about
how long one superloop pass takes, and that is what rendering blocks.

## Tasks

```
1-max-pass-time   longest superloop pass per ~100 ms window, sent over MAVLink   (open question 1)
```

## Acceptance gate

`platformio run` builds; `test_native/run_tests.ps1` passes; on the bench a
pymavlink listener shows an idle max pass in the low milliseconds and a PWM
tab switch in the ~1.85 s range (the baseline, recorded in the task file).
