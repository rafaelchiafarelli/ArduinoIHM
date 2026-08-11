#include "mini_test.h"
#include "PWM.h"

namespace {

struct FakeSimplexRegs {
    uint8_t tccrA = 0xAA;   // poisoned with a recognizable non-zero pattern
    uint8_t tccrB = 0xAA;
    uint16_t icr = 0xBEEF;  // poisoned: must stay untouched unless frequency_variable
    uint16_t ocrA = 0xBEEF;

    SimplexPWMRegisters handles() {
        return {{&tccrA}, {&tccrB}, {&icr}, {&ocrA}};
    }
};

struct FakeComplexRegs {
    uint8_t tccrA = 0xAA;
    uint8_t tccrB = 0xAA;
    uint16_t icr = 0xBEEF;
    uint16_t ocrA = 0xBEEF;
    uint16_t ocrB = 0xBEEF;
    uint16_t ocrC = 0xBEEF;

    ComplexPWMRegisters handles() {
        return {{&tccrA}, {&tccrB}, {&icr}, {&ocrA}, {&ocrB}, {&ocrC}};
    }
};

}  // namespace

// ===== applySimplexPWMConfig ==============================================

TEST(ApplySimplexPWMConfig, VariableFrequencyWritesICRAndSetsBothCOMBits) {
    FakeSimplexRegs regs;
    applySimplexPWMConfig(regs.handles(), frequency_variable, /*inverting*/false,
                           /*enabled*/true, /*frequency*/12345, /*dutyCycle*/999);

    // COM=0b10 (bits7:6), WGMn1:0=0b10 (bits1:0) -> 1000 0010 = 0x82
    CHECK_EQ(regs.tccrA, (uint8_t)0x82);
    // WGMn3:2=0b11 (bits4:3), CSn2:0=0b001 (bits2:0) -> 0001 1001 = 0x19
    CHECK_EQ(regs.tccrB, (uint8_t)0x19);
    CHECK_EQ(regs.icr, (uint16_t)12345);
    CHECK_EQ(regs.ocrA, (uint16_t)999);
}

TEST(ApplySimplexPWMConfig, FixedFrequencyDoesNotTouchICR) {
    FakeSimplexRegs regs;
    applySimplexPWMConfig(regs.handles(), frequency_62_500HZ, false, true, 0, 500);
    CHECK_EQ(regs.icr, (uint16_t)0xBEEF);  // untouched -- only ICR-as-TOP modes write it
    CHECK_EQ(regs.ocrA, (uint16_t)500);
}

TEST(ApplySimplexPWMConfig, DisabledStillConfiguresTimerButDisconnectsPin) {
    FakeSimplexRegs regs;
    applySimplexPWMConfig(regs.handles(), frequency_31_250HZ, false, /*enabled*/false, 0, 0);
    // COM=0b00 (disconnected), WGMn1:0=0b10 (9-bit) -> 0000 0010 = 0x02
    CHECK_EQ(regs.tccrA, (uint8_t)0x02);
    // The timer itself keeps running at the configured rate even when the
    // pin is disconnected -- WGMn3:2/CSn2:0 are NOT gated by `enabled`.
    CHECK_EQ(regs.tccrB, (uint8_t)0x09);  // WGMn3:2=01, CS=001 -> 0000 1001
}

TEST(ApplySimplexPWMConfig, NonInvertingSetsOnlyCOMn1) {
    FakeSimplexRegs regs;
    // 9-bit mode: exactly the width where the original bug omitted COMn1.
    applySimplexPWMConfig(regs.handles(), frequency_31_250HZ, false, true, 0, 0);
    CHECK_EQ((uint8_t)(regs.tccrA >> 6), (uint8_t)0b10);
}

TEST(ApplySimplexPWMConfig, InvertingSetsBothCOMBits) {
    FakeSimplexRegs regs;
    applySimplexPWMConfig(regs.handles(), frequency_31_250HZ, true, true, 0, 0);
    CHECK_EQ((uint8_t)(regs.tccrA >> 6), (uint8_t)0b11);
}

TEST(ApplySimplexPWMConfig, TenBitModeAlsoSetsCOMn1) {
    FakeSimplexRegs regs;
    applySimplexPWMConfig(regs.handles(), frequency_15_625HZ, false, true, 0, 0);
    CHECK_EQ((uint8_t)(regs.tccrA >> 6), (uint8_t)0b10);
}

// ===== applyComplexPWMConfig ==============================================

