#pragma once
// MESSAGE IHM_RELAY_COMMAND PACKING

#define MAVLINK_MSG_ID_IHM_RELAY_COMMAND 309


typedef struct __mavlink_ihm_relay_command_t {
 uint8_t mask; /*<  Relays to change; bit i = relay i (0-7).*/
 uint8_t state; /*<  Target state for the masked relays; bit i = relay i, 1 = on. Bits outside mask are ignored.*/
} mavlink_ihm_relay_command_t;

#define MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN 2
#define MAVLINK_MSG_ID_IHM_RELAY_COMMAND_MIN_LEN 2
#define MAVLINK_MSG_ID_309_LEN 2
#define MAVLINK_MSG_ID_309_MIN_LEN 2

#define MAVLINK_MSG_ID_IHM_RELAY_COMMAND_CRC 171
#define MAVLINK_MSG_ID_309_CRC 171



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IHM_RELAY_COMMAND { \
    309, \
    "IHM_RELAY_COMMAND", \
    2, \
    {  { "mask", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_ihm_relay_command_t, mask) }, \
         { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_ihm_relay_command_t, state) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IHM_RELAY_COMMAND { \
    "IHM_RELAY_COMMAND", \
    2, \
    {  { "mask", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_ihm_relay_command_t, mask) }, \
         { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_ihm_relay_command_t, state) }, \
         } \
}
#endif

/**
 * @brief Pack a ihm_relay_command message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param mask  Relays to change; bit i = relay i (0-7).
 * @param state  Target state for the masked relays; bit i = relay i, 1 = on. Bits outside mask are ignored.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_relay_command_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t mask, uint8_t state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN];
    _mav_put_uint8_t(buf, 0, mask);
    _mav_put_uint8_t(buf, 1, state);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN);
#else
    mavlink_ihm_relay_command_t packet;
    packet.mask = mask;
    packet.state = state;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_RELAY_COMMAND;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_CRC);
}

/**
 * @brief Pack a ihm_relay_command message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param mask  Relays to change; bit i = relay i (0-7).
 * @param state  Target state for the masked relays; bit i = relay i, 1 = on. Bits outside mask are ignored.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_relay_command_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t mask, uint8_t state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN];
    _mav_put_uint8_t(buf, 0, mask);
    _mav_put_uint8_t(buf, 1, state);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN);
#else
    mavlink_ihm_relay_command_t packet;
    packet.mask = mask;
    packet.state = state;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_RELAY_COMMAND;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN);
#endif
}

/**
 * @brief Pack a ihm_relay_command message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mask  Relays to change; bit i = relay i (0-7).
 * @param state  Target state for the masked relays; bit i = relay i, 1 = on. Bits outside mask are ignored.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_relay_command_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t mask,uint8_t state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN];
    _mav_put_uint8_t(buf, 0, mask);
    _mav_put_uint8_t(buf, 1, state);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN);
#else
    mavlink_ihm_relay_command_t packet;
    packet.mask = mask;
    packet.state = state;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_RELAY_COMMAND;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_CRC);
}

/**
 * @brief Encode a ihm_relay_command struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ihm_relay_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_relay_command_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ihm_relay_command_t* ihm_relay_command)
{
    return mavlink_msg_ihm_relay_command_pack(system_id, component_id, msg, ihm_relay_command->mask, ihm_relay_command->state);
}

/**
 * @brief Encode a ihm_relay_command struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ihm_relay_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_relay_command_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ihm_relay_command_t* ihm_relay_command)
{
    return mavlink_msg_ihm_relay_command_pack_chan(system_id, component_id, chan, msg, ihm_relay_command->mask, ihm_relay_command->state);
}

/**
 * @brief Encode a ihm_relay_command struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ihm_relay_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_relay_command_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ihm_relay_command_t* ihm_relay_command)
{
    return mavlink_msg_ihm_relay_command_pack_status(system_id, component_id, _status, msg,  ihm_relay_command->mask, ihm_relay_command->state);
}

/**
 * @brief Send a ihm_relay_command message
 * @param chan MAVLink channel to send the message
 *
 * @param mask  Relays to change; bit i = relay i (0-7).
 * @param state  Target state for the masked relays; bit i = relay i, 1 = on. Bits outside mask are ignored.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ihm_relay_command_send(mavlink_channel_t chan, uint8_t mask, uint8_t state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN];
    _mav_put_uint8_t(buf, 0, mask);
    _mav_put_uint8_t(buf, 1, state);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_RELAY_COMMAND, buf, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_CRC);
#else
    mavlink_ihm_relay_command_t packet;
    packet.mask = mask;
    packet.state = state;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_RELAY_COMMAND, (const char *)&packet, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_CRC);
#endif
}

/**
 * @brief Send a ihm_relay_command message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ihm_relay_command_send_struct(mavlink_channel_t chan, const mavlink_ihm_relay_command_t* ihm_relay_command)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ihm_relay_command_send(chan, ihm_relay_command->mask, ihm_relay_command->state);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_RELAY_COMMAND, (const char *)ihm_relay_command, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_CRC);
#endif
}

#if MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ihm_relay_command_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t mask, uint8_t state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, mask);
    _mav_put_uint8_t(buf, 1, state);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_RELAY_COMMAND, buf, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_CRC);
#else
    mavlink_ihm_relay_command_t *packet = (mavlink_ihm_relay_command_t *)msgbuf;
    packet->mask = mask;
    packet->state = state;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_RELAY_COMMAND, (const char *)packet, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_MIN_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_CRC);
#endif
}
#endif

#endif

// MESSAGE IHM_RELAY_COMMAND UNPACKING


/**
 * @brief Get field mask from ihm_relay_command message
 *
 * @return  Relays to change; bit i = relay i (0-7).
 */
static inline uint8_t mavlink_msg_ihm_relay_command_get_mask(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field state from ihm_relay_command message
 *
 * @return  Target state for the masked relays; bit i = relay i, 1 = on. Bits outside mask are ignored.
 */
static inline uint8_t mavlink_msg_ihm_relay_command_get_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Decode a ihm_relay_command message into a struct
 *
 * @param msg The message to decode
 * @param ihm_relay_command C-struct to decode the message contents into
 */
static inline void mavlink_msg_ihm_relay_command_decode(const mavlink_message_t* msg, mavlink_ihm_relay_command_t* ihm_relay_command)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ihm_relay_command->mask = mavlink_msg_ihm_relay_command_get_mask(msg);
    ihm_relay_command->state = mavlink_msg_ihm_relay_command_get_state(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN? msg->len : MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN;
        memset(ihm_relay_command, 0, MAVLINK_MSG_ID_IHM_RELAY_COMMAND_LEN);
    memcpy(ihm_relay_command, _MAV_PAYLOAD(msg), len);
#endif
}
