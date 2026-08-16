#pragma once
#include <avr/io.h>
#include <stdint.h>

#define ANALOG_INPUT_COUNT 5
// Index 4 is BattVoltage (house-keeping), not one of the 4 an0-3_in channels
// -- same ADC peripheral, so kept in one table rather than duplicating setup.
#define ANALOG_INPUT_BATT_VOLTAGE_INDEX 4

class AnalogInputs
{
    private:
        // ADMUX MUX4:0 channel per index: an0_in, an1_in, an2_in, an3_in,
        // BattVoltage (D54/PF0, D55/PF1, D56/PF2, D57/PF3, D61/PF7). All in
        // 0-7, so MUX5 (ADCSRB) is never needed here.
        const uint8_t channels[ANALOG_INPUT_COUNT];

        // Written only from ISR(ADC_vect) (via isr_handler()), read from
        // the foreground -- volatile so neither side caches a stale copy
        // or reorders across the read/write boundary, same pattern as
        // main.cpp's bMap/newDataAvailable. A torn 16-bit read is possible
        // on this 8-bit target (same accepted risk as bMap elsewhere in
        // this codebase) -- worst case is one stale/mixed sample on
        // non-critical telemetry, not guarded with ATOMIC_BLOCK to avoid
        // reintroducing the interrupt-disabling cost this fix removes.
        volatile uint16_t results[ANALOG_INPUT_COUNT] = {0,0,0,0,0};

        // ISR-local only: never read outside isr_handler(). Index of the
        // channel currently mid-conversion (already selected in ADMUX,
        // ADSC already set) -- when isr_handler() fires, that channel's
        // result is ready.
        uint8_t currentChannel = 0;

    public:
        AnalogInputs() : channels{0, 1, 2, 3, 7}
        {
            setup();
        }

        void setup()
        {
            currentChannel = 0;
            // AVCC reference (REFS0=1, REFS1=0), right-adjusted result,
            // channel 0 selected.
            ADMUX = (1 << REFS0) | (channels[0] & 0x1F);
            // Enable ADC + conversion-complete interrupt, prescaler 128 ->
            // 16MHz/128 = 125kHz ADC clock, inside the datasheet's
            // recommended 50-200kHz range for full 10-bit accuracy.
            ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
            // Kick off the free-running round-robin scan -- isr_handler()
            // re-triggers the next conversion each time one completes, so
            // after this one call the scan keeps itself going forever.
            ADCSRA |= (1 << ADSC);
        }

        // Non-blocking: returns the most recently completed conversion for
        // this channel. Freshness is bounded by one full round-robin scan
        // (5 channels x ~104-200us each depending on mux-switch settling,
        // so worst case ~1ms) rather than by the caller's own read cadence
        // -- an improvement over the old ~100ms poll interval, not just a
        // side effect of removing the block.
        uint16_t read(uint8_t index) const
        {
            if (index >= ANALOG_INPUT_COUNT)
                return 0;
            return results[index];
        }

        // Called from ISR(ADC_vect) in main.cpp -- the project's single
        // ISR-owning composition root (see ARCHITECTURE.md/src/README.md).
        // Reads the just-completed conversion for currentChannel, advances
        // to the next channel round-robin, and restarts the ADC -- keeps
        // the scan running continuously without ever blocking the
        // foreground superloop.
        void isr_handler()
        {
            uint16_t low = ADCL;  // must read ADCL before ADCH (latches the pair)
            uint16_t high = ADCH;
            results[currentChannel] = (uint16_t)((high << 8) | low);

            currentChannel++;
            if (currentChannel >= ANALOG_INPUT_COUNT)
            {
                currentChannel = 0;
            }
            ADMUX = (ADMUX & 0xE0) | (channels[currentChannel] & 0x1F);
            ADCSRA |= (1 << ADSC);
        }
};
