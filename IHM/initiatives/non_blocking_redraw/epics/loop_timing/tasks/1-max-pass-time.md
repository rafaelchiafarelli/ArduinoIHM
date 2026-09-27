# Task 1: max-pass-time

**Status:** planned -- blocked on initiative open question 1 (message vs field)
**Depends on:** nothing

## Contract

### Delivers

1. **Measurement** (`src/main.cpp`): the duration of every superloop pass,
   taken from a free-running timer rather than `millis()` (sub-millisecond
   resolution; which timer is this task's choice, recorded here). Keep the
   maximum over each ~100 ms telemetry window, then reset it.
2. **Pure helper**, host-tested (`test_native/`): the max-and-reset window
   logic, including timer wraparound.
3. **Transport** per open question 1. Either a new `IHM_LOOP_STATS` (310,
   board -> PC: `max_pass_us` uint32, `passes` uint16 in the window) sent in
   the ~100 ms block, or a new field in `IHM_BOARD_STATE`. Dialect +
   regenerate + `mavlink/README.md` + `MavlinkComms::send...` (layering
   rule: the class only packs).
4. **`mavlink/scripts/loop_stats.py`**: `--port`, prints max pass per
   window; `--watch` keeps printing.
5. **Baseline** recorded in this file: idle, RE0 switch to each tab, an
   on-screen action, a `PWM_CHANNEL_CONFIG` repaint.

## Definition of done

`python mavlink/generate.py` clean; `platformio run` builds;
`test_native/run_tests.ps1` passes; baseline measured on the bench (ask
Rafael before flashing) and written here; task file marked done in the
same commit.
