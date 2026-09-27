# Task 2: mirror-bindings

**Status:** planned
**Depends on:** `wire/2`, `board_editing/1`

## Contract

1. `MirrorBindings`: config telemetry -> `bus_status_instance` +
   `bus_status_dirty`, mapped the way the board fills them (including any
   text fields from `board_editing/1`).
2. The "SERIAL tab shows PC-sent config only" caveat is removed from
   `HOW_TO_USE.md` and from `desktop_mirror`'s records.
3. `tests/MirrorBindingsTest.cpp` covers the new mapping.
