# Task 2: knob-editing

**Status:** planned -- blocked on initiative open question 2 (hex ID editing)
**Depends on:** task 1, `config_model/1`

## Contract

1. One `on_press` action per editable field (Janus generates the enum;
   `src/janus_actions.cpp` handles it), following `value_editing`'s
   pattern: RE2 on the focused field calls the `config_model` step rule,
   applies it to the live `SerialConfig`, and marks the bindings dirty.
2. Hex ID editing per open question 2.
3. Edits take effect live, with the same meaning as a PC-sent config.
