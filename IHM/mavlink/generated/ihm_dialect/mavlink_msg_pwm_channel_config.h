#pragma once
// MESSAGE PWM_CHANNEL_CONFIG PACKING

#define MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG 303


typedef struct __mavlink_pwm_channel_config_t {
 uint16_t frequency; /*<  
        Raw ICRn TOP (timer period). Only read when f_selector ==
        PWM_FREQUENCY_VARIABLE; ignored for every fixed selector.
      */
 uint8_t channel; /*<  Target PWM channel, 0-3. Out-of-range configs are dropped by the board.*/
 uint8_t f_selector; /*<  Frequency selector for the whole channel.*/
 uint8_t out1_enabled; /*<  Output A: 1 = drive the pin, 0 = disconnect it.*/
 uint8_t out1_inverting; /*<  Output A: 0 = non-inverting (high from period start), 1 = inverting.*/
 uint8_t out1_duty_percent; /*<  Output A duty cycle, 0-100. Scaled against the selector's resolution on-board.*/
 uint8_t out2_enabled; /*<  Output B enable. Ignored for channels 0 and 1.*/
 uint8_t out2_inverting; /*<  Output B edge polarity. Ignored for channels 0 and 1.*/
 uint8_t out2_duty_percent; /*<  Output B duty cycle, 0-100. Ignored for channels 0 and 1.*/
 uint8_t out3_enabled; /*<  Output C enable. Ignored for channels 0 and 1.*/
 uint8_t out3_inverting; /*<  Output C edge polarity. Ignored for channels 0 and 1.*/
 uint8_t out3_duty_percent; /*<  Output C duty cycle, 0-100. Ignored for channels 0 and 1.*/
} mavlink_pwm_channel_config_t;

#define MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN 13
#define MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_MIN_LEN 13
#define MAVLINK_MSG_ID_303_LEN 13
#define MAVLINK_MSG_ID_303_MIN_LEN 13

#define MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_CRC 93
#define MAVLINK_MSG_ID_303_CRC 93



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_PWM_CHANNEL_CONFIG { \
    303, \
    "PWM_CHANNEL_CONFIG", \
    12, \
    {  { "channel", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_pwm_channel_config_t, channel) }, \
         { "f_selector", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_pwm_channel_config_t, f_selector) }, \
         { "frequency", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_pwm_channel_config_t, frequency) }, \
         { "out1_enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_pwm_channel_config_t, out1_enabled) }, \
         { "out1_inverting", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_pwm_channel_config_t, out1_inverting) }, \
         { "out1_duty_percent", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_pwm_channel_config_t, out1_duty_percent) }, \
         { "out2_enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_pwm_channel_config_t, out2_enabled) }, \
         { "out2_inverting", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_pwm_channel_config_t, out2_inverting) }, \
         { "out2_duty_percent", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_pwm_channel_config_t, out2_duty_percent) }, \
         { "out3_enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_pwm_channel_config_t, out3_enabled) }, \
         { "out3_inverting", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_pwm_channel_config_t, out3_inverting) }, \
         { "out3_duty_percent", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_pwm_channel_config_t, out3_duty_percent) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_PWM_CHANNEL_CONFIG { \
    "PWM_CHANNEL_CONFIG", \
    12, \
    {  { "channel", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_pwm_channel_config_t, channel) }, \
         { "f_selector", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_pwm_channel_config_t, f_selector) }, \
         { "frequency", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_pwm_channel_config_t, frequency) }, \
         { "out1_enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_pwm_channel_config_t, out1_enabled) }, \
         { "out1_inverting", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_pwm_channel_config_t, out1_inverting) }, \
         { "out1_duty_percent", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_pwm_channel_config_t, out1_duty_percent) }, \
         { "out2_enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_pwm_channel_config_t, out2_enabled) }, \
         { "out2_inverting", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_pwm_channel_config_t, out2_inverting) }, \
         { "out2_duty_percent", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_pwm_channel_config_t, out2_duty_percent) }, \
         { "out3_enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_pwm_channel_config_t, out3_enabled) }, \
         { "out3_inverting", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_pwm_channel_config_t, out3_inverting) }, \
         { "out3_duty_percent", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_pwm_channel_config_t, out3_duty_percent) }, \
         } \
}
#endif

