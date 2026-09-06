#include "mini_test.h"
#include "PWMChannelConfig.h"

// ===== PWMChannelConfig (simplex) =========================================

TEST(PWMChannelConfig, DefaultsToDisabledFixedFrequencyFiftyPercent) {
    PWMChannelConfig cfg;
    CHECK_FALSE(cfg.enabled);
    CHECK_FALSE(cfg.inverting);
    CHECK_EQ(cfg.frequency, frequency_62_500HZ);
    CHECK_EQ(cfg.dutyCyclePercent, (uint8_t)50);
}

TEST(PWMChannelConfig, ModeDefaultsToOff) {
    PWMChannelConfig cfg;
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Off);
}

TEST(PWMChannelConfig, CycleModeNextGoesOffThenFixedThenVariableThenWraps) {
    PWMChannelConfig cfg;
    cfg.frequency = frequency_31_250HZ;  // arbitrary non-default fixed frequency

    cfg.cycleModeNext();  // Off -> Fixed
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Fixed);
    CHECK_TRUE(cfg.enabled);
    CHECK_EQ(cfg.frequency, frequency_31_250HZ);  // preserved, not reset

    cfg.cycleModeNext();  // Fixed -> Variable
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Variable);
    CHECK_EQ(cfg.frequency, frequency_variable);

    cfg.cycleModeNext();  // Variable -> Off
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Off);
    CHECK_FALSE(cfg.enabled);
}

TEST(PWMChannelConfig, CycleModeNextFromOffWithVariableFrequencyPickedResetsToAFixedFrequency) {
    // If the channel was last in Variable mode before being turned off,
    // going Off -> Fixed must not silently stay on frequency_variable.
    PWMChannelConfig cfg;
    cfg.enabled = false;
    cfg.frequency = frequency_variable;
    cfg.cycleModeNext();
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Fixed);
    CHECK_TRUE(cfg.frequency != frequency_variable);
}

TEST(PWMChannelConfig, CycleModePreviousGoesOffThenVariableThenFixedThenWraps) {
    PWMChannelConfig cfg;

    cfg.cycleModePrevious();  // Off -> Variable
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Variable);

    cfg.cycleModePrevious();  // Variable -> Fixed
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Fixed);
    CHECK_EQ(cfg.frequency, frequency_62_500HZ);

    cfg.cycleModePrevious();  // Fixed -> Off
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Off);
}

TEST(PWMChannelConfig, ToggleEdgeFlipsBothWays) {
    PWMChannelConfig cfg;
    cfg.toggleEdge();
    CHECK_TRUE(cfg.inverting);
    cfg.toggleEdge();
    CHECK_FALSE(cfg.inverting);
}

TEST(PWMChannelConfig, SelectNextFrequencyCyclesThroughAllAndWraps) {
    PWMChannelConfig cfg;
    for (int i = 0; i < (int)NUMBER_OF_PWM_FREQUENCIES; i++) {
        CHECK_EQ((int)cfg.frequency, i);
        cfg.selectNextFrequency();
    }
    CHECK_EQ(cfg.frequency, frequency_62_500HZ);  // wrapped
}

TEST(PWMChannelConfig, SelectPreviousFrequencyWrapsBelowFirst) {
    PWMChannelConfig cfg;
    cfg.selectPreviousFrequency();
    CHECK_EQ((int)cfg.frequency, (int)NUMBER_OF_PWM_FREQUENCIES - 1);
}

TEST(PWMChannelConfig, DutyCycleIncreasesByStepAndClampsAt100) {
    PWMChannelConfig cfg;
    cfg.dutyCyclePercent = 99;
    cfg.increaseDutyCycle();
    CHECK_EQ(cfg.dutyCyclePercent, (uint8_t)100);
    cfg.increaseDutyCycle();
    CHECK_EQ(cfg.dutyCyclePercent, (uint8_t)100);  // stays clamped
}

TEST(PWMChannelConfig, DutyCycleDecreasesByStepAndClampsAt0) {
    PWMChannelConfig cfg;
    cfg.dutyCyclePercent = 1;
    cfg.decreaseDutyCycle();
    CHECK_EQ(cfg.dutyCyclePercent, (uint8_t)0);
    cfg.decreaseDutyCycle();
    CHECK_EQ(cfg.dutyCyclePercent, (uint8_t)0);  // stays clamped
}

TEST(PWMChannelConfig, VariableTopIncreasesAndClampsAt65535) {
    PWMChannelConfig cfg;
    cfg.variableTopValue = 65500;
    cfg.increaseVariableTop();
    CHECK_EQ(cfg.variableTopValue, (uint16_t)65535);
    cfg.increaseVariableTop();
    CHECK_EQ(cfg.variableTopValue, (uint16_t)65535);  // stays clamped
}

TEST(PWMChannelConfig, VariableTopDecreasesAndClampsAt1) {
    PWMChannelConfig cfg;
    cfg.variableTopValue = 50;
    cfg.decreaseVariableTop();
    CHECK_EQ(cfg.variableTopValue, (uint16_t)1);
    cfg.decreaseVariableTop();
    CHECK_EQ(cfg.variableTopValue, (uint16_t)1);  // stays clamped, never reaches 0
}

