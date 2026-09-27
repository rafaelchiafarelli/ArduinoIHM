# Task 3: telemetry-to-bindings

**Status:** planned (labels: copied source; SERIAL tab: PC-sent config -- open questions 2 and 3 answered)
**Depends on:** task 2

## Contract

`MirrorBindings.{h,cpp}` in the companion, called from the existing
MAVLink receive path:

1. `IHM_PWM_STATE` (307) -> `pwm_instance` + `pwm_dirty`, field for field
   as `main.cpp`'s `mirrorPwmToUi()` does. That includes the
   `chN_state_label` frequency text (via `formatFrequencyLabel`, sourced
   per open question 2) and the `chN_inverting_label` text. Only changed
   fields are marked dirty.
2. `IHM_RELAY_STATE` (304) -> `relay_instance` + `relay_dirty`.
3. `bus_status_instance` per open question 3.
4. Thread rule: the listener thread never touches the bindings. It
   posts the decoded payload to the UI thread, which owns every Janus
   call.
5. A unit check (a small console test is enough) that a known 307
   payload produces the same label strings that the board's
   `test_native/test_pwm_label_format.cpp` expects.
