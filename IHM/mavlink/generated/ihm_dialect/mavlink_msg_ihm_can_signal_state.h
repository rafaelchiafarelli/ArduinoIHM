#pragma once
// MESSAGE IHM_CAN_SIGNAL_STATE PACKING

#define MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE 311


typedef struct __mavlink_ihm_can_signal_state_t {
 uint32_t can_id; /*<  CAN identifier (standard or extended, see extended_id).*/
 uint16_t period_ms; /*<  Repeat interval in ms. 0 = send once.*/
 uint16_t repeat_count; /*<  Number of times to send. 0 = until disabled.*/
 uint8_t bus_id; /*<  CAN bus this snapshot describes: 0 or 1.*/
 uint8_t extended_id; /*<  0 = standard 11-bit ID, 1 = extended 29-bit ID.*/
 uint8_t dlc; /*<  Data length, 0-8.*/
 uint8_t data[8]; /*<  Frame payload bytes; only the first dlc bytes are used.*/
 uint8_t enable; /*<  1 = enabled, 0 = disabled.*/
} mavlink_ihm_can_signal_state_t;

#define MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN 20
#define MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_MIN_LEN 20
#define MAVLINK_MSG_ID_311_LEN 20
#define MAVLINK_MSG_ID_311_MIN_LEN 20

#define MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_CRC 31
#define MAVLINK_MSG_ID_311_CRC 31

#define MAVLINK_MSG_IHM_CAN_SIGNAL_STATE_FIELD_DATA_LEN 8

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IHM_CAN_SIGNAL_STATE { \
    311, \
    "IHM_CAN_SIGNAL_STATE", \
    8, \
    {  { "bus_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_ihm_can_signal_state_t, bus_id) }, \
         { "can_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ihm_can_signal_state_t, can_id) }, \
         { "extended_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_ihm_can_signal_state_t, extended_id) }, \
         { "dlc", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_ihm_can_signal_state_t, dlc) }, \
         { "data", NULL, MAVLINK_TYPE_UINT8_T, 8, 11, offsetof(mavlink_ihm_can_signal_state_t, data) }, \
         { "period_ms", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_ihm_can_signal_state_t, period_ms) }, \
         { "repeat_count", NULL, MAVLINK_TYPE_UINT16_T, 0, 6, offsetof(mavlink_ihm_can_signal_state_t, repeat_count) }, \
         { "enable", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_ihm_can_signal_state_t, enable) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IHM_CAN_SIGNAL_STATE { \
    "IHM_CAN_SIGNAL_STATE", \
    8, \
    {  { "bus_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_ihm_can_signal_state_t, bus_id) }, \
         { "can_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ihm_can_signal_state_t, can_id) }, \
         { "extended_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_ihm_can_signal_state_t, extended_id) }, \
         { "dlc", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_ihm_can_signal_state_t, dlc) }, \
         { "data", NULL, MAVLINK_TYPE_UINT8_T, 8, 11, offsetof(mavlink_ihm_can_signal_state_t, data) }, \
         { "period_ms", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_ihm_can_signal_state_t, period_ms) }, \
         { "repeat_count", NULL, MAVLINK_TYPE_UINT16_T, 0, 6, offsetof(mavlink_ihm_can_signal_state_t, repeat_count) }, \
         { "enable", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_ihm_can_signal_state_t, enable) }, \
         } \
}
#endif

