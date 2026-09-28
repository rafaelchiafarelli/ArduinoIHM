#include "mini_test.h"
#include "SerialConfig.h"
#include "SerialConfigWire.h"
#include <string.h>

// SerialConfig model (serial_config / config_model task 1): defaults,
// validation, RE2 step helpers, digit-at-a-time acceleration and the
// 301/302 adapters.

TEST(SerialConfig, DefaultsAreDisabledAndInRange) {
    SerialConfig c;
    memset(&c, 0xAB, sizeof(c));
    serialConfigDefaults(c);
    for (int b = 0; b < SERIAL_CAN_BUSES; b++) {
        CHECK_EQ((int)c.can[b].gen.enable, 0);
        CHECK_EQ((int)c.can[b].gen.dlc, 8);
        CHECK_EQ((int)c.can[b].gen.period_ms, 100);
        CHECK_EQ((int)c.can[b].gen.data[7], 0);
        CHECK_TRUE(canGeneratorValid(c.can[b].gen));
    }
    CHECK_EQ((int)c.rs485.gen.enable, 0);
    CHECK_EQ((int)c.rs485.gen.length, 8);
    CHECK_TRUE(rs485GeneratorValid(c.rs485.gen));
}

TEST(SerialConfig, CanValidationRanges) {
    SerialConfig c;
    serialConfigDefaults(c);
    CanGeneratorConfig g = c.can[0].gen;
    g.id = 0x7FF; CHECK_TRUE(canGeneratorValid(g));
    g.id = 0x800; CHECK_TRUE(!canGeneratorValid(g));
    g.extended = 1; CHECK_TRUE(canGeneratorValid(g));
    g.id = 0x1FFFFFFF; CHECK_TRUE(canGeneratorValid(g));
    g.id = 0x20000000; CHECK_TRUE(!canGeneratorValid(g));
    g.id = 0; g.dlc = 9; CHECK_TRUE(!canGeneratorValid(g));
    g.dlc = 8; g.enable = 2; CHECK_TRUE(!canGeneratorValid(g));
}

TEST(SerialConfig, Rs485ValidationRanges) {
    SerialConfig c;
    serialConfigDefaults(c);
    Rs485GeneratorConfig g = c.rs485.gen;
    g.length = 32; CHECK_TRUE(rs485GeneratorValid(g));
    g.length = 33; CHECK_TRUE(!rs485GeneratorValid(g));
}

TEST(SerialConfig, LeavingExtendedClampsTheId) {
    SerialConfig c;
    serialConfigDefaults(c);
    CanGeneratorConfig g = c.can[1].gen;
    CHECK_TRUE(canSetExtended(g, true));
    g.id = 0x12345678;
    CHECK_TRUE(!canSetExtended(g, true));   // already extended: no change
    CHECK_TRUE(canSetExtended(g, false));
    CHECK_EQ((uint32_t)g.id, (uint32_t)0x7FF);
    CHECK_TRUE(canGeneratorValid(g));
}

TEST(SerialConfig, StepValueClampsBothEnds) {
    CHECK_EQ(serialStepValue(5, -10, 100), (uint32_t)0);
    CHECK_EQ(serialStepValue(95, 10, 100), (uint32_t)100);
    CHECK_EQ(serialStepValue(50, 16, 100), (uint32_t)66);
    CHECK_EQ(serialStepValue(0, -1, 100), (uint32_t)0);
    CHECK_EQ(serialStepValue(0x1FFFFFF0, 0x10000000, SERIAL_CAN_EXT_ID_MAX), (uint32_t)SERIAL_CAN_EXT_ID_MAX);
    CHECK_EQ(serialStepValue(7, 0, 100), (uint32_t)7);
}

TEST(SerialConfig, StepValueLeavesAnAboveMaxStartAloneGoingUp) {
    // A PC may send period 65535; the RE2 ceiling is 65530.
    CHECK_EQ(serialStepValue(65535, 10, SERIAL_PERIOD_EDIT_MAX_MS), (uint32_t)65535);
    CHECK_EQ(serialStepValue(65535, -10, SERIAL_PERIOD_EDIT_MAX_MS), (uint32_t)65525);
}

