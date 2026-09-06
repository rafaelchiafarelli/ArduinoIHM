#pragma once
#include <stdint.h>
#include "PWMTiming.h"
/**
 * UI-editable PWM configuration: pure data + pure mutation/conversion logic,
 * with no Display and no AVR dependency, so it's fully unit-testable on the
 * host. PWMSimplex/PWMComplex (lib/Elements) render this; the compute*
 * functions below produce the exact argument tuples PWM::setupPWMChannelN
 * (lib/MultiOutput) expects, so the on-screen state and the register writes
 * can never drift apart from a conversion bug living in two places.
 */

// -- single-output ("simplex") channels: PWM_0 / PWM_1 ----------------------

// A simplex channel's on-screen "Mode" field folds output-enable together
// with fixed-vs-variable frequency selection into one three-way cycle
// (Off -> Fixed -> Variable -> Off -> ...): there is no separate ACTIVATE
// field for a single-output channel the way PWMComplexOutputConfig has one
// per output below, so this is the only place "enabled" is reachable from
// the UI. Always derived from (enabled, frequency); never stored directly.
// PWMComplexChannelConfig reuses this same enum and the same cycle for its
// own channel-level Mode field (see below).
enum class PWMChannelMode : uint8_t { Off, Fixed, Variable };

struct PWMChannelConfig {
    bool enabled = false;
    bool inverting = false;                // false = rising edge, true = falling edge
    PWMFrequency frequency = frequency_62_500HZ;
    uint16_t variableTopValue = 1000;      // only meaningful when frequency == frequency_variable
    uint8_t dutyCyclePercent = 50;         // 0..100

    PWMChannelMode mode() const;
    void cycleModeNext();
    void cycleModePrevious();

    void toggleEdge();
    void selectNextFrequency();
    void selectPreviousFrequency();
    void increaseDutyCycle();
    void decreaseDutyCycle();
    void increaseVariableTop();
    void decreaseVariableTop();
};

/** The exact argument tuple PWM::setupPWMChannel0/1 expects. */
struct SimplexPWMCallArgs {
    PWMFrequency frequency;
    bool inverting;
    bool enabled;
    uint16_t rawFrequency;  // ICRn TOP, meaningful only when frequency == frequency_variable
    uint16_t rawDutyCycle;  // OCRnA compare value, scaled to the channel's actual resolution
};

SimplexPWMCallArgs computeSimplexCallArgs(const PWMChannelConfig& cfg);

// -- three-output ("complex") channels: PWM_2 / PWM_3 ------------------------
// All three outputs share one timer (one frequency/TOP); everything else
// (enabled/inverting/duty) is independent per output.

struct PWMComplexOutputConfig {
    bool enabled = false;
    bool inverting = false;
    uint8_t dutyCyclePercent = 50;

    void toggleEnabled();
    void toggleEdge();
    void increaseDutyCycle();
    void decreaseDutyCycle();
};

struct PWMComplexChannelConfig {
    bool enabled = false;                  // channel-level Off gate -- see mode() below
    PWMFrequency frequency = frequency_62_500HZ;
    uint16_t variableTopValue = 1000;
    PWMComplexOutputConfig outputA;
    PWMComplexOutputConfig outputB;
    PWMComplexOutputConfig outputC;

    // Same three-way Mode cycle as PWMChannelConfig above
    // (Off -> Fixed -> Variable -> Off -> ...), so the simplex and complex
    // channels behave identically on this field. The per-output ACTIVATE
    // toggles (PWMComplexOutputConfig::toggleEnabled) are unchanged and stay
    // independent; `enabled` sits on top of them as a channel-level gate --
    // when it is false, computeComplexCallArgs forces every output's enable
    // low regardless of its own toggle, so no OCnX pin is driven. As in the
    // simplex Off case, the shared timer itself keeps running. Always
    // derived from (enabled, frequency); never stored directly.
    PWMChannelMode mode() const;
    void cycleModeNext();
    void cycleModePrevious();

    void selectNextFrequency();
    void selectPreviousFrequency();
    void increaseVariableTop();
    void decreaseVariableTop();
};

/** The exact argument tuple PWM::setupPWMChannel2/3 expects. */
struct ComplexPWMCallArgs {
    PWMFrequency frequency;
    uint16_t rawFrequency;
    bool invertingA; bool enabledA; uint16_t rawDutyCycleA;
    bool invertingB; bool enabledB; uint16_t rawDutyCycleB;
    bool invertingC; bool enabledC; uint16_t rawDutyCycleC;
};

ComplexPWMCallArgs computeComplexCallArgs(const PWMComplexChannelConfig& cfg);
