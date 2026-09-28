# Initiative: serial_config

Make the SERIAL tab configure the buses instead of only showing names and
placeholders: CAN0, CAN1 and RS-485 settings are edited on the board with
the knobs and from the PC companion, kept in EEPROM, and reported back to
the PC.

This initiative is the **framework** every serial setting goes through
(decision 4). It ships with the generator settings only; each bus
initiative adds its own settings on top of it.

## Decisions (Rafael, 2026-09-27)

1. **Layout: one compact row per bus**, at the top of the tab. Done as a
   bug fix first (`fixes/000010/serial-tab-rows`: LED, name, captioned
   values). This initiative builds the editable version on top of it.
2. **Configured from the board and from the companion.** Board: the same
   pattern as the PWM tab today (RE1 selects a field, RE2 changes it).
   Companion: a new SERIAL panel. This answered the former `navigation`
   initiative's open question 5 as **editable**.
3. **Settings in scope here:** only the **signal generator** fields the
   MAVLink messages already carry (CAN: enable, ID, standard/extended,
   DLC, data, period, repeat count; RS-485: enable, length, data, period,
   repeat count), **persisted in EEPROM** (survives a power cycle).
4. **Hybrid ownership (2026-09-27).** `serial_config` owns what is common
   to every bus: the `SerialConfig` container and its EEPROM image, the
   SERIAL tab's editing mechanics and row layout rule, the wire pattern
   and the companion's SERIAL panel. **Bus parameters and protocol
   settings belong to the initiative that uses them** and are added
   through the extension contract below, in the same task as the code
   that acts on them, so no setting sits on the screen doing nothing:

   | Settings | Owner |
   |---|---|
   | CAN bitrate | `can_bus` -> `mcp2515_driver/4-bitrate-setting` |
   | CAN protocol mode, J1939 source address, UDS / CANopen settings | each `can_bus` protocol epic, once scoped |
   | RS-485 mode, baud, format, Modbus slave address (+ write protect, response delay, status display if chosen) | `rs485_modbus` -> `rs485_modes/1-settings` |

   Implementation order: `serial_config` first.

## Extension contract (what this initiative guarantees to bus initiatives)

A bus initiative adds a setting in **one task of its own**, touching:

1. **Model:** a field in its bus's section of `SerialConfig`, with a
   default, validation and an RE2 step rule built from `config_model`'s
   step helpers (numeric step with clamp, enum cycle, toggle).
2. **EEPROM:** the field appended to the image. Fields past the stored
   payload length load as their defaults (`config_model/2`), so adding a
   field never wipes the user's existing settings.
3. **Wire:** per open question 7.
4. **Screen:** the field placed in its bus's row per the row layout rule
   (open question 1), with its binding and action as in `board_editing`.
5. **Companion:** the field added to the SERIAL panel and mirror bindings,
   following `companion_panel`'s pattern.
6. **Apply:** on the change event, calling the owning driver (see the
   no-polling rule).

`config_model/1` writes this recipe into `lib/BusConfig/README.md` once
the code exists; this section is the plan-level version.

## Rule: no polling in the serial part (Rafael, 2026-09-27)

Everything that talks to CAN or RS-485 is strictly interrupt-driven (see
`can_bus` and `rs485_modbus`). For this initiative: applying a changed
`SerialConfig` to a bus happens on the change event (an edit, a MAVLink
message, a Modbus write), never by the superloop re-checking the config.
The MAVLink link itself (301/302 and the new messages) is exempt.

## Depends on

- Nothing to start. The generators don't transmit yet (`can_bus`'s
  `can_generator` and `rs485_modbus`'s `rs485_modes` make them), so until
  those land the generator settings are **stored, shown and reported
  only**, as they are today. Nothing in this initiative drives a CAN
  controller or USART3.
- The on-screen controls are final (the `navigation` initiative was closed
  and removed 2026-09-27): "selected" means the RE1 focus ring, as on the
  PWM tab.

## Epics

| Epic | Scope |
|---|---|
| `config_model` | Pure `SerialConfig` model: generator fields, ranges, RE2 step helpers, extensible EEPROM image (version + length + CRC). Host-tested. |
| `wire` | MAVLink: board -> PC telemetry of the config (so the companion and its mirror show the board's truth), and the mechanism bus initiatives use to carry their settings (Q7). PC -> board generator settings stay on 301/302. |
| `board_editing` | SERIAL tab fields become focusable and editable (RE1/RE2), with the row layout rule, bindings and actions; EEPROM save policy. |
| `companion_panel` | IHMPCController SERIAL panel (sends 301/302, shows the readback); mirror bindings for `bus_status` from the new telemetry. |

Order: `config_model` -> `wire` -> `board_editing` and `companion_panel`
(these two in parallel).

## Open questions -- all answered (Rafael, 2026-09-27)

1. **Row layout rule: same logic as the PWM tab.** Every value is its own
   focusable field: RE1 moves between fields, RE2 changes the focused one.
   No button press is needed to edit. Two rows per bus:
   - row 1: enable switch, bus name, ID and DLC (CAN) / LEN (RS-485);
   - row 2: period, repeat count, byte index (`B3`) and that byte's
     value, plus EXT for CAN. The byte index picks which data byte the
     value field shows and edits.
   A bus initiative adds its fields to its bus's rows; a row that changes
   with a mode (e.g. RS-485 in Modbus mode) is that initiative's to design.
2. **Numeric fields: RE2 with digit-at-a-time acceleration.** A step starts
   at one unit. About 4 fast clicks in a row (< 80 ms apart) move it up one
   digit (x16 for hex fields, x10 for decimal ones); a pause of about
   300 ms drops it one digit; a pause of 1.5 s or a focus change resets it
   to one unit. The step never exceeds the field's top digit. RE2 is
   sampled every 25 ms with one pending direction, so ~15-20 clicks/s is
   the practical rate: standard ID ~2-3 s worst case, extended ID
   ~6-10 s, period ~4-6 s.
3. **Save policy: each bus's enable switch.** Edits apply live in RAM.
   Toggling any bus's enable switch, on or off, from the board or from
   the PC, saves the whole `SerialConfig` to EEPROM. Edits made without a
   toggle are lost at power-off.
4. **Boot: load the saved config and apply it as saved.** A generator
   saved as enabled runs at power-up (Rafael's call, with the safety
   trade-off stated).
5. **Generator steps:** period 0-65530 ms in 10 ms units; repeat count
   0 (forever) to 65535; data bytes 0x00-0xFF; DLC 0-8; RS-485 length
   0-32; all with the acceleration from Q2 where they're numeric.
6. **PC vs board edits: last writer wins**, as for PWM; the companion
   shows the board's readback.
7. **Extension settings travel generically:** one pair
   `IHM_SERIAL_SETTING` (PC -> board: bus, key, int32 value) and
   `IHM_SERIAL_SETTING_STATE` (board -> PC: bus, key, value, status). A
   new setting is a new key in the key table (`mavlink/README.md`), with
   no new message. The companion is updated to match.

## Not yet scoped

- ~~RS-485 driver on Serial3~~ -> planned in `rs485_modbus` (`uart3_driver`,
  `rs485_modes`: raw generator and Modbus slave), 2026-09-27.

## Branch chain

```
dev -> features -> serial_config -> epics -> <epic> -> tasks -> <task>
```

Planned 2026-09-27 as initiative-only. Create the epic and task branches
off `serial_config` when implementation starts.
