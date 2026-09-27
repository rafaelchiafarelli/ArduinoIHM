# Task 3: wire-into-main

**Status:** dropped 2026-09-27, never built. Rafael kept the controls already on `dev` as the final design; see the initiative README, "Closing decisions". The contract below is kept as the record of what was planned.
**Branch:** `3-wire-into-main` (from `tasks`)
**Depends on:** `nav_state_machine/1`, `nav_state_machine/2`, `input_map/1`, `input_map/2`

## Contract

### Delivers

1. **`src/main.cpp`**: one `Navigator` fed each pass by
   `dir[RE0]` -> `turn(±1)`, `clicked(BTN_PB_RE0)` -> `enter()`,
   `clicked(BTN_B0)` -> `back()`, and `millis()` -> `tick()`.
   Its events are mapped to Janus:
   - `TAB_CHANGED`: `janus_switch_screen` + the existing tab-strip
     repaint (today's RE0 branch, moved).
   - `FOCUS_CHANGED`: focus the table's widget for the current
     option or setting; at `NAV_TAB`, clear widget focus.
   - `EDIT_*`: forwarded to a stub `navEdit(...)` that does nothing yet.
     `value_editing` fills it in.
   - `TIMEOUT`: clear widget focus (the tab stays).
2. **Removed:** RE1's `janus_focus_move` branch and pbRE1's
   `janus_focus_activate` branch; the `BTN_MASK_ROT*` aliases from
   `input_map/1`. RE1, pbRE1, RE2, pbRE2 and B1-B3 are still read and
   still reported in `IHM_BOARD_STATE`, but drive nothing. **Since
   written:** RE2 steps a focused PWM frequency label (fixes/000006) and
   depends on the RE1 focus this item removes; decide its fate here (see
   the initiative README).
3. Update `ARCHITECTURE.md`'s input/UI section and
   `lib/RotaryEncoder/README.md`'s role column.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; the epic's
bench step passes over `sim_input.py --port COM3`; task file marked done
in the same commit.
