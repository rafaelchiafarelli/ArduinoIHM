#pragma once
#include <stdint.h>
#include "PWMChannelConfig.h"

/**
 * Adapter from the PC->board PWM_CHANNEL_CONFIG wire message (mavlink/
 * ihm_dialect.xml) to the UI-side config types in PWMChannelConfig.h.
 *
 * PwmWireConfig deliberately mirrors the message's fields without
 * including mavlink.h, so this stays AVR-free and host-testable. The
 * register call itself is NOT here: feed the result to the existing
 * computeSimplexCallArgs / computeComplexCallArgs, which own the
 * percent -> raw OCR scaling (PWM::setupPWMChannelN takes raw OCR counts,
 * not percent).
 */
struct PwmWireOutput {
    uint8_t enabled;
    uint8_t inverting;
    uint8_t duty_percent;   // untrusted; clamped to 100 by the adapters
};

struct PwmWireConfig {
    uint8_t channel;        // 0-3
    uint8_t f_selector;     // wire PWM_FREQUENCY value == PWMFrequency
    uint16_t frequency;     // raw ICRn TOP; only used for frequency_variable
    PwmWireOutput out[3];   // A, B, C; [1]/[2] ignored for channels 0/1
};

/** channel <= 3 and f_selector < NUMBER_OF_PWM_FREQUENCIES. Duty is clamped, not rejected. */
bool pwmWireConfigValid(const PwmWireConfig& w);

/** Channels 0/1. Precondition: pwmWireConfigValid(w). */
PWMChannelConfig pwmWireToSimplex(const PwmWireConfig& w);

/** Channels 2/3. Precondition: pwmWireConfigValid(w). */
PWMComplexChannelConfig pwmWireToComplex(const PwmWireConfig& w);
