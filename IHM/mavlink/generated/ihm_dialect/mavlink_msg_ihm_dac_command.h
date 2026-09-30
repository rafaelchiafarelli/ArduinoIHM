#pragma once
// MESSAGE IHM_DAC_COMMAND PACKING

#define MAVLINK_MSG_ID_IHM_DAC_COMMAND 315


typedef struct __mavlink_ihm_dac_command_t {
 uint16_t value; /*<  DAC code, 0-4095.*/
 uint8_t channel; /*<  Target DAC, 0-1.*/
} mavlink_ihm_dac_command_t;

#define MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN 3
#define MAVLINK_MSG_ID_IHM_DAC_COMMAND_MIN_LEN 3
#define MAVLINK_MSG_ID_315_LEN 3
#define MAVLINK_MSG_ID_315_MIN_LEN 3

#define MAVLINK_MSG_ID_IHM_DAC_COMMAND_CRC 187
#define MAVLINK_MSG_ID_315_CRC 187



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IHM_DAC_COMMAND { \
    315, \
    "IHM_DAC_COMMAND", \
    2, \
    {  { "channel", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_ihm_dac_command_t, channel) }, \
         { "value", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_ihm_dac_command_t, value) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IHM_DAC_COMMAND { \
    "IHM_DAC_COMMAND", \
    2, \
    {  { "channel", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_ihm_dac_command_t, channel) }, \
         { "value", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_ihm_dac_command_t, value) }, \
         } \
}
#endif

/**
 * @brief Pack a ihm_dac_command message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param channel  Target DAC, 0-1.
 * @param value  DAC code, 0-4095.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_dac_command_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t channel, uint16_t value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN];
    _mav_put_uint16_t(buf, 0, value);
    _mav_put_uint8_t(buf, 2, channel);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN);
#else
    mavlink_ihm_dac_command_t packet;
    packet.value = value;
    packet.channel = channel;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_DAC_COMMAND;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IHM_DAC_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_CRC);
}

/**
 * @brief Pack a ihm_dac_command message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param channel  Target DAC, 0-1.
 * @param value  DAC code, 0-4095.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_dac_command_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t channel, uint16_t value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN];
    _mav_put_uint16_t(buf, 0, value);
    _mav_put_uint8_t(buf, 2, channel);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN);
#else
    mavlink_ihm_dac_command_t packet;
    packet.value = value;
    packet.channel = channel;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_DAC_COMMAND;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_DAC_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_DAC_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN);
#endif
}

/**
 * @brief Pack a ihm_dac_command message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param channel  Target DAC, 0-1.
 * @param value  DAC code, 0-4095.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_dac_command_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t channel,uint16_t value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN];
    _mav_put_uint16_t(buf, 0, value);
    _mav_put_uint8_t(buf, 2, channel);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN);
#else
    mavlink_ihm_dac_command_t packet;
    packet.value = value;
    packet.channel = channel;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_DAC_COMMAND;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IHM_DAC_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_CRC);
}

/**
 * @brief Encode a ihm_dac_command struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ihm_dac_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_dac_command_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ihm_dac_command_t* ihm_dac_command)
{
    return mavlink_msg_ihm_dac_command_pack(system_id, component_id, msg, ihm_dac_command->channel, ihm_dac_command->value);
}

/**
 * @brief Encode a ihm_dac_command struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ihm_dac_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_dac_command_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ihm_dac_command_t* ihm_dac_command)
{
    return mavlink_msg_ihm_dac_command_pack_chan(system_id, component_id, chan, msg, ihm_dac_command->channel, ihm_dac_command->value);
}

/**
 * @brief Encode a ihm_dac_command struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ihm_dac_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_dac_command_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ihm_dac_command_t* ihm_dac_command)
{
    return mavlink_msg_ihm_dac_command_pack_status(system_id, component_id, _status, msg,  ihm_dac_command->channel, ihm_dac_command->value);
}

/**
 * @brief Send a ihm_dac_command message
 * @param chan MAVLink channel to send the message
 *
 * @param channel  Target DAC, 0-1.
 * @param value  DAC code, 0-4095.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ihm_dac_command_send(mavlink_channel_t chan, uint8_t channel, uint16_t value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN];
    _mav_put_uint16_t(buf, 0, value);
    _mav_put_uint8_t(buf, 2, channel);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_DAC_COMMAND, buf, MAVLINK_MSG_ID_IHM_DAC_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_CRC);
#else
    mavlink_ihm_dac_command_t packet;
    packet.value = value;
    packet.channel = channel;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_DAC_COMMAND, (const char *)&packet, MAVLINK_MSG_ID_IHM_DAC_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_CRC);
#endif
}

/**
 * @brief Send a ihm_dac_command message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ihm_dac_command_send_struct(mavlink_channel_t chan, const mavlink_ihm_dac_command_t* ihm_dac_command)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ihm_dac_command_send(chan, ihm_dac_command->channel, ihm_dac_command->value);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_DAC_COMMAND, (const char *)ihm_dac_command, MAVLINK_MSG_ID_IHM_DAC_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_CRC);
#endif
}

#if MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ihm_dac_command_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t channel, uint16_t value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, value);
    _mav_put_uint8_t(buf, 2, channel);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_DAC_COMMAND, buf, MAVLINK_MSG_ID_IHM_DAC_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_CRC);
#else
    mavlink_ihm_dac_command_t *packet = (mavlink_ihm_dac_command_t *)msgbuf;
    packet->value = value;
    packet->channel = channel;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_DAC_COMMAND, (const char *)packet, MAVLINK_MSG_ID_IHM_DAC_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN, MAVLINK_MSG_ID_IHM_DAC_COMMAND_CRC);
#endif
}
#endif

#endif

// MESSAGE IHM_DAC_COMMAND UNPACKING


/**
 * @brief Get field channel from ihm_dac_command message
 *
 * @return  Target DAC, 0-1.
 */
static inline uint8_t mavlink_msg_ihm_dac_command_get_channel(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Get field value from ihm_dac_command message
 *
 * @return  DAC code, 0-4095.
 */
static inline uint16_t mavlink_msg_ihm_dac_command_get_value(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Decode a ihm_dac_command message into a struct
 *
 * @param msg The message to decode
 * @param ihm_dac_command C-struct to decode the message contents into
 */
static inline void mavlink_msg_ihm_dac_command_decode(const mavlink_message_t* msg, mavlink_ihm_dac_command_t* ihm_dac_command)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ihm_dac_command->value = mavlink_msg_ihm_dac_command_get_value(msg);
    ihm_dac_command->channel = mavlink_msg_ihm_dac_command_get_channel(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN? msg->len : MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN;
        memset(ihm_dac_command, 0, MAVLINK_MSG_ID_IHM_DAC_COMMAND_LEN);
    memcpy(ihm_dac_command, _MAV_PAYLOAD(msg), len);
#endif
}
