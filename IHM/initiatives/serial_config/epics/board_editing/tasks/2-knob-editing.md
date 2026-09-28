# Task 2: knob-editing

**Status:** planned
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
