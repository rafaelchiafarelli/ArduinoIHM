#pragma once
#include <util/atomic.h>
#include <BinaryOutputs.h>

// Strobe/control-line indices into BinaryOutputs' 20-slot table -- see
// BinaryOutputs/README.md ("indices 10, 11, 14, 15") and MultiOutput/README.md
// ("The multiplexed output bus") for the full protocol these implement.
#define MUX_SERVO_STROBE 10 // dig_0 (PH6) -- Servo's 74LS373 latch.
                             // Physically wired, but no firmware in this
                             // repo drives it -- servo control is not
                             // part of the IHM solution (removed
                             // 2026-08-16, see CHANGELOG.md). A future
                             // servo controller is separate, planned work.
#define MUX_OUTPUT_EN 11    // shared OE (PB4) -- active-LOW at the chip
#define MUX_RELAY_STROBE 14 // dig_1 (PG5) -- Relay's 74LS373 latch
#define MUX_MOTOR_STROBE 15 // dig_2 (PF4) -- Motor's 74LS373 latch

/**
 * Driver for the 8-bit data bus shared by Relay/ServoMotor/MotorDC, each
 * sitting behind its own 74LS373 latch. A 74LS373 is transparent, not
 * edge-triggered: its outputs follow the bus continuously while its strobe
 * (LE) is high, and hold whatever was present the instant that line falls.
 * So write() settles the whole byte on the bus first, then raises the
 * target device's strobe, then drops it again -- the falling edge is what
 * actually captures the byte into that one device's latch, leaving the
 * other two untouched.
 *
 * The bus is physically shared, so a write from ServoMotor's ISR context
 * could otherwise interleave with a write from Relay/MotorDC's foreground
 * context and tear either one mid-sequence -- ATOMIC_BLOCK brackets the
 * whole settle-strobe-drop sequence against interrupts to prevent that.
 */
class MultiplexedBus
{
    // A reference, not a copy -- BinaryOutputs' 20-slot table is 160 bytes,
    // and every device (Relay/ServoMotor/MotorDC) already keeps its own
    // by-value copy of it; wrapping a fourth copy here would cost another
    // 160 bytes of RAM for data that's already owned by MultiOutput's
    // long-lived `bnOuts` member.
    const BinaryOutputs &bus;

public:
    MultiplexedBus(const BinaryOutputs &bnOuts) : bus(bnOuts) {}

    // OE is active-low at the 74LS373 (confirmed against the KiCad symbol:
    // its OE pin is the schematic's only "inverted" pin on this part) --
    // driving it LOW enables all three latches' outputs. This is a static
    // configuration, held low permanently, not part of the per-write
    // strobe sequence above.
    void enableOutputs() const
    {
        bus.SetOutput(MUX_OUTPUT_EN, false);
    }

    void write(uint8_t strobeIndex, uint8_t value) const
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            for (uint8_t bit = 0; bit < 8; bit++)
            {
                bus.SetOutput(bit, (value >> bit) & 0x01);
            }
            bus.SetOutput(strobeIndex, true);
            bus.SetOutput(strobeIndex, false);
        }
    }
};