TEST(ApplyComplexPWMConfig, AllThreeOutputsCombineIntoOneTCCRAWrite) {
    FakeComplexRegs regs;
    // A: enabled, non-inverting (0b10) | B: enabled, inverting (0b11) | C: disabled (0b00)
    applyComplexPWMConfig(regs.handles(), frequency_variable, 1000,
                           /*A*/ false, true,
                           /*B*/ true, true,
                           /*C*/ false, false,
                           111, 222, 333);
    // bits7:6=10 (A), bits5:4=11 (B), bits3:2=00 (C), bits1:0=10 (WGM, variable)
    // 1011 0010 = 0xB2
    CHECK_EQ(regs.tccrA, (uint8_t)0xB2);
    CHECK_EQ(regs.icr, (uint16_t)1000);
    CHECK_EQ(regs.ocrA, (uint16_t)111);
    CHECK_EQ(regs.ocrB, (uint16_t)222);
    CHECK_EQ(regs.ocrC, (uint16_t)333);
}

TEST(ApplyComplexPWMConfig, ConfiguringOnlyOneOutputDoesNotClobberTheOthers) {
    // This is the clobbering bug this file exists to make impossible: the
    // original code sometimes used `TCCR1A = ...` per output (instead of
    // `|=`), so enabling B after A wiped A's bits. Here everything is
    // computed from independent per-output inputs and combined in one
    // expression, so there is nothing to clobber.
    FakeComplexRegs regs;
    applyComplexPWMConfig(regs.handles(), frequency_62_500HZ, 0,
                           /*A*/ false, true,   // enabled, non-inverting
                           /*B*/ false, true,   // enabled, non-inverting
                           /*C*/ false, true,   // enabled, non-inverting
                           10, 20, 30);
    uint8_t comA = (uint8_t)((regs.tccrA >> 6) & 0b11);
    uint8_t comB = (uint8_t)((regs.tccrA >> 4) & 0b11);
    uint8_t comC = (uint8_t)((regs.tccrA >> 2) & 0b11);
    CHECK_EQ(comA, (uint8_t)0b10);
    CHECK_EQ(comB, (uint8_t)0b10);
    CHECK_EQ(comC, (uint8_t)0b10);
}

TEST(ApplyComplexPWMConfig, EachOutputKeepsItsOwnInvertingChoiceIndependently) {
    FakeComplexRegs regs;
    applyComplexPWMConfig(regs.handles(), frequency_7812_5HZ, 0,
                           /*A*/ true, true,    // enabled, inverting
                           /*B*/ false, true,   // enabled, non-inverting
                           /*C*/ true, false,   // disabled (inverting flag irrelevant)
                           0, 0, 0);
    uint8_t comA = (uint8_t)((regs.tccrA >> 6) & 0b11);
    uint8_t comB = (uint8_t)((regs.tccrA >> 4) & 0b11);
    uint8_t comC = (uint8_t)((regs.tccrA >> 2) & 0b11);
    CHECK_EQ(comA, (uint8_t)0b11);  // inverting
    CHECK_EQ(comB, (uint8_t)0b10);  // non-inverting
    CHECK_EQ(comC, (uint8_t)0b00);  // disabled
}

TEST(ApplyComplexPWMConfig, NineAndTenBitModesSetCOMn1ForEveryOutput) {
    // Same "missing COMn1" bug class as the simplex case, but here it must
    // hold independently for A, B and C.
    FakeComplexRegs regs;
    applyComplexPWMConfig(regs.handles(), frequency_1953_125HZ, 0,
                           false, true, false, true, false, true,
                           0, 0, 0);
    CHECK_EQ((uint8_t)((regs.tccrA >> 6) & 0b11), (uint8_t)0b10);
    CHECK_EQ((uint8_t)((regs.tccrA >> 4) & 0b11), (uint8_t)0b10);
    CHECK_EQ((uint8_t)((regs.tccrA >> 2) & 0b11), (uint8_t)0b10);
}

TEST(ApplyComplexPWMConfig, FixedFrequencyDoesNotTouchICR) {
    FakeComplexRegs regs;
    applyComplexPWMConfig(regs.handles(), frequency_244_140625HZ, 0,
                           false, true, false, true, false, true, 1, 2, 3);
    CHECK_EQ(regs.icr, (uint16_t)0xBEEF);
}

TEST(ApplyComplexPWMConfig, TccrBCarriesSharedWgmAndPrescalerRegardlessOfPerOutputState) {
    FakeComplexRegs regs;
    applyComplexPWMConfig(regs.handles(), frequency_976_5625HZ, 0,
                           false, false, false, false, false, false, 0, 0, 0);
    // All outputs disabled, but the timer's own mode/prescaler bits are
    // still programmed: WGMn3:2=01, CS=011 (/64) -> 0000 1011 = 0x0B
    CHECK_EQ(regs.tccrB, (uint8_t)0x0B);
}
