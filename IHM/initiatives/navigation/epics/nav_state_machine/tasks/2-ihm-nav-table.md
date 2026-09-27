# Task 2: ihm-nav-table

**Status:** dropped 2026-09-27, never built. Rafael kept the controls already on `dev` as the final design; see the initiative README, "Closing decisions". The contract below is kept as the record of what was planned.
**Branch:** `2-ihm-nav-table` (from `tasks`)
**Depends on:** `nav_state_machine/1-navigator`

## Contract

### Delivers

1. **`src/NavTable.h` + `NavTable.cpp`** (IHM-specific, so under `src/`,
   not `lib/`): a PROGMEM table, plus the counts interface `Navigator`
   needs.
   - Per tab: the Janus screen it maps to (`pwm_screen`, `bus_status_screen`,
     `relay_screen`), in nav-strip order.
   - Per option: the widget that shows L1 focus (see open question 1).
   - Per setting: the widget that shows L2 focus, plus a `NavSettingKind`
     (`NAV_SET_TOGGLE`, `NAV_SET_DUTY`, `NAV_SET_FREQUENCY`,
     `NAV_SET_READONLY`) and an id `value_editing` uses to find the
     action / PWM field.
2. Contents, subject to the open questions:
   - **PWM**: CH0, CH1 -> Enabled, Inverting, Frequency, Duty.
     CH2, CH3 -> Frequency, A/B/C Enabled, A/B/C Duty. Note that the
     complex channels have no Inverting widget today.
   - **SERIAL**: CAN0, CAN1, RS485 (see open question 5).
   - **Output**: Relay 0..7 (see open question 2).
3. Any yaml change open question 1 requires (for example, wrapping PWM
   rows in a `box`) plus a Janus regenerate.
4. **`test_native/test_nav_table.cpp`**: counts are consistent. Every
   widget pointer is non-null. Every setting's kind matches its widget
   kind (a toggle kind points at a toggle).

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; task file
marked done in the same commit.
