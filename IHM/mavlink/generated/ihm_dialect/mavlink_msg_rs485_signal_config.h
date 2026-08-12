#pragma once
// MESSAGE RS485_SIGNAL_CONFIG PACKING

#define MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG 302


typedef struct __mavlink_rs485_signal_config_t {
 uint16_t period_ms; /*<  Repeat interval in ms. 0 = send once (one-shot).*/
 uint16_t repeat_count; /*<  Number of times to send. 0 = repeat until disabled.*/
 uint8_t length; /*<  Valid byte count in data, 0-32.*/
 uint8_t data[32]; /*<  Raw payload bytes; only the first length bytes are used.*/
 uint8_t enable; /*<  1 = start/keep running, 0 = stop. Resend with 0 to stop a running generator.*/
} mavlink_rs485_signal_config_t;

#define MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN 38
#define MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN 38
#define MAVLINK_MSG_ID_302_LEN 38
#define MAVLINK_MSG_ID_302_MIN_LEN 38

#define MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_CRC 161
#define MAVLINK_MSG_ID_302_CRC 161

#define MAVLINK_MSG_RS485_SIGNAL_CONFIG_FIELD_DATA_LEN 32

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_RS485_SIGNAL_CONFIG { \
    302, \
    "RS485_SIGNAL_CONFIG", \
    5, \
    {  { "length", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_rs485_signal_config_t, length) }, \
         { "data", NULL, MAVLINK_TYPE_UINT8_T, 32, 5, offsetof(mavlink_rs485_signal_config_t, data) }, \
         { "period_ms", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_rs485_signal_config_t, period_ms) }, \
         { "repeat_count", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_rs485_signal_config_t, repeat_count) }, \
         { "enable", NULL, MAVLINK_TYPE_UINT8_T, 0, 37, offsetof(mavlink_rs485_signal_config_t, enable) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_RS485_SIGNAL_CONFIG { \
    "RS485_SIGNAL_CONFIG", \
    5, \
    {  { "length", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_rs485_signal_config_t, length) }, \
         { "data", NULL, MAVLINK_TYPE_UINT8_T, 32, 5, offsetof(mavlink_rs485_signal_config_t, data) }, \
         { "period_ms", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_rs485_signal_config_t, period_ms) }, \
         { "repeat_count", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_rs485_signal_config_t, repeat_count) }, \
         { "enable", NULL, MAVLINK_TYPE_UINT8_T, 0, 37, offsetof(mavlink_rs485_signal_config_t, enable) }, \
         } \
}
#endif

