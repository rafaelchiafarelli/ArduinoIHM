# Epic: main_loop_timing

Protocol timers need a bounded superloop: J1939 address claim must
answer within 250 ms, UDS P2 is 50 ms, ISO-TP timeouts are about 1 s.
Today a full-screen redraw blocks for about 1 s.

## Tasks

```
1-bound-the-loop   choose and implement: Janus non_blocking render and/or ISR-side CAN work   (blocked: Q4)
```

## Acceptance gate

Measured worst-case superloop period (instrumented, e.g. via
`timeStatistics`) under a full screen switch stays below a bound the task
records (proposal: 20 ms).
