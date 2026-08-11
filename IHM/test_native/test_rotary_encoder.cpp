#include "mini_test.h"
#include "RotaryEncoder.h"

// RotaryEncoder only ever stores its BinaryInputs* (never dereferences it --
// see RotaryEncoder.h), so nullptr is a safe stand-in for these tests.

TEST(RotaryEncoder, Encoder1NeverReactsToItsOwnPushButtonPin) {
    // Encoder1's quadrature signal pins are BinaryInputs indices 9 (PL2)
    // and 10 (PL1); index 11 (PL0) is its push button, not a signal pin.
    // This is the exact bug being fixed: Pin1 used to be 11 instead of 10,
    // so toggling the push button pin looked like encoder rotation. With
    // pins 9/10 held constant, toggling bit 11 alone must never produce a
    // direction.
    RotaryEncoder enc(nullptr);
    uint16_t bMap = 0;
    enc.ms_handler(bMap);
    CHECK_TRUE(enc.getDirection(1) == not_supported);

    for (int i = 0; i < 8; i++) {
        bMap ^= (1 << 11);
        enc.ms_handler(bMap);
        CHECK_TRUE(enc.getDirection(1) == not_supported);
    }
}

TEST(RotaryEncoder, Encoder1DecodesIdenticallyToEncoder0WhenShiftedToPins9And10) {
    // Same decode algorithm for every encoder, applied to a different pair
    // of bMap bits -- so feeding the identical relative transition sequence
    // through encoder0's pins (6/7, unmodified/trusted) and through
    // encoder1's pins (9/10, the pins this fix touches) must produce
    // identical results at every step, regardless of the direction-decode
    // table's specifics.
    const uint8_t transitions[] = {0b01, 0b11, 0b10, 0b00};  // one full detent

    RotaryEncoder viaEncoder0(nullptr);
    DIRECTION_TYPE resultsFrom0[4];
    uint16_t bMap0 = 0;
    for (int i = 0; i < 4; i++) {
        bMap0 = (uint16_t)((bMap0 & ~(uint16_t)(0b11 << 6)) | ((uint16_t)transitions[i] << 6));
        viaEncoder0.ms_handler(bMap0);
        resultsFrom0[i] = viaEncoder0.getDirection(0);
    }

    RotaryEncoder viaEncoder1(nullptr);
    uint16_t bMap1 = 0;
    for (int i = 0; i < 4; i++) {
        bMap1 = (uint16_t)((bMap1 & ~(uint16_t)(0b11 << 9)) | ((uint16_t)transitions[i] << 9));
        viaEncoder1.ms_handler(bMap1);
        DIRECTION_TYPE resultFrom1 = viaEncoder1.getDirection(1);
        CHECK_TRUE(resultFrom1 == resultsFrom0[i]);
    }
}

TEST(RotaryEncoder, Encoder0And2PinsAreUnaffectedByThisFix) {
    // encoder0 (pins 6/7) and encoder2 (pins 12/13) were already correct --
    // this just confirms the fix to encoder1 didn't disturb them.
    RotaryEncoder enc(nullptr);
    uint16_t bMap = 0;
    enc.ms_handler(bMap);
    for (int i = 0; i < 6; i++) {
        bMap ^= (1 << 8);  // rot0's push button pin, not encoder0's signal pins
        enc.ms_handler(bMap);
        CHECK_TRUE(enc.getDirection(0) == not_supported);
        CHECK_TRUE(enc.getDirection(2) == not_supported);
    }
}
