#pragma once
// MESSAGE IHM_RS485_SIGNAL_STATE PACKING

#define MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE 312


typedef struct __mavlink_ihm_rs485_signal_state_t {
 uint16_t period_ms; /*<  Repeat interval in ms. 0 = send once.*/
 uint16_t repeat_count; /*<  Number of times to send. 0 = until disabled.*/
 uint8_t length; /*<  Valid byte count in data, 0-32.*/
 uint8_t data[32]; /*<  Raw payload bytes; only the first length bytes are used.*/
 uint8_t enable; /*<  1 = enabled, 0 = disabled.*/
} mavlink_ihm_rs485_signal_state_t;

#define MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN 38
#define MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_MIN_LEN 38
#define MAVLINK_MSG_ID_312_LEN 38
#define MAVLINK_MSG_ID_312_MIN_LEN 38

#define MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_CRC 199
#define MAVLINK_MSG_ID_312_CRC 199

#define MAVLINK_MSG_IHM_RS485_SIGNAL_STATE_FIELD_DATA_LEN 32

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IHM_RS485_SIGNAL_STATE { \
    312, \
    "IHM_RS485_SIGNAL_STATE", \
    5, \
    {  { "length", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_ihm_rs485_signal_state_t, length) }, \
         { "data", NULL, MAVLINK_TYPE_UINT8_T, 32, 5, offsetof(mavlink_ihm_rs485_signal_state_t, data) }, \
         { "period_ms", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_ihm_rs485_signal_state_t, period_ms) }, \
         { "repeat_count", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_ihm_rs485_signal_state_t, repeat_count) }, \
         { "enable", NULL, MAVLINK_TYPE_UINT8_T, 0, 37, offsetof(mavlink_ihm_rs485_signal_state_t, enable) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IHM_RS485_SIGNAL_STATE { \
    "IHM_RS485_SIGNAL_STATE", \
    5, \
    {  { "length", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_ihm_rs485_signal_state_t, length) }, \
         { "data", NULL, MAVLINK_TYPE_UINT8_T, 32, 5, offsetof(mavlink_ihm_rs485_signal_state_t, data) }, \
         { "period_ms", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_ihm_rs485_signal_state_t, period_ms) }, \
         { "repeat_count", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_ihm_rs485_signal_state_t, repeat_count) }, \
         { "enable", NULL, MAVLINK_TYPE_UINT8_T, 0, 37, offsetof(mavlink_ihm_rs485_signal_state_t, enable) }, \
         } \
}
#endif

