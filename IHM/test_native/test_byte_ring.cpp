#include "mini_test.h"
#include "ByteRing.h"

TEST(ByteRing, StartsEmpty) {
    ByteRing<8> r;
    uint8_t c;
    CHECK_FALSE(r.pop(&c));
    CHECK_EQ(r.used(), (uint8_t)0);
    CHECK_EQ(r.space(), (uint8_t)7);
}

TEST(ByteRing, PopsInPushOrder) {
    ByteRing<8> r;
    CHECK_TRUE(r.push(1));
    CHECK_TRUE(r.push(2));
    CHECK_TRUE(r.push(3));
    uint8_t c;
    CHECK_TRUE(r.pop(&c)); CHECK_EQ(c, (uint8_t)1);
    CHECK_TRUE(r.pop(&c)); CHECK_EQ(c, (uint8_t)2);
    CHECK_TRUE(r.pop(&c)); CHECK_EQ(c, (uint8_t)3);
    CHECK_FALSE(r.pop(&c));
}

TEST(ByteRing, CapacityIsNMinusOne) {
    ByteRing<8> r;
    for (uint8_t i = 0; i < 7; i++)
        CHECK_TRUE(r.push(i));
    CHECK_FALSE(r.push(99));
    CHECK_EQ(r.used(), (uint8_t)7);
    CHECK_EQ(r.space(), (uint8_t)0);
}

TEST(ByteRing, WrapsAround) {
    ByteRing<4> r;
    uint8_t c;
    for (uint8_t round = 0; round < 10; round++) {
        CHECK_TRUE(r.push(round));
        CHECK_TRUE(r.push((uint8_t)(round + 100)));
        CHECK_TRUE(r.pop(&c)); CHECK_EQ(c, round);
        CHECK_TRUE(r.pop(&c)); CHECK_EQ(c, (uint8_t)(round + 100));
    }
    CHECK_EQ(r.used(), (uint8_t)0);
}

TEST(ByteRing, WriteIsAllOrNothing) {
    ByteRing<8> r;
    const uint8_t frame[5] = {10, 11, 12, 13, 14};
    CHECK_TRUE(r.write(frame, 5));
    CHECK_EQ(r.used(), (uint8_t)5);
    // 2 free slots left -- a 5-byte frame must not be partially enqueued.
    CHECK_FALSE(r.write(frame, 5));
    CHECK_EQ(r.used(), (uint8_t)5);
    uint8_t c;
    for (uint8_t i = 0; i < 5; i++) {
        CHECK_TRUE(r.pop(&c));
        CHECK_EQ(c, frame[i]);
    }
    CHECK_FALSE(r.pop(&c));
}

TEST(ByteRing, WriteWrapsAround) {
    ByteRing<8> r;
    uint8_t c;
    for (uint8_t i = 0; i < 6; i++) { r.push(0); r.pop(&c); } // move indices near the end
    const uint8_t frame[6] = {1, 2, 3, 4, 5, 6};
    CHECK_TRUE(r.write(frame, 6));
    for (uint8_t i = 0; i < 6; i++) {
        CHECK_TRUE(r.pop(&c));
        CHECK_EQ(c, frame[i]);
    }
}

TEST(ByteRing, OversizedWriteRejected) {
    ByteRing<8> r;
    uint8_t big[300] = {0};
    CHECK_FALSE(r.write(big, 300)); // len > 255 must not wrap the size check
    CHECK_EQ(r.used(), (uint8_t)0);
}
