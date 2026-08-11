#include "Timer2Config.h"

uint8_t timer2InterruptMask() {
    return (uint8_t)(1 << TIMER2_OCIE2A_BIT);
}
