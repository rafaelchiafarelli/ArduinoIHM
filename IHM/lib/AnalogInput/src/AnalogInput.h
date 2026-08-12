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

    public:
        AnalogInputs() : channels{0, 1, 2, 3, 7}
        {
            setup();
        }

        void setup() const
        {
            // AVCC reference (REFS0=1, REFS1=0), right-adjusted result.
            ADMUX = (1 << REFS0);
            // Enable ADC, prescaler 128 -> 16MHz/128 = 125kHz ADC clock,
            // inside the datasheet's recommended 50-200kHz range for full
            // 10-bit accuracy.
            ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
        }

        // KNOWN GAP, not accepted design -- see IHM/ARCHITECTURE.md's Known
        // gaps list (item 9). This blocks the caller for ~104us/channel (13
        // ADC clocks at 125kHz) via a polling loop below. That violates
        // this project's own rule against blocking waits (ARCHITECTURE.md's
        // "no delay(), a timer tick drives scheduling instead") just as
        // much as a delay() call would, even though it's polling a hardware
        // flag rather than counting cycles. Called from main.cpp's
        // superloop (the ~100ms timeCounter>=100 block, not any ISR), so
        // today's cost is ~520us worst case for all 5 channels once per
        // ~100ms -- tolerable for now, but this should become
        // interrupt-driven (ADIE + ISR(ADC_vect), cycling channels
        // asynchronously) rather than fixed as "good enough because it's
        // infrequent."
        uint16_t read(uint8_t index) const
        {
            if (index >= ANALOG_INPUT_COUNT)
                return 0;
            ADMUX = (ADMUX & 0xE0) | (channels[index] & 0x1F);
            ADCSRA |= (1 << ADSC);
            while (ADCSRA & (1 << ADSC))
                ;
            uint16_t low = ADCL;  // must read ADCL before ADCH (latches the pair)
            uint16_t high = ADCH;
            return (uint16_t)((high << 8) | low);
        }
};
