#include "mini_test.h"
#include "PWMTiming.h"

// Expected values below are independently derived from the ATmega2560
// datasheet (ch. 17): Fast PWM modes 5/6/7 (8/9/10-bit, fixed TOP) and mode
// 14 (TOP = ICRn), and the standard CSn2:0 prescaler encoding. Every
// PWMFrequency case is checked, so every branch of pwmTimingFor() is
// exercised by this file.

static void expectTiming(PWMFrequency f, uint8_t wgmA, uint8_t wgmB, uint8_t cs,
                          bool usesIcrAsTop) {
    PWMTimingBits t = pwmTimingFor(f);
    CHECK_EQ(t.wgmA, wgmA);
    CHECK_EQ(t.wgmB, wgmB);
    CHECK_EQ(t.cs, cs);
    CHECK_EQ(t.usesInputCaptureAsTop, usesIcrAsTop);
}

TEST(PWMTiming, Variable_Mode14_NoPrescale_UsesICRAsTop) {
    expectTiming(frequency_variable, 0b10, 0b11, 0b001, true);
}

// -- no prescaling (CS = 0b001) -------------------------------------------
TEST(PWMTiming, F62500_8bit_NoPrescale) {
    expectTiming(frequency_62_500HZ, 0b01, 0b01, 0b001, false);
}
TEST(PWMTiming, F31250_9bit_NoPrescale) {
    expectTiming(frequency_31_250HZ, 0b10, 0b01, 0b001, false);
}
TEST(PWMTiming, F16625_10bit_NoPrescale) {
    expectTiming(frequency_16_625HZ, 0b11, 0b01, 0b001, false);
}

// -- /8 prescaling (CS = 0b010) --------------------------------------------
TEST(PWMTiming, F7812_5_8bit_Prescale8) {
    expectTiming(frequency_7812_5HZ, 0b01, 0b01, 0b010, false);
}
TEST(PWMTiming, F3906_25_9bit_Prescale8) {
    expectTiming(frequency_3906_25HZ, 0b10, 0b01, 0b010, false);
}
TEST(PWMTiming, F1953_125_10bit_Prescale8) {
    expectTiming(frequency_1953_125HZ, 0b11, 0b01, 0b010, false);
}

// -- /64 prescaling (CS = 0b011) -------------------------------------------
TEST(PWMTiming, F976_5625_8bit_Prescale64) {
    expectTiming(frequency_976_5625HZ, 0b01, 0b01, 0b011, false);
}
TEST(PWMTiming, F488_28125_9bit_Prescale64) {
    expectTiming(frequency_488_28125HZ, 0b10, 0b01, 0b011, false);
}
TEST(PWMTiming, F244_140625_10bit_Prescale64) {
    expectTiming(frequency_244_140625HZ, 0b11, 0b01, 0b011, false);
}

// -- /256 prescaling (CS = 0b100) -- no 8-bit variant exists ---------------
TEST(PWMTiming, F122_0703125_9bit_Prescale256) {
    expectTiming(frequency_122_0703125HZ, 0b10, 0b01, 0b100, false);
}
TEST(PWMTiming, F61_03515625_10bit_Prescale256) {
    expectTiming(frequency_61_03515625HZ, 0b11, 0b01, 0b100, false);
}

// -- /1024 prescaling (CS = 0b101) -- no 8-bit variant exists ---------------
TEST(PWMTiming, F30_517578125_9bit_Prescale1024) {
    expectTiming(frequency_30_517578125HZ, 0b10, 0b01, 0b101, false);
}
TEST(PWMTiming, F15_2587890625_10bit_Prescale1024) {
    expectTiming(frequency_15_2587890625HZ, 0b11, 0b01, 0b101, false);
}

// -- pwmCompareOutputBits: every (enabled, inverting) combination ----------

TEST(PWMCompareOutputBits, DisabledIsDisconnectedRegardlessOfInverting) {
    CHECK_EQ(pwmCompareOutputBits(false, false), (uint8_t)0b00);
    CHECK_EQ(pwmCompareOutputBits(false, true), (uint8_t)0b00);
}

TEST(PWMCompareOutputBits, EnabledNonInvertingIsClearOnCompareSetAtBottom) {
    CHECK_EQ(pwmCompareOutputBits(true, false), (uint8_t)0b10);
}

TEST(PWMCompareOutputBits, EnabledInvertingIsSetOnCompareClearAtBottom) {
    CHECK_EQ(pwmCompareOutputBits(true, true), (uint8_t)0b11);
}

TEST(PWMCompareOutputBits, NeverReturnsTheReservedZeroOneCombination) {
    // 0b01 is reserved for these Fast PWM widths (see PWMTiming.h) --
    // forgetting COMnX1 and producing it by accident was the original bug.
    bool enabledValues[] = {false, true};
    bool invertingValues[] = {false, true};
    for (bool en : enabledValues) {
        for (bool inv : invertingValues) {
            CHECK_TRUE(pwmCompareOutputBits(en, inv) != 0b01);
        }
    }
}
