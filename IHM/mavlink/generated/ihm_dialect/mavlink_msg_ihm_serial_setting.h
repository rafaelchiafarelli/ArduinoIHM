#pragma once
// MESSAGE IHM_SERIAL_SETTING PACKING

#define MAVLINK_MSG_ID_IHM_SERIAL_SETTING 313


typedef struct __mavlink_ihm_serial_setting_t {
 int32_t value; /*<  New value.*/
 uint16_t key; /*<  Setting key, per bus (mavlink/README.md key table).*/
 uint8_t bus; /*<  Bus the setting belongs to.*/
} mavlink_ihm_serial_setting_t;

#define MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN 7
#define MAVLINK_MSG_ID_IHM_SERIAL_SETTING_MIN_LEN 7
#define MAVLINK_MSG_ID_313_LEN 7
#define MAVLINK_MSG_ID_313_MIN_LEN 7

#define MAVLINK_MSG_ID_IHM_SERIAL_SETTING_CRC 51
#define MAVLINK_MSG_ID_313_CRC 51



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IHM_SERIAL_SETTING { \
    313, \
    "IHM_SERIAL_SETTING", \
    3, \
    {  { "bus", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_ihm_serial_setting_t, bus) }, \
         { "key", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_ihm_serial_setting_t, key) }, \
         { "value", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_ihm_serial_setting_t, value) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IHM_SERIAL_SETTING { \
    "IHM_SERIAL_SETTING", \
    3, \
    {  { "bus", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_ihm_serial_setting_t, bus) }, \
         { "key", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_ihm_serial_setting_t, key) }, \
         { "value", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_ihm_serial_setting_t, value) }, \
         } \
}
#endif

