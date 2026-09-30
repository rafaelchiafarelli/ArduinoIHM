#pragma once
// MESSAGE IHM_DAC_STATE PACKING

#define MAVLINK_MSG_ID_IHM_DAC_STATE 316


typedef struct __mavlink_ihm_dac_state_t {
 uint16_t value[2]; /*<  Code last written per DAC (0 at boot).*/
 uint8_t present; /*<  Bit i = DAC i ACKed its last I2C write (or the boot probe).*/
} mavlink_ihm_dac_state_t;

#define MAVLINK_MSG_ID_IHM_DAC_STATE_LEN 5
#define MAVLINK_MSG_ID_IHM_DAC_STATE_MIN_LEN 5
#define MAVLINK_MSG_ID_316_LEN 5
#define MAVLINK_MSG_ID_316_MIN_LEN 5

#define MAVLINK_MSG_ID_IHM_DAC_STATE_CRC 143
#define MAVLINK_MSG_ID_316_CRC 143

#define MAVLINK_MSG_IHM_DAC_STATE_FIELD_VALUE_LEN 2

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IHM_DAC_STATE { \
    316, \
    "IHM_DAC_STATE", \
    2, \
    {  { "value", NULL, MAVLINK_TYPE_UINT16_T, 2, 0, offsetof(mavlink_ihm_dac_state_t, value) }, \
         { "present", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_ihm_dac_state_t, present) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IHM_DAC_STATE { \
    "IHM_DAC_STATE", \
    2, \
    {  { "value", NULL, MAVLINK_TYPE_UINT16_T, 2, 0, offsetof(mavlink_ihm_dac_state_t, value) }, \
         { "present", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_ihm_dac_state_t, present) }, \
         } \
}
#endif

/**
 * @brief Pack a ihm_dac_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param value  Code last written per DAC (0 at boot).
 * @param present  Bit i = DAC i ACKed its last I2C write (or the boot probe).
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_dac_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const uint16_t *value, uint8_t present)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_DAC_STATE_LEN];
    _mav_put_uint8_t(buf, 4, present);
    _mav_put_uint16_t_array(buf, 0, value, 2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN);
#else
    mavlink_ihm_dac_state_t packet;
    packet.present = present;
    mav_array_assign_uint16_t(packet.value, value, 2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_DAC_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IHM_DAC_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_CRC);
}

/**
 * @brief Pack a ihm_dac_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param value  Code last written per DAC (0 at boot).
 * @param present  Bit i = DAC i ACKed its last I2C write (or the boot probe).
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_dac_state_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               const uint16_t *value, uint8_t present)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_DAC_STATE_LEN];
    _mav_put_uint8_t(buf, 4, present);
    _mav_put_uint16_t_array(buf, 0, value, 2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN);
#else
    mavlink_ihm_dac_state_t packet;
    packet.present = present;
    mav_array_memcpy(packet.value, value, sizeof(uint16_t)*2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_DAC_STATE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_DAC_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_DAC_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN);
#endif
}

/**
 * @brief Pack a ihm_dac_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param value  Code last written per DAC (0 at boot).
 * @param present  Bit i = DAC i ACKed its last I2C write (or the boot probe).
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_dac_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const uint16_t *value,uint8_t present)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_DAC_STATE_LEN];
    _mav_put_uint8_t(buf, 4, present);
    _mav_put_uint16_t_array(buf, 0, value, 2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN);
#else
    mavlink_ihm_dac_state_t packet;
    packet.present = present;
    mav_array_assign_uint16_t(packet.value, value, 2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_DAC_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IHM_DAC_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_CRC);
}

/**
 * @brief Encode a ihm_dac_state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ihm_dac_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_dac_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ihm_dac_state_t* ihm_dac_state)
{
    return mavlink_msg_ihm_dac_state_pack(system_id, component_id, msg, ihm_dac_state->value, ihm_dac_state->present);
}

/**
 * @brief Encode a ihm_dac_state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ihm_dac_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_dac_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ihm_dac_state_t* ihm_dac_state)
{
    return mavlink_msg_ihm_dac_state_pack_chan(system_id, component_id, chan, msg, ihm_dac_state->value, ihm_dac_state->present);
}

/**
 * @brief Encode a ihm_dac_state struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ihm_dac_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_dac_state_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ihm_dac_state_t* ihm_dac_state)
{
    return mavlink_msg_ihm_dac_state_pack_status(system_id, component_id, _status, msg,  ihm_dac_state->value, ihm_dac_state->present);
}

/**
 * @brief Send a ihm_dac_state message
 * @param chan MAVLink channel to send the message
 *
 * @param value  Code last written per DAC (0 at boot).
 * @param present  Bit i = DAC i ACKed its last I2C write (or the boot probe).
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ihm_dac_state_send(mavlink_channel_t chan, const uint16_t *value, uint8_t present)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_DAC_STATE_LEN];
    _mav_put_uint8_t(buf, 4, present);
    _mav_put_uint16_t_array(buf, 0, value, 2);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_DAC_STATE, buf, MAVLINK_MSG_ID_IHM_DAC_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_CRC);
#else
    mavlink_ihm_dac_state_t packet;
    packet.present = present;
    mav_array_assign_uint16_t(packet.value, value, 2);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_DAC_STATE, (const char *)&packet, MAVLINK_MSG_ID_IHM_DAC_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_CRC);
#endif
}

/**
 * @brief Send a ihm_dac_state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ihm_dac_state_send_struct(mavlink_channel_t chan, const mavlink_ihm_dac_state_t* ihm_dac_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ihm_dac_state_send(chan, ihm_dac_state->value, ihm_dac_state->present);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_DAC_STATE, (const char *)ihm_dac_state, MAVLINK_MSG_ID_IHM_DAC_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_IHM_DAC_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ihm_dac_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const uint16_t *value, uint8_t present)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 4, present);
    _mav_put_uint16_t_array(buf, 0, value, 2);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_DAC_STATE, buf, MAVLINK_MSG_ID_IHM_DAC_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_CRC);
#else
    mavlink_ihm_dac_state_t *packet = (mavlink_ihm_dac_state_t *)msgbuf;
    packet->present = present;
    mav_array_assign_uint16_t(packet->value, value, 2);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_DAC_STATE, (const char *)packet, MAVLINK_MSG_ID_IHM_DAC_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN, MAVLINK_MSG_ID_IHM_DAC_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE IHM_DAC_STATE UNPACKING


/**
 * @brief Get field value from ihm_dac_state message
 *
 * @return  Code last written per DAC (0 at boot).
 */
static inline uint16_t mavlink_msg_ihm_dac_state_get_value(const mavlink_message_t* msg, uint16_t *value)
{
    return _MAV_RETURN_uint16_t_array(msg, value, 2,  0);
}

/**
 * @brief Get field present from ihm_dac_state message
 *
 * @return  Bit i = DAC i ACKed its last I2C write (or the boot probe).
 */
static inline uint8_t mavlink_msg_ihm_dac_state_get_present(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Decode a ihm_dac_state message into a struct
 *
 * @param msg The message to decode
 * @param ihm_dac_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_ihm_dac_state_decode(const mavlink_message_t* msg, mavlink_ihm_dac_state_t* ihm_dac_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ihm_dac_state_get_value(msg, ihm_dac_state->value);
    ihm_dac_state->present = mavlink_msg_ihm_dac_state_get_present(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IHM_DAC_STATE_LEN? msg->len : MAVLINK_MSG_ID_IHM_DAC_STATE_LEN;
        memset(ihm_dac_state, 0, MAVLINK_MSG_ID_IHM_DAC_STATE_LEN);
    memcpy(ihm_dac_state, _MAV_PAYLOAD(msg), len);
#endif
}
