#include "PWM.h"

void applySimplexPWMConfig(SimplexPWMRegisters regs, PWMFrequency f_selector,
                            bool isInverting, bool enabled,
                            uint16_t frequency, uint16_t dutyCycle) {
    PWMTimingBits t = pwmTimingFor(f_selector);
    uint8_t comA = pwmCompareOutputBits(enabled, isInverting);

    regs.tccrA = (uint8_t)((comA << 6) | t.wgmA);
    regs.tccrB = (uint8_t)((t.wgmB << 3) | t.cs);
    if (t.usesInputCaptureAsTop) {
        regs.icr = frequency;
    }
    regs.ocrA = dutyCycle;
}

void applyComplexPWMConfig(ComplexPWMRegisters regs, PWMFrequency f_selector,
                            uint16_t frequency,
                            bool isInvertingA, bool enabledA,
                            bool isInvertingB, bool enabledB,
                            bool isInvertingC, bool enabledC,
                            uint16_t dutyCycleA, uint16_t dutyCycleB, uint16_t dutyCycleC) {
    PWMTimingBits t = pwmTimingFor(f_selector);
    uint8_t comA = pwmCompareOutputBits(enabledA, isInvertingA);
    uint8_t comB = pwmCompareOutputBits(enabledB, isInvertingB);
    uint8_t comC = pwmCompareOutputBits(enabledC, isInvertingC);

    // Single combined write -- each output's bits are computed independently
    // above and OR'd together here, so enabling/configuring one output can
    // never clobber another's already-computed bits (unlike the previous
    // per-output `TCCR1A = ...` sequence this replaces).
    regs.tccrA = (uint8_t)((comA << 6) | (comB << 4) | (comC << 2) | t.wgmA);
    regs.tccrB = (uint8_t)((t.wgmB << 3) | t.cs);
    if (t.usesInputCaptureAsTop) {
        regs.icr = frequency;
    }
    regs.ocrA = dutyCycleA;
    regs.ocrB = dutyCycleB;
    regs.ocrC = dutyCycleC;
}