TEST(SerialConfig, StepEnumClampsOrWraps) {
    CHECK_EQ((int)serialStepEnum(0, -1, 4, false), 0);
    CHECK_EQ((int)serialStepEnum(3, 1, 4, false), 3);
    CHECK_EQ((int)serialStepEnum(3, 1, 4, true), 0);
    CHECK_EQ((int)serialStepEnum(0, -1, 4, true), 3);
    CHECK_EQ((int)serialStepEnum(9, 0, 4, false), 3);   // out-of-range start
    CHECK_EQ((int)serialStepEnum(0, 1, 0, false), 0);   // empty enum
}

TEST(SerialConfig, SetFlagSetsNeverFlips) {
    uint8_t f = 0;
    CHECK_TRUE(serialSetFlag(f, true));
    CHECK_EQ((int)f, 1);
    CHECK_TRUE(!serialSetFlag(f, true));
    CHECK_EQ((int)f, 1);
    CHECK_TRUE(serialSetFlag(f, false));
    CHECK_EQ((int)f, 0);
}

// --- acceleration --------------------------------------------------------

namespace {
// Clicks `n` times, `gapMs` apart, starting at *t; returns the last step.
uint32_t clicks(DigitAccel& a, uint32_t* t, int n, uint32_t gapMs, uint8_t base, uint8_t maxLevel) {
    uint32_t step = 0;
    for (int i = 0; i < n; i++) {
        *t += gapMs;
        step = digitAccelStep(a, *t, base, maxLevel);
    }
    return step;
}
}  // namespace

TEST(DigitAccel, SlowClicksStayAtOneUnit) {
    DigitAccel a; digitAccelReset(a);
    uint32_t t = 1000;
    CHECK_EQ(clicks(a, &t, 20, 150, 16, 7), (uint32_t)1);
}

TEST(DigitAccel, FourFastGapsRaiseOneDigit) {
    DigitAccel a; digitAccelReset(a);
    uint32_t t = 1000;
    CHECK_EQ(digitAccelStep(a, t, 16, 7), (uint32_t)1);   // first click
    CHECK_EQ(clicks(a, &t, 3, 50, 16, 7), (uint32_t)1);   // 3 fast gaps
    CHECK_EQ(clicks(a, &t, 1, 50, 16, 7), (uint32_t)16);  // 4th fast gap
    CHECK_EQ(clicks(a, &t, 4, 50, 16, 7), (uint32_t)256); // 4 more
}

TEST(DigitAccel, LevelIsCappedAtTheTopDigit) {
    DigitAccel a; digitAccelReset(a);
    uint32_t t = 0;
    CHECK_EQ(clicks(a, &t, 60, 40, 16, 2), (uint32_t)256);   // standard ID: 0x100
    DigitAccel p; digitAccelReset(p);
    t = 0;
    CHECK_EQ(clicks(p, &t, 60, 40, 10, 3), (uint32_t)1000);  // period: x1000 units
}

TEST(DigitAccel, PauseDropsOneDigitAndLongPauseResets) {
    DigitAccel a; digitAccelReset(a);
    uint32_t t = 0;
    CHECK_EQ(clicks(a, &t, 13, 40, 16, 7), (uint32_t)4096);  // level 3
    CHECK_EQ(clicks(a, &t, 1, 400, 16, 7), (uint32_t)256);   // pause: one down
    CHECK_EQ(clicks(a, &t, 1, 400, 16, 7), (uint32_t)16);    // another
    CHECK_EQ(clicks(a, &t, 1, 2000, 16, 7), (uint32_t)1);    // long pause: reset
}

