# Initiative: rs485_modbus

Modbus RTU on the board's RS-485 port (Serial3), with the board as a
**slave (server)** first: a PLC, SCADA system or PC polls it and can
drive it.

## Decisions (Rafael, 2026-09-27)

1. **Slave first.** Master (client) mode is a later epic (see "Not yet
   scoped").
2. **What the slave exposes:**
   - read-only telemetry: buttons, encoders, analog inputs, relay and
     PWM state;
   - writable **relays**;
   - writable **PWM** (frequency, duty, enable, invert per channel);
   - writable **bus config** (the `serial_config` settings).
3. **One RS-485 mode at a time:** off / raw generator
   (`RS485_SIGNAL_CONFIG`, 302) / Modbus slave (later: Modbus master).
   The mode is part of `serial_config`'s protocol mode, so raw bytes
   never get injected into a Modbus network.

## Hardware (from `IOs IHM.xlsx` and the schematic, 2026-09-27)

- RS-485 is **Serial3**: TX3 = D14, RX3 = D15, on a 3-pin header.
- **No transceiver and no driver-enable (DE/RE) pin on the board.** An
  external module is needed: either one that switches direction by
  itself, or a MAX485-type module that needs a spare GPIO for DE/RE
  (open question 1).
- USART3 is unused today. `lib/Uart2` is USART2-specific, but its
  `ByteRing` can be reused.
- RAM: 59 % used (about 3.3 KB free), shared with `can_bus`'s plans
  (proposed ceiling 80 %). A Modbus RTU frame is at most 256 B.

## How it fits the existing architecture

`MavlinkComms::fast_handler()` already parses Serial2 inside the
preemptible Timer2 tick (~1 ms), so the protocol keeps running while the
superloop is blocked by a ~1 s screen redraw. **Proposal:** the Modbus
slave does the same. It frames, parses and answers from the tick, reading
state snapshots; writes are queued and applied by the superloop (the
same split as `PWM_CHANNEL_CONFIG`). Response time then doesn't depend
on the UI. Modbus masters typically time out after 100-1000 ms.

## Epics

| Epic | Scope |
|---|---|
| `uart3_driver` | Interrupt-driven USART3: RX/TX rings, direction control (DE/RE or auto), transmit-complete handling, byte timestamps for frame gaps. |
| `rtu_framing` | Pure RTU layer: CRC-16/MODBUS, frame end by 3.5-character silence, address filter, request parse, response and exception building. Host-tested. |
| `slave_register_map` | The register map, function codes 01/02/03/04/05/06/15/16, applying writes with the same validation as MAVLink, and an integrator-facing map document. |
| `rs485_modes` | Mode switch (off / raw generator / Modbus slave) driven by `serial_config`; the raw generator implemented on Serial3; slave address, baud and parity applied from the config. |

Order: `uart3_driver` and `rtu_framing` (in parallel) ->
`slave_register_map` -> `rs485_modes`.

## Depends on

- **`serial_config`**: RS-485 baud/parity, the RS-485 mode and the slave
  address live in its `SerialConfig`, so its open question 5 (value
  lists) must include them (pointer added there). The bus-config
  registers write through the same model.
- **`can_bus`** only for the shared RAM budget (its open question 7). The
  timing concern is avoided by the Timer2-tick proposal above.

## Open questions (each blocks the task named in brackets)

1. **RS-485 module and direction control:** an auto-direction module, or
   MAX485-type with DE/RE on which GPIO? Termination and bias resistors:
   on the module or external? [uart3_driver/1]
2. **Where the slave runs:** in the Timer2 tick, as proposed above, or in
   the superloop (simpler, but replies stall during redraws)?
   [rtu_framing/2, slave_register_map/2]
3. **Register map details:** addresses; 0- or 1-based documentation;
   analog as raw ADC counts or scaled; 32-bit values (CAN IDs) as two
   registers and their word order; PWM frequency as the selector enum or
   in Hz. [slave_register_map/1]
4. **Defaults:** 19200 baud 8E1 (the Modbus spec default) or 9600 8N1;
   slave address 1. [rs485_modes/1]
5. **Function codes:** proposal 01, 02, 03, 04, 05, 06, 15, 16. Add 08
   (diagnostics echo) and 43/14 (device identification)?
   [slave_register_map/2]
6. **Writing bus config over Modbus:** changing the RS-485 baud, parity
   or address over Modbus cuts the link that sent it. Apply only after a
   "commit" register write? And should Modbus be allowed to change the
   RS-485 mode at all? [slave_register_map/3]
7. **USART3 driver code:** duplicate `lib/Uart2` as `lib/Uart3`, or turn
   it into one driver for both ports? The second touches
   `serial_commands`' Serial2 transport, which is bench-verified.
   [uart3_driver/1]

## Not yet scoped

- **Modbus master** (poll other devices; show or log their registers,
  optionally to SD via `can_bus`'s `sd_storage`).
- Modbus TCP (no Ethernet on the board).

## Branch chain

```
dev -> features -> rs485_modbus -> epics -> <epic> -> tasks -> <task>
```

Planned 2026-09-27 as initiative-only (this clone's `epics`/`tasks` names
belong to the open desktop_mirror chain).
