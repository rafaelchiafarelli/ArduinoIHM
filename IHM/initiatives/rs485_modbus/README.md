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
   - writable **bus config** (the `SerialConfig` settings).
3. **One RS-485 mode at a time:** off / raw generator
   (`RS485_SIGNAL_CONFIG`, 302) / Modbus slave (later: Modbus master).
   The mode is an RS-485 setting in `SerialConfig`, so raw bytes never
   get injected into a Modbus network.
4. **This initiative owns the RS-485 settings** (2026-09-27, hybrid
   ownership in `serial_config`'s README): mode, baud, format, slave
   address and any Modbus options are added to `serial_config`'s framework
   by `rs485_modes/1-settings`, not planned inside `serial_config`.

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

## Rule: strictly event-driven (Rafael, 2026-09-27)

**No polling in the serial part.** Nothing scans the RX ring on a tick,
and nothing checks serial state from the superloop. The chain is:

1. **USART3 RX ISR** stores the byte and re-arms a one-shot **silence
   timer** (`can_bus`'s `event_timer`) for 3.5 character times.
2. **Silence-timer expiry** = end of frame. The expiry ISR (interrupts
   re-enabled first) checks the CRC and address, runs the slave engine
   on state snapshots, and queues the response.
3. **UDRE ISR** sends it; the **TXC ISR** releases DE/RE after the last
   stop bit.
4. Writes are queued; the superloop gets a "work pending" event and
   applies them through the same functions as MAVLink.

Response time therefore doesn't depend on the ~1 s screen redraws.
Modbus masters typically time out after 100-1000 ms. The MAVLink link on
Serial2 is exempt from the rule and stays as it is; its handler is
always the first call after `sei()` in the Timer2 ISR (`fixes/000011`).

## Epics

| Epic | Scope |
|---|---|
| `uart3_driver` | Interrupt-driven USART3: RX ISR re-arms the silence timer per byte, UDRE sends, TXC releases DE/RE. No ring is ever polled. |
| `rtu_framing` | Pure RTU layer: CRC-16/MODBUS, frame end by 3.5-character silence, address filter, request parse, response and exception building. Host-tested. |
| `slave_register_map` | The register map, function codes 01/02/03/04/05/06/15/16, applying writes with the same validation as MAVLink, and an integrator-facing map document. |
| `rs485_modes` | The RS-485 settings (mode, baud, format, slave address) as a `serial_config` extension; the mode switch (off / raw generator / Modbus slave); the raw generator on Serial3; port settings applied from the config. |

Order: `uart3_driver` and `rtu_framing` (in parallel) ->
`slave_register_map` -> `rs485_modes`.

## Depends on

- **`serial_config`** (implemented first): its framework (`SerialConfig`,
  EEPROM image, SERIAL tab editing, wire, companion panel) and extension
  contract. `rs485_modes/1` adds the RS-485 settings through it; the
  bus-config registers write through the same model.
- **`can_bus`'s `event_timer` epic** (one-shot compare + software queue)
  for the silence timer, and its RAM budget (its open question 7).
  `event_timer` must land before `uart3_driver`'s frame timing.

## Open questions (each blocks the task named in brackets)

1. **RS-485 module and direction control:** an auto-direction module, or
   MAX485-type with DE/RE on which GPIO? Termination and bias resistors:
   on the module or external? [uart3_driver/1]
2. ~~Where the slave runs~~ **Answered 2026-09-27: strictly
   event-driven** (the rule above). Remaining: **ISR budget.** Answering
   an FC 03 read of 125 registers inside the silence-timer ISR means
   building ~255 bytes of response. Proposal: the ISR builds from
   snapshots with a bounded copy (~200 us) and interrupts enabled, so
   USART2 at 250000 baud never overruns. [rtu_framing/2]
3. **Register map details:** addresses; 0- or 1-based documentation;
   analog as raw ADC counts or scaled; 32-bit values (CAN IDs) as two
   registers and their word order; PWM frequency as the selector enum or
   in Hz. [slave_register_map/1]
4. **Defaults:** 19200 baud 8E1 (the Modbus spec default) or 9600 8N1;
   slave address 1; mode off. [rs485_modes/1]
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
8. **Format field:** one combined setting `8E1 / 8O1 / 8N2 / 8N1` (the
   spec says no parity uses 2 stop bits, many devices use 8N1 anyway), or
   separate parity and stop-bit settings? [rs485_modes/1]
9. **Optional settings:** add any of **write protect** (the panel blocks
   Modbus writes, reads keep working), **response delay** (0-50 ms before
   replying, for slow masters/converters), and a **status display**
   (activity LED on a valid request for our address; the `rtu_framing/2`
   counters on the screen and to the PC)? [rs485_modes/1]

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
