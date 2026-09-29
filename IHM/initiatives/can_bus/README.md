# Initiative: can_bus

Real CAN on the board's two external CAN modules, then (as later epics)
the protocols Rafael asked about on 2026-09-27: SAE J1939, ISO 14229
(UDS) and CANopen, with the SD card for storage. This initiative is the
driver foundation. `serial_config` (done and closed 2026-09-28) built the
settings framework; this initiative adds its own settings to it (see "Settings"
below).

## Hardware (from `IOs IHM.xlsx`; to confirm, open question 1)

- **MCU:** ATmega2560 at 16 MHz. **RAM 8 KB**, 59 % used on 2026-09-27
  (about 3.3 KB free; the binding constraint). **Flash 256 KB**, about
  175 KB free. **EEPROM 4 KB**, unused.
- **CAN:** no CAN controller on the board. The schematic has only `Can0`
  and `Can1` connectors, and the sheet lists two SPI modules:
  CAN-1 CS D18 / INT D19 (INT2), CAN-2 CS D28 / INT D3 (INT5). That fits
  MCP2515 modules: classic CAN 2.0B up to 1 Mbit/s, 2 RX buffers, 3 TX
  buffers, 6 filters / 2 masks. **No CAN FD.**
- **SD card:** on the same SPI bus. CS is D42 in one sheet and D34 in
  the other. `lib/SD` (old Arduino SD) is present and unused; FAT costs
  about 600 B of RAM.
- **RS-485:** Serial3 (D14/D15), not CAN, and out of scope here.
- **TFT:** parallel, not on SPI.
- **Timing:** a full-screen redraw blocks the superloop for about 1 s
  (2 s seen on the bench), and the MCP2515 holds only 2 frames. At
  250 kbit/s a busy bus can deliver a frame every ~0.5 ms.

## Rule: strictly event-driven (Rafael, 2026-09-27)

**No polling anywhere in the serial part** (CAN, RS-485/Modbus). Every
step is triggered by an interrupt:

- MCP2515 INT for RX, TX-complete and errors;
- a one-shot hardware timer compare for periods and timeouts;
- no periodic tick scanning rings or deadlines;
- no main-loop checks of serial state.

The superloop only receives "work pending" events (e.g. apply a queued
write, update the UI). The existing MAVLink link on Serial2 is exempt
and stays as it is; its handler is always the first call after `sei()`
in the Timer2 ISR (`fixes/000011`).

Timer resources: Timer1/3/4/5 are PWM, Timer2 Compare-A is the system
tick, and Timer0's overflow drives Arduino's `millis()`. Free: Timer2
Compare-B, and Timer0 Compare-A/B.

## Epics (foundation)

