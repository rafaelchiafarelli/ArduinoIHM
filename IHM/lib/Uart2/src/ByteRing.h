#pragma once
#include <stdint.h>

// Single-producer / single-consumer byte ring, lock-free on AVR: head is
// written only by the producer, tail only by the consumer, and both are
// single-byte (atomic) loads/stores. One slot is kept empty to tell full
// from empty, so capacity is N - 1.
//
// buf is volatile too, not just the indices: otherwise GCC may sink the
// data store past the (volatile) head publish, and the consumer would read
// a slot before it's written. Hardware-independent -- natively tested in
// test_native/test_byte_ring.cpp.
template <uint8_t N>
class ByteRing
{
    static_assert(N >= 2 && N <= 128 && (N & (N - 1)) == 0, "ByteRing size must be a power of 2 in 2..128");
    static const uint8_t MASK = N - 1;

    volatile uint8_t buf[N];
    volatile uint8_t head; // next slot to write (producer-owned)
    volatile uint8_t tail; // next slot to read (consumer-owned)

public:
    ByteRing() : head(0), tail(0) {}

    // Producer side. False (byte not stored) when full.
    bool push(uint8_t c)
    {
        uint8_t h = head;
        uint8_t next = (uint8_t)((h + 1) & MASK);
        if (next == tail)
            return false;
        buf[h] = c;
        head = next;
        return true;
    }

    // Consumer side. False when empty.
    bool pop(uint8_t *c)
    {
        uint8_t t = tail;
        if (t == head)
            return false;
        *c = buf[t];
        tail = (uint8_t)((t + 1) & MASK);
        return true;
    }

    uint8_t used() const { return (uint8_t)((head - tail) & MASK); }
    uint8_t space() const { return (uint8_t)(MASK - used()); }

    // Producer side, all-or-nothing: either every byte is enqueued or none
    // is. head is published once at the end, so the consumer never sees a
    // partial block.
    bool write(const uint8_t *data, uint16_t len)
    {
        if (len > space())
            return false;
        uint8_t h = head;
        for (uint16_t i = 0; i < len; i++)
        {
            buf[h] = data[i];
            h = (uint8_t)((h + 1) & MASK);
        }
        head = h;
        return true;
    }
};
