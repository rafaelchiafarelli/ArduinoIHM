#include "mini_test.h"
#include "PWMWireConfig.h"

namespace {
PwmWireConfig make(uint8_t channel, uint8_t f, uint16_t top = 0) {
    PwmWireConfig w{};
    w.channel = channel;
    w.f_selector = f;
    w.frequency = top;
    return w;
}
}  // namespace

TEST(PWMWireConfig, AcceptsEveryChannelZeroToThree) {
    for (uint8_t ch = 0; ch <= 3; ch++) CHECK_TRUE(pwmWireConfigValid(make(ch, 0)));
}

TEST(PWMWireConfig, RejectsChannelFour) {
    CHECK_FALSE(pwmWireConfigValid(make(4, 0)));
}

TEST(PWMWireConfig, SelectorBoundary) {
    CHECK_TRUE(pwmWireConfigValid(make(0, (uint8_t)frequency_variable)));
    CHECK_FALSE(pwmWireConfigValid(make(0, (uint8_t)NUMBER_OF_PWM_FREQUENCIES)));
    CHECK_FALSE(pwmWireConfigValid(make(0, 255)));
}

TEST(PWMWireConfig, SimplexMapsOutputAOnlyAndIgnoresBAndC) {
    PwmWireConfig w = make(1, (uint8_t)frequency_62_500HZ);
    w.out[0] = {1, 1, 40};
    w.out[1] = {1, 1, 99};
    w.out[2] = {1, 1, 99};
    PWMChannelConfig c = pwmWireToSimplex(w);
    CHECK_TRUE(c.enabled);
    CHECK_TRUE(c.inverting);
    CHECK_EQ(c.dutyCyclePercent, (uint8_t)40);
    CHECK_EQ(c.frequency, frequency_62_500HZ);
}

TEST(PWMWireConfig, ComplexMapsAllThreeOutputsIndependently) {
    PwmWireConfig w = make(2, (uint8_t)frequency_31_250HZ);
    w.out[0] = {1, 0, 10};
    w.out[1] = {0, 1, 20};
    w.out[2] = {1, 1, 30};
    PWMComplexChannelConfig c = pwmWireToComplex(w);
    CHECK_TRUE(c.enabled);
    CHECK_TRUE(c.outputA.enabled);  CHECK_FALSE(c.outputA.inverting); CHECK_EQ(c.outputA.dutyCyclePercent, (uint8_t)10);
    CHECK_FALSE(c.outputB.enabled); CHECK_TRUE(c.outputB.inverting);  CHECK_EQ(c.outputB.dutyCyclePercent, (uint8_t)20);
    CHECK_TRUE(c.outputC.enabled);  CHECK_TRUE(c.outputC.inverting);  CHECK_EQ(c.outputC.dutyCyclePercent, (uint8_t)30);
}

TEST(PWMWireConfig, DutyAbove100IsClamped) {
    PwmWireConfig w = make(0, 0);
    w.out[0] = {1, 0, 150};
    CHECK_EQ(pwmWireToSimplex(w).dutyCyclePercent, (uint8_t)100);
    w.channel = 2;
    w.out[1] = {1, 0, 255};
    CHECK_EQ(pwmWireToComplex(w).outputB.dutyCyclePercent, (uint8_t)100);
}

TEST(PWMWireConfig, VariableTopSurvivesIntoBothShapes) {
    PwmWireConfig w = make(0, (uint8_t)frequency_variable, 1234);
    CHECK_EQ(pwmWireToSimplex(w).variableTopValue, (uint16_t)1234);
    w.channel = 3;
    CHECK_EQ(pwmWireToComplex(w).variableTopValue, (uint16_t)1234);
}

// Guards the percent-vs-raw trap: PWM::setupPWMChannelN takes a RAW OCR
// count, so the wire's percent must be scaled by the mode's TOP.
TEST(PWMWireConfig, FiftyPercentBecomesHalfOfTopNotTheNumberFifty) {
    PwmWireConfig w = make(0, (uint8_t)frequency_62_500HZ);
    w.out[0] = {1, 0, 50};
    SimplexPWMCallArgs a = computeSimplexCallArgs(pwmWireToSimplex(w));
    uint16_t top = pwmResolutionTop(frequency_62_500HZ, 0);
    CHECK_EQ(a.rawDutyCycle, (uint16_t)((50u * top) / 100));
    CHECK_TRUE(a.rawDutyCycle != 50);
    CHECK_TRUE(a.enabled);
}

TEST(PWMWireConfig, ComplexDisabledOutputStaysDisabledInCallArgs) {
    PwmWireConfig w = make(2, (uint8_t)frequency_31_250HZ);
    w.out[0] = {1, 0, 50};
    ComplexPWMCallArgs a = computeComplexCallArgs(pwmWireToComplex(w));
    CHECK_TRUE(a.enabledA);
    CHECK_FALSE(a.enabledB);
    CHECK_FALSE(a.enabledC);
}

// ---- On-screen duty editing (pwmStepDuty) ----

TEST(PWMDutyEdit, StepsByOnePercentEachWay) {
    CHECK_EQ(pwmStepDuty(50, 1), 51);
    CHECK_EQ(pwmStepDuty(50, -1), 49);
    CHECK_EQ(pwmStepDuty(50, 0), 50);
}

TEST(PWMDutyEdit, ClampsAtZeroAndHundred) {
    CHECK_EQ(pwmStepDuty(100, 1), 100);
    CHECK_EQ(pwmStepDuty(0, -1), 0);
    CHECK_EQ(pwmStepDuty(98, 10), 100);
    CHECK_EQ(pwmStepDuty(3, -100), 0);
}

TEST(PWMDutyEdit, OutOfRangeStartIsTreatedAsHundred) {
    CHECK_EQ(pwmStepDuty(150, 0), 100);
    CHECK_EQ(pwmStepDuty(150, -1), 99);
    CHECK_EQ(pwmStepDuty(255, 1), 100);
}

// ---- On-screen switch editing (pwmWireSetOutputBit) ----

TEST(PWMSwitchEdit, SetsEnableAndReportsChange) {
    PwmWireConfig w = make(2, 0);
    CHECK_TRUE(pwmWireSetOutputBit(w, 1, false, true));
    CHECK_EQ(w.out[1].enabled, 1);
    CHECK_EQ(w.out[1].inverting, 0);
    CHECK_EQ(w.out[0].enabled, 0);
    CHECK_TRUE(pwmWireSetOutputBit(w, 1, false, false));
    CHECK_EQ(w.out[1].enabled, 0);
}

TEST(PWMSwitchEdit, SetsInvertingOnly) {
    PwmWireConfig w = make(0, 0);
    CHECK_TRUE(pwmWireSetOutputBit(w, 0, true, true));
    CHECK_EQ(w.out[0].inverting, 1);
    CHECK_EQ(w.out[0].enabled, 0);
}

TEST(PWMSwitchEdit, SameValueIsNoChange) {
    PwmWireConfig w = make(0, 0);
    CHECK_FALSE(pwmWireSetOutputBit(w, 0, false, false));
    w.out[0].enabled = 2;   // nonzero from the wire counts as on
    CHECK_FALSE(pwmWireSetOutputBit(w, 0, false, true));
    CHECK_EQ(w.out[0].enabled, 2);
}

TEST(PWMSwitchEdit, OutputOutOfRangeIsIgnored) {
    PwmWireConfig w = make(3, 0);
    CHECK_FALSE(pwmWireSetOutputBit(w, 3, false, true));
}
