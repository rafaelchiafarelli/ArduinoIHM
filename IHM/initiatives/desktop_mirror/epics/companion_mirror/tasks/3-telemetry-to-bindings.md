# Task 3: telemetry-to-bindings

**Status:** done 2026-09-27.

## Delivered

- `MirrorBindings.{h,cpp}`: `MirrorApplyPwmState` (307 -> `pwm_instance`,
  same fields as `main.cpp`'s `mirrorPwmToUi`, frequency label from the
  copied `formatFrequencyLabel`, "inverted"/"non-inverted" text as in
  `main.cpp`, duty clamped to 100, out-of-range channel/selector ignored),
  `MirrorApplyRelayState` (304), `MirrorBindingsInit` (the board's
  `initPwmDefaults` boot values). A field is marked dirty only when its
  value changes.
- Label source (open question 2): verbatim copies of `PWMTiming.h`,
  `PWMChannelConfig.h`, `PWMLabelFormat.{h,cpp}` in the companion's
  `board/`. `board/SOURCE.txt` records ArduinoIHM commit 3d1063c, so the
  copies stay byte-identical and drift shows up with a plain `diff`.
- SERIAL tab (open question 3): the companion sends no CAN/RS485 config
  (it has no panel for it), so `bus_status_instance` stays at the
  generated defaults. That matches a board no tool has configured.
  Nothing to map.
- Threading: the listener already posts `WM_APP_PWM_STATE` /
  `WM_APP_RELAY_STATE`, and `WndProc` calls the two apply functions on
  the UI thread. `WM_CREATE` calls `MirrorBindingsInit()` before the boot
  render.
- Backups: `IHMPCController.cpp.mirror-bindings.bak`,
  `IHMPCController.vcxproj{,.filters}.mirror-bindings.bak`.

## Verification

- `tests/run_mirror_bindings_test.cmd` (`tests/MirrorBindingsTest.cpp`):
  boot labels, the board label test's strings (`F:15625Hz`, `F:15Hz`,
  `F:16000Hz`, `F:8000000Hz`), polarity text, duty clamp, dirty only on
  change, relay bits, out-of-range rejection. All checks pass.
- Debug x64 and Win32 build. `3-boot-values.png`: the mirror shows the
  boot values (F:62500Hz, 50 % bars, "non-inverte..." cut off by the
  widget width, as on the board's layout).
- Repainting on new telemetry is task 4.
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
