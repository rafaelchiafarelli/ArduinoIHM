# Initiative: serial_config

Make the SERIAL tab configure the buses instead of only showing names and
placeholders: CAN0, CAN1 and RS-485 settings are edited on the board with
the knobs and from the PC companion, kept in EEPROM, and reported back to
the PC.

## Decisions (Rafael, 2026-09-27)

1. **Layout: one compact row per bus**, at the top of the tab. Done as a
   bug fix first (`fixes/000010/serial-tab-rows`: LED, name, captioned
   values). This initiative builds the editable version on top of it.
2. **Configured from the board and from the companion.** Board: the same
   pattern as the PWM tab today (RE1 selects a field, RE2 changes it).
   Companion: a new SERIAL panel. This answers navigation's open
   question 5 as **editable** (pointer added to `navigation/README.md`).
3. **Settings in scope:**
   - the **signal generator** fields the MAVLink messages already carry
     (CAN: enable, ID, standard/extended, DLC, data, period, repeat count;
     RS-485: enable, length, data, period, repeat count);
   - **bus parameters** (CAN bitrate; RS-485 baud and parity);
   - **protocol mode** per CAN bus (raw / J1939 / UDS / CANopen) plus
     e.g. the J1939 source address;
   - **persisted in EEPROM** (survives a power cycle).

## Depends on

- **`can_bus`** (sibling initiative, planned the same day) for bus
  parameters, protocol mode and the CAN generator to act on real
  hardware. Until it lands, these settings are **stored, shown and
  reported only**. Nothing in this initiative drives a CAN controller.
  The RS-485 generator needs a Serial3 driver, which neither initiative
  covers yet (see "Not yet scoped").
- `navigation` (parked): until its navigator lands, "selected" means the
  RE1 focus ring, as on the PWM tab.

## Epics

| Epic | Scope |
|---|---|
| `config_model` | Pure `SerialConfig` model: fields, ranges, RE2 step rules, EEPROM image (version + CRC). Host-tested. |
| `wire` | MAVLink: a PC -> board message for bus parameters and protocol mode, and board -> PC telemetry of the whole config (so the companion and its mirror show the board's truth). |
| `board_editing` | SERIAL tab fields become focusable and editable (RE1/RE2), with new bindings and actions; EEPROM save policy. |
| `companion_panel` | IHMPCController SERIAL panel (sends 301/302 plus the new message, shows the readback); mirror bindings for `bus_status` from the new telemetry. |

Order: `config_model` -> `wire` -> `board_editing` and `companion_panel`
(these two in parallel).

## Open questions (each blocks the task named in brackets)

1. **What fits on a compact row?** 320 px at the medium font is about
   29 characters. Enable, bitrate, ID, EXT, DLC and period fit. The
   8 data bytes (16 hex characters) and the repeat count don't. Options:
   a second line per bus shown while it's selected, a collapsible box per
   bus, or data bytes editable from the PC only. [board_editing/1]
2. **Editing a hex ID with knobs:** digit by digit (RE1 moves between
   digits) or RE2 steps with acceleration? [board_editing/2]
3. **EEPROM save policy:** on every change (100k write cycles per cell;
   one RE2 sweep is dozens of writes), a few seconds after the last edit,
   or an explicit save action? [board_editing/3]
4. **Boot behaviour:** a generator saved as enabled either starts
   transmitting at power-up or comes up disabled. On a live vehicle bus
   that's a safety choice. [config_model/2]
5. **Value lists:** proposal: CAN 125/250/500/1000 kbit/s; RS-485
   9600/19200/38400/57600/115200 baud, parity N/E/O; protocol raw /
   J1939 / UDS / CANopen; J1939 source address 0-253. [config_model/1]
   *Pointer (2026-09-27):* `rs485_modbus` needs the RS-485 mode (off /
   raw generator / Modbus slave) and a Modbus slave address (1-247) in
   this list too.
6. **PC vs board edits:** last writer wins, as for PWM (proposal).
   [wire/2]

## Not yet scoped

- ~~RS-485 driver on Serial3~~ -> planned in `rs485_modbus` (`uart3_driver`,
  `rs485_modes`: raw generator and Modbus slave), 2026-09-27.

## Branch chain

```
dev -> features -> serial_config -> epics -> <epic> -> tasks -> <task>
```

Planned 2026-09-27 as initiative-only (this clone's `epics`/`tasks` names
belong to the open desktop_mirror chain). Create the epic and task
branches off `serial_config` when implementation starts.
