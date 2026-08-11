#include "mini_test.h"
#include "Timer2Config.h"

TEST(Timer2InterruptMask, EnablesOnlyCompareA) {
    uint8_t mask = timer2InterruptMask();
    CHECK_EQ((uint8_t)(mask & (1 << TIMER2_OCIE2A_BIT)), (uint8_t)(1 << TIMER2_OCIE2A_BIT));
    CHECK_EQ((uint8_t)(mask & (1 << TIMER2_OCIE2B_BIT)), (uint8_t)0);
    CHECK_EQ((uint8_t)(mask & (1 << TIMER2_TOIE2_BIT)), (uint8_t)0);
}

TEST(Timer2InterruptMask, IsExactlyOneBit) {
    // Compare-B and Overflow have no ISR in main.cpp -- if either bit were
    // set here, that vector would fire with nothing handling it.
    CHECK_EQ(timer2InterruptMask(), (uint8_t)0x02);
}
