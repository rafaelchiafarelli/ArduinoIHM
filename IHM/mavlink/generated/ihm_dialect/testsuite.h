/** @file
 *    @brief MAVLink comm protocol testsuite generated from ihm_dialect.xml
 *    @see https://mavlink.io/en/
 */
#pragma once
#ifndef IHM_DIALECT_TESTSUITE_H
#define IHM_DIALECT_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL

static void mavlink_test_ihm_dialect(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{

    mavlink_test_ihm_dialect(system_id, component_id, last_msg);
}
#endif




static void mavlink_test_ihm_board_state(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_IHM_BOARD_STATE >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ihm_board_state_t packet_in = {
        17235,{ 17339, 17340, 17341, 17342 },17755,41,108,175,242,53,120,187
    };
    mavlink_ihm_board_state_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.batt_voltage = packet_in.batt_voltage;
        packet1.time_statistics = packet_in.time_statistics;
        packet1.buttons = packet_in.buttons;
        packet1.encoder0_direction = packet_in.encoder0_direction;
        packet1.encoder1_direction = packet_in.encoder1_direction;
        packet1.encoder2_direction = packet_in.encoder2_direction;
        packet1.rotation = packet_in.rotation;
        packet1.charging = packet_in.charging;
        packet1.time_counter = packet_in.time_counter;
        
        mav_array_memcpy(packet1.analog_in, packet_in.analog_in, sizeof(uint16_t)*4);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ihm_board_state_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ihm_board_state_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ihm_board_state_pack(system_id, component_id, &msg , packet1.buttons , packet1.encoder0_direction , packet1.encoder1_direction , packet1.encoder2_direction , packet1.rotation , packet1.charging , packet1.batt_voltage , packet1.analog_in , packet1.time_statistics , packet1.time_counter );
    mavlink_msg_ihm_board_state_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ihm_board_state_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.buttons , packet1.encoder0_direction , packet1.encoder1_direction , packet1.encoder2_direction , packet1.rotation , packet1.charging , packet1.batt_voltage , packet1.analog_in , packet1.time_statistics , packet1.time_counter );
    mavlink_msg_ihm_board_state_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ihm_board_state_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ihm_board_state_send(MAVLINK_COMM_1 , packet1.buttons , packet1.encoder0_direction , packet1.encoder1_direction , packet1.encoder2_direction , packet1.rotation , packet1.charging , packet1.batt_voltage , packet1.analog_in , packet1.time_statistics , packet1.time_counter );
    mavlink_msg_ihm_board_state_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("IHM_BOARD_STATE") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_IHM_BOARD_STATE) != NULL);
#endif
}

static void mavlink_test_can_signal_config(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CAN_SIGNAL_CONFIG >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_can_signal_config_t packet_in = {
        963497464,17443,17547,29,96,163,{ 230, 231, 232, 233, 234, 235, 236, 237 },254
    };
    mavlink_can_signal_config_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.can_id = packet_in.can_id;
        packet1.period_ms = packet_in.period_ms;
        packet1.repeat_count = packet_in.repeat_count;
        packet1.bus_id = packet_in.bus_id;
        packet1.extended_id = packet_in.extended_id;
        packet1.dlc = packet_in.dlc;
        packet1.enable = packet_in.enable;
        
        mav_array_memcpy(packet1.data, packet_in.data, sizeof(uint8_t)*8);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CAN_SIGNAL_CONFIG_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CAN_SIGNAL_CONFIG_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_can_signal_config_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_can_signal_config_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_can_signal_config_pack(system_id, component_id, &msg , packet1.bus_id , packet1.can_id , packet1.extended_id , packet1.dlc , packet1.data , packet1.period_ms , packet1.repeat_count , packet1.enable );
    mavlink_msg_can_signal_config_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_can_signal_config_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.bus_id , packet1.can_id , packet1.extended_id , packet1.dlc , packet1.data , packet1.period_ms , packet1.repeat_count , packet1.enable );
    mavlink_msg_can_signal_config_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_can_signal_config_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_can_signal_config_send(MAVLINK_COMM_1 , packet1.bus_id , packet1.can_id , packet1.extended_id , packet1.dlc , packet1.data , packet1.period_ms , packet1.repeat_count , packet1.enable );
    mavlink_msg_can_signal_config_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("CAN_SIGNAL_CONFIG") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_CAN_SIGNAL_CONFIG) != NULL);
#endif
}

static void mavlink_test_rs485_signal_config(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_rs485_signal_config_t packet_in = {
        17235,17339,17,{ 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115 },180
    };
    mavlink_rs485_signal_config_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.period_ms = packet_in.period_ms;
        packet1.repeat_count = packet_in.repeat_count;
        packet1.length = packet_in.length;
        packet1.enable = packet_in.enable;
        
        mav_array_memcpy(packet1.data, packet_in.data, sizeof(uint8_t)*32);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rs485_signal_config_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_rs485_signal_config_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rs485_signal_config_pack(system_id, component_id, &msg , packet1.length , packet1.data , packet1.period_ms , packet1.repeat_count , packet1.enable );
    mavlink_msg_rs485_signal_config_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rs485_signal_config_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.length , packet1.data , packet1.period_ms , packet1.repeat_count , packet1.enable );
    mavlink_msg_rs485_signal_config_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_rs485_signal_config_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rs485_signal_config_send(MAVLINK_COMM_1 , packet1.length , packet1.data , packet1.period_ms , packet1.repeat_count , packet1.enable );
    mavlink_msg_rs485_signal_config_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("RS485_SIGNAL_CONFIG") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG) != NULL);
#endif
}

static void mavlink_test_ihm_dialect(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_ihm_board_state(system_id, component_id, last_msg);
    mavlink_test_can_signal_config(system_id, component_id, last_msg);
    mavlink_test_rs485_signal_config(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // IHM_DIALECT_TESTSUITE_H
