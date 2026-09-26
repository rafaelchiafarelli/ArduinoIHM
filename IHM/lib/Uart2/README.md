# Uart2

Interrupt-driven driver for USART2 (Serial2: RX2 = D17, TX2 = D16), the
board's protocol port. Serial0 (`Serial`) stays debug-only.

- `ByteRing<N>`: header-only SPSC byte ring (power of 2, lock-free on AVR),
  natively tested in `test_native/test_byte_ring.cpp`.
- `USART2_RX_vect`: reads the status and the byte, pushes the byte into the
  RX ring, and counts line errors/drops. `USART2_UDRE_vect`: pops one byte
  into `UDR2`, and disables itself when the ring is empty. The ISRs do
  nothing else.
- `uart2::read()`: the consumer drains the RX ring (in firmware that's
  `MavlinkComms::fast_handler()`, bounded, from the Timer2 tick).
- `uart2::writeFrame()`: all-or-nothing enqueue (never a partial frame,
  never blocks); a frame that doesn't fit is dropped and counted.
- Ring sizes: `UART2_RX_RING_SIZE` / `UART2_TX_RING_SIZE` (default 64 / 128,
  `-D` overridable).

**Don't reference `Serial2` in firmware code.** That links the Arduino
core's `HardwareSerial2.cpp`, which defines the same two vectors, and the
build fails with duplicate ISRs. The `serial2_loopback_test` bench env uses
`Serial2` and doesn't link this library, so it's fine.
