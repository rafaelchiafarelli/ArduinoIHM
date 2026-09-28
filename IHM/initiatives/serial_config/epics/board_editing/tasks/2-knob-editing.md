# Task 2: knob-editing

**Status:** done (2026-09-27)
**Depends on:** task 1, `config_model/1`

## Contract

1. One `on_press` action per editable field (Janus generates the enum;
   `src/janus_actions.cpp` handles it), following `value_editing`'s
   pattern: RE2 on the focused field calls the `config_model` step rule,
   applies it to the live `SerialConfig`, and marks the bindings dirty.
2. Numeric fields step with `config_model`'s acceleration helper
   (initiative question 2); the enable switch and EXT are set by RE2
   (CW = on), never flipped, like the PWM switches; a press on the enable
   switch flips it.
3. Edits take effect live, with the same meaning as a PC-sent config.

## Result

- Pure `lib/BusConfig/src/SerialEdit.h/.cpp`: `serialEditStep()` (bus,
  field, direction, acceleration state, time) and `serialToggleEnable()`;
  12 native tests (186 total pass).
- `main.cpp`: RE2 on a focused SERIAL field maps its action to (bus,
  field) and calls `serialEditStep()` with `millis()`; the acceleration
  resets when RE2 lands on a different field. `janus_actions.cpp`: pbRE1
  on an enable switch flips it (`serialPressEnable`); a press on the other
  fields does nothing (as the duty bars). Both call
  `serialEnableToggled()`, the save hook task 3 fills.
- `mirrorSerialToUi()` now marks only changed fields dirty, so an RE2
  click repaints one label instead of all 22 (fast turning depends on it).
- RAM 62.3 % -> 62.4 %, flash 32.9 % -> 34.0 %.
