# Task 2: eeprom-image

**Status:** planned
**Depends on:** task 1

## Contract

1. An EEPROM layout for `SerialConfig`: magic, layout version, payload
   length, the packed config, CRC-16. Pure `serialize(const SerialConfig&,
   uint8_t*)` and `deserialize(...)`, so they're host-testable.
2. **Append-only extension rule:** extension fields are appended to the
   payload. On load, fields past the stored payload length take their
   defaults and everything before it is kept, so a firmware that adds a
   setting doesn't wipe the user's existing ones. Bad magic or CRC ->
   all defaults. The version is bumped only for a non-append change
   (-> all defaults).
3. An AVR-only `loadSerialConfig()` / `saveSerialConfig()` using
   `avr/eeprom.h` `eeprom_update_block` (writes only changed bytes).
4. Boot rule per initiative question 4: the loaded config is used as
   saved (no forcing).
5. Native tests: round trip, corrupt CRC -> defaults, a shorter (older)
   payload -> old fields kept + new fields default, other version ->
   defaults.
6. Record the EEPROM byte range reserved for `SerialConfig` (with room
   for extensions) in `ARCHITECTURE.md` (nothing else uses EEPROM today).
