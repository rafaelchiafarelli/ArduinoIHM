# Task 2: click-edges

**Status:** planned
**Branch:** `2-click-edges` (from `tasks`)
**Depends on:** `input_map/1-input-names`

## Contract

### Delivers

1. **`lib/RotaryEncoder/ButtonClicks.h`** (header-only, no AVR includes):
   `class ButtonClicks { void update(uint8_t btnMap); bool clicked(uint8_t mask) const; bool anyActivity() const; }`.
   - `update` is called once per superloop pass with the active-low
     `btnMap`. `clicked(mask)` is true for exactly the pass on which that
     button went from released to pressed. This is the edge detection
     `main.cpp` does inline with `prevBtnMap` today.
   - Initial state is "all released" (`0xFF`), so a button held at boot
     does not fire.
   - `anyActivity()`: at least one click happened this pass. Bit 7, the
     sentinel, is ignored.
   - No debounce timing in this task. The pass rate is the only filter,
     the same as today.
2. **`test_native/test_button_clicks.cpp`**: single-fire on press, no fire
   while held, fire again after release+press, two buttons in the same
   pass, sentinel ignored, held-at-boot does not fire.
3. **`src/main.cpp`**: the inline `prevBtnMap` edge detection for pbRE1 is
   replaced by `ButtonClicks`. No behavior change.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; task file
marked done in the same commit.