| Epic | Scope |
|---|---|
| `event_timer` | One-shot hardware timer compare + a small software timer queue: the only time source for periods and timeouts (CAN generator, protocol timers, and `rs485_modbus`'s frame-silence timer). |
| `spi_sharing` | One SPI discipline for CAN-1, CAN-2 and the SD card, including access from an ISR. |
| `mcp2515_driver` | Driver for both modules: bit timing from the crystal; everything from the INT line (RX handed straight to the protocol layer, TX-complete starts the next queued frame, error state); filters. |
| `can_generator` | `CAN_SIGNAL_CONFIG` (301) actually transmits: each period is an `event_timer` expiry, never a checked deadline. |
| `main_loop_timing` | UI responsiveness only: with the serial part interrupt-driven, protocol timing no longer depends on the superloop. Optional Janus non-blocking render. |
| `sd_storage` | Mount, CS pin, a small file API, buffered writes, RAM budget. |

Order: `event_timer` and `spi_sharing` -> `mcp2515_driver` ->
`can_generator`; `sd_storage` in parallel; `main_loop_timing` whenever
the UI needs it. Protocol epics start once
the relevant foundation epics are done.

## Settings (hybrid ownership, 2026-09-27)

`serial_config` owns the `SerialConfig` framework and the generator
settings. Bus parameters and protocol settings belong here and are added
through its extension recipe (`lib/BusConfig/README.md`),
in the same task as the code that acts on them:

- **CAN bitrate** per bus: `mcp2515_driver/4-bitrate-setting`.
- **Protocol mode** per bus (raw / J1939 / UDS / CANopen) and protocol
  settings (J1939 source address, UDS/CANopen addressing): a settings
  task in each protocol epic when it is scoped (open question 5).

## Protocols: feasibility on this hardware (2026-09-27), not yet scoped

Each becomes its own epic once open question 5 picks it.

- **J1939** (29-bit IDs, 250/500 kbit/s)
  - *Fits:* address claim (J1939-81); single-frame PGNs; BAM and
    RTS/CTS transport for small payloads; DM1.
  - *SD card:* holds the PGN/SPN database (a compact file converted on
    the PC from a DBC), looked up per PGN with a small RAM cache.
  - *Limits:* no 1785-byte reassembly in RAM (stream to SD instead);
    full-bus logging of a busy 250 kbit/s bus to SD is unreliable
    (100-250 ms SD write stalls vs about 3 KB of RAM), filtered logging
    works; no J1939-22 (FD). The SAE Digital Annex is licensed, so we
    supply our own subset.
- **ISO 14229 (UDS)** over ISO-TP (ISO 15765-2)
  - *Fits:* a tester or a server with 0x10, 0x11, 0x22, 0x2E, 0x19,
    0x3E, 0x31. As a tester, flashing an ECU (0x34/0x36/0x37) can stream
    the image from SD.
  - *Limits:* messages of about 256-512 B, not 4095. P2 = 50 ms and the
    ISO-TP timeouts need `main_loop_timing`. SecurityAccess (0x27)
    seed/key algorithms are OEM-specific.
- **CANopen** (CiA 301)
  - *Fits:* a minimal node or master with NMT, heartbeat, SDO
    (expedited + segmented), a few PDOs, and the object dictionary in
    flash. EDS files could live on SD.
  - *Limits:* full stacks (CANopenNode, CanFestival) are too RAM-heavy
    for 8 KB, so we'd write a small stack.

## Open questions (each blocks the epic/task named in brackets)

1. **Module part numbers:** which CAN modules, crystal frequency
   (8 or 16 MHz sets the bit timing) and transceiver, on both hardware
   revisions? [mcp2515_driver/1]
2. **SD card:** CS pin D42 or D34? Is the socket fitted on each
   revision? [sd_storage/1]
3. **Serial1:** CAN-1 uses D18/D19, which are TX1/RX1, so Serial1 can't
   be used. OK? [spi_sharing/1]
4. ~~Timing strategy~~ **Answered 2026-09-27: strictly event-driven**
   (see the rule above). Protocol work runs from the MCP2515 INT and
   `event_timer` ISRs. Remaining question: **ISR budget and nesting.**
   Those ISRs must re-enable interrupts (`sei()`) like the Timer2 tick so
   USART2 at 250000 baud never overruns, and bound their work.
   Proposal: at most ~200 us per ISR entry. [mcp2515_driver/2, event_timer/1]
8. **Which free timer for `event_timer`:** Timer0 Compare-A/B (4 us
   steps, shares Timer0 with `millis()`), or Timer2 Compare-B (8 us
   steps, shares the tick's timer)? [event_timer/1]
5. **First protocol and role:** J1939 node or sniffer, UDS tester or
   server, CANopen master or node, and in which order? [protocol epics]
6. **J1939 database:** source (which DBC), file format on SD, licensing.
   [J1939 epic]
7. **RAM ceiling:** proposal: stay at or below 80 % (about 6.5 KB),
   leaving ~1.5 KB for stack. Two 16-frame RX rings (~450 B) + SD
   (~600 B) fit; protocol buffers must fit what's left.
   [mcp2515_driver/2, sd_storage/1]
9. **Bitrate values and default:** proposal 125 / 250 / 500 / 1000
   kbit/s, default 250 (J1939's usual rate). Can a bus be set to
   listen-only from the screen? And when does a new bitrate take effect:
   on each RE2 step, or once the field is left? (Settings are saved to
   EEPROM when a bus's enable switch toggles -- `serial_config`'s rule.)
   [mcp2515_driver/4]

## Out of scope

RS-485 on Serial3; CAN FD (the hardware can't); the SERIAL tab's editing
mechanics and row layout (`serial_config`; this initiative only adds its
fields to them).

## Branch chain

```
dev -> features -> can_bus -> epics -> <epic> -> tasks -> <task>
```

Planned 2026-09-27 as initiative-only (this clone's `epics`/`tasks` names
belong to the open desktop_mirror chain).
