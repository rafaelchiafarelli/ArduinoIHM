#pragma once
// MESSAGE IHM_SIMULATE_BUTTON PACKING

#define MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON 306


typedef struct __mavlink_ihm_simulate_button_t {
 uint8_t button_mask; /*<  Buttons to press this pass; bit i = button i, bit 7 ignored.*/
} mavlink_ihm_simulate_button_t;

#define MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN 1
#define MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_MIN_LEN 1
#define MAVLINK_MSG_ID_306_LEN 1
#define MAVLINK_MSG_ID_306_MIN_LEN 1

#define MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_CRC 133
#define MAVLINK_MSG_ID_306_CRC 133



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IHM_SIMULATE_BUTTON { \
    306, \
    "IHM_SIMULATE_BUTTON", \
    1, \
    {  { "button_mask", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_ihm_simulate_button_t, button_mask) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IHM_SIMULATE_BUTTON { \
    "IHM_SIMULATE_BUTTON", \
    1, \
    {  { "button_mask", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_ihm_simulate_button_t, button_mask) }, \
         } \
}
#endif

/**
 * @brief Pack a ihm_simulate_button message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param button_mask  Buttons to press this pass; bit i = button i, bit 7 ignored.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_simulate_button_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t button_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN];
    _mav_put_uint8_t(buf, 0, button_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN);
#else
    mavlink_ihm_simulate_button_t packet;
    packet.button_mask = button_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_CRC);
}

/**
 * @brief Pack a ihm_simulate_button message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param button_mask  Buttons to press this pass; bit i = button i, bit 7 ignored.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_simulate_button_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t button_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN];
    _mav_put_uint8_t(buf, 0, button_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN);
#else
    mavlink_ihm_simulate_button_t packet;
    packet.button_mask = button_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN);
#endif
}

/**
 * @brief Pack a ihm_simulate_button message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param button_mask  Buttons to press this pass; bit i = button i, bit 7 ignored.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_simulate_button_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t button_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN];
    _mav_put_uint8_t(buf, 0, button_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN);
#else
    mavlink_ihm_simulate_button_t packet;
    packet.button_mask = button_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_CRC);
}

/**
 * @brief Encode a ihm_simulate_button struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ihm_simulate_button C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_simulate_button_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ihm_simulate_button_t* ihm_simulate_button)
{
    return mavlink_msg_ihm_simulate_button_pack(system_id, component_id, msg, ihm_simulate_button->button_mask);
}

/**
 * @brief Encode a ihm_simulate_button struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ihm_simulate_button C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_simulate_button_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ihm_simulate_button_t* ihm_simulate_button)
{
    return mavlink_msg_ihm_simulate_button_pack_chan(system_id, component_id, chan, msg, ihm_simulate_button->button_mask);
}

/**
 * @brief Encode a ihm_simulate_button struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ihm_simulate_button C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_simulate_button_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ihm_simulate_button_t* ihm_simulate_button)
{
    return mavlink_msg_ihm_simulate_button_pack_status(system_id, component_id, _status, msg,  ihm_simulate_button->button_mask);
}

/**
 * @brief Send a ihm_simulate_button message
 * @param chan MAVLink channel to send the message
 *
 * @param button_mask  Buttons to press this pass; bit i = button i, bit 7 ignored.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ihm_simulate_button_send(mavlink_channel_t chan, uint8_t button_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN];
    _mav_put_uint8_t(buf, 0, button_mask);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON, buf, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_CRC);
#else
    mavlink_ihm_simulate_button_t packet;
    packet.button_mask = button_mask;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON, (const char *)&packet, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_CRC);
#endif
}

/**
 * @brief Send a ihm_simulate_button message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ihm_simulate_button_send_struct(mavlink_channel_t chan, const mavlink_ihm_simulate_button_t* ihm_simulate_button)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ihm_simulate_button_send(chan, ihm_simulate_button->button_mask);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON, (const char *)ihm_simulate_button, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_CRC);
#endif
}

#if MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ihm_simulate_button_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t button_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, button_mask);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON, buf, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_CRC);
#else
    mavlink_ihm_simulate_button_t *packet = (mavlink_ihm_simulate_button_t *)msgbuf;
    packet->button_mask = button_mask;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON, (const char *)packet, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_CRC);
#endif
}
#endif

#endif

// MESSAGE IHM_SIMULATE_BUTTON UNPACKING


/**
 * @brief Get field button_mask from ihm_simulate_button message
 *
 * @return  Buttons to press this pass; bit i = button i, bit 7 ignored.
 */
static inline uint8_t mavlink_msg_ihm_simulate_button_get_button_mask(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a ihm_simulate_button message into a struct
 *
 * @param msg The message to decode
 * @param ihm_simulate_button C-struct to decode the message contents into
 */
static inline void mavlink_msg_ihm_simulate_button_decode(const mavlink_message_t* msg, mavlink_ihm_simulate_button_t* ihm_simulate_button)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ihm_simulate_button->button_mask = mavlink_msg_ihm_simulate_button_get_button_mask(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN? msg->len : MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN;
        memset(ihm_simulate_button, 0, MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON_LEN);
    memcpy(ihm_simulate_button, _MAV_PAYLOAD(msg), len);
#endif
}
