#include "mini_test.h"
#include "mavlink.h"
#include <string.h>

// serial_config / wire task 1: the SERIAL messages' ids and pack/decode
// round trips -- the generator readbacks (311/312) and the generic setting
// pair (313/314), signed values included.

TEST(SerialMessages, IdsAre311To314) {
    CHECK_EQ((int)MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE, 311);
    CHECK_EQ((int)MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE, 312);
    CHECK_EQ((int)MAVLINK_MSG_ID_IHM_SERIAL_SETTING, 313);
    CHECK_EQ((int)MAVLINK_MSG_ID_IHM_SERIAL_SETTING_STATE, 314);
}

TEST(SerialMessages, StatesFitThePayloadCap) {
    CHECK_TRUE(MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN <= 64);
    CHECK_TRUE(MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN <= 64);
}

TEST(SerialMessages, CanSignalStateRoundTrip) {
    mavlink_ihm_can_signal_state_t s;
    memset(&s, 0, sizeof(s));
    s.bus_id = 1; s.can_id = 0x1FFFFFFF; s.extended_id = 1; s.dlc = 8;
    for (int i = 0; i < 8; i++) s.data[i] = (uint8_t)(0xA0 + i);
    s.period_ms = 65530; s.repeat_count = 65535; s.enable = 1;
    mavlink_message_t msg;
    mavlink_msg_ihm_can_signal_state_encode(1, 1, &msg, &s);
    mavlink_ihm_can_signal_state_t out;
    mavlink_msg_ihm_can_signal_state_decode(&msg, &out);
    CHECK_EQ((int)out.bus_id, 1);
    CHECK_EQ((uint32_t)out.can_id, (uint32_t)0x1FFFFFFF);
    CHECK_EQ((int)out.data[7], 0xA7);
    CHECK_EQ((int)out.period_ms, 65530);
    CHECK_EQ((int)out.repeat_count, 65535);
    CHECK_EQ((int)out.enable, 1);
}

TEST(SerialMessages, Rs485SignalStateRoundTrip) {
    mavlink_ihm_rs485_signal_state_t s;
    memset(&s, 0, sizeof(s));
    s.length = 32; s.data[31] = 0x55; s.period_ms = 10; s.repeat_count = 3; s.enable = 1;
    mavlink_message_t msg;
    mavlink_msg_ihm_rs485_signal_state_encode(1, 1, &msg, &s);
    mavlink_ihm_rs485_signal_state_t out;
    mavlink_msg_ihm_rs485_signal_state_decode(&msg, &out);
    CHECK_EQ((int)out.length, 32);
    CHECK_EQ((int)out.data[31], 0x55);
    CHECK_EQ((int)out.repeat_count, 3);
}

TEST(SerialMessages, SettingPairRoundTripsSignedValues) {
    mavlink_message_t msg;
    mavlink_msg_ihm_serial_setting_pack(1, 1, &msg, IHM_SERIAL_BUS_RS485, 0x1234, -123456);
    mavlink_ihm_serial_setting_t set;
    mavlink_msg_ihm_serial_setting_decode(&msg, &set);
    CHECK_EQ((int)set.bus, (int)IHM_SERIAL_BUS_RS485);
    CHECK_EQ((int)set.key, 0x1234);
    CHECK_EQ((int)set.value, -123456);

    mavlink_msg_ihm_serial_setting_state_pack(1, 1, &msg, IHM_SERIAL_BUS_CAN1, 7, 2147483647,
                                              IHM_SERIAL_SETTING_UNKNOWN_KEY);
    mavlink_ihm_serial_setting_state_t st;
    mavlink_msg_ihm_serial_setting_state_decode(&msg, &st);
    CHECK_EQ((int)st.bus, 1);
    CHECK_EQ((int)st.key, 7);
    CHECK_EQ((long)st.value, 2147483647L);
    CHECK_EQ((int)st.status, (int)IHM_SERIAL_SETTING_UNKNOWN_KEY);
}
