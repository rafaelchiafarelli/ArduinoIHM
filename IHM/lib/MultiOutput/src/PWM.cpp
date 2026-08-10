#include <avr/io.h>
#include "PWM.h"
/**
 * Binds each PWM channel to its real timer registers and hands off to
 * applySimplexPWMConfig / applyComplexPWMConfig (PWMConfig.cpp) for all the
 * actual bit-level decisions. See PWM.h for the channel-to-timer mapping.
 */

void PWM::setupPWMChannel0(PWMFrequency f_selector,bool isInverting, bool enabled, uint16_t frequency, uint16_t dutyCycle){
    applySimplexPWMConfig({{&TCCR3A}, {&TCCR3B}, {&ICR3}, {&OCR3A}},
                           f_selector, isInverting, enabled, frequency, dutyCycle);
}

void PWM::setupPWMChannel1(PWMFrequency f_selector,bool isInverting, bool enabled, uint16_t frequency, uint16_t dutyCycle){
    applySimplexPWMConfig({{&TCCR5A}, {&TCCR5B}, {&ICR5}, {&OCR5A}},
                           f_selector, isInverting, enabled, frequency, dutyCycle);
}

void PWM::setupPWMChannel2(PWMFrequency f_selector,uint16_t frequency,bool isInvertingA,bool enabledA, bool isInvertingB,bool enabledB, bool isInvertingC,bool enabledC,  uint16_t dutyCycleA, uint16_t dutyCycleB, uint16_t dutyCycleC){
    applyComplexPWMConfig({{&TCCR1A}, {&TCCR1B}, {&ICR1}, {&OCR1A}, {&OCR1B}, {&OCR1C}},
                           f_selector, frequency,
                           isInvertingA, enabledA, isInvertingB, enabledB, isInvertingC, enabledC,
                           dutyCycleA, dutyCycleB, dutyCycleC);
}

void PWM::setupPWMChannel3(PWMFrequency f_selector,uint16_t frequency,bool isInvertingA,bool enabledA, bool isInvertingB,bool enabledB, bool isInvertingC,bool enabledC,  uint16_t dutyCycleA, uint16_t dutyCycleB, uint16_t dutyCycleC){
    applyComplexPWMConfig({{&TCCR4A}, {&TCCR4B}, {&ICR4}, {&OCR4A}, {&OCR4B}, {&OCR4C}},
                           f_selector, frequency,
                           isInvertingA, enabledA, isInvertingB, enabledB, isInvertingC, enabledC,
                           dutyCycleA, dutyCycleB, dutyCycleC);
}
