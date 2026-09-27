# Initiative: can_bus

Real CAN on the board's two external CAN modules, then (as later epics)
the protocols Rafael asked about on 2026-09-27: SAE J1939, ISO 14229
(UDS) and CANopen, with the SD card for storage. This initiative is the
driver foundation; `serial_config` configures it.

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

## Epics (foundation)

| Epic | Scope |
|---|---|
| `spi_sharing` | One SPI discipline for CAN-1, CAN-2 and the SD card, including access from an ISR. |
| `mcp2515_driver` | Driver for both modules: bit timing from the crystal, INT-driven RX into a RAM ring, TX, filters, error state. |
| `can_generator` | `CAN_SIGNAL_CONFIG` (301) actually transmits: period and repeat count, per bus. |
| `main_loop_timing` | Bound the superloop's worst case so protocol timers hold (Janus non-blocking render and/or ISR-side CAN work). |
| `sd_storage` | Mount, CS pin, a small file API, buffered writes, RAM budget. |

Order: `spi_sharing` -> `mcp2515_driver` -> `can_generator`;
`main_loop_timing` and `sd_storage` in parallel. Protocol epics start once
the relevant foundation epics are done.

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
4. **Timing strategy:** RX in the INT ISR (fast, but the SPI must then be
   ISR-safe everywhere), Janus `render_mode: non_blocking` (bounded
   loop, needs `main.cpp` changes), or both (proposal: both).
   [main_loop_timing/1]
5. **First protocol and role:** J1939 node or sniffer, UDS tester or
   server, CANopen master or node, and in which order? [protocol epics]
6. **J1939 database:** source (which DBC), file format on SD, licensing.
   [J1939 epic]
7. **RAM ceiling:** proposal: stay at or below 80 % (about 6.5 KB),
   leaving ~1.5 KB for stack. Two 16-frame RX rings (~450 B) + SD
   (~600 B) fit; protocol buffers must fit what's left.
   [mcp2515_driver/2, sd_storage/1]

## Out of scope

RS-485 on Serial3; CAN FD (the hardware can't); the SERIAL tab UI
(`serial_config`).

## Branch chain

```
dev -> features -> can_bus -> epics -> <epic> -> tasks -> <task>
```

Planned 2026-09-27 as initiative-only (this clone's `epics`/`tasks` names
belong to the open desktop_mirror chain).
