#include "Uart2.h"
#include "ByteRing.h"
#include <avr/io.h>
#include <avr/interrupt.h>

namespace
{
    ByteRing<UART2_RX_RING_SIZE> rxRing; // producer: RX ISR, consumer: read()
    ByteRing<UART2_TX_RING_SIZE> txRing; // producer: writeFrame(), consumer: UDRE ISR
    volatile uint8_t rxDropped = 0;
    volatile uint8_t rxLineErrors = 0;
    volatile uint8_t txDropped = 0; // superloop-only, volatile just for the getter
}

ISR(USART2_RX_vect)
{
    // UCSR2A must be read before UDR2 -- reading UDR2 pops the FIFO and
    // with it the error flags for this byte.
    uint8_t status = UCSR2A;
    uint8_t c = UDR2;
    if ((status & (_BV(DOR2) | _BV(FE2))) && rxLineErrors != 0xFF)
        rxLineErrors++;
    if (!rxRing.push(c) && rxDropped != 0xFF)
        rxDropped++;
}

ISR(USART2_UDRE_vect)
{
    uint8_t c;
    if (txRing.pop(&c))
        UDR2 = c;
    else
        UCSR2B &= (uint8_t)~_BV(UDRIE2); // drained -- re-armed by writeFrame()
}

namespace uart2
{
    void begin(uint32_t baud)
    {
        // U2X: 250000 -> UBRR 7, exact at 16 MHz.
        UCSR2A = _BV(U2X2);
        uint16_t ubrr = (uint16_t)((F_CPU / 8UL / baud) - 1);
        UBRR2H = (uint8_t)(ubrr >> 8);
        UBRR2L = (uint8_t)ubrr;
        UCSR2C = _BV(UCSZ21) | _BV(UCSZ20); // 8N1
        UCSR2B = _BV(RXEN2) | _BV(TXEN2) | _BV(RXCIE2);
    }

    bool read(uint8_t *c)
    {
        return rxRing.pop(c);
    }

    bool writeFrame(const uint8_t *buf, uint16_t len)
    {
        if (!txRing.write(buf, len))
        {
            if (txDropped != 0xFF)
                txDropped++;
            return false;
        }
        // Not atomic (UCSR2B is outside sbi range), but safe: the UDRE ISR
        // only ever clears UDRIE2, and only when the ring is empty. The
        // frame was published above, so an ISR landing inside this
        // read-modify-write sends a byte and leaves UDRIE2 alone. At worst
        // this sets it on an already-drained ring, which costs one spurious
        // UDRE interrupt that clears it again.
        UCSR2B |= _BV(UDRIE2);
        return true;
    }

    uint8_t rxDroppedCount() { return rxDropped; }
    uint8_t rxLineErrorCount() { return rxLineErrors; }
    uint8_t txDroppedCount() { return txDropped; }
}
