#include "mini_test.h"
#include "ButtonMap.h"

TEST(BuildButtonMap, SentinelBit7IsAlwaysSet) {
    CHECK_EQ((uint8_t)(buildButtonMap(0) & 0x80), (uint8_t)0x80);
    CHECK_EQ((uint8_t)(buildButtonMap(0xFFFF) & 0x80), (uint8_t)0x80);
}

TEST(BuildButtonMap, PushButtons0Through3PassThroughUnchanged) {
    CHECK_EQ((uint8_t)(buildButtonMap(0b0000000000000000) & 0x0F), (uint8_t)0b0000);
    CHECK_EQ((uint8_t)(buildButtonMap(0b0000000000001111) & 0x0F), (uint8_t)0b1111);
    CHECK_EQ((uint8_t)(buildButtonMap(0b0000000000000101) & 0x0F), (uint8_t)0b0101);
}

TEST(BuildButtonMap, Rot0ButtonIsBit8MappedToOutputBit6) {
    // rot0's button lives at BinaryInputs pin-table index 8 (PL4).
    CHECK_EQ((uint8_t)(buildButtonMap(1 << 8) & 0x40), (uint8_t)0x40);
    CHECK_EQ((uint8_t)(buildButtonMap((uint16_t)~(1 << 8)) & 0x40), (uint8_t)0x00);
}

TEST(BuildButtonMap, Rot1ButtonIsBit11MappedToOutputBit5) {
    // This is the bug this file exists to pin down: rot1's actual push
    // button is at pin-table index 11 (PL0). The old code read index 10
    // (PL1), which is one of rot1's own *encoder signal* pins, not a
    // button at all.
    CHECK_EQ((uint8_t)(buildButtonMap(1 << 11) & 0x20), (uint8_t)0x20);
    CHECK_EQ((uint8_t)(buildButtonMap((uint16_t)~(1 << 11)) & 0x20), (uint8_t)0x00);
}

TEST(BuildButtonMap, Bit10NoLongerAffectsRot1ButtonBit) {
    // Regression guard for the fix above: bit 10 (PL1, an encoder signal
    // pin) must NOT be able to fake a rot1 button press.
    uint8_t withBit10Set = buildButtonMap(1 << 10);
    uint8_t withNothingSet = buildButtonMap(0);
    CHECK_EQ((uint8_t)(withBit10Set & 0x20), (uint8_t)(withNothingSet & 0x20));
    CHECK_EQ((uint8_t)(withBit10Set & 0x20), (uint8_t)0x00);
}

TEST(BuildButtonMap, Rot2ButtonIsBit14MappedToOutputBit4) {
    // rot2's button lives at pin-table index 14 (PC7).
    CHECK_EQ((uint8_t)(buildButtonMap(1 << 14) & 0x10), (uint8_t)0x10);
    CHECK_EQ((uint8_t)(buildButtonMap((uint16_t)~(1 << 14)) & 0x10), (uint8_t)0x00);
}

TEST(BuildButtonMap, AllThreeEncoderButtonsAreIndependent) {
    uint16_t allThree = (1 << 8) | (1 << 11) | (1 << 14);
    uint8_t result = buildButtonMap(allThree);
    CHECK_EQ((uint8_t)(result & 0x40), (uint8_t)0x40);  // rot0
    CHECK_EQ((uint8_t)(result & 0x20), (uint8_t)0x20);  // rot1
    CHECK_EQ((uint8_t)(result & 0x10), (uint8_t)0x10);  // rot2

    uint8_t onlyRot1 = buildButtonMap(1 << 11);
    CHECK_EQ((uint8_t)(onlyRot1 & 0x40), (uint8_t)0x00);
    CHECK_EQ((uint8_t)(onlyRot1 & 0x20), (uint8_t)0x20);
    CHECK_EQ((uint8_t)(onlyRot1 & 0x10), (uint8_t)0x00);
}