// ===== computeSimplexCallArgs ==============================================

TEST(ComputeSimplexCallArgs, PassesFrequencyInvertingEnabledThrough) {
    PWMChannelConfig cfg;
    cfg.enabled = true;
    cfg.inverting = true;
    cfg.frequency = frequency_31_250HZ;
    SimplexPWMCallArgs args = computeSimplexCallArgs(cfg);
    CHECK_EQ(args.frequency, frequency_31_250HZ);
    CHECK_TRUE(args.inverting);
    CHECK_TRUE(args.enabled);
}

TEST(ComputeSimplexCallArgs, ScalesDutyCycleForEightBitResolution) {
    PWMChannelConfig cfg;
    cfg.frequency = frequency_62_500HZ;  // 8-bit, TOP=255
    cfg.dutyCyclePercent = 50;
    SimplexPWMCallArgs args = computeSimplexCallArgs(cfg);
    CHECK_EQ(args.rawDutyCycle, (uint16_t)127);  // 50% of 255, truncated
}

TEST(ComputeSimplexCallArgs, ScalesDutyCycleForTenBitResolution) {
    PWMChannelConfig cfg;
    cfg.frequency = frequency_15_625HZ;  // 10-bit, TOP=1023
    cfg.dutyCyclePercent = 100;
    SimplexPWMCallArgs args = computeSimplexCallArgs(cfg);
    CHECK_EQ(args.rawDutyCycle, (uint16_t)1023);
}

TEST(ComputeSimplexCallArgs, ZeroPercentDutyCycleIsAlwaysZero) {
    PWMChannelConfig cfg;
    cfg.frequency = frequency_variable;
    cfg.variableTopValue = 40000;
    cfg.dutyCyclePercent = 0;
    SimplexPWMCallArgs args = computeSimplexCallArgs(cfg);
    CHECK_EQ(args.rawDutyCycle, (uint16_t)0);
}

TEST(ComputeSimplexCallArgs, VariableFrequencyScalesDutyCycleAgainstVariableTop) {
    PWMChannelConfig cfg;
    cfg.frequency = frequency_variable;
    cfg.variableTopValue = 40000;
    cfg.dutyCyclePercent = 25;
    SimplexPWMCallArgs args = computeSimplexCallArgs(cfg);
    CHECK_EQ(args.rawFrequency, (uint16_t)40000);
    CHECK_EQ(args.rawDutyCycle, (uint16_t)10000);  // 25% of 40000
}

// ===== PWMComplexOutputConfig ==============================================

TEST(PWMComplexOutputConfig, ToggleEnabledAndEdgeFlipBothWays) {
    PWMComplexOutputConfig out;
    out.toggleEnabled();
    CHECK_TRUE(out.enabled);
    out.toggleEnabled();
    CHECK_FALSE(out.enabled);
    out.toggleEdge();
    CHECK_TRUE(out.inverting);
    out.toggleEdge();
    CHECK_FALSE(out.inverting);
}

TEST(PWMComplexOutputConfig, DutyCycleClampsAtBothEnds) {
    PWMComplexOutputConfig out;
    out.dutyCyclePercent = 100;
    out.increaseDutyCycle();
    CHECK_EQ(out.dutyCyclePercent, (uint8_t)100);
    out.dutyCyclePercent = 0;
    out.decreaseDutyCycle();
    CHECK_EQ(out.dutyCyclePercent, (uint8_t)0);
}

// ===== PWMComplexChannelConfig =============================================

TEST(PWMComplexChannelConfig, ModeDefaultsToOff) {
    PWMComplexChannelConfig cfg;
    CHECK_FALSE(cfg.enabled);
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Off);
}

TEST(PWMComplexChannelConfig, CycleModeNextGoesOffThenFixedThenVariableThenWraps) {
    PWMComplexChannelConfig cfg;
    cfg.frequency = frequency_31_250HZ;  // arbitrary non-default fixed frequency

    cfg.cycleModeNext();  // Off -> Fixed
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Fixed);
    CHECK_TRUE(cfg.enabled);
    CHECK_EQ(cfg.frequency, frequency_31_250HZ);  // preserved, not reset

    cfg.cycleModeNext();  // Fixed -> Variable
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Variable);
    CHECK_EQ(cfg.frequency, frequency_variable);

    cfg.cycleModeNext();  // Variable -> Off
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Off);
    CHECK_FALSE(cfg.enabled);
}

TEST(PWMComplexChannelConfig, CycleModeNextFromOffWithVariableFrequencyPickedResetsToAFixedFrequency) {
    PWMComplexChannelConfig cfg;
    cfg.enabled = false;
    cfg.frequency = frequency_variable;
    cfg.cycleModeNext();
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Fixed);
    CHECK_TRUE(cfg.frequency != frequency_variable);
}

