#pragma once
// MESSAGE IHM_SIMULATE_ENCODER PACKING

#define MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER 305


typedef struct __mavlink_ihm_simulate_encoder_t {
 uint8_t encoder; /*<  Target encoder, 0-2. Out-of-range is dropped by the board.*/
 uint8_t direction; /*<  CW or CCW; NOT_SUPPORTED is a no-op.*/
} mavlink_ihm_simulate_encoder_t;

#define MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN 2
#define MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_MIN_LEN 2
#define MAVLINK_MSG_ID_305_LEN 2
#define MAVLINK_MSG_ID_305_MIN_LEN 2

#define MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_CRC 19
#define MAVLINK_MSG_ID_305_CRC 19



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IHM_SIMULATE_ENCODER { \
    305, \
    "IHM_SIMULATE_ENCODER", \
    2, \
    {  { "encoder", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_ihm_simulate_encoder_t, encoder) }, \
         { "direction", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_ihm_simulate_encoder_t, direction) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IHM_SIMULATE_ENCODER { \
    "IHM_SIMULATE_ENCODER", \
    2, \
    {  { "encoder", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_ihm_simulate_encoder_t, encoder) }, \
         { "direction", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_ihm_simulate_encoder_t, direction) }, \
         } \
}
#endif

/**
 * @brief Pack a ihm_simulate_encoder message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param encoder  Target encoder, 0-2. Out-of-range is dropped by the board.
 * @param direction  CW or CCW; NOT_SUPPORTED is a no-op.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_simulate_encoder_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t encoder, uint8_t direction)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN];
    _mav_put_uint8_t(buf, 0, encoder);
    _mav_put_uint8_t(buf, 1, direction);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN);
#else
    mavlink_ihm_simulate_encoder_t packet;
    packet.encoder = encoder;
    packet.direction = direction;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_CRC);
}

/**
 * @brief Pack a ihm_simulate_encoder message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param encoder  Target encoder, 0-2. Out-of-range is dropped by the board.
 * @param direction  CW or CCW; NOT_SUPPORTED is a no-op.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_simulate_encoder_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t encoder, uint8_t direction)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN];
    _mav_put_uint8_t(buf, 0, encoder);
    _mav_put_uint8_t(buf, 1, direction);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN);
#else
    mavlink_ihm_simulate_encoder_t packet;
    packet.encoder = encoder;
    packet.direction = direction;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN);
#endif
}

/**
 * @brief Pack a ihm_simulate_encoder message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param encoder  Target encoder, 0-2. Out-of-range is dropped by the board.
 * @param direction  CW or CCW; NOT_SUPPORTED is a no-op.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_simulate_encoder_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t encoder,uint8_t direction)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN];
    _mav_put_uint8_t(buf, 0, encoder);
    _mav_put_uint8_t(buf, 1, direction);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN);
#else
    mavlink_ihm_simulate_encoder_t packet;
    packet.encoder = encoder;
    packet.direction = direction;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_CRC);
}

/**
 * @brief Encode a ihm_simulate_encoder struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ihm_simulate_encoder C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_simulate_encoder_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ihm_simulate_encoder_t* ihm_simulate_encoder)
{
    return mavlink_msg_ihm_simulate_encoder_pack(system_id, component_id, msg, ihm_simulate_encoder->encoder, ihm_simulate_encoder->direction);
}

/**
 * @brief Encode a ihm_simulate_encoder struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ihm_simulate_encoder C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_simulate_encoder_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ihm_simulate_encoder_t* ihm_simulate_encoder)
{
    return mavlink_msg_ihm_simulate_encoder_pack_chan(system_id, component_id, chan, msg, ihm_simulate_encoder->encoder, ihm_simulate_encoder->direction);
}

/**
 * @brief Encode a ihm_simulate_encoder struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ihm_simulate_encoder C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_simulate_encoder_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ihm_simulate_encoder_t* ihm_simulate_encoder)
{
    return mavlink_msg_ihm_simulate_encoder_pack_status(system_id, component_id, _status, msg,  ihm_simulate_encoder->encoder, ihm_simulate_encoder->direction);
}

/**
 * @brief Send a ihm_simulate_encoder message
 * @param chan MAVLink channel to send the message
 *
 * @param encoder  Target encoder, 0-2. Out-of-range is dropped by the board.
 * @param direction  CW or CCW; NOT_SUPPORTED is a no-op.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ihm_simulate_encoder_send(mavlink_channel_t chan, uint8_t encoder, uint8_t direction)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN];
    _mav_put_uint8_t(buf, 0, encoder);
    _mav_put_uint8_t(buf, 1, direction);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER, buf, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_CRC);
#else
    mavlink_ihm_simulate_encoder_t packet;
    packet.encoder = encoder;
    packet.direction = direction;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER, (const char *)&packet, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_CRC);
#endif
}

/**
 * @brief Send a ihm_simulate_encoder message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ihm_simulate_encoder_send_struct(mavlink_channel_t chan, const mavlink_ihm_simulate_encoder_t* ihm_simulate_encoder)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ihm_simulate_encoder_send(chan, ihm_simulate_encoder->encoder, ihm_simulate_encoder->direction);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER, (const char *)ihm_simulate_encoder, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_CRC);
#endif
}

#if MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ihm_simulate_encoder_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t encoder, uint8_t direction)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, encoder);
    _mav_put_uint8_t(buf, 1, direction);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER, buf, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_CRC);
#else
    mavlink_ihm_simulate_encoder_t *packet = (mavlink_ihm_simulate_encoder_t *)msgbuf;
    packet->encoder = encoder;
    packet->direction = direction;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER, (const char *)packet, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_MIN_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_CRC);
#endif
}
#endif

#endif

// MESSAGE IHM_SIMULATE_ENCODER UNPACKING


/**
 * @brief Get field encoder from ihm_simulate_encoder message
 *
 * @return  Target encoder, 0-2. Out-of-range is dropped by the board.
 */
static inline uint8_t mavlink_msg_ihm_simulate_encoder_get_encoder(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field direction from ihm_simulate_encoder message
 *
 * @return  CW or CCW; NOT_SUPPORTED is a no-op.
 */
static inline uint8_t mavlink_msg_ihm_simulate_encoder_get_direction(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Decode a ihm_simulate_encoder message into a struct
 *
 * @param msg The message to decode
 * @param ihm_simulate_encoder C-struct to decode the message contents into
 */
static inline void mavlink_msg_ihm_simulate_encoder_decode(const mavlink_message_t* msg, mavlink_ihm_simulate_encoder_t* ihm_simulate_encoder)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ihm_simulate_encoder->encoder = mavlink_msg_ihm_simulate_encoder_get_encoder(msg);
    ihm_simulate_encoder->direction = mavlink_msg_ihm_simulate_encoder_get_direction(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN? msg->len : MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN;
        memset(ihm_simulate_encoder, 0, MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER_LEN);
    memcpy(ihm_simulate_encoder, _MAV_PAYLOAD(msg), len);
#endif
}