/**
 * @brief Pack a pwm_channel_config message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param channel  Target PWM channel, 0-3. Out-of-range configs are dropped by the board.
 * @param f_selector  Frequency selector for the whole channel.
 * @param frequency  
        Raw ICRn TOP (timer period). Only read when f_selector ==
        PWM_FREQUENCY_VARIABLE; ignored for every fixed selector.
      
 * @param out1_enabled  Output A: 1 = drive the pin, 0 = disconnect it.
 * @param out1_inverting  Output A: 0 = non-inverting (high from period start), 1 = inverting.
 * @param out1_duty_percent  Output A duty cycle, 0-100. Scaled against the selector's resolution on-board.
 * @param out2_enabled  Output B enable. Ignored for channels 0 and 1.
 * @param out2_inverting  Output B edge polarity. Ignored for channels 0 and 1.
 * @param out2_duty_percent  Output B duty cycle, 0-100. Ignored for channels 0 and 1.
 * @param out3_enabled  Output C enable. Ignored for channels 0 and 1.
 * @param out3_inverting  Output C edge polarity. Ignored for channels 0 and 1.
 * @param out3_duty_percent  Output C duty cycle, 0-100. Ignored for channels 0 and 1.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_pwm_channel_config_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t channel, uint8_t f_selector, uint16_t frequency, uint8_t out1_enabled, uint8_t out1_inverting, uint8_t out1_duty_percent, uint8_t out2_enabled, uint8_t out2_inverting, uint8_t out2_duty_percent, uint8_t out3_enabled, uint8_t out3_inverting, uint8_t out3_duty_percent)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN];
    _mav_put_uint16_t(buf, 0, frequency);
    _mav_put_uint8_t(buf, 2, channel);
    _mav_put_uint8_t(buf, 3, f_selector);
    _mav_put_uint8_t(buf, 4, out1_enabled);
    _mav_put_uint8_t(buf, 5, out1_inverting);
    _mav_put_uint8_t(buf, 6, out1_duty_percent);
    _mav_put_uint8_t(buf, 7, out2_enabled);
    _mav_put_uint8_t(buf, 8, out2_inverting);
    _mav_put_uint8_t(buf, 9, out2_duty_percent);
    _mav_put_uint8_t(buf, 10, out3_enabled);
    _mav_put_uint8_t(buf, 11, out3_inverting);
    _mav_put_uint8_t(buf, 12, out3_duty_percent);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN);
#else
    mavlink_pwm_channel_config_t packet;
    packet.frequency = frequency;
    packet.channel = channel;
    packet.f_selector = f_selector;
    packet.out1_enabled = out1_enabled;
    packet.out1_inverting = out1_inverting;
    packet.out1_duty_percent = out1_duty_percent;
    packet.out2_enabled = out2_enabled;
    packet.out2_inverting = out2_inverting;
    packet.out2_duty_percent = out2_duty_percent;
    packet.out3_enabled = out3_enabled;
    packet.out3_inverting = out3_inverting;
    packet.out3_duty_percent = out3_duty_percent;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_CRC);
}

/**
 * @brief Pack a pwm_channel_config message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param channel  Target PWM channel, 0-3. Out-of-range configs are dropped by the board.
 * @param f_selector  Frequency selector for the whole channel.
 * @param frequency  
        Raw ICRn TOP (timer period). Only read when f_selector ==
        PWM_FREQUENCY_VARIABLE; ignored for every fixed selector.
      
 * @param out1_enabled  Output A: 1 = drive the pin, 0 = disconnect it.
 * @param out1_inverting  Output A: 0 = non-inverting (high from period start), 1 = inverting.
 * @param out1_duty_percent  Output A duty cycle, 0-100. Scaled against the selector's resolution on-board.
 * @param out2_enabled  Output B enable. Ignored for channels 0 and 1.
 * @param out2_inverting  Output B edge polarity. Ignored for channels 0 and 1.
 * @param out2_duty_percent  Output B duty cycle, 0-100. Ignored for channels 0 and 1.
 * @param out3_enabled  Output C enable. Ignored for channels 0 and 1.
 * @param out3_inverting  Output C edge polarity. Ignored for channels 0 and 1.
 * @param out3_duty_percent  Output C duty cycle, 0-100. Ignored for channels 0 and 1.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_pwm_channel_config_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t channel, uint8_t f_selector, uint16_t frequency, uint8_t out1_enabled, uint8_t out1_inverting, uint8_t out1_duty_percent, uint8_t out2_enabled, uint8_t out2_inverting, uint8_t out2_duty_percent, uint8_t out3_enabled, uint8_t out3_inverting, uint8_t out3_duty_percent)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN];
    _mav_put_uint16_t(buf, 0, frequency);
    _mav_put_uint8_t(buf, 2, channel);
    _mav_put_uint8_t(buf, 3, f_selector);
    _mav_put_uint8_t(buf, 4, out1_enabled);
    _mav_put_uint8_t(buf, 5, out1_inverting);
    _mav_put_uint8_t(buf, 6, out1_duty_percent);
    _mav_put_uint8_t(buf, 7, out2_enabled);
    _mav_put_uint8_t(buf, 8, out2_inverting);
    _mav_put_uint8_t(buf, 9, out2_duty_percent);
    _mav_put_uint8_t(buf, 10, out3_enabled);
    _mav_put_uint8_t(buf, 11, out3_inverting);
    _mav_put_uint8_t(buf, 12, out3_duty_percent);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN);
#else
    mavlink_pwm_channel_config_t packet;
    packet.frequency = frequency;
    packet.channel = channel;
    packet.f_selector = f_selector;
    packet.out1_enabled = out1_enabled;
    packet.out1_inverting = out1_inverting;
    packet.out1_duty_percent = out1_duty_percent;
    packet.out2_enabled = out2_enabled;
    packet.out2_inverting = out2_inverting;
    packet.out2_duty_percent = out2_duty_percent;
    packet.out3_enabled = out3_enabled;
    packet.out3_inverting = out3_inverting;
    packet.out3_duty_percent = out3_duty_percent;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN);
#endif
}

/**
 * @brief Pack a pwm_channel_config message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param channel  Target PWM channel, 0-3. Out-of-range configs are dropped by the board.
 * @param f_selector  Frequency selector for the whole channel.
 * @param frequency  
        Raw ICRn TOP (timer period). Only read when f_selector ==
        PWM_FREQUENCY_VARIABLE; ignored for every fixed selector.
      
 * @param out1_enabled  Output A: 1 = drive the pin, 0 = disconnect it.
 * @param out1_inverting  Output A: 0 = non-inverting (high from period start), 1 = inverting.
 * @param out1_duty_percent  Output A duty cycle, 0-100. Scaled against the selector's resolution on-board.
 * @param out2_enabled  Output B enable. Ignored for channels 0 and 1.
 * @param out2_inverting  Output B edge polarity. Ignored for channels 0 and 1.
 * @param out2_duty_percent  Output B duty cycle, 0-100. Ignored for channels 0 and 1.
 * @param out3_enabled  Output C enable. Ignored for channels 0 and 1.
 * @param out3_inverting  Output C edge polarity. Ignored for channels 0 and 1.
 * @param out3_duty_percent  Output C duty cycle, 0-100. Ignored for channels 0 and 1.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_pwm_channel_config_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t channel,uint8_t f_selector,uint16_t frequency,uint8_t out1_enabled,uint8_t out1_inverting,uint8_t out1_duty_percent,uint8_t out2_enabled,uint8_t out2_inverting,uint8_t out2_duty_percent,uint8_t out3_enabled,uint8_t out3_inverting,uint8_t out3_duty_percent)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN];
    _mav_put_uint16_t(buf, 0, frequency);
    _mav_put_uint8_t(buf, 2, channel);
    _mav_put_uint8_t(buf, 3, f_selector);
    _mav_put_uint8_t(buf, 4, out1_enabled);
    _mav_put_uint8_t(buf, 5, out1_inverting);
    _mav_put_uint8_t(buf, 6, out1_duty_percent);
    _mav_put_uint8_t(buf, 7, out2_enabled);
    _mav_put_uint8_t(buf, 8, out2_inverting);
    _mav_put_uint8_t(buf, 9, out2_duty_percent);
    _mav_put_uint8_t(buf, 10, out3_enabled);
    _mav_put_uint8_t(buf, 11, out3_inverting);
    _mav_put_uint8_t(buf, 12, out3_duty_percent);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN);
#else
    mavlink_pwm_channel_config_t packet;
    packet.frequency = frequency;
    packet.channel = channel;
    packet.f_selector = f_selector;
    packet.out1_enabled = out1_enabled;
    packet.out1_inverting = out1_inverting;
    packet.out1_duty_percent = out1_duty_percent;
    packet.out2_enabled = out2_enabled;
    packet.out2_inverting = out2_inverting;
    packet.out2_duty_percent = out2_duty_percent;
    packet.out3_enabled = out3_enabled;
    packet.out3_inverting = out3_inverting;
    packet.out3_duty_percent = out3_duty_percent;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_CRC);
}

/**
 * @brief Encode a pwm_channel_config struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param pwm_channel_config C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_pwm_channel_config_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_pwm_channel_config_t* pwm_channel_config)
{
    return mavlink_msg_pwm_channel_config_pack(system_id, component_id, msg, pwm_channel_config->channel, pwm_channel_config->f_selector, pwm_channel_config->frequency, pwm_channel_config->out1_enabled, pwm_channel_config->out1_inverting, pwm_channel_config->out1_duty_percent, pwm_channel_config->out2_enabled, pwm_channel_config->out2_inverting, pwm_channel_config->out2_duty_percent, pwm_channel_config->out3_enabled, pwm_channel_config->out3_inverting, pwm_channel_config->out3_duty_percent);
}

/**
 * @brief Encode a pwm_channel_config struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param pwm_channel_config C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_pwm_channel_config_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_pwm_channel_config_t* pwm_channel_config)
{
    return mavlink_msg_pwm_channel_config_pack_chan(system_id, component_id, chan, msg, pwm_channel_config->channel, pwm_channel_config->f_selector, pwm_channel_config->frequency, pwm_channel_config->out1_enabled, pwm_channel_config->out1_inverting, pwm_channel_config->out1_duty_percent, pwm_channel_config->out2_enabled, pwm_channel_config->out2_inverting, pwm_channel_config->out2_duty_percent, pwm_channel_config->out3_enabled, pwm_channel_config->out3_inverting, pwm_channel_config->out3_duty_percent);
}

/**
 * @brief Encode a pwm_channel_config struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param pwm_channel_config C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_pwm_channel_config_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_pwm_channel_config_t* pwm_channel_config)
{
    return mavlink_msg_pwm_channel_config_pack_status(system_id, component_id, _status, msg,  pwm_channel_config->channel, pwm_channel_config->f_selector, pwm_channel_config->frequency, pwm_channel_config->out1_enabled, pwm_channel_config->out1_inverting, pwm_channel_config->out1_duty_percent, pwm_channel_config->out2_enabled, pwm_channel_config->out2_inverting, pwm_channel_config->out2_duty_percent, pwm_channel_config->out3_enabled, pwm_channel_config->out3_inverting, pwm_channel_config->out3_duty_percent);
}

/**
 * @brief Send a pwm_channel_config message
 * @param chan MAVLink channel to send the message
 *
 * @param channel  Target PWM channel, 0-3. Out-of-range configs are dropped by the board.
 * @param f_selector  Frequency selector for the whole channel.
 * @param frequency  
        Raw ICRn TOP (timer period). Only read when f_selector ==
        PWM_FREQUENCY_VARIABLE; ignored for every fixed selector.
      
 * @param out1_enabled  Output A: 1 = drive the pin, 0 = disconnect it.
 * @param out1_inverting  Output A: 0 = non-inverting (high from period start), 1 = inverting.
 * @param out1_duty_percent  Output A duty cycle, 0-100. Scaled against the selector's resolution on-board.
 * @param out2_enabled  Output B enable. Ignored for channels 0 and 1.
 * @param out2_inverting  Output B edge polarity. Ignored for channels 0 and 1.
 * @param out2_duty_percent  Output B duty cycle, 0-100. Ignored for channels 0 and 1.
 * @param out3_enabled  Output C enable. Ignored for channels 0 and 1.
 * @param out3_inverting  Output C edge polarity. Ignored for channels 0 and 1.
 * @param out3_duty_percent  Output C duty cycle, 0-100. Ignored for channels 0 and 1.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_pwm_channel_config_send(mavlink_channel_t chan, uint8_t channel, uint8_t f_selector, uint16_t frequency, uint8_t out1_enabled, uint8_t out1_inverting, uint8_t out1_duty_percent, uint8_t out2_enabled, uint8_t out2_inverting, uint8_t out2_duty_percent, uint8_t out3_enabled, uint8_t out3_inverting, uint8_t out3_duty_percent)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN];
    _mav_put_uint16_t(buf, 0, frequency);
    _mav_put_uint8_t(buf, 2, channel);
    _mav_put_uint8_t(buf, 3, f_selector);
    _mav_put_uint8_t(buf, 4, out1_enabled);
    _mav_put_uint8_t(buf, 5, out1_inverting);
    _mav_put_uint8_t(buf, 6, out1_duty_percent);
    _mav_put_uint8_t(buf, 7, out2_enabled);
    _mav_put_uint8_t(buf, 8, out2_inverting);
    _mav_put_uint8_t(buf, 9, out2_duty_percent);
    _mav_put_uint8_t(buf, 10, out3_enabled);
    _mav_put_uint8_t(buf, 11, out3_inverting);
    _mav_put_uint8_t(buf, 12, out3_duty_percent);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG, buf, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_CRC);
#else
    mavlink_pwm_channel_config_t packet;
    packet.frequency = frequency;
    packet.channel = channel;
    packet.f_selector = f_selector;
    packet.out1_enabled = out1_enabled;
    packet.out1_inverting = out1_inverting;
    packet.out1_duty_percent = out1_duty_percent;
    packet.out2_enabled = out2_enabled;
    packet.out2_inverting = out2_inverting;
    packet.out2_duty_percent = out2_duty_percent;
    packet.out3_enabled = out3_enabled;
    packet.out3_inverting = out3_inverting;
    packet.out3_duty_percent = out3_duty_percent;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG, (const char *)&packet, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_CRC);
#endif
}

/**
 * @brief Send a pwm_channel_config message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_pwm_channel_config_send_struct(mavlink_channel_t chan, const mavlink_pwm_channel_config_t* pwm_channel_config)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_pwm_channel_config_send(chan, pwm_channel_config->channel, pwm_channel_config->f_selector, pwm_channel_config->frequency, pwm_channel_config->out1_enabled, pwm_channel_config->out1_inverting, pwm_channel_config->out1_duty_percent, pwm_channel_config->out2_enabled, pwm_channel_config->out2_inverting, pwm_channel_config->out2_duty_percent, pwm_channel_config->out3_enabled, pwm_channel_config->out3_inverting, pwm_channel_config->out3_duty_percent);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG, (const char *)pwm_channel_config, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_CRC);
#endif
}

#if MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_pwm_channel_config_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t channel, uint8_t f_selector, uint16_t frequency, uint8_t out1_enabled, uint8_t out1_inverting, uint8_t out1_duty_percent, uint8_t out2_enabled, uint8_t out2_inverting, uint8_t out2_duty_percent, uint8_t out3_enabled, uint8_t out3_inverting, uint8_t out3_duty_percent)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, frequency);
    _mav_put_uint8_t(buf, 2, channel);
    _mav_put_uint8_t(buf, 3, f_selector);
    _mav_put_uint8_t(buf, 4, out1_enabled);
    _mav_put_uint8_t(buf, 5, out1_inverting);
    _mav_put_uint8_t(buf, 6, out1_duty_percent);
    _mav_put_uint8_t(buf, 7, out2_enabled);
    _mav_put_uint8_t(buf, 8, out2_inverting);
    _mav_put_uint8_t(buf, 9, out2_duty_percent);
    _mav_put_uint8_t(buf, 10, out3_enabled);
    _mav_put_uint8_t(buf, 11, out3_inverting);
    _mav_put_uint8_t(buf, 12, out3_duty_percent);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG, buf, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_CRC);
#else
    mavlink_pwm_channel_config_t *packet = (mavlink_pwm_channel_config_t *)msgbuf;
    packet->frequency = frequency;
    packet->channel = channel;
    packet->f_selector = f_selector;
    packet->out1_enabled = out1_enabled;
    packet->out1_inverting = out1_inverting;
    packet->out1_duty_percent = out1_duty_percent;
    packet->out2_enabled = out2_enabled;
    packet->out2_inverting = out2_inverting;
    packet->out2_duty_percent = out2_duty_percent;
    packet->out3_enabled = out3_enabled;
    packet->out3_inverting = out3_inverting;
    packet->out3_duty_percent = out3_duty_percent;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG, (const char *)packet, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_MIN_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_CRC);
#endif
}
#endif

#endif

// MESSAGE PWM_CHANNEL_CONFIG UNPACKING


/**
 * @brief Get field channel from pwm_channel_config message
 *
 * @return  Target PWM channel, 0-3. Out-of-range configs are dropped by the board.
 */