TEST(DigitAccel, MediumGapBreaksTheFastRunButKeepsTheLevel) {
    DigitAccel a; digitAccelReset(a);
    uint32_t t = 0;
    CHECK_EQ(clicks(a, &t, 5, 40, 10, 4), (uint32_t)10);    // level 1
    CHECK_EQ(clicks(a, &t, 3, 40, 10, 4), (uint32_t)10);    // run of 3
    CHECK_EQ(clicks(a, &t, 1, 150, 10, 4), (uint32_t)10);   // run broken
    CHECK_EQ(clicks(a, &t, 3, 40, 10, 4), (uint32_t)10);    // run of 3 again
    CHECK_EQ(clicks(a, &t, 1, 40, 10, 4), (uint32_t)100);   // 4th: up
}

TEST(DigitAccel, ResetGoesBackToOneUnit) {
    DigitAccel a; digitAccelReset(a);
    uint32_t t = 0;
    CHECK_EQ(clicks(a, &t, 9, 40, 16, 7), (uint32_t)256);
    digitAccelReset(a);   // focus moved
    CHECK_EQ(clicks(a, &t, 1, 40, 16, 7), (uint32_t)1);
}

TEST(DigitAccel, SurvivesMillisWraparound) {
    DigitAccel a; digitAccelReset(a);
    uint32_t t = 0xFFFFFFFFUL - 100;
    CHECK_EQ(clicks(a, &t, 9, 40, 16, 7), (uint32_t)256);   // crosses 0
}

// --- 301/302 adapters ----------------------------------------------------

TEST(SerialConfigWire, CanRoundTrip) {
    CanGeneratorConfig g;
    g.enable = 1; g.id = 0x18FEF100; g.extended = 1; g.dlc = 5;
    for (int i = 0; i < 8; i++) g.data[i] = (uint8_t)(0x10 + i);
    g.period_ms = 250; g.repeat_count = 7;
    mavlink_can_signal_config_t m;
    canGeneratorToWire(1, g, &m);
    CHECK_EQ((int)m.bus_id, 1);
    CanGeneratorConfig back;
    CHECK_TRUE(canGeneratorFromWire(m, &back));
    CHECK_EQ((uint32_t)back.id, (uint32_t)0x18FEF100);
    CHECK_EQ((int)back.extended, 1);
    CHECK_EQ((int)back.dlc, 5);
    CHECK_EQ((int)back.data[7], 0x17);
    CHECK_EQ((int)back.period_ms, 250);
    CHECK_EQ((int)back.repeat_count, 7);
    CHECK_EQ((int)back.enable, 1);
}

TEST(SerialConfigWire, CanRejectsOutOfRangeAndLeavesOutAlone) {
    mavlink_can_signal_config_t m;
    memset(&m, 0, sizeof(m));
    CanGeneratorConfig out;
    memset(&out, 0x5A, sizeof(out));
    m.bus_id = 2;
    CHECK_TRUE(!canGeneratorFromWire(m, &out));
    m.bus_id = 0; m.dlc = 9;
    CHECK_TRUE(!canGeneratorFromWire(m, &out));
    m.dlc = 8; m.can_id = 0x800;   // standard ID too big
    CHECK_TRUE(!canGeneratorFromWire(m, &out));
    CHECK_EQ((int)out.dlc, 0x5A);
}

TEST(SerialConfigWire, Rs485RoundTripAndReject) {
    Rs485GeneratorConfig g;
    g.enable = 1; g.length = 32;
    for (int i = 0; i < 32; i++) g.data[i] = (uint8_t)i;
    g.period_ms = 10; g.repeat_count = 0;
    mavlink_rs485_signal_config_t m;
    rs485GeneratorToWire(g, &m);
    Rs485GeneratorConfig back;
    CHECK_TRUE(rs485GeneratorFromWire(m, &back));
    CHECK_EQ((int)back.length, 32);
    CHECK_EQ((int)back.data[31], 31);
    CHECK_EQ((int)back.period_ms, 10);
    m.length = 33;
    CHECK_TRUE(!rs485GeneratorFromWire(m, &back));
    CHECK_EQ((int)back.length, 32);
}
