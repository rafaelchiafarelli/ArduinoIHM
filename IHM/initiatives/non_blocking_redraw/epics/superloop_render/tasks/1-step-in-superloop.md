# Task 1: step-in-superloop

**Status:** planned -- blocked on initiative open questions 2 and 3
**Depends on:** `janus_render/1`, `loop_timing/1`

## Contract

### Delivers

1. **`src/main.cpp`**: one render step per superloop pass, within the
   budget from open question 2, after input handling and before the
   telemetry block.
2. **Every blocking render call site converted** to start a job instead:
   the boot render (`setup()`), the RE0 tab switch, the ACTION re-render
   (`janus_render_screen` after pbRE1), `navigate` targets, the dirty
   repaints (RE2 edits, `PWM_CHANNEL_CONFIG`, `IHM_RELAY_COMMAND`, the
   ~100 ms status refresh), RE1 focus moves and box toggles. Grep for
   `janus_render_`, `janus_switch_screen` and `janus_focus_` to be sure
   none is left.
3. **Input while a job runs** per open question 3. A second RE0 switch
   mid-job supersedes it (the handoff's rule 4).
4. **`ARCHITECTURE.md`** UI section: rendering is stepped, never blocking.
   `ui-rotation.md` gets a pointer that its scheduler question is answered
   here.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; `loop_stats.py`
shows max pass <= 20 ms for a PWM tab switch on the bench; task file marked
done in the same commit.