static inline uint8_t mavlink_msg_pwm_channel_config_get_channel(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Get field f_selector from pwm_channel_config message
 *
 * @return  Frequency selector for the whole channel.
 */
static inline uint8_t mavlink_msg_pwm_channel_config_get_f_selector(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  3);
}

/**
 * @brief Get field frequency from pwm_channel_config message
 *
 * @return  
        Raw ICRn TOP (timer period). Only read when f_selector ==
        PWM_FREQUENCY_VARIABLE; ignored for every fixed selector.
      
 */
static inline uint16_t mavlink_msg_pwm_channel_config_get_frequency(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field out1_enabled from pwm_channel_config message
 *
 * @return  Output A: 1 = drive the pin, 0 = disconnect it.
 */
static inline uint8_t mavlink_msg_pwm_channel_config_get_out1_enabled(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field out1_inverting from pwm_channel_config message
 *
 * @return  Output A: 0 = non-inverting (high from period start), 1 = inverting.
 */
static inline uint8_t mavlink_msg_pwm_channel_config_get_out1_inverting(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field out1_duty_percent from pwm_channel_config message
 *
 * @return  Output A duty cycle, 0-100. Scaled against the selector's resolution on-board.
 */
static inline uint8_t mavlink_msg_pwm_channel_config_get_out1_duty_percent(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Get field out2_enabled from pwm_channel_config message
 *
 * @return  Output B enable. Ignored for channels 0 and 1.
 */
static inline uint8_t mavlink_msg_pwm_channel_config_get_out2_enabled(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  7);
}

/**
 * @brief Get field out2_inverting from pwm_channel_config message
 *
 * @return  Output B edge polarity. Ignored for channels 0 and 1.
 */
static inline uint8_t mavlink_msg_pwm_channel_config_get_out2_inverting(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field out2_duty_percent from pwm_channel_config message
 *
 * @return  Output B duty cycle, 0-100. Ignored for channels 0 and 1.
 */
static inline uint8_t mavlink_msg_pwm_channel_config_get_out2_duty_percent(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field out3_enabled from pwm_channel_config message
 *
 * @return  Output C enable. Ignored for channels 0 and 1.
 */
static inline uint8_t mavlink_msg_pwm_channel_config_get_out3_enabled(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Get field out3_inverting from pwm_channel_config message
 *
 * @return  Output C edge polarity. Ignored for channels 0 and 1.
 */
static inline uint8_t mavlink_msg_pwm_channel_config_get_out3_inverting(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  11);
}

/**
 * @brief Get field out3_duty_percent from pwm_channel_config message
 *
 * @return  Output C duty cycle, 0-100. Ignored for channels 0 and 1.
 */
static inline uint8_t mavlink_msg_pwm_channel_config_get_out3_duty_percent(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Decode a pwm_channel_config message into a struct
 *
 * @param msg The message to decode
 * @param pwm_channel_config C-struct to decode the message contents into
 */
static inline void mavlink_msg_pwm_channel_config_decode(const mavlink_message_t* msg, mavlink_pwm_channel_config_t* pwm_channel_config)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    pwm_channel_config->frequency = mavlink_msg_pwm_channel_config_get_frequency(msg);
    pwm_channel_config->channel = mavlink_msg_pwm_channel_config_get_channel(msg);
    pwm_channel_config->f_selector = mavlink_msg_pwm_channel_config_get_f_selector(msg);
    pwm_channel_config->out1_enabled = mavlink_msg_pwm_channel_config_get_out1_enabled(msg);
    pwm_channel_config->out1_inverting = mavlink_msg_pwm_channel_config_get_out1_inverting(msg);
    pwm_channel_config->out1_duty_percent = mavlink_msg_pwm_channel_config_get_out1_duty_percent(msg);
    pwm_channel_config->out2_enabled = mavlink_msg_pwm_channel_config_get_out2_enabled(msg);
    pwm_channel_config->out2_inverting = mavlink_msg_pwm_channel_config_get_out2_inverting(msg);
    pwm_channel_config->out2_duty_percent = mavlink_msg_pwm_channel_config_get_out2_duty_percent(msg);
    pwm_channel_config->out3_enabled = mavlink_msg_pwm_channel_config_get_out3_enabled(msg);
    pwm_channel_config->out3_inverting = mavlink_msg_pwm_channel_config_get_out3_inverting(msg);
    pwm_channel_config->out3_duty_percent = mavlink_msg_pwm_channel_config_get_out3_duty_percent(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN? msg->len : MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN;
        memset(pwm_channel_config, 0, MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG_LEN);
    memcpy(pwm_channel_config, _MAV_PAYLOAD(msg), len);
#endif
}
