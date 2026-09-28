#include "mini_test.h"
#include "SerialEdit.h"

// RE2 editing of the SERIAL tab (serial_config / board_editing task 2).

namespace {
struct Fixture {
    SerialConfig c;
    uint8_t idx[SERIAL_BUS_COUNT];
    DigitAccel a;
    uint32_t t;
    Fixture() : idx{0, 0, 0}, t(10000) {
        serialConfigDefaults(c);
        digitAccelReset(a);
    }
    // One click, `gap` ms after the previous one.
    uint8_t click(uint8_t bus, SerialField f, bool cw, uint32_t gap = 500) {
        t += gap;
        return serialEditStep(c, idx, bus, f, cw, a, t);
    }
};
}  // namespace

TEST(SerialEdit, EnableIsSetNeverFlippedAndReportsTheToggle) {
    Fixture x;
    CHECK_EQ((int)x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_ENABLE, true), SERIAL_EDIT_CHANGED | SERIAL_EDIT_ENABLE_TOGGLED);
    CHECK_EQ((int)x.c.can[0].gen.enable, 1);
    CHECK_EQ((int)x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_ENABLE, true), 0);   // already on
    CHECK_EQ((int)x.click(SERIAL_BUS_RS485, SERIAL_FIELD_ENABLE, false), 0); // already off
}

TEST(SerialEdit, PressFlipsEnable) {
    Fixture x;
    CHECK_EQ((int)serialToggleEnable(x.c, SERIAL_BUS_RS485), SERIAL_EDIT_CHANGED | SERIAL_EDIT_ENABLE_TOGGLED);
    CHECK_EQ((int)x.c.rs485.gen.enable, 1);
    serialToggleEnable(x.c, SERIAL_BUS_RS485);
    CHECK_EQ((int)x.c.rs485.gen.enable, 0);
    CHECK_EQ((int)serialToggleEnable(x.c, 3), 0);
}

TEST(SerialEdit, StandardIdAcceleratesToThe0x100DigitAndClamps) {
    Fixture x;
    for (int i = 0; i < 40; i++) x.click(SERIAL_BUS_CAN1, SERIAL_FIELD_ID, true, 40);
    // 1 x4 + 16 x4 (level 1 reached after 4 fast gaps) then 256s: clamps at 0x7FF.
    CHECK_EQ((uint32_t)x.c.can[1].gen.id, (uint32_t)0x7FF);
    CHECK_EQ((int)x.click(SERIAL_BUS_CAN1, SERIAL_FIELD_ID, true, 40), 0);   // at the limit
}

TEST(SerialEdit, ExtendedIdReachesItsTopDigit) {
    Fixture x;
    x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_EXTENDED, true);
    CHECK_EQ((int)x.c.can[0].gen.extended, 1);
    for (int i = 0; i < 40; i++) x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_ID, true, 40);
    CHECK_EQ((uint32_t)x.c.can[0].gen.id, (uint32_t)SERIAL_CAN_EXT_ID_MAX);
    x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_EXTENDED, false);
    CHECK_EQ((uint32_t)x.c.can[0].gen.id, (uint32_t)0x7FF);   // clamped back
}

TEST(SerialEdit, SlowIdClicksStepOneAndStopAtZero) {
    Fixture x;
    x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_ID, true);
    x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_ID, true);
    CHECK_EQ((uint32_t)x.c.can[0].gen.id, (uint32_t)2);
    x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_ID, false);
    x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_ID, false);
    CHECK_EQ((int)x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_ID, false), 0);
    CHECK_EQ((uint32_t)x.c.can[0].gen.id, (uint32_t)0);
}

TEST(SerialEdit, PeriodStepsIn10MsUnitsUpTo65530) {
    Fixture x;
    x.click(SERIAL_BUS_RS485, SERIAL_FIELD_PERIOD, true);
    CHECK_EQ((int)x.c.rs485.gen.period_ms, 110);
    for (int i = 0; i < 80; i++) x.click(SERIAL_BUS_RS485, SERIAL_FIELD_PERIOD, true, 40);
    CHECK_EQ((int)x.c.rs485.gen.period_ms, 65530);
    for (int i = 0; i < 80; i++) x.click(SERIAL_BUS_RS485, SERIAL_FIELD_PERIOD, false, 40);
    CHECK_EQ((int)x.c.rs485.gen.period_ms, 0);
}