/**
 * @brief Pack a ihm_can_signal_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param bus_id  CAN bus this snapshot describes: 0 or 1.
 * @param can_id  CAN identifier (standard or extended, see extended_id).
 * @param extended_id  0 = standard 11-bit ID, 1 = extended 29-bit ID.
 * @param dlc  Data length, 0-8.
 * @param data  Frame payload bytes; only the first dlc bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once.
 * @param repeat_count  Number of times to send. 0 = until disabled.
 * @param enable  1 = enabled, 0 = disabled.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_can_signal_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t bus_id, uint32_t can_id, uint8_t extended_id, uint8_t dlc, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN];
    _mav_put_uint32_t(buf, 0, can_id);
    _mav_put_uint16_t(buf, 4, period_ms);
    _mav_put_uint16_t(buf, 6, repeat_count);
    _mav_put_uint8_t(buf, 8, bus_id);
    _mav_put_uint8_t(buf, 9, extended_id);
    _mav_put_uint8_t(buf, 10, dlc);
    _mav_put_uint8_t(buf, 19, enable);
    _mav_put_uint8_t_array(buf, 11, data, 8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN);
#else
    mavlink_ihm_can_signal_state_t packet;
    packet.can_id = can_id;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.bus_id = bus_id;
    packet.extended_id = extended_id;
    packet.dlc = dlc;
    packet.enable = enable;
    mav_array_assign_uint8_t(packet.data, data, 8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_CRC);
}

/**
 * @brief Pack a ihm_can_signal_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param bus_id  CAN bus this snapshot describes: 0 or 1.
 * @param can_id  CAN identifier (standard or extended, see extended_id).
 * @param extended_id  0 = standard 11-bit ID, 1 = extended 29-bit ID.
 * @param dlc  Data length, 0-8.
 * @param data  Frame payload bytes; only the first dlc bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once.
 * @param repeat_count  Number of times to send. 0 = until disabled.
 * @param enable  1 = enabled, 0 = disabled.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_can_signal_state_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t bus_id, uint32_t can_id, uint8_t extended_id, uint8_t dlc, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN];
    _mav_put_uint32_t(buf, 0, can_id);
    _mav_put_uint16_t(buf, 4, period_ms);
    _mav_put_uint16_t(buf, 6, repeat_count);
    _mav_put_uint8_t(buf, 8, bus_id);
    _mav_put_uint8_t(buf, 9, extended_id);
    _mav_put_uint8_t(buf, 10, dlc);
    _mav_put_uint8_t(buf, 19, enable);
    _mav_put_uint8_t_array(buf, 11, data, 8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN);
#else
    mavlink_ihm_can_signal_state_t packet;
    packet.can_id = can_id;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.bus_id = bus_id;
    packet.extended_id = extended_id;
    packet.dlc = dlc;
    packet.enable = enable;
    mav_array_memcpy(packet.data, data, sizeof(uint8_t)*8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN);
#endif
}

/**
 * @brief Pack a ihm_can_signal_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param bus_id  CAN bus this snapshot describes: 0 or 1.
 * @param can_id  CAN identifier (standard or extended, see extended_id).
 * @param extended_id  0 = standard 11-bit ID, 1 = extended 29-bit ID.
 * @param dlc  Data length, 0-8.
 * @param data  Frame payload bytes; only the first dlc bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once.
 * @param repeat_count  Number of times to send. 0 = until disabled.
 * @param enable  1 = enabled, 0 = disabled.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_can_signal_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t bus_id,uint32_t can_id,uint8_t extended_id,uint8_t dlc,const uint8_t *data,uint16_t period_ms,uint16_t repeat_count,uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN];
    _mav_put_uint32_t(buf, 0, can_id);
    _mav_put_uint16_t(buf, 4, period_ms);
    _mav_put_uint16_t(buf, 6, repeat_count);
    _mav_put_uint8_t(buf, 8, bus_id);
    _mav_put_uint8_t(buf, 9, extended_id);
    _mav_put_uint8_t(buf, 10, dlc);
    _mav_put_uint8_t(buf, 19, enable);
    _mav_put_uint8_t_array(buf, 11, data, 8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN);
#else
    mavlink_ihm_can_signal_state_t packet;
    packet.can_id = can_id;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.bus_id = bus_id;
    packet.extended_id = extended_id;
    packet.dlc = dlc;
    packet.enable = enable;
    mav_array_assign_uint8_t(packet.data, data, 8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_CRC);
}

/**
 * @brief Encode a ihm_can_signal_state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ihm_can_signal_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_can_signal_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ihm_can_signal_state_t* ihm_can_signal_state)
{
    return mavlink_msg_ihm_can_signal_state_pack(system_id, component_id, msg, ihm_can_signal_state->bus_id, ihm_can_signal_state->can_id, ihm_can_signal_state->extended_id, ihm_can_signal_state->dlc, ihm_can_signal_state->data, ihm_can_signal_state->period_ms, ihm_can_signal_state->repeat_count, ihm_can_signal_state->enable);
}

/**
 * @brief Encode a ihm_can_signal_state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ihm_can_signal_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_can_signal_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ihm_can_signal_state_t* ihm_can_signal_state)
{
    return mavlink_msg_ihm_can_signal_state_pack_chan(system_id, component_id, chan, msg, ihm_can_signal_state->bus_id, ihm_can_signal_state->can_id, ihm_can_signal_state->extended_id, ihm_can_signal_state->dlc, ihm_can_signal_state->data, ihm_can_signal_state->period_ms, ihm_can_signal_state->repeat_count, ihm_can_signal_state->enable);
}

/**
 * @brief Encode a ihm_can_signal_state struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ihm_can_signal_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_can_signal_state_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ihm_can_signal_state_t* ihm_can_signal_state)
{
    return mavlink_msg_ihm_can_signal_state_pack_status(system_id, component_id, _status, msg,  ihm_can_signal_state->bus_id, ihm_can_signal_state->can_id, ihm_can_signal_state->extended_id, ihm_can_signal_state->dlc, ihm_can_signal_state->data, ihm_can_signal_state->period_ms, ihm_can_signal_state->repeat_count, ihm_can_signal_state->enable);
}

/**
 * @brief Send a ihm_can_signal_state message
 * @param chan MAVLink channel to send the message
 *
 * @param bus_id  CAN bus this snapshot describes: 0 or 1.
 * @param can_id  CAN identifier (standard or extended, see extended_id).
 * @param extended_id  0 = standard 11-bit ID, 1 = extended 29-bit ID.
 * @param dlc  Data length, 0-8.
 * @param data  Frame payload bytes; only the first dlc bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once.
 * @param repeat_count  Number of times to send. 0 = until disabled.
 * @param enable  1 = enabled, 0 = disabled.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ihm_can_signal_state_send(mavlink_channel_t chan, uint8_t bus_id, uint32_t can_id, uint8_t extended_id, uint8_t dlc, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN];
    _mav_put_uint32_t(buf, 0, can_id);
    _mav_put_uint16_t(buf, 4, period_ms);
    _mav_put_uint16_t(buf, 6, repeat_count);
    _mav_put_uint8_t(buf, 8, bus_id);
    _mav_put_uint8_t(buf, 9, extended_id);
    _mav_put_uint8_t(buf, 10, dlc);
    _mav_put_uint8_t(buf, 19, enable);
    _mav_put_uint8_t_array(buf, 11, data, 8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE, buf, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_CRC);
#else
    mavlink_ihm_can_signal_state_t packet;
    packet.can_id = can_id;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.bus_id = bus_id;
    packet.extended_id = extended_id;
    packet.dlc = dlc;
    packet.enable = enable;
    mav_array_assign_uint8_t(packet.data, data, 8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE, (const char *)&packet, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_CRC);
#endif
}

/**
 * @brief Send a ihm_can_signal_state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ihm_can_signal_state_send_struct(mavlink_channel_t chan, const mavlink_ihm_can_signal_state_t* ihm_can_signal_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ihm_can_signal_state_send(chan, ihm_can_signal_state->bus_id, ihm_can_signal_state->can_id, ihm_can_signal_state->extended_id, ihm_can_signal_state->dlc, ihm_can_signal_state->data, ihm_can_signal_state->period_ms, ihm_can_signal_state->repeat_count, ihm_can_signal_state->enable);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE, (const char *)ihm_can_signal_state, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ihm_can_signal_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t bus_id, uint32_t can_id, uint8_t extended_id, uint8_t dlc, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, can_id);
    _mav_put_uint16_t(buf, 4, period_ms);
    _mav_put_uint16_t(buf, 6, repeat_count);
    _mav_put_uint8_t(buf, 8, bus_id);
    _mav_put_uint8_t(buf, 9, extended_id);
    _mav_put_uint8_t(buf, 10, dlc);
    _mav_put_uint8_t(buf, 19, enable);
    _mav_put_uint8_t_array(buf, 11, data, 8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE, buf, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_CRC);
#else
    mavlink_ihm_can_signal_state_t *packet = (mavlink_ihm_can_signal_state_t *)msgbuf;
    packet->can_id = can_id;
    packet->period_ms = period_ms;
    packet->repeat_count = repeat_count;
    packet->bus_id = bus_id;
    packet->extended_id = extended_id;
    packet->dlc = dlc;
    packet->enable = enable;
    mav_array_assign_uint8_t(packet->data, data, 8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE, (const char *)packet, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE IHM_CAN_SIGNAL_STATE UNPACKING


/**
 * @brief Get field bus_id from ihm_can_signal_state message
 *
 * @return  CAN bus this snapshot describes: 0 or 1.
 */