TEST(PWMComplexChannelConfig, CycleModePreviousGoesOffThenVariableThenFixedThenWraps) {
    PWMComplexChannelConfig cfg;

    cfg.cycleModePrevious();  // Off -> Variable
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Variable);

    cfg.cycleModePrevious();  // Variable -> Fixed
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Fixed);
    CHECK_EQ(cfg.frequency, frequency_62_500HZ);

    cfg.cycleModePrevious();  // Fixed -> Off
    CHECK_TRUE(cfg.mode() == PWMChannelMode::Off);
}

TEST(PWMComplexChannelConfig, FrequencySelectionCyclesAndWraps) {
    PWMComplexChannelConfig cfg;
    cfg.selectPreviousFrequency();
    CHECK_EQ((int)cfg.frequency, (int)NUMBER_OF_PWM_FREQUENCIES - 1);
    cfg.selectNextFrequency();
    CHECK_EQ(cfg.frequency, frequency_62_500HZ);
}

TEST(PWMComplexChannelConfig, VariableTopClampsAtBothEnds) {
    PWMComplexChannelConfig cfg;
    cfg.variableTopValue = 65535;
    cfg.increaseVariableTop();
    CHECK_EQ(cfg.variableTopValue, (uint16_t)65535);
    cfg.variableTopValue = 1;
    cfg.decreaseVariableTop();
    CHECK_EQ(cfg.variableTopValue, (uint16_t)1);
}

TEST(PWMComplexChannelConfig, OutputsStartIndependentAndStayIndependent) {
    PWMComplexChannelConfig cfg;
    cfg.outputA.toggleEnabled();
    cfg.outputB.dutyCyclePercent = 80;
    CHECK_TRUE(cfg.outputA.enabled);
    CHECK_FALSE(cfg.outputB.enabled);
    CHECK_FALSE(cfg.outputC.enabled);
    CHECK_EQ(cfg.outputB.dutyCyclePercent, (uint8_t)80);
    CHECK_EQ(cfg.outputA.dutyCyclePercent, (uint8_t)50);
}

// ===== computeComplexCallArgs ==============================================

TEST(ComputeComplexCallArgs, EachOutputMapsToItsOwnArgsIndependently) {
    PWMComplexChannelConfig cfg;
    cfg.enabled = true;                   // channel not Off -- per-output enables apply
    cfg.frequency = frequency_7812_5HZ;  // 8-bit, TOP=255
    cfg.outputA.enabled = true;
    cfg.outputA.inverting = false;
    cfg.outputA.dutyCyclePercent = 0;
    cfg.outputB.enabled = true;
    cfg.outputB.inverting = true;
    cfg.outputB.dutyCyclePercent = 50;
    cfg.outputC.enabled = false;
    cfg.outputC.inverting = false;
    cfg.outputC.dutyCyclePercent = 100;

    ComplexPWMCallArgs args = computeComplexCallArgs(cfg);

    CHECK_EQ(args.frequency, frequency_7812_5HZ);
    CHECK_TRUE(args.enabledA);
    CHECK_FALSE(args.invertingA);
    CHECK_EQ(args.rawDutyCycleA, (uint16_t)0);

    CHECK_TRUE(args.enabledB);
    CHECK_TRUE(args.invertingB);
    CHECK_EQ(args.rawDutyCycleB, (uint16_t)127);  // 50% of 255

    CHECK_FALSE(args.enabledC);
    CHECK_EQ(args.rawDutyCycleC, (uint16_t)255);  // 100% of 255, disabled or not
}

TEST(ComputeComplexCallArgs, VariableFrequencyCarriesRawFrequencyThrough) {
    PWMComplexChannelConfig cfg;
    cfg.frequency = frequency_variable;
    cfg.variableTopValue = 20000;
    ComplexPWMCallArgs args = computeComplexCallArgs(cfg);
    CHECK_EQ(args.rawFrequency, (uint16_t)20000);
}

TEST(ComputeComplexCallArgs, ChannelOffForcesEveryOutputEnableLow) {
    PWMComplexChannelConfig cfg;
    cfg.enabled = false;                 // channel-level Off (mode() == Off)
    cfg.outputA.enabled = true;          // per-output toggles still set...
    cfg.outputB.enabled = true;
    cfg.outputC.enabled = true;
    cfg.outputA.dutyCyclePercent = 40;

    ComplexPWMCallArgs args = computeComplexCallArgs(cfg);

    CHECK_FALSE(args.enabledA);          // ...but masked away while the channel is Off
    CHECK_FALSE(args.enabledB);
    CHECK_FALSE(args.enabledC);
    // Duty/frequency still computed -- Off only gates the pin, like simplex.
    CHECK_EQ(args.rawDutyCycleA, (uint16_t)102);  // 40% of 255 (frequency_62_500HZ default)
}

TEST(ComputeComplexCallArgs, TurningTheChannelBackOnRestoresPerOutputEnables) {
    PWMComplexChannelConfig cfg;
    cfg.outputA.enabled = true;
    cfg.outputC.enabled = true;

    cfg.cycleModeNext();  // Off -> Fixed: channel now enabled

    ComplexPWMCallArgs args = computeComplexCallArgs(cfg);
    CHECK_TRUE(args.enabledA);
    CHECK_FALSE(args.enabledB);          // was never toggled on
    CHECK_TRUE(args.enabledC);
}
