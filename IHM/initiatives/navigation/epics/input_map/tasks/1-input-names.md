# Task 1: input-names

**Status:** planned
**Branch:** `1-input-names` (from `tasks`)
**Depends on:** nothing (see the initiative README: the chain starts after serial_commands is in `dev`)

## Contract

### Delivers

1. **`lib/RotaryEncoder/RotaryEncoder.h`**: encoder index constants
   `RE0 = 0`, `RE1 = 1`, `RE2 = 2`. Existing `MAX_NUMBER_EMCODERS` stays.
2. **`lib/RotaryEncoder/ButtonMap.h`**: `btnMap` bit masks by enumeration
   name: `BTN_B0..BTN_B3` (bits 0-3), `BTN_PB_RE2` (bit 4), `BTN_PB_RE1`
   (bit 5), `BTN_PB_RE0` (bit 6). The old `BTN_MASK_ROT0/1/2` become
   aliases of the new names; they are removed in `nav_state_machine/3`.
3. **`src/main.cpp`**: raw `dir[0]` / `dir[1]` / `BTN_MASK_ROT1` uses
   switch to the new names. Pure rename, no behavior change.
4. **`lib/RotaryEncoder/README.md`**: the enumeration table (name, MCU pin,
   Mega pin, `BinaryInputs` index, `btnMap` bit), copied from the
   initiative README.
5. **`mavlink/scripts/sim_input.py`**: accept `pbRE0/pbRE1/pbRE2` and
   `B0..B3` as button names. The existing `rot0` / `0` spellings keep
   working.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes
(`test_button_map.cpp` updated to the new names); task file marked done
in the same commit.
