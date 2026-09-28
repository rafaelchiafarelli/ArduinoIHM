# Task 2: mirror-bindings

**Status:** done (2026-09-28) -- unit check passes; bench pending (see Result)
**Depends on:** `wire/2`, `board_editing/1`

## Contract

1. `MirrorBindings`: config telemetry -> `bus_status_instance` +
   `bus_status_dirty`, mapped the way the board fills them (including any
   text fields from `board_editing/1`).
2. The "SERIAL tab shows PC-sent config only" caveat is removed from
   `HOW_TO_USE.md` and from `desktop_mirror`'s records.
3. `tests/MirrorBindingsTest.cpp` covers the new mapping.

## Result

- `MirrorBindings.{h,cpp}`: `MirrorApplySerialState()` fills
  `bus_status_instance` from 311/312 as the firmware's `mirrorSerialToUi()`
  does (changed fields only marked dirty; repeat text `x inf` / `x<n>`),
  called from the SERIAL panel's `WM_APP_SERIAL_STATE` handler.
  `MirrorBindingsInit()` seeds the SERIAL tab with `serialConfigDefaults`.
- **Known difference:** the board's `B<n>` byte selection is on-screen UI
  state not in 311/312, so the mirror always shows `B0` and byte 0. Adding
  it would be a wire change (a field in 311/312 or a UI-state message) --
  not done here; flagged to Rafael.
- The "SERIAL tab shows defaults only" caveat is replaced in
  `HOW_TO_USE.md` (`desktop_mirror`'s own records were removed when that
  initiative closed, so there is nothing else to update).
- `tests/MirrorBindingsTest.cpp`: SERIAL boot values, a CAN1 state (value,
  dirty-only-on-change, repeat text), RS-485, out-of-range bus, and the
  314 WPARAM/LPARAM packing. `tests\run_mirror_bindings_test.cmd`: all
  checks passed.
- **Pending, bench:** the mirror's SERIAL tab follows a knob edit on the
  board.
