# Epic: main_loop_timing

With the serial part strictly event-driven (initiative rule), protocol
timing (J1939 250 ms, UDS P2 50 ms, ISO-TP ~1 s) runs in ISRs and no
longer depends on the superloop. What's left is UI responsiveness: a
full-screen redraw blocks the superloop for about 1 s, which delays
applying queued writes and UI updates (and pauses `IHM_UI_STATE`).

## Tasks

```
1-bound-the-loop   Janus non_blocking render so the superloop stays responsive   (optional; no protocol depends on it)
```

## Acceptance gate

Measured worst-case superloop period (instrumented, e.g. via
`timeStatistics`) under a full screen switch stays below a bound the task
records (proposal: 20 ms).
