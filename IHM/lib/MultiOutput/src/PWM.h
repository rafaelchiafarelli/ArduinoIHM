#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>
#include "PWMTiming.h"
#include "RegisterIO.h"

/**
 * Register handles for a single-output ("simplex") PWM channel: Timer1/3/4/5
 * driving only their A compare unit (OCnA). Used by PWM channels 0 and 1.
 */
struct SimplexPWMRegisters {
    Reg8 tccrA;
    Reg8 tccrB;
    Reg16 icr;
    Reg16 ocrA;
};

/**
 * Register handles for a three-output ("complex") PWM channel: Timer1/3/4/5
 * driving all three compare units (OCnA/B/C). Used by PWM channels 2 and 3.
 */
struct ComplexPWMRegisters {
    Reg8 tccrA;
    Reg8 tccrB;
    Reg16 icr;
    Reg16 ocrA;
    Reg16 ocrB;
    Reg16 ocrC;
};

/**
 * Compute and write the TCCRnA/TCCRnB/(ICRn)/OCRnA register values for a
 * single-output PWM channel. Pure composition of pwmTimingFor() and
 * pwmCompareOutputBits() (see PWMTiming.h) -- no AVR headers are needed
 * here, so this is directly unit-testable on the host via fake Reg8/Reg16
 * backing bytes.
 */
void applySimplexPWMConfig(SimplexPWMRegisters regs, PWMFrequency f_selector,
                            bool isInverting, bool enabled,
                            uint16_t frequency, uint16_t dutyCycle);

/**
 * Same as applySimplexPWMConfig, but for a three-output channel: each
 * output's compare bits are computed independently and combined into a
 * single TCCRnA write (never three separate read-modify-writes), so one
 * output's configuration can never clobber another's.
 */
void applyComplexPWMConfig(ComplexPWMRegisters regs, PWMFrequency f_selector,
                            uint16_t frequency,
                            bool isInvertingA, bool enabledA,
                            bool isInvertingB, bool enabledB,
                            bool isInvertingC, bool enabledC,
                            uint16_t dutyCycleA, uint16_t dutyCycleB, uint16_t dutyCycleC);

/**
 * Binds the four PWM channels to their real AVR timers:
 *   channel 0 -> Timer3 (simplex, OC3A)
 *   channel 1 -> Timer5 (simplex, OC5A)
 *   channel 2 -> Timer1 (complex, OC1A/B/C)
 *   channel 3 -> Timer4 (complex, OC4A/B/C)
 * All the actual bit-level logic lives in applySimplexPWMConfig /
 * applyComplexPWMConfig above; these methods only say which registers.
 */
class PWM
{
    public:
        PWM(){};
        void setupPWMChannel0(PWMFrequency f_selector,bool isInverting, bool enabled, uint16_t frequency, uint16_t dutyCycle);
        void setupPWMChannel1(PWMFrequency f_selector,bool isInverting, bool enabled, uint16_t frequency, uint16_t dutyCycle);
        void setupPWMChannel2(PWMFrequency f_selector,uint16_t frequency,bool isInvertingA,bool enabledA, bool isInvertingB,bool enabledB, bool isInvertingC,bool enabledC,  uint16_t dutyCycleA, uint16_t dutyCycleB, uint16_t dutyCycleC);
        void setupPWMChannel3(PWMFrequency f_selector,uint16_t frequency,bool isInvertingA,bool enabledA, bool isInvertingB,bool enabledB, bool isInvertingC,bool enabledC,  uint16_t dutyCycleA, uint16_t dutyCycleB, uint16_t dutyCycleC);

};

#endif /* PWM_H_ */
