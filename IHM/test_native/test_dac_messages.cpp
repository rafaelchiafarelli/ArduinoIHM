#include "mini_test.h"
#include "mavlink.h"

// IHM_DAC_COMMAND (315) / IHM_DAC_STATE (316): the PC sets one DAC's 12-bit
// code, the board reports both codes and which DACs answered on I2C. These
// check the generated pack/decode pairs keep every field, including the
// full-scale code.
TEST(DacMessages, IdsAre315And316) {
    mavlink_message_t msg;
    mavlink_msg_ihm_dac_command_pack(255, 0, &msg, 0, 0);
    CHECK_EQ((uint32_t)msg.msgid, (uint32_t)315);
    const uint16_t v[2] = {0, 0};
    mavlink_msg_ihm_dac_state_pack(1, 1, &msg, v, 0);
    CHECK_EQ((uint32_t)msg.msgid, (uint32_t)316);
}

TEST(DacMessages, CommandRoundTripsFullScale) {
    mavlink_message_t msg;
    mavlink_msg_ihm_dac_command_pack(255, 0, &msg, 1, 4095);
    mavlink_ihm_dac_command_t c;
    mavlink_msg_ihm_dac_command_decode(&msg, &c);
    CHECK_EQ((int)c.channel, 1);
    CHECK_EQ((int)c.value, 4095);
}

TEST(DacMessages, StateRoundTripsBothCodesAndPresentMask) {
    mavlink_message_t msg;
    const uint16_t v[2] = {1234, 4095};
    mavlink_msg_ihm_dac_state_pack(1, 1, &msg, v, 0x02);
    mavlink_ihm_dac_state_t s;
    mavlink_msg_ihm_dac_state_decode(&msg, &s);
    CHECK_EQ((int)s.value[0], 1234);
    CHECK_EQ((int)s.value[1], 4095);
    CHECK_EQ((int)s.present, 0x02);
}
