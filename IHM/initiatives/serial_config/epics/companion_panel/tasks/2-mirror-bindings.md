# Task 2: mirror-bindings

**Status:** done (2026-09-28) -- bench-verified 2026-09-28
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

## Bench (2026-09-28, old board revision: flashed via COM7, MAVLink on COM3)

Script checks, all passed: 311/312 stream at 10/s (one bus per tick); a
301 is applied and read back; an out-of-range 301 (standard ID 0x800) is
dropped; a 313 is answered by 314 "unknown key"; RE0/RE1/RE2 (simulated)
reach the CAN0 ID field, one slow click is +1, 12 fast clicks (40 ms
apart) give +836 (acceleration); after a reset (COM7 DTR) the
toggle-saved config loads and the later un-toggled knob edit is gone.
Companion driven headlessly (WM_COMMAND / WM_SETTEXT): connects, fills
all three readback lines, a panel Send reads "Board applied CAN1", bad
input is refused locally, Set shows the 314 answer, a board knob edit
shows in the panel. Not checked: the mirror's SERIAL tab by pixel (same
handler as the readback lines; the mapping is unit-tested), and the knob
feel by hand (clicks were simulated).
