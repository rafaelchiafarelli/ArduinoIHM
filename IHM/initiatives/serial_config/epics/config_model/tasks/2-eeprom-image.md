# Task 2: eeprom-image

**Status:** planned -- blocked on initiative open question 4 (boot behaviour)
**Depends on:** task 1

## Contract

1. A fixed EEPROM layout for `SerialConfig`: magic, layout version, the
   packed config, CRC-16. Pure `serialize(const SerialConfig&, uint8_t*)`
   and `deserialize(...)` (returns defaults on bad magic/version/CRC), so
   they're host-testable.
2. An AVR-only `loadSerialConfig()` / `saveSerialConfig()` using
   `avr/eeprom.h` `eeprom_update_block` (writes only changed bytes).
3. Boot rule per open question 4 (e.g. generators forced disabled after
   load).
4. Native tests: round trip, corrupt CRC -> defaults, old version ->
   defaults.
5. Record the EEPROM byte range used in `ARCHITECTURE.md` (nothing else
   uses EEPROM today).