/**
 * @brief Pack a ihm_rs485_signal_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param length  Valid byte count in data, 0-32.
 * @param data  Raw payload bytes; only the first length bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once.
 * @param repeat_count  Number of times to send. 0 = until disabled.
 * @param enable  1 = enabled, 0 = disabled.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_rs485_signal_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t length, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN];
    _mav_put_uint16_t(buf, 0, period_ms);
    _mav_put_uint16_t(buf, 2, repeat_count);
    _mav_put_uint8_t(buf, 4, length);
    _mav_put_uint8_t(buf, 37, enable);
    _mav_put_uint8_t_array(buf, 5, data, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN);
#else
    mavlink_ihm_rs485_signal_state_t packet;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.length = length;
    packet.enable = enable;
    mav_array_assign_uint8_t(packet.data, data, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_CRC);
}

/**
 * @brief Pack a ihm_rs485_signal_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param length  Valid byte count in data, 0-32.
 * @param data  Raw payload bytes; only the first length bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once.
 * @param repeat_count  Number of times to send. 0 = until disabled.
 * @param enable  1 = enabled, 0 = disabled.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_rs485_signal_state_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t length, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN];
    _mav_put_uint16_t(buf, 0, period_ms);
    _mav_put_uint16_t(buf, 2, repeat_count);
    _mav_put_uint8_t(buf, 4, length);
    _mav_put_uint8_t(buf, 37, enable);
    _mav_put_uint8_t_array(buf, 5, data, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN);
#else
    mavlink_ihm_rs485_signal_state_t packet;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.length = length;
    packet.enable = enable;
    mav_array_memcpy(packet.data, data, sizeof(uint8_t)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN);
#endif
}

/**
 * @brief Pack a ihm_rs485_signal_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param length  Valid byte count in data, 0-32.
 * @param data  Raw payload bytes; only the first length bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once.
 * @param repeat_count  Number of times to send. 0 = until disabled.
 * @param enable  1 = enabled, 0 = disabled.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_rs485_signal_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t length,const uint8_t *data,uint16_t period_ms,uint16_t repeat_count,uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN];
    _mav_put_uint16_t(buf, 0, period_ms);
    _mav_put_uint16_t(buf, 2, repeat_count);
    _mav_put_uint8_t(buf, 4, length);
    _mav_put_uint8_t(buf, 37, enable);
    _mav_put_uint8_t_array(buf, 5, data, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN);
#else
    mavlink_ihm_rs485_signal_state_t packet;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.length = length;
    packet.enable = enable;
    mav_array_assign_uint8_t(packet.data, data, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_CRC);
}

/**
 * @brief Encode a ihm_rs485_signal_state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ihm_rs485_signal_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_rs485_signal_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ihm_rs485_signal_state_t* ihm_rs485_signal_state)
{
    return mavlink_msg_ihm_rs485_signal_state_pack(system_id, component_id, msg, ihm_rs485_signal_state->length, ihm_rs485_signal_state->data, ihm_rs485_signal_state->period_ms, ihm_rs485_signal_state->repeat_count, ihm_rs485_signal_state->enable);
}

/**
 * @brief Encode a ihm_rs485_signal_state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ihm_rs485_signal_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_rs485_signal_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ihm_rs485_signal_state_t* ihm_rs485_signal_state)
{
    return mavlink_msg_ihm_rs485_signal_state_pack_chan(system_id, component_id, chan, msg, ihm_rs485_signal_state->length, ihm_rs485_signal_state->data, ihm_rs485_signal_state->period_ms, ihm_rs485_signal_state->repeat_count, ihm_rs485_signal_state->enable);
}

/**
 * @brief Encode a ihm_rs485_signal_state struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ihm_rs485_signal_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_rs485_signal_state_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ihm_rs485_signal_state_t* ihm_rs485_signal_state)
{
    return mavlink_msg_ihm_rs485_signal_state_pack_status(system_id, component_id, _status, msg,  ihm_rs485_signal_state->length, ihm_rs485_signal_state->data, ihm_rs485_signal_state->period_ms, ihm_rs485_signal_state->repeat_count, ihm_rs485_signal_state->enable);
}

/**
 * @brief Send a ihm_rs485_signal_state message
 * @param chan MAVLink channel to send the message
 *
 * @param length  Valid byte count in data, 0-32.
 * @param data  Raw payload bytes; only the first length bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once.
 * @param repeat_count  Number of times to send. 0 = until disabled.
 * @param enable  1 = enabled, 0 = disabled.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ihm_rs485_signal_state_send(mavlink_channel_t chan, uint8_t length, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN];
    _mav_put_uint16_t(buf, 0, period_ms);
    _mav_put_uint16_t(buf, 2, repeat_count);
    _mav_put_uint8_t(buf, 4, length);
    _mav_put_uint8_t(buf, 37, enable);
    _mav_put_uint8_t_array(buf, 5, data, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE, buf, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_CRC);
#else
    mavlink_ihm_rs485_signal_state_t packet;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.length = length;
    packet.enable = enable;
    mav_array_assign_uint8_t(packet.data, data, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE, (const char *)&packet, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_CRC);
#endif
}

/**
 * @brief Send a ihm_rs485_signal_state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ihm_rs485_signal_state_send_struct(mavlink_channel_t chan, const mavlink_ihm_rs485_signal_state_t* ihm_rs485_signal_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ihm_rs485_signal_state_send(chan, ihm_rs485_signal_state->length, ihm_rs485_signal_state->data, ihm_rs485_signal_state->period_ms, ihm_rs485_signal_state->repeat_count, ihm_rs485_signal_state->enable);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE, (const char *)ihm_rs485_signal_state, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ihm_rs485_signal_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t length, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, period_ms);
    _mav_put_uint16_t(buf, 2, repeat_count);
    _mav_put_uint8_t(buf, 4, length);
    _mav_put_uint8_t(buf, 37, enable);
    _mav_put_uint8_t_array(buf, 5, data, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE, buf, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_CRC);
#else
    mavlink_ihm_rs485_signal_state_t *packet = (mavlink_ihm_rs485_signal_state_t *)msgbuf;
    packet->period_ms = period_ms;
    packet->repeat_count = repeat_count;
    packet->length = length;
    packet->enable = enable;
    mav_array_assign_uint8_t(packet->data, data, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE, (const char *)packet, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE IHM_RS485_SIGNAL_STATE UNPACKING


/**
 * @brief Get field length from ihm_rs485_signal_state message
 *
 * @return  Valid byte count in data, 0-32.
 */
static inline uint8_t mavlink_msg_ihm_rs485_signal_state_get_length(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field data from ihm_rs485_signal_state message
 *
 * @return  Raw payload bytes; only the first length bytes are used.
 */
static inline uint16_t mavlink_msg_ihm_rs485_signal_state_get_data(const mavlink_message_t* msg, uint8_t *data)
{
    return _MAV_RETURN_uint8_t_array(msg, data, 32,  5);
}

/**
 * @brief Get field period_ms from ihm_rs485_signal_state message
 *
 * @return  Repeat interval in ms. 0 = send once.
 */
static inline uint16_t mavlink_msg_ihm_rs485_signal_state_get_period_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field repeat_count from ihm_rs485_signal_state message
 *
 * @return  Number of times to send. 0 = until disabled.
 */
static inline uint16_t mavlink_msg_ihm_rs485_signal_state_get_repeat_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  2);
}

/**
 * @brief Get field enable from ihm_rs485_signal_state message
 *
 * @return  1 = enabled, 0 = disabled.
 */
static inline uint8_t mavlink_msg_ihm_rs485_signal_state_get_enable(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  37);
}

/**
 * @brief Decode a ihm_rs485_signal_state message into a struct
 *
 * @param msg The message to decode
 * @param ihm_rs485_signal_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_ihm_rs485_signal_state_decode(const mavlink_message_t* msg, mavlink_ihm_rs485_signal_state_t* ihm_rs485_signal_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ihm_rs485_signal_state->period_ms = mavlink_msg_ihm_rs485_signal_state_get_period_ms(msg);
    ihm_rs485_signal_state->repeat_count = mavlink_msg_ihm_rs485_signal_state_get_repeat_count(msg);
    ihm_rs485_signal_state->length = mavlink_msg_ihm_rs485_signal_state_get_length(msg);
    mavlink_msg_ihm_rs485_signal_state_get_data(msg, ihm_rs485_signal_state->data);
    ihm_rs485_signal_state->enable = mavlink_msg_ihm_rs485_signal_state_get_enable(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN? msg->len : MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN;
        memset(ihm_rs485_signal_state, 0, MAVLINK_MSG_ID_IHM_RS485_SIGNAL_STATE_LEN);
    memcpy(ihm_rs485_signal_state, _MAV_PAYLOAD(msg), len);
#endif
}