static inline uint8_t mavlink_msg_ihm_can_signal_state_get_bus_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field can_id from ihm_can_signal_state message
 *
 * @return  CAN identifier (standard or extended, see extended_id).
 */
static inline uint32_t mavlink_msg_ihm_can_signal_state_get_can_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field extended_id from ihm_can_signal_state message
 *
 * @return  0 = standard 11-bit ID, 1 = extended 29-bit ID.
 */
static inline uint8_t mavlink_msg_ihm_can_signal_state_get_extended_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field dlc from ihm_can_signal_state message
 *
 * @return  Data length, 0-8.
 */
static inline uint8_t mavlink_msg_ihm_can_signal_state_get_dlc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Get field data from ihm_can_signal_state message
 *
 * @return  Frame payload bytes; only the first dlc bytes are used.
 */
static inline uint16_t mavlink_msg_ihm_can_signal_state_get_data(const mavlink_message_t* msg, uint8_t *data)
{
    return _MAV_RETURN_uint8_t_array(msg, data, 8,  11);
}

/**
 * @brief Get field period_ms from ihm_can_signal_state message
 *
 * @return  Repeat interval in ms. 0 = send once.
 */
static inline uint16_t mavlink_msg_ihm_can_signal_state_get_period_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  4);
}

/**
 * @brief Get field repeat_count from ihm_can_signal_state message
 *
 * @return  Number of times to send. 0 = until disabled.
 */
