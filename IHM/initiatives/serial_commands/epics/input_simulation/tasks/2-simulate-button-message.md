# Task 2: simulate-button-message

**Status:** done
**Branch:** none -- committed directly as `62f6d79` (2026-09-20) without a
task file. This file was written 2026-09-25 to record it.
**Depends on:** nothing

## Contract (as delivered)

1. **Dialect:** `IHM_SIMULATE_BUTTON`, id 306, PC -> board, one field,
   `button_mask` (`uint8_t`). Bit i is button i in `IHM_BOARD_STATE.buttons`
   layout: B0-B3 are bits 0-3, pbRE2/pbRE1/pbRE0 are bits 4/5/6. Bit 7 is
   ignored.
2. **`MavlinkComms`:** masks accumulate in `simulatedButtonMask` (ISR side).
   `consumeSimulatedButtons()` is an atomic take-and-clear.
3. **`main.cpp`:** `btnMap &= ~consumeSimulatedButtons()` right after
   `buildButtonMap()`, so each button reads as pressed (active-low) for one
   superloop pass. Real presses still win.
4. **`mavlink/scripts/sim_input.py`:** `button` / `encoder` subcommands.

## Bench (2026-09-25)

10/10 pbRE1 presses toggled a relay from pymavlink after `serial_transport`
task 6, and the companion's Push button did the same. Presses less than
~0.7 s apart merge while the board redraws after an action.
