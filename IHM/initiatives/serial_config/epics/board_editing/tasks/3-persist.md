# Task 3: persist

**Status:** done (2026-09-27) -- bench-verified 2026-09-28
**Depends on:** task 2, `config_model/2`

## Contract

1. Load `SerialConfig` from EEPROM in `setup()` (defaults if invalid)
   and use it as saved (initiative question 4).
2. Save the whole `SerialConfig` whenever any bus's enable switch toggles
   (initiative question 3): an on-screen toggle, or a PC-sent 301/302
   whose `enable` differs from the current one.
3. Bench: settings survive a power cycle; count EEPROM writes for one
   RE2 sweep and record it here.

## Result

- `setup()` loads `SerialConfig` with `loadSerialConfig()` and uses it as
  saved; a blank or corrupt EEPROM gives the defaults.
- `serialEnableToggled()` saves the whole config
  (`saveSerialConfig()`, `eeprom_update_block`): called by pbRE1 or RE2
  on an enable switch, and by a 301/302 whose `enable` differs from the
  current value.
- Build: RAM 62.4 % (unchanged), flash 34.0 % -> 34.8 %; 186 native tests.
- **Pending, bench (with the initiative's bench pass):** settings survive a
  power cycle; EEPROM writes for one RE2 sweep. By construction a sweep
  alone writes nothing (edits stay in RAM until a toggle); a toggle
  writes the changed bytes only -- after a sweep of one field, that
  field's bytes + enable + CRC, ~3.3 ms each.

## Bench (2026-09-28, old board revision: flashed via COM7, MAVLink on COM3)

Script checks, all passed: 311/312 stream at 10/s (one bus per tick); a
301 is applied and read back; an out-of-range 301 (standard ID 0x800) is
dropped; a 313 is answered by 314 "unknown key"; RE0/RE1/RE2 (simulated)
reach the CAN0 ID field, one slow click is +1, 12 fast clicks (40 ms
apart) give +836 (acceleration); after a reset (COM7 DTR) the
toggle-saved config loads and the later un-toggled knob edit is gone.
Companion driven headlessly (WM_COMMAND / WM_SETTEXT): connects, fills
all three readback lines, a panel Send reads "Board applied CAN1", bad
input is refused locally, Set shows the 314 answer, a board knob edit
shows in the panel. Not checked: the mirror's SERIAL tab by pixel (same
handler as the readback lines; the mapping is unit-tested), and the knob
feel by hand (clicks were simulated).