static inline uint16_t mavlink_msg_ihm_can_signal_state_get_repeat_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  6);
}

/**
 * @brief Get field enable from ihm_can_signal_state message
 *
 * @return  1 = enabled, 0 = disabled.
 */
static inline uint8_t mavlink_msg_ihm_can_signal_state_get_enable(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  19);
}

/**
 * @brief Decode a ihm_can_signal_state message into a struct
 *
 * @param msg The message to decode
 * @param ihm_can_signal_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_ihm_can_signal_state_decode(const mavlink_message_t* msg, mavlink_ihm_can_signal_state_t* ihm_can_signal_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ihm_can_signal_state->can_id = mavlink_msg_ihm_can_signal_state_get_can_id(msg);
    ihm_can_signal_state->period_ms = mavlink_msg_ihm_can_signal_state_get_period_ms(msg);
    ihm_can_signal_state->repeat_count = mavlink_msg_ihm_can_signal_state_get_repeat_count(msg);
    ihm_can_signal_state->bus_id = mavlink_msg_ihm_can_signal_state_get_bus_id(msg);
    ihm_can_signal_state->extended_id = mavlink_msg_ihm_can_signal_state_get_extended_id(msg);
    ihm_can_signal_state->dlc = mavlink_msg_ihm_can_signal_state_get_dlc(msg);
    mavlink_msg_ihm_can_signal_state_get_data(msg, ihm_can_signal_state->data);
    ihm_can_signal_state->enable = mavlink_msg_ihm_can_signal_state_get_enable(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN? msg->len : MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN;
        memset(ihm_can_signal_state, 0, MAVLINK_MSG_ID_IHM_CAN_SIGNAL_STATE_LEN);
    memcpy(ihm_can_signal_state, _MAV_PAYLOAD(msg), len);
#endif
}
