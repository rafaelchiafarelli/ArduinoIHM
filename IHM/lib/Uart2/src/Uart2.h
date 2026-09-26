#pragma once
#include <stdint.h>

// Interrupt-driven USART2 (Serial2: RX2 = D17, TX2 = D16) -- the board's
// protocol port. Replaces the Arduino core's Serial2 for firmware code:
// this file defines USART2_RX_vect / USART2_UDRE_vect itself, so nothing
// in the firmware may reference Serial2 (that would link the core's
// HardwareSerial2.cpp, which defines the same vectors -> duplicate ISR).
//
// The ISRs only move bytes between UDR2 and the rings. Parsing/decoding is
// the consumer's job (MavlinkComms::fast_handler, from the Timer2 tick).
// See lib/Uart2/README.md.

#ifndef UART2_RX_RING_SIZE
#define UART2_RX_RING_SIZE 64
#endif
#ifndef UART2_TX_RING_SIZE
#define UART2_TX_RING_SIZE 128
#endif

namespace uart2
{
    // U2X, 8N1, RX + TX + RX-complete interrupt enabled.
    void begin(uint32_t baud);

    // Consumer side of the RX ring. False when nothing is buffered.
    bool read(uint8_t *c);

    // All-or-nothing enqueue of one complete frame, then kicks the TX ISR.
    // False (frame dropped and counted) when the TX ring lacks room -- never
    // blocks, and never sends a partial frame. Single producer: call from
    // one context only (the superloop).
    bool writeFrame(const uint8_t *buf, uint16_t len);

    // Saturating diagnostics counters (stick at 255).
    uint8_t rxDroppedCount();   // byte arrived with the RX ring full
    uint8_t rxLineErrorCount(); // hardware overrun (DOR2) or framing error (FE2)
    uint8_t txDroppedCount();   // writeFrame() found no room
}
