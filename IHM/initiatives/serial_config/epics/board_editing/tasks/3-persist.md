# Task 3: persist

**Status:** planned -- blocked on initiative open question 3 (save policy)
**Depends on:** task 2, `config_model/2`

## Contract

1. Load `SerialConfig` from EEPROM in `setup()` (defaults if invalid),
   applying the boot rule from open question 4.
2. Save per open question 3, from board edits and from PC-sent configs
   alike.
3. Bench: settings survive a power cycle; count EEPROM writes for one
   RE2 sweep and record it here.
