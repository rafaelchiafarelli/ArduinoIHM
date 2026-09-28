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

## Open questions (each blocks the task named in brackets)

1. **Row layout rule.** 320 px at the medium font is about 29
   characters. Enable, ID, EXT, DLC and period fit; the 8 data bytes
   (16 hex characters) and the repeat count don't. Options: a second line
   per bus shown while it's selected, a collapsible box per bus, or data
   bytes editable from the PC only. The rule also has to leave room for
   the bus initiatives' fields, and say how a row changes with the bus's
   mode (e.g. RS-485 in Modbus mode shows baud/format/address, not the
   generator's LEN/period), which may need a Janus capability.
   [board_editing/1]
2. **Editing a hex ID with knobs:** digit by digit (RE1 moves between
   digits) or RE2 steps with acceleration? [board_editing/2]
3. **EEPROM save policy:** on every change (100k write cycles per cell;
   one RE2 sweep is dozens of writes), a few seconds after the last edit,
   or an explicit save action? Settings that reconfigure a live link
   (baud, address) may want to apply on leaving the field, not on every
   RE2 step; the rule belongs here so every extension follows it.
   [board_editing/2, board_editing/3]
4. **Boot behaviour:** a generator saved as enabled either starts
   transmitting at power-up or comes up disabled. On a live vehicle bus
   that's a safety choice. [config_model/2]
5. **Generator value steps:** RE2 step sizes and ranges for period
   (proposal: 10 ms steps, 0-65535), repeat count (0 = forever, 1-65535)
   and data bytes. Bus parameter value lists moved to their owners
   (decision 4). [config_model/1]
6. **PC vs board edits:** last writer wins, as for PWM (proposal).
   [wire/2]
7. **How extension settings travel.** (a) Each bus initiative adds its own
   MAVLink message pair; or (b) this initiative defines one generic pair,
   e.g. `IHM_SERIAL_SETTING` (PC -> board: bus, key, int32 value) and a
   key/value readback, so a new setting is a new key with no new message
   and no CRC change for existing ones. Proposal: (b). [wire/1]

## Not yet scoped

- ~~RS-485 driver on Serial3~~ -> planned in `rs485_modbus` (`uart3_driver`,
  `rs485_modes`: raw generator and Modbus slave), 2026-09-27.

## Branch chain

```
dev -> features -> serial_config -> epics -> <epic> -> tasks -> <task>
```

Planned 2026-09-27 as initiative-only. Create the epic and task branches
off `serial_config` when implementation starts.
