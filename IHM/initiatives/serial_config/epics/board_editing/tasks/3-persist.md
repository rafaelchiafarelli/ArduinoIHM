# Task 3: persist

**Status:** planned
**Depends on:** task 2, `config_model/2`

## Contract

1. Load `SerialConfig` from EEPROM in `setup()` (defaults if invalid)
   and use it as saved (initiative question 4).
2. Save the whole `SerialConfig` whenever any bus's enable switch toggles
   (initiative question 3): an on-screen toggle, or a PC-sent 301/302
   whose `enable` differs from the current one.
3. Bench: settings survive a power cycle; count EEPROM writes for one
   RE2 sweep and record it here.
