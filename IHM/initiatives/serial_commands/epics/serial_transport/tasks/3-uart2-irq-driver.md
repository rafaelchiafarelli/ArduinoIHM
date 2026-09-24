# Task 3: uart2-irq-driver

**Status:** not started
**Branch:** `3-uart2-irq-driver` (from `tasks`)
**Depends on:** nothing

## Contract

### Delivers

1. **`lib/Uart2/src/ByteRing.h`**: header-only `template <uint8_t N> class ByteRing`
   (N a power of 2, 2..128). SPSC and lock-free: `push(c)` / `pop(&c)`,
   `used()`, `space()`, and `write(const uint8_t*, uint16_t len)`, which is
   all-or-nothing (either every byte is enqueued or none is). No AVR
   includes.
2. **`lib/Uart2/src/Uart2.h` + `Uart2.cpp`**: namespace `uart2`:
   - `void begin(uint32_t baud)`: U2X, 8N1, RXEN/TXEN/RXCIE.
   - `bool read(uint8_t* c)`: pop one received byte (consumer side).
   - `bool writeFrame(const uint8_t* buf, uint16_t len)`: all-or-nothing
     enqueue, then enable `UDRIE2`; on no room, count and return false.
   - Saturating `uint8_t` counters: `rxDroppedCount()` (ring full),
     `rxLineErrorCount()` (DOR2|FE2), `txDroppedCount()`.
   - `ISR(USART2_RX_vect)` and `ISR(USART2_UDRE_vect)`: byte moves only.
   - Ring sizes `UART2_RX_RING_SIZE` / `UART2_TX_RING_SIZE`, default 64,
     `#ifndef`-guarded.
3. **`test_native/test_byte_ring.cpp`**: push/pop order, full/empty,
   wraparound, all-or-nothing `write`. `lib/Uart2/src` added to the runner's
   include allowlist.
4. `lib/Uart2/README.md`: short.

Nothing calls it yet (task 4 does). Firmware must not reference `Serial2`
(see epic README).

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; task file marked
done in the same commit.