TEST(SerialEdit, RepeatGoesFromZeroUpAndBack) {
    Fixture x;
    x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_REPEAT, true);
    CHECK_EQ((int)x.c.can[0].gen.repeat_count, 1);
    x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_REPEAT, false);
    CHECK_EQ((int)x.c.can[0].gen.repeat_count, 0);   // 0 = until disabled
    for (int i = 0; i < 80; i++) x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_REPEAT, true, 40);
    CHECK_EQ((int)x.c.can[0].gen.repeat_count, 65535);
}

TEST(SerialEdit, DlcShrinkPullsTheByteIndexIn) {
    Fixture x;
    for (int i = 0; i < 10; i++) x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_BYTE_INDEX, true);
    CHECK_EQ((int)x.idx[0], 7);   // DLC 8: 0..7
    for (int i = 0; i < 5; i++) x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_DLC, false);
    CHECK_EQ((int)x.c.can[0].gen.dlc, 3);
    CHECK_EQ((int)x.idx[0], 2);
}

TEST(SerialEdit, ByteValueEditsTheIndexedByteOnly) {
    Fixture x;
    x.click(SERIAL_BUS_CAN1, SERIAL_FIELD_BYTE_INDEX, true);   // B1
    digitAccelReset(x.a);   // focus moved to the value field, as main.cpp does
    for (int i = 0; i < 9; i++) x.click(SERIAL_BUS_CAN1, SERIAL_FIELD_BYTE_VALUE, true, 40);
    // 1 x4, then x16 x4 after 4 fast gaps, then capped at x16: 4 + 16*5 = 84.
    CHECK_EQ((int)x.c.can[1].gen.data[1], 4 + 16 * 5);
    CHECK_EQ((int)x.c.can[1].gen.data[0], 0);
    for (int i = 0; i < 40; i++) x.click(SERIAL_BUS_CAN1, SERIAL_FIELD_BYTE_VALUE, true, 40);
    CHECK_EQ((int)x.c.can[1].gen.data[1], 0xFF);
}

TEST(SerialEdit, Rs485LengthAndZeroLengthByteFields) {
    Fixture x;
    for (int i = 0; i < 10; i++) x.click(SERIAL_BUS_RS485, SERIAL_FIELD_LENGTH, false);
    CHECK_EQ((int)x.c.rs485.gen.length, 0);
    CHECK_EQ((int)x.click(SERIAL_BUS_RS485, SERIAL_FIELD_BYTE_INDEX, true), 0);
    CHECK_EQ((int)x.click(SERIAL_BUS_RS485, SERIAL_FIELD_BYTE_VALUE, true), 0);
    for (int i = 0; i < 40; i++) x.click(SERIAL_BUS_RS485, SERIAL_FIELD_LENGTH, true, 40);
    CHECK_EQ((int)x.c.rs485.gen.length, 32);
}

TEST(SerialEdit, FieldsFromTheOtherBusKindDoNothing) {
    Fixture x;
    CHECK_EQ((int)x.click(SERIAL_BUS_RS485, SERIAL_FIELD_ID, true), 0);
    CHECK_EQ((int)x.click(SERIAL_BUS_RS485, SERIAL_FIELD_DLC, true), 0);
    CHECK_EQ((int)x.click(SERIAL_BUS_RS485, SERIAL_FIELD_EXTENDED, true), 0);
    CHECK_EQ((int)x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_LENGTH, true), 0);
    CHECK_EQ((int)x.click(3, SERIAL_FIELD_ENABLE, true), 0);
}

TEST(SerialEdit, EveryEditKeepsTheConfigValid) {
    Fixture x;
    for (int i = 0; i < 30; i++) {
        x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_ID, true, 40);
        x.click(SERIAL_BUS_CAN0, SERIAL_FIELD_DLC, true, 40);
        x.click(SERIAL_BUS_RS485, SERIAL_FIELD_LENGTH, true, 40);
    }
    CHECK_TRUE(canGeneratorValid(x.c.can[0].gen));
    CHECK_TRUE(rs485GeneratorValid(x.c.rs485.gen));
}