/**
 * @brief Pack a ihm_serial_setting message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param bus  Bus the setting belongs to.
 * @param key  Setting key, per bus (mavlink/README.md key table).
 * @param value  New value.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_serial_setting_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t bus, uint16_t key, int32_t value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN];
    _mav_put_int32_t(buf, 0, value);
    _mav_put_uint16_t(buf, 4, key);
    _mav_put_uint8_t(buf, 6, bus);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN);
#else
    mavlink_ihm_serial_setting_t packet;
    packet.value = value;
    packet.key = key;
    packet.bus = bus;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_SERIAL_SETTING;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_MIN_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_CRC);
}

/**
 * @brief Pack a ihm_serial_setting message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param bus  Bus the setting belongs to.
 * @param key  Setting key, per bus (mavlink/README.md key table).
 * @param value  New value.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_serial_setting_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t bus, uint16_t key, int32_t value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN];
    _mav_put_int32_t(buf, 0, value);
    _mav_put_uint16_t(buf, 4, key);
    _mav_put_uint8_t(buf, 6, bus);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN);
#else
    mavlink_ihm_serial_setting_t packet;
    packet.value = value;
    packet.key = key;
    packet.bus = bus;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_SERIAL_SETTING;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_MIN_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_MIN_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN);
#endif
}

/**
 * @brief Pack a ihm_serial_setting message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param bus  Bus the setting belongs to.
 * @param key  Setting key, per bus (mavlink/README.md key table).
 * @param value  New value.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_serial_setting_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t bus,uint16_t key,int32_t value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN];
    _mav_put_int32_t(buf, 0, value);
    _mav_put_uint16_t(buf, 4, key);
    _mav_put_uint8_t(buf, 6, bus);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN);
#else
    mavlink_ihm_serial_setting_t packet;
    packet.value = value;
    packet.key = key;
    packet.bus = bus;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_SERIAL_SETTING;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_MIN_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_CRC);
}

/**
 * @brief Encode a ihm_serial_setting struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ihm_serial_setting C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_serial_setting_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ihm_serial_setting_t* ihm_serial_setting)
{
    return mavlink_msg_ihm_serial_setting_pack(system_id, component_id, msg, ihm_serial_setting->bus, ihm_serial_setting->key, ihm_serial_setting->value);
}

/**
 * @brief Encode a ihm_serial_setting struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ihm_serial_setting C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_serial_setting_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ihm_serial_setting_t* ihm_serial_setting)
{
    return mavlink_msg_ihm_serial_setting_pack_chan(system_id, component_id, chan, msg, ihm_serial_setting->bus, ihm_serial_setting->key, ihm_serial_setting->value);
}

/**
 * @brief Encode a ihm_serial_setting struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ihm_serial_setting C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_serial_setting_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ihm_serial_setting_t* ihm_serial_setting)
{
    return mavlink_msg_ihm_serial_setting_pack_status(system_id, component_id, _status, msg,  ihm_serial_setting->bus, ihm_serial_setting->key, ihm_serial_setting->value);
}

/**
 * @brief Send a ihm_serial_setting message
 * @param chan MAVLink channel to send the message
 *
 * @param bus  Bus the setting belongs to.
 * @param key  Setting key, per bus (mavlink/README.md key table).
 * @param value  New value.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ihm_serial_setting_send(mavlink_channel_t chan, uint8_t bus, uint16_t key, int32_t value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN];
    _mav_put_int32_t(buf, 0, value);
    _mav_put_uint16_t(buf, 4, key);
    _mav_put_uint8_t(buf, 6, bus);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SERIAL_SETTING, buf, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_MIN_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_CRC);
#else
    mavlink_ihm_serial_setting_t packet;
    packet.value = value;
    packet.key = key;
    packet.bus = bus;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SERIAL_SETTING, (const char *)&packet, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_MIN_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_CRC);
#endif
}

/**
 * @brief Send a ihm_serial_setting message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ihm_serial_setting_send_struct(mavlink_channel_t chan, const mavlink_ihm_serial_setting_t* ihm_serial_setting)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ihm_serial_setting_send(chan, ihm_serial_setting->bus, ihm_serial_setting->key, ihm_serial_setting->value);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SERIAL_SETTING, (const char *)ihm_serial_setting, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_MIN_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_CRC);
#endif
}

#if MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ihm_serial_setting_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t bus, uint16_t key, int32_t value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int32_t(buf, 0, value);
    _mav_put_uint16_t(buf, 4, key);
    _mav_put_uint8_t(buf, 6, bus);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SERIAL_SETTING, buf, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_MIN_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_CRC);
#else
    mavlink_ihm_serial_setting_t *packet = (mavlink_ihm_serial_setting_t *)msgbuf;
    packet->value = value;
    packet->key = key;
    packet->bus = bus;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_SERIAL_SETTING, (const char *)packet, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_MIN_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_CRC);
#endif
}
#endif

#endif

// MESSAGE IHM_SERIAL_SETTING UNPACKING


/**
 * @brief Get field bus from ihm_serial_setting message
 *
 * @return  Bus the setting belongs to.
 */
static inline uint8_t mavlink_msg_ihm_serial_setting_get_bus(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Get field key from ihm_serial_setting message
 *
 * @return  Setting key, per bus (mavlink/README.md key table).
 */
static inline uint16_t mavlink_msg_ihm_serial_setting_get_key(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  4);
}

/**
 * @brief Get field value from ihm_serial_setting message
 *
 * @return  New value.
 */
static inline int32_t mavlink_msg_ihm_serial_setting_get_value(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Decode a ihm_serial_setting message into a struct
 *
 * @param msg The message to decode
 * @param ihm_serial_setting C-struct to decode the message contents into
 */
static inline void mavlink_msg_ihm_serial_setting_decode(const mavlink_message_t* msg, mavlink_ihm_serial_setting_t* ihm_serial_setting)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ihm_serial_setting->value = mavlink_msg_ihm_serial_setting_get_value(msg);
    ihm_serial_setting->key = mavlink_msg_ihm_serial_setting_get_key(msg);
    ihm_serial_setting->bus = mavlink_msg_ihm_serial_setting_get_bus(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN? msg->len : MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN;
        memset(ihm_serial_setting, 0, MAVLINK_MSG_ID_IHM_SERIAL_SETTING_LEN);
    memcpy(ihm_serial_setting, _MAV_PAYLOAD(msg), len);
#endif
}
