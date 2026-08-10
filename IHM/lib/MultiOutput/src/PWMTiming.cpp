#include "PWMTiming.h"

PWMTimingBits pwmTimingFor(PWMFrequency f) {
    // Values verified against the ATmega2560 datasheet (ch. 17): Fast PWM
    // modes 5/6/7 (8/9/10-bit, TOP fixed) and mode 14 (TOP = ICRn), and the
    // standard CSn2:0 prescaler encoding (1/8/64/256/1024).
    switch (f) {
        case frequency_variable:
            return {0b10, 0b11, 0b001, true};   // mode 14, no prescaling

        case frequency_62_500HZ:
            return {0b01, 0b01, 0b001, false};  // mode 5 (8-bit), no prescaling
        case frequency_31_250HZ:
            return {0b10, 0b01, 0b001, false};  // mode 6 (9-bit), no prescaling
        case frequency_16_625HZ:
            return {0b11, 0b01, 0b001, false};  // mode 7 (10-bit), no prescaling

        case frequency_7812_5HZ:
            return {0b01, 0b01, 0b010, false};  // mode 5, /8
        case frequency_3906_25HZ:
            return {0b10, 0b01, 0b010, false};  // mode 6, /8
        case frequency_1953_125HZ:
            return {0b11, 0b01, 0b010, false};  // mode 7, /8

        case frequency_976_5625HZ:
            return {0b01, 0b01, 0b011, false};  // mode 5, /64
        case frequency_488_28125HZ:
            return {0b10, 0b01, 0b011, false};  // mode 6, /64
        case frequency_244_140625HZ:
            return {0b11, 0b01, 0b011, false};  // mode 7, /64

        case frequency_122_0703125HZ:
            return {0b10, 0b01, 0b100, false};  // mode 6, /256
        case frequency_61_03515625HZ:
            return {0b11, 0b01, 0b100, false};  // mode 7, /256

        case frequency_30_517578125HZ:
            return {0b10, 0b01, 0b101, false};  // mode 6, /1024
        case frequency_15_2587890625HZ:
            return {0b11, 0b01, 0b101, false};  // mode 7, /1024

        default:
            return {0b00, 0b00, 0b000, false};  // timer stopped: unreachable for a valid PWMFrequency
    }
}

uint8_t pwmCompareOutputBits(bool enabled, bool inverting) {
    if (!enabled) {
        return 0b00;
    }
    return inverting ? 0b11 : 0b10;
}
