#include "ButtonMap.h"

uint8_t buildButtonMap(uint16_t bMap) {
    uint8_t btnMap = 0x80;
    btnMap |= 0x0f & bMap;

    uint16_t rot2btn = (0b0100000000000000 & bMap) >> 14;  // PC7
    btnMap |= ((uint8_t)rot2btn) << 4;

    uint16_t rot1btn = (0b0000100000000000 & bMap) >> 11;  // PL0
    btnMap |= ((uint8_t)rot1btn) << 5;

    uint16_t rot0btn = (0b0000000100000000 & bMap) >> 8;  // PL4
    btnMap |= ((uint8_t)rot0btn) << 6;

    return btnMap;
}
