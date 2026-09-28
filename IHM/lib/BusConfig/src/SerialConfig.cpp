#include "SerialConfig.h"
#include <string.h>

void serialConfigDefaults(SerialConfig& c)
{
    memset(&c, 0, sizeof(c));
    for (uint8_t b = 0; b < SERIAL_CAN_BUSES; b++) {
        c.can[b].gen.dlc = SERIAL_CAN_DATA_MAX;
        c.can[b].gen.period_ms = 100;
    }
    c.rs485.gen.length = 8;
    c.rs485.gen.period_ms = 100;
}

uint32_t canIdMax(uint8_t extended)
{
    return extended ? SERIAL_CAN_EXT_ID_MAX : SERIAL_CAN_STD_ID_MAX;
}

bool canGeneratorValid(const CanGeneratorConfig& g)
{
    return g.enable <= 1 && g.extended <= 1 && g.dlc <= SERIAL_CAN_DATA_MAX &&
           g.id <= canIdMax(g.extended);
}

bool rs485GeneratorValid(const Rs485GeneratorConfig& g)
{
    return g.enable <= 1 && g.length <= SERIAL_RS485_DATA_MAX;
}

bool canSetExtended(CanGeneratorConfig& g, bool on)
{
    uint8_t want = on ? 1 : 0;
    if (g.extended == want)
        return false;
    g.extended = want;
    if (g.id > canIdMax(want))
        g.id = canIdMax(want);
    return true;
}

uint32_t serialStepValue(uint32_t v, int32_t delta, uint32_t max)
{
    if (delta > 0) {
        if (v >= max)
            return v;
        uint32_t room = max - v;
        return (uint32_t)delta >= room ? max : v + (uint32_t)delta;
    }
    if (delta < 0) {
        uint32_t down = (uint32_t)(-(int64_t)delta);
        return down >= v ? 0 : v - down;
    }
    return v;
}

uint8_t serialStepEnum(uint8_t v, int8_t steps, uint8_t count, bool wrap)
{
    if (count == 0)
        return 0;
    int16_t i = (int16_t)(v < count ? v : count - 1) + steps;
    if (wrap) {
        i %= count;
        if (i < 0)
            i += count;
    } else {
        if (i < 0)
            i = 0;
        if (i > count - 1)
            i = count - 1;
    }
    return (uint8_t)i;
}

bool serialSetFlag(uint8_t& flag, bool on)
{
    uint8_t want = on ? 1 : 0;
    if (flag == want)
        return false;
    flag = want;
    return true;
}

void digitAccelReset(DigitAccel& a)
{
    a.lastMs = 0;
    a.level = 0;
    a.fastRun = 0;
    a.active = 0;
}

uint32_t digitAccelStep(DigitAccel& a, uint32_t nowMs, uint8_t base, uint8_t maxLevel)
{
    if (!a.active) {
        a.active = 1;
        a.level = 0;
        a.fastRun = 0;
    } else {
        uint32_t gap = nowMs - a.lastMs;
        if (gap >= DIGIT_ACCEL_RESET_MS) {
            a.level = 0;
            a.fastRun = 0;
        } else if (gap >= DIGIT_ACCEL_DROP_MS) {
            if (a.level > 0)
                a.level--;
            a.fastRun = 0;
        } else if (gap < DIGIT_ACCEL_FAST_MS) {
            if (++a.fastRun >= DIGIT_ACCEL_FAST_RUN) {
                a.fastRun = 0;
                if (a.level < maxLevel)
                    a.level++;
            }
        } else {
            a.fastRun = 0;
        }
    }
    if (a.level > maxLevel)
        a.level = maxLevel;
    a.lastMs = nowMs;

    uint32_t step = 1;
    for (uint8_t i = 0; i < a.level; i++)
        step *= base;
    return step;
}
