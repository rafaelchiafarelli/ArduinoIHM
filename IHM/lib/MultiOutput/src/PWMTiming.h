#pragma once
#include <stdint.h>
/**
 * Pure, hardware-independent PWM timing math for the ATmega2560's 16-bit
 * timers (Timer1/3/4/5). These timers are register-compatible with each
 * other -- TCCRnA/TCCRnB have the same bit layout on all four -- so a single
 * table and a single compare-output-mode rule serve every PWM channel in
 * this firmware. No AVR headers are included here; this compiles and is
 * fully unit-testable on the host.
 */

typedef enum{
    frequency_62_500HZ,
    frequency_31_250HZ,
    frequency_16_625HZ,
    frequency_7812_5HZ,
    frequency_3906_25HZ,
    frequency_1953_125HZ,
    frequency_976_5625HZ,
    frequency_488_28125HZ,
    frequency_244_140625HZ,
    frequency_122_0703125HZ,
    frequency_61_03515625HZ,
    frequency_30_517578125HZ,
    frequency_15_2587890625HZ,
    frequency_variable

}PWMFrequency;

/**
 * WGMn3:0 (Waveform Generation Mode) and CSn2:0 (clock select / prescaler)
 * bits for one PWMFrequency choice, split the way they land in the two
 * hardware registers:
 *   wgmA: WGMn1:0 -- occupies TCCRnA bits 1:0 (2-bit value, 0..3)
 *   wgmB: WGMn3:2 -- occupies TCCRnB bits 4:3 (2-bit value, 0..3)
 *   cs:   CSn2:0  -- occupies TCCRnB bits 2:0 (3-bit value, 0..7)
 * usesInputCaptureAsTop is true only for frequency_variable (Fast PWM mode
 * 14, TOP = ICRn): the caller must load ICRn with the requested frequency
 * before the duty-cycle compare registers are meaningful.
 */
struct PWMTimingBits {
    uint8_t wgmA;
    uint8_t wgmB;
    uint8_t cs;
    bool usesInputCaptureAsTop;
};

PWMTimingBits pwmTimingFor(PWMFrequency f);

/**
 * Returns the 2-bit COMnX1:0 field (in bits 1:0 of the result -- shift into
 * place by the caller) for a single PWM output, per the ATmega2560
 * datasheet's Fast PWM compare output mode table (modes 5/6/7/14, the only
 * ones used by this firmware):
 *   disabled               -> 0b00 (output compare disconnected from the pin)
 *   enabled, non-inverting -> 0b10 (clear on compare match, set at BOTTOM)
 *   enabled, inverting     -> 0b11 (set on compare match, clear at BOTTOM)
 * 0b01 is never returned: for these modes it is reserved (only meaningful
 * in the OCnA-toggle mode, WGM13:0 == 15, which nothing here uses).
 * Producing 0b01 by only setting COMnX0 and forgetting COMnX1 was exactly
 * the bug this function replaces.
 */
uint8_t pwmCompareOutputBits(bool enabled, bool inverting);
