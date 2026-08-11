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
    PWMFrequency frequency = frequency_62_500HZ;
    uint16_t variableTopValue = 1000;
    PWMComplexOutputConfig outputA;
    PWMComplexOutputConfig outputB;
    PWMComplexOutputConfig outputC;

    // A complex channel has no channel-level enable (each output has its own,
    // via PWMComplexOutputConfig::toggleEnabled), so its Mode field is only
    // ever a two-way Fixed/Variable toggle -- unlike PWMChannelConfig's
    // three-way cycle above.
    void toggleMode();

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
