#include "PWMChannelConfig.h"

namespace {
constexpr uint8_t DUTY_CYCLE_STEP_PERCENT = 1;
constexpr uint16_t VARIABLE_TOP_STEP = 100;
constexpr uint16_t VARIABLE_TOP_MIN = 1;  // 0 would be a degenerate, zero-length PWM period
constexpr uint16_t VARIABLE_TOP_MAX = 0xFFFF;

uint16_t scaleDutyCycleToRaw(uint8_t dutyCyclePercent, uint16_t top) {
    return (uint16_t)(((uint32_t)dutyCyclePercent * top) / 100);
}
}  // namespace

// -- PWMChannelConfig (simplex) ---------------------------------------------

PWMChannelMode PWMChannelConfig::mode() const {
    if (!enabled) {
        return PWMChannelMode::Off;
    }
    return (frequency == frequency_variable) ? PWMChannelMode::Variable : PWMChannelMode::Fixed;
}

void PWMChannelConfig::cycleModeNext() {
    switch (mode()) {
        case PWMChannelMode::Off:
            enabled = true;
            if (frequency == frequency_variable) {
                frequency = frequency_62_500HZ;
            }
            break;
        case PWMChannelMode::Fixed:
            frequency = frequency_variable;
            break;
        case PWMChannelMode::Variable:
            enabled = false;
            break;
    }
}

void PWMChannelConfig::cycleModePrevious() {
    switch (mode()) {
        case PWMChannelMode::Off:
            enabled = true;
            frequency = frequency_variable;
            break;
        case PWMChannelMode::Variable:
            frequency = frequency_62_500HZ;
            break;
        case PWMChannelMode::Fixed:
            enabled = false;
            break;
    }
}

void PWMChannelConfig::toggleEdge() { inverting = !inverting; }

void PWMChannelConfig::selectNextFrequency() {
    frequency = (PWMFrequency)((frequency + 1) % NUMBER_OF_PWM_FREQUENCIES);
}

void PWMChannelConfig::selectPreviousFrequency() {
    frequency = (PWMFrequency)((frequency + NUMBER_OF_PWM_FREQUENCIES - 1) % NUMBER_OF_PWM_FREQUENCIES);
}

void PWMChannelConfig::increaseDutyCycle() {
    dutyCyclePercent = (uint8_t)((dutyCyclePercent + DUTY_CYCLE_STEP_PERCENT > 100)
                                      ? 100
                                      : dutyCyclePercent + DUTY_CYCLE_STEP_PERCENT);
}

void PWMChannelConfig::decreaseDutyCycle() {
    dutyCyclePercent = (uint8_t)((dutyCyclePercent < DUTY_CYCLE_STEP_PERCENT)
                                      ? 0
                                      : dutyCyclePercent - DUTY_CYCLE_STEP_PERCENT);
}

void PWMChannelConfig::increaseVariableTop() {
    uint32_t next = (uint32_t)variableTopValue + VARIABLE_TOP_STEP;
    variableTopValue = (uint16_t)(next > VARIABLE_TOP_MAX ? VARIABLE_TOP_MAX : next);
}

void PWMChannelConfig::decreaseVariableTop() {
    variableTopValue = (uint16_t)((variableTopValue < VARIABLE_TOP_MIN + VARIABLE_TOP_STEP)
                                       ? VARIABLE_TOP_MIN
                                       : variableTopValue - VARIABLE_TOP_STEP);
}

SimplexPWMCallArgs computeSimplexCallArgs(const PWMChannelConfig& cfg) {
    uint16_t top = pwmResolutionTop(cfg.frequency, cfg.variableTopValue);
    return {
        cfg.frequency,
        cfg.inverting,
        cfg.enabled,
        cfg.variableTopValue,
        scaleDutyCycleToRaw(cfg.dutyCyclePercent, top),
    };
}

// -- PWMComplexOutputConfig / PWMComplexChannelConfig ------------------------

void PWMComplexOutputConfig::toggleEnabled() { enabled = !enabled; }
void PWMComplexOutputConfig::toggleEdge() { inverting = !inverting; }

void PWMComplexOutputConfig::increaseDutyCycle() {
    dutyCyclePercent = (uint8_t)((dutyCyclePercent + DUTY_CYCLE_STEP_PERCENT > 100)
                                      ? 100
                                      : dutyCyclePercent + DUTY_CYCLE_STEP_PERCENT);
}

void PWMComplexOutputConfig::decreaseDutyCycle() {
    dutyCyclePercent = (uint8_t)((dutyCyclePercent < DUTY_CYCLE_STEP_PERCENT)
                                      ? 0
                                      : dutyCyclePercent - DUTY_CYCLE_STEP_PERCENT);
}

void PWMComplexChannelConfig::toggleMode() {
    frequency = (frequency == frequency_variable) ? frequency_62_500HZ : frequency_variable;
}

void PWMComplexChannelConfig::selectNextFrequency() {
    frequency = (PWMFrequency)((frequency + 1) % NUMBER_OF_PWM_FREQUENCIES);
}

void PWMComplexChannelConfig::selectPreviousFrequency() {
    frequency = (PWMFrequency)((frequency + NUMBER_OF_PWM_FREQUENCIES - 1) % NUMBER_OF_PWM_FREQUENCIES);
}

void PWMComplexChannelConfig::increaseVariableTop() {
    uint32_t next = (uint32_t)variableTopValue + VARIABLE_TOP_STEP;
    variableTopValue = (uint16_t)(next > VARIABLE_TOP_MAX ? VARIABLE_TOP_MAX : next);
}

void PWMComplexChannelConfig::decreaseVariableTop() {
    variableTopValue = (uint16_t)((variableTopValue < VARIABLE_TOP_MIN + VARIABLE_TOP_STEP)
                                       ? VARIABLE_TOP_MIN
                                       : variableTopValue - VARIABLE_TOP_STEP);
}

ComplexPWMCallArgs computeComplexCallArgs(const PWMComplexChannelConfig& cfg) {
    uint16_t top = pwmResolutionTop(cfg.frequency, cfg.variableTopValue);
    return {
        cfg.frequency,
        cfg.variableTopValue,
        cfg.outputA.inverting, cfg.outputA.enabled, scaleDutyCycleToRaw(cfg.outputA.dutyCyclePercent, top),
        cfg.outputB.inverting, cfg.outputB.enabled, scaleDutyCycleToRaw(cfg.outputB.dutyCyclePercent, top),
        cfg.outputC.inverting, cfg.outputC.enabled, scaleDutyCycleToRaw(cfg.outputC.dutyCyclePercent, top),
    };
}
