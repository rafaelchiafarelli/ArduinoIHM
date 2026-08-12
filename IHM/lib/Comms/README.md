# Comms

`SerialCommunication` (`SerialCommunication.h/.cpp`): a hand-rolled UART
framing protocol, bypassing Arduino's `HardwareSerial` for both RX
parsing and TX.

## Protocol

`#HEADER ... *checksum$TERMINATOR`, semicolon-separated numeric fields in
the payload. `receive(uint8_t rcv)` is a byte-at-a-time state machine
(`Searching` -> `Found` -> `Ending`) that accumulates into
`ReceivedBytes[]` and sets `able_to_parse` once a complete, correctly
terminated frame has arrived. `fast_handler(uint16_t dest[], size_t
max_size)` checks that flag and, if set, XOR-checksum-validates
(`Parse()`) and decodes the fields (`decode()`, via `strdup`+`strsep` --
heap allocation on every frame, worth knowing on an 8KB-RAM AVR) into
`dest[]`.

TX (`send()`/`send_one()`) drives `UDR0`/`UCSR0B` directly for
interrupt-driven transmission (`TXEN0`/`TXCIE0`).

## A wiring gap, not by design

**`receive()` is never called anywhere in this codebase.** It's the only
entry point that feeds bytes into the framing state machine, but nothing
-- not `main.cpp`, not any ISR -- calls `comms.receive(byte)`. The actual
USART0 RX interrupt (`ISR(USART0_RX_vect)` in
`lib/ArduinoLib/src/HardwareSerial0.cpp`) only fills the standard Arduino
`Serial` ring buffer; it doesn't forward to `SerialCommunication`.

Consequence: `able_to_parse` can never become true, so
`fast_handler()` always returns `false`, `newDataAvailable` in
`main.cpp` is always false, and `voltage0`/`voltage1` (fed to the two
`MCP4725` DACs) never actually get updated from real serial input today.
The framing/checksum logic itself is written and was fixed for a
buffer-overrun bug on 2026-08-11 (`IHM/CHANGELOG.md`), but the receive
path was never connected to a byte source. To make this work, something
needs to either override `ISR(USART0_RX_vect)` to call
`comms.receive()` directly, or poll `Serial.available()`/`Serial.read()`
somewhere (main loop or the Timer2 tick) and feed each byte to
`receive()`.

## Invocation

`comms.fast_handler(receivedRawData, 10)` is called from inside
`ISR(TIMER2_COMPA_vect)`'s ~25ms branch -- despite the `fast_handler`
name (shared convention across modules for "not the ultra-slow one"),
this runs on the 25ms cadence, not every ~1ms tick.

## Hardware resources

USART0 registers directly (`UCSR0A/B`, `UDR0`).