/**
 * @brief Pack a rs485_signal_config message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param length  Valid byte count in data, 0-32.
 * @param data  Raw payload bytes; only the first length bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once (one-shot).
 * @param repeat_count  Number of times to send. 0 = repeat until disabled.
 * @param enable  1 = start/keep running, 0 = stop. Resend with 0 to stop a running generator.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rs485_signal_config_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t length, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN];
    _mav_put_uint16_t(buf, 0, period_ms);
    _mav_put_uint16_t(buf, 2, repeat_count);
    _mav_put_uint8_t(buf, 4, length);
    _mav_put_uint8_t(buf, 37, enable);
    _mav_put_uint8_t_array(buf, 5, data, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN);
#else
    mavlink_rs485_signal_config_t packet;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.length = length;
    packet.enable = enable;
    mav_array_assign_uint8_t(packet.data, data, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_CRC);
}

/**
 * @brief Pack a rs485_signal_config message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param length  Valid byte count in data, 0-32.
 * @param data  Raw payload bytes; only the first length bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once (one-shot).
 * @param repeat_count  Number of times to send. 0 = repeat until disabled.
 * @param enable  1 = start/keep running, 0 = stop. Resend with 0 to stop a running generator.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rs485_signal_config_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t length, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN];
    _mav_put_uint16_t(buf, 0, period_ms);
    _mav_put_uint16_t(buf, 2, repeat_count);
    _mav_put_uint8_t(buf, 4, length);
    _mav_put_uint8_t(buf, 37, enable);
    _mav_put_uint8_t_array(buf, 5, data, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN);
#else
    mavlink_rs485_signal_config_t packet;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.length = length;
    packet.enable = enable;
    mav_array_memcpy(packet.data, data, sizeof(uint8_t)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN);
#endif
}

/**
 * @brief Pack a rs485_signal_config message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param length  Valid byte count in data, 0-32.
 * @param data  Raw payload bytes; only the first length bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once (one-shot).
 * @param repeat_count  Number of times to send. 0 = repeat until disabled.
 * @param enable  1 = start/keep running, 0 = stop. Resend with 0 to stop a running generator.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rs485_signal_config_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t length,const uint8_t *data,uint16_t period_ms,uint16_t repeat_count,uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN];
    _mav_put_uint16_t(buf, 0, period_ms);
    _mav_put_uint16_t(buf, 2, repeat_count);
    _mav_put_uint8_t(buf, 4, length);
    _mav_put_uint8_t(buf, 37, enable);
    _mav_put_uint8_t_array(buf, 5, data, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN);
#else
    mavlink_rs485_signal_config_t packet;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.length = length;
    packet.enable = enable;
    mav_array_assign_uint8_t(packet.data, data, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_CRC);
}

/**
 * @brief Encode a rs485_signal_config struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param rs485_signal_config C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rs485_signal_config_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_rs485_signal_config_t* rs485_signal_config)
{
    return mavlink_msg_rs485_signal_config_pack(system_id, component_id, msg, rs485_signal_config->length, rs485_signal_config->data, rs485_signal_config->period_ms, rs485_signal_config->repeat_count, rs485_signal_config->enable);
}

/**
 * @brief Encode a rs485_signal_config struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rs485_signal_config C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rs485_signal_config_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_rs485_signal_config_t* rs485_signal_config)
{
    return mavlink_msg_rs485_signal_config_pack_chan(system_id, component_id, chan, msg, rs485_signal_config->length, rs485_signal_config->data, rs485_signal_config->period_ms, rs485_signal_config->repeat_count, rs485_signal_config->enable);
}

/**
 * @brief Encode a rs485_signal_config struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param rs485_signal_config C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rs485_signal_config_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_rs485_signal_config_t* rs485_signal_config)
{
    return mavlink_msg_rs485_signal_config_pack_status(system_id, component_id, _status, msg,  rs485_signal_config->length, rs485_signal_config->data, rs485_signal_config->period_ms, rs485_signal_config->repeat_count, rs485_signal_config->enable);
}

/**
 * @brief Send a rs485_signal_config message
 * @param chan MAVLink channel to send the message
 *
 * @param length  Valid byte count in data, 0-32.
 * @param data  Raw payload bytes; only the first length bytes are used.
 * @param period_ms  Repeat interval in ms. 0 = send once (one-shot).
 * @param repeat_count  Number of times to send. 0 = repeat until disabled.
 * @param enable  1 = start/keep running, 0 = stop. Resend with 0 to stop a running generator.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rs485_signal_config_send(mavlink_channel_t chan, uint8_t length, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN];
    _mav_put_uint16_t(buf, 0, period_ms);
    _mav_put_uint16_t(buf, 2, repeat_count);
    _mav_put_uint8_t(buf, 4, length);
    _mav_put_uint8_t(buf, 37, enable);
    _mav_put_uint8_t_array(buf, 5, data, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG, buf, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_CRC);
#else
    mavlink_rs485_signal_config_t packet;
    packet.period_ms = period_ms;
    packet.repeat_count = repeat_count;
    packet.length = length;
    packet.enable = enable;
    mav_array_assign_uint8_t(packet.data, data, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG, (const char *)&packet, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_CRC);
#endif
}

/**
 * @brief Send a rs485_signal_config message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_rs485_signal_config_send_struct(mavlink_channel_t chan, const mavlink_rs485_signal_config_t* rs485_signal_config)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_rs485_signal_config_send(chan, rs485_signal_config->length, rs485_signal_config->data, rs485_signal_config->period_ms, rs485_signal_config->repeat_count, rs485_signal_config->enable);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG, (const char *)rs485_signal_config, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_CRC);
#endif
}

#if MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_rs485_signal_config_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t length, const uint8_t *data, uint16_t period_ms, uint16_t repeat_count, uint8_t enable)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, period_ms);
    _mav_put_uint16_t(buf, 2, repeat_count);
    _mav_put_uint8_t(buf, 4, length);
    _mav_put_uint8_t(buf, 37, enable);
    _mav_put_uint8_t_array(buf, 5, data, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG, buf, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_CRC);
#else
    mavlink_rs485_signal_config_t *packet = (mavlink_rs485_signal_config_t *)msgbuf;
    packet->period_ms = period_ms;
    packet->repeat_count = repeat_count;
    packet->length = length;
    packet->enable = enable;
    mav_array_assign_uint8_t(packet->data, data, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG, (const char *)packet, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_CRC);
#endif
}
#endif

#endif

// MESSAGE RS485_SIGNAL_CONFIG UNPACKING


/**
 * @brief Get field length from rs485_signal_config message
 *
 * @return  Valid byte count in data, 0-32.
 */
static inline uint8_t mavlink_msg_rs485_signal_config_get_length(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field data from rs485_signal_config message
 *
 * @return  Raw payload bytes; only the first length bytes are used.
 */
static inline uint16_t mavlink_msg_rs485_signal_config_get_data(const mavlink_message_t* msg, uint8_t *data)
{
    return _MAV_RETURN_uint8_t_array(msg, data, 32,  5);
}

/**
 * @brief Get field period_ms from rs485_signal_config message
 *
 * @return  Repeat interval in ms. 0 = send once (one-shot).
 */
static inline uint16_t mavlink_msg_rs485_signal_config_get_period_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field repeat_count from rs485_signal_config message
 *
 * @return  Number of times to send. 0 = repeat until disabled.
 */
static inline uint16_t mavlink_msg_rs485_signal_config_get_repeat_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  2);
}

/**
 * @brief Get field enable from rs485_signal_config message
 *
 * @return  1 = start/keep running, 0 = stop. Resend with 0 to stop a running generator.
 */
static inline uint8_t mavlink_msg_rs485_signal_config_get_enable(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  37);
}

/**
 * @brief Decode a rs485_signal_config message into a struct
 *
 * @param msg The message to decode
 * @param rs485_signal_config C-struct to decode the message contents into
 */
static inline void mavlink_msg_rs485_signal_config_decode(const mavlink_message_t* msg, mavlink_rs485_signal_config_t* rs485_signal_config)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    rs485_signal_config->period_ms = mavlink_msg_rs485_signal_config_get_period_ms(msg);
    rs485_signal_config->repeat_count = mavlink_msg_rs485_signal_config_get_repeat_count(msg);
    rs485_signal_config->length = mavlink_msg_rs485_signal_config_get_length(msg);
    mavlink_msg_rs485_signal_config_get_data(msg, rs485_signal_config->data);
    rs485_signal_config->enable = mavlink_msg_rs485_signal_config_get_enable(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN? msg->len : MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN;
        memset(rs485_signal_config, 0, MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG_LEN);
    memcpy(rs485_signal_config, _MAV_PAYLOAD(msg), len);
#endif
}
