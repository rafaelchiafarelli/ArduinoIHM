#include "mini_test.h"
#include "RelayCommand.h"

TEST(RelayCommand, ApplySetsOnlyMaskedBits) {
    // relay 0 on, relay 3 off; everything else untouched
    CHECK_EQ(relayCommandApply(0b10001000, 0b00001001, 0b00000001), 0b10000001);
}

TEST(RelayCommand, ApplyIgnoresStateBitsOutsideMask) {
    CHECK_EQ(relayCommandApply(0x00, 0b00000010, 0xFF), 0b00000010);
}

TEST(RelayCommand, ApplyIsASetNotAFlip) {
    uint8_t once = relayCommandApply(0x00, 0x01, 0x01);
    CHECK_EQ(relayCommandApply(once, 0x01, 0x01), 0x01);
}

TEST(RelayCommand, EmptyMaskChangesNothing) {
    CHECK_EQ(relayCommandApply(0xA5, 0x00, 0xFF), 0xA5);
}

TEST(RelayCommand, AccumulateNewerFrameWinsOnOverlap) {
    uint8_t m = 0, s = 0;
    relayCommandAccumulate(m, s, 0b00000011, 0b00000011); // 0 on, 1 on
    relayCommandAccumulate(m, s, 0b00000110, 0b00000100); // 1 off, 2 on
    CHECK_EQ(m, 0b00000111);
    CHECK_EQ(s, 0b00000101);
}

TEST(RelayCommand, AccumulatedEqualsFramesAppliedInOrder) {
    uint8_t m = 0, s = 0;
    relayCommandAccumulate(m, s, 0xF0, 0xA0);
    relayCommandAccumulate(m, s, 0x3C, 0x0C);
    uint8_t oneByOne = relayCommandApply(relayCommandApply(0x5A, 0xF0, 0xA0), 0x3C, 0x0C);
    CHECK_EQ(relayCommandApply(0x5A, m, s), oneByOne);
}
