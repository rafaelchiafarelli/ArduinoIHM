#include "PWMWireConfig.h"

namespace {
uint8_t clampDuty(uint8_t percent) { return percent > 100 ? 100 : percent; }
}  // namespace

bool pwmWireConfigValid(const PwmWireConfig& w) {
    return w.channel <= 3 && w.f_selector < NUMBER_OF_PWM_FREQUENCIES;
}

PWMChannelConfig pwmWireToSimplex(const PwmWireConfig& w) {
    PWMChannelConfig c;
    c.enabled = w.out[0].enabled != 0;
    c.inverting = w.out[0].inverting != 0;
    c.frequency = (PWMFrequency)w.f_selector;
    c.variableTopValue = w.frequency;
    c.dutyCyclePercent = clampDuty(w.out[0].duty_percent);
    return c;
}

PWMComplexChannelConfig pwmWireToComplex(const PwmWireConfig& w) {
    PWMComplexChannelConfig c;
    // The channel-level Off gate stays open: the wire message carries only
    // per-output enables, and those do the gating.
    c.enabled = true;
    c.frequency = (PWMFrequency)w.f_selector;
    c.variableTopValue = w.frequency;
    PWMComplexOutputConfig* outs[3] = { &c.outputA, &c.outputB, &c.outputC };
    for (int i = 0; i < 3; i++) {
        outs[i]->enabled = w.out[i].enabled != 0;
        outs[i]->inverting = w.out[i].inverting != 0;
        outs[i]->dutyCyclePercent = clampDuty(w.out[i].duty_percent);
    }
    return c;
}
