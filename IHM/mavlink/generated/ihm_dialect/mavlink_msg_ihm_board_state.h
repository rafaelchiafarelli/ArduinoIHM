#pragma once
// MESSAGE IHM_BOARD_STATE PACKING

#define MAVLINK_MSG_ID_IHM_BOARD_STATE 300


typedef struct __mavlink_ihm_board_state_t {
 uint16_t batt_voltage; /*<  
        Raw battery-voltage ADC reading, 10-bit (0-1023) -- ATmega2560's
        built-in ADC, no external converter on this input.
      */
 uint16_t analog_in[4]; /*<  
        Raw an0_in..an3_in ADC readings, 10-bit each (0-1023).
      */
 uint16_t time_statistics; /*<  
        Raw value of main.cpp's volatile timeStatistics accumulator
        (accumulated TCNT2 ticks). Send as-is, unaveraged -- pair with
        time_counter below to compute the same ratio the firmware's own
        status bar does.
      */
 uint8_t buttons; /*<  
        Bitmask of the 7 push-buttons, 1 = pressed. Bits 0-3: btn0-btn3.
        Bits 4-6: encoder0/1/2 push-buttons. Bit 7 unused (BinaryInput's own
        buildButtonMap() hardcodes this bit as a sentinel; we don't carry
        that here, it's just left 0).
      */
 uint8_t encoder0_direction; /*<  Encoder 0 movement since last sample.*/
 uint8_t encoder1_direction; /*<  Encoder 1 movement since last sample.*/
 uint8_t encoder2_direction; /*<  Encoder 2 movement since last sample.*/
 uint8_t rotation; /*<  House-keeping orientation/rotation sensor. 0/1.*/
 uint8_t charging; /*<  House-keeping charging-status input. 0/1.*/
 uint8_t time_counter; /*<  
        Raw value of main.cpp's volatile timeCounter (number of ticks
        accumulated into time_statistics so far, resets at 100).
      */
} mavlink_ihm_board_state_t;

#define MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN 19
#define MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN 19
#define MAVLINK_MSG_ID_300_LEN 19
#define MAVLINK_MSG_ID_300_MIN_LEN 19

#define MAVLINK_MSG_ID_IHM_BOARD_STATE_CRC 157
#define MAVLINK_MSG_ID_300_CRC 157

#define MAVLINK_MSG_IHM_BOARD_STATE_FIELD_ANALOG_IN_LEN 4

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IHM_BOARD_STATE { \
    300, \
    "IHM_BOARD_STATE", \
    10, \
    {  { "buttons", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_ihm_board_state_t, buttons) }, \
         { "encoder0_direction", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_ihm_board_state_t, encoder0_direction) }, \
         { "encoder1_direction", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_ihm_board_state_t, encoder1_direction) }, \
         { "encoder2_direction", NULL, MAVLINK_TYPE_UINT8_T, 0, 15, offsetof(mavlink_ihm_board_state_t, encoder2_direction) }, \
         { "rotation", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_ihm_board_state_t, rotation) }, \
         { "charging", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_ihm_board_state_t, charging) }, \
         { "batt_voltage", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_ihm_board_state_t, batt_voltage) }, \
         { "analog_in", NULL, MAVLINK_TYPE_UINT16_T, 4, 2, offsetof(mavlink_ihm_board_state_t, analog_in) }, \
         { "time_statistics", NULL, MAVLINK_TYPE_UINT16_T, 0, 10, offsetof(mavlink_ihm_board_state_t, time_statistics) }, \
         { "time_counter", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_ihm_board_state_t, time_counter) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IHM_BOARD_STATE { \
    "IHM_BOARD_STATE", \
    10, \
    {  { "buttons", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_ihm_board_state_t, buttons) }, \
         { "encoder0_direction", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_ihm_board_state_t, encoder0_direction) }, \
         { "encoder1_direction", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_ihm_board_state_t, encoder1_direction) }, \
         { "encoder2_direction", NULL, MAVLINK_TYPE_UINT8_T, 0, 15, offsetof(mavlink_ihm_board_state_t, encoder2_direction) }, \
         { "rotation", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_ihm_board_state_t, rotation) }, \
         { "charging", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_ihm_board_state_t, charging) }, \
         { "batt_voltage", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_ihm_board_state_t, batt_voltage) }, \
         { "analog_in", NULL, MAVLINK_TYPE_UINT16_T, 4, 2, offsetof(mavlink_ihm_board_state_t, analog_in) }, \
         { "time_statistics", NULL, MAVLINK_TYPE_UINT16_T, 0, 10, offsetof(mavlink_ihm_board_state_t, time_statistics) }, \
         { "time_counter", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_ihm_board_state_t, time_counter) }, \
         } \
}
#endif

/**
 * @brief Pack a ihm_board_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param buttons  
        Bitmask of the 7 push-buttons, 1 = pressed. Bits 0-3: btn0-btn3.
        Bits 4-6: encoder0/1/2 push-buttons. Bit 7 unused (BinaryInput's own
        buildButtonMap() hardcodes this bit as a sentinel; we don't carry
        that here, it's just left 0).
      
 * @param encoder0_direction  Encoder 0 movement since last sample.
 * @param encoder1_direction  Encoder 1 movement since last sample.
 * @param encoder2_direction  Encoder 2 movement since last sample.
 * @param rotation  House-keeping orientation/rotation sensor. 0/1.
 * @param charging  House-keeping charging-status input. 0/1.
 * @param batt_voltage  
        Raw battery-voltage ADC reading, 10-bit (0-1023) -- ATmega2560's
        built-in ADC, no external converter on this input.
      
 * @param analog_in  
        Raw an0_in..an3_in ADC readings, 10-bit each (0-1023).
      
 * @param time_statistics  
        Raw value of main.cpp's volatile timeStatistics accumulator
        (accumulated TCNT2 ticks). Send as-is, unaveraged -- pair with
        time_counter below to compute the same ratio the firmware's own
        status bar does.
      
 * @param time_counter  
        Raw value of main.cpp's volatile timeCounter (number of ticks
        accumulated into time_statistics so far, resets at 100).
      
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_board_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t buttons, uint8_t encoder0_direction, uint8_t encoder1_direction, uint8_t encoder2_direction, uint8_t rotation, uint8_t charging, uint16_t batt_voltage, const uint16_t *analog_in, uint16_t time_statistics, uint8_t time_counter)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN];
    _mav_put_uint16_t(buf, 0, batt_voltage);
    _mav_put_uint16_t(buf, 10, time_statistics);
    _mav_put_uint8_t(buf, 12, buttons);
    _mav_put_uint8_t(buf, 13, encoder0_direction);
    _mav_put_uint8_t(buf, 14, encoder1_direction);
    _mav_put_uint8_t(buf, 15, encoder2_direction);
    _mav_put_uint8_t(buf, 16, rotation);
    _mav_put_uint8_t(buf, 17, charging);
    _mav_put_uint8_t(buf, 18, time_counter);
    _mav_put_uint16_t_array(buf, 2, analog_in, 4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN);
#else
    mavlink_ihm_board_state_t packet;
    packet.batt_voltage = batt_voltage;
    packet.time_statistics = time_statistics;
    packet.buttons = buttons;
    packet.encoder0_direction = encoder0_direction;
    packet.encoder1_direction = encoder1_direction;
    packet.encoder2_direction = encoder2_direction;
    packet.rotation = rotation;
    packet.charging = charging;
    packet.time_counter = time_counter;
    mav_array_assign_uint16_t(packet.analog_in, analog_in, 4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_BOARD_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_CRC);
}

/**
 * @brief Pack a ihm_board_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param buttons  
        Bitmask of the 7 push-buttons, 1 = pressed. Bits 0-3: btn0-btn3.
        Bits 4-6: encoder0/1/2 push-buttons. Bit 7 unused (BinaryInput's own
        buildButtonMap() hardcodes this bit as a sentinel; we don't carry
        that here, it's just left 0).
      
 * @param encoder0_direction  Encoder 0 movement since last sample.
 * @param encoder1_direction  Encoder 1 movement since last sample.
 * @param encoder2_direction  Encoder 2 movement since last sample.
 * @param rotation  House-keeping orientation/rotation sensor. 0/1.
 * @param charging  House-keeping charging-status input. 0/1.
 * @param batt_voltage  
        Raw battery-voltage ADC reading, 10-bit (0-1023) -- ATmega2560's
        built-in ADC, no external converter on this input.
      
 * @param analog_in  
        Raw an0_in..an3_in ADC readings, 10-bit each (0-1023).
      
 * @param time_statistics  
        Raw value of main.cpp's volatile timeStatistics accumulator
        (accumulated TCNT2 ticks). Send as-is, unaveraged -- pair with
        time_counter below to compute the same ratio the firmware's own
        status bar does.
      
 * @param time_counter  
        Raw value of main.cpp's volatile timeCounter (number of ticks
        accumulated into time_statistics so far, resets at 100).
      
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_board_state_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t buttons, uint8_t encoder0_direction, uint8_t encoder1_direction, uint8_t encoder2_direction, uint8_t rotation, uint8_t charging, uint16_t batt_voltage, const uint16_t *analog_in, uint16_t time_statistics, uint8_t time_counter)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN];
    _mav_put_uint16_t(buf, 0, batt_voltage);
    _mav_put_uint16_t(buf, 10, time_statistics);
    _mav_put_uint8_t(buf, 12, buttons);
    _mav_put_uint8_t(buf, 13, encoder0_direction);
    _mav_put_uint8_t(buf, 14, encoder1_direction);
    _mav_put_uint8_t(buf, 15, encoder2_direction);
    _mav_put_uint8_t(buf, 16, rotation);
    _mav_put_uint8_t(buf, 17, charging);
    _mav_put_uint8_t(buf, 18, time_counter);
    _mav_put_uint16_t_array(buf, 2, analog_in, 4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN);
#else
    mavlink_ihm_board_state_t packet;
    packet.batt_voltage = batt_voltage;
    packet.time_statistics = time_statistics;
    packet.buttons = buttons;
    packet.encoder0_direction = encoder0_direction;
    packet.encoder1_direction = encoder1_direction;
    packet.encoder2_direction = encoder2_direction;
    packet.rotation = rotation;
    packet.charging = charging;
    packet.time_counter = time_counter;
    mav_array_memcpy(packet.analog_in, analog_in, sizeof(uint16_t)*4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_BOARD_STATE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN);
#endif
}

/**
 * @brief Pack a ihm_board_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param buttons  
        Bitmask of the 7 push-buttons, 1 = pressed. Bits 0-3: btn0-btn3.
        Bits 4-6: encoder0/1/2 push-buttons. Bit 7 unused (BinaryInput's own
        buildButtonMap() hardcodes this bit as a sentinel; we don't carry
        that here, it's just left 0).
      
 * @param encoder0_direction  Encoder 0 movement since last sample.
 * @param encoder1_direction  Encoder 1 movement since last sample.
 * @param encoder2_direction  Encoder 2 movement since last sample.
 * @param rotation  House-keeping orientation/rotation sensor. 0/1.
 * @param charging  House-keeping charging-status input. 0/1.
 * @param batt_voltage  
        Raw battery-voltage ADC reading, 10-bit (0-1023) -- ATmega2560's
        built-in ADC, no external converter on this input.
      
 * @param analog_in  
        Raw an0_in..an3_in ADC readings, 10-bit each (0-1023).
      
 * @param time_statistics  
        Raw value of main.cpp's volatile timeStatistics accumulator
        (accumulated TCNT2 ticks). Send as-is, unaveraged -- pair with
        time_counter below to compute the same ratio the firmware's own
        status bar does.
      
 * @param time_counter  
        Raw value of main.cpp's volatile timeCounter (number of ticks
        accumulated into time_statistics so far, resets at 100).
      
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_board_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t buttons,uint8_t encoder0_direction,uint8_t encoder1_direction,uint8_t encoder2_direction,uint8_t rotation,uint8_t charging,uint16_t batt_voltage,const uint16_t *analog_in,uint16_t time_statistics,uint8_t time_counter)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN];
    _mav_put_uint16_t(buf, 0, batt_voltage);
    _mav_put_uint16_t(buf, 10, time_statistics);
    _mav_put_uint8_t(buf, 12, buttons);
    _mav_put_uint8_t(buf, 13, encoder0_direction);
    _mav_put_uint8_t(buf, 14, encoder1_direction);
    _mav_put_uint8_t(buf, 15, encoder2_direction);
    _mav_put_uint8_t(buf, 16, rotation);
    _mav_put_uint8_t(buf, 17, charging);
    _mav_put_uint8_t(buf, 18, time_counter);
    _mav_put_uint16_t_array(buf, 2, analog_in, 4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN);
#else
    mavlink_ihm_board_state_t packet;
    packet.batt_voltage = batt_voltage;
    packet.time_statistics = time_statistics;
    packet.buttons = buttons;
    packet.encoder0_direction = encoder0_direction;
    packet.encoder1_direction = encoder1_direction;
    packet.encoder2_direction = encoder2_direction;
    packet.rotation = rotation;
    packet.charging = charging;
    packet.time_counter = time_counter;
    mav_array_assign_uint16_t(packet.analog_in, analog_in, 4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_BOARD_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_CRC);
}

/**
 * @brief Encode a ihm_board_state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ihm_board_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_board_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ihm_board_state_t* ihm_board_state)
{
    return mavlink_msg_ihm_board_state_pack(system_id, component_id, msg, ihm_board_state->buttons, ihm_board_state->encoder0_direction, ihm_board_state->encoder1_direction, ihm_board_state->encoder2_direction, ihm_board_state->rotation, ihm_board_state->charging, ihm_board_state->batt_voltage, ihm_board_state->analog_in, ihm_board_state->time_statistics, ihm_board_state->time_counter);
}

/**
 * @brief Encode a ihm_board_state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ihm_board_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_board_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ihm_board_state_t* ihm_board_state)
{
    return mavlink_msg_ihm_board_state_pack_chan(system_id, component_id, chan, msg, ihm_board_state->buttons, ihm_board_state->encoder0_direction, ihm_board_state->encoder1_direction, ihm_board_state->encoder2_direction, ihm_board_state->rotation, ihm_board_state->charging, ihm_board_state->batt_voltage, ihm_board_state->analog_in, ihm_board_state->time_statistics, ihm_board_state->time_counter);
}

/**
 * @brief Encode a ihm_board_state struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ihm_board_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_board_state_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ihm_board_state_t* ihm_board_state)
{
    return mavlink_msg_ihm_board_state_pack_status(system_id, component_id, _status, msg,  ihm_board_state->buttons, ihm_board_state->encoder0_direction, ihm_board_state->encoder1_direction, ihm_board_state->encoder2_direction, ihm_board_state->rotation, ihm_board_state->charging, ihm_board_state->batt_voltage, ihm_board_state->analog_in, ihm_board_state->time_statistics, ihm_board_state->time_counter);
}

/**
 * @brief Send a ihm_board_state message
 * @param chan MAVLink channel to send the message
 *
 * @param buttons  
        Bitmask of the 7 push-buttons, 1 = pressed. Bits 0-3: btn0-btn3.
        Bits 4-6: encoder0/1/2 push-buttons. Bit 7 unused (BinaryInput's own
        buildButtonMap() hardcodes this bit as a sentinel; we don't carry
        that here, it's just left 0).
      
 * @param encoder0_direction  Encoder 0 movement since last sample.
 * @param encoder1_direction  Encoder 1 movement since last sample.
 * @param encoder2_direction  Encoder 2 movement since last sample.
 * @param rotation  House-keeping orientation/rotation sensor. 0/1.
 * @param charging  House-keeping charging-status input. 0/1.
 * @param batt_voltage  
        Raw battery-voltage ADC reading, 10-bit (0-1023) -- ATmega2560's
        built-in ADC, no external converter on this input.
      
 * @param analog_in  
        Raw an0_in..an3_in ADC readings, 10-bit each (0-1023).
      
 * @param time_statistics  
        Raw value of main.cpp's volatile timeStatistics accumulator
        (accumulated TCNT2 ticks). Send as-is, unaveraged -- pair with
        time_counter below to compute the same ratio the firmware's own
        status bar does.
      
 * @param time_counter  
        Raw value of main.cpp's volatile timeCounter (number of ticks
        accumulated into time_statistics so far, resets at 100).
      
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ihm_board_state_send(mavlink_channel_t chan, uint8_t buttons, uint8_t encoder0_direction, uint8_t encoder1_direction, uint8_t encoder2_direction, uint8_t rotation, uint8_t charging, uint16_t batt_voltage, const uint16_t *analog_in, uint16_t time_statistics, uint8_t time_counter)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN];
    _mav_put_uint16_t(buf, 0, batt_voltage);
    _mav_put_uint16_t(buf, 10, time_statistics);
    _mav_put_uint8_t(buf, 12, buttons);
    _mav_put_uint8_t(buf, 13, encoder0_direction);
    _mav_put_uint8_t(buf, 14, encoder1_direction);
    _mav_put_uint8_t(buf, 15, encoder2_direction);
    _mav_put_uint8_t(buf, 16, rotation);
    _mav_put_uint8_t(buf, 17, charging);
    _mav_put_uint8_t(buf, 18, time_counter);
    _mav_put_uint16_t_array(buf, 2, analog_in, 4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_BOARD_STATE, buf, MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_CRC);
#else
    mavlink_ihm_board_state_t packet;
    packet.batt_voltage = batt_voltage;
    packet.time_statistics = time_statistics;
    packet.buttons = buttons;
    packet.encoder0_direction = encoder0_direction;
    packet.encoder1_direction = encoder1_direction;
    packet.encoder2_direction = encoder2_direction;
    packet.rotation = rotation;
    packet.charging = charging;
    packet.time_counter = time_counter;
    mav_array_assign_uint16_t(packet.analog_in, analog_in, 4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_BOARD_STATE, (const char *)&packet, MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_CRC);
#endif
}

/**
 * @brief Send a ihm_board_state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ihm_board_state_send_struct(mavlink_channel_t chan, const mavlink_ihm_board_state_t* ihm_board_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ihm_board_state_send(chan, ihm_board_state->buttons, ihm_board_state->encoder0_direction, ihm_board_state->encoder1_direction, ihm_board_state->encoder2_direction, ihm_board_state->rotation, ihm_board_state->charging, ihm_board_state->batt_voltage, ihm_board_state->analog_in, ihm_board_state->time_statistics, ihm_board_state->time_counter);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_BOARD_STATE, (const char *)ihm_board_state, MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ihm_board_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t buttons, uint8_t encoder0_direction, uint8_t encoder1_direction, uint8_t encoder2_direction, uint8_t rotation, uint8_t charging, uint16_t batt_voltage, const uint16_t *analog_in, uint16_t time_statistics, uint8_t time_counter)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, batt_voltage);
    _mav_put_uint16_t(buf, 10, time_statistics);
    _mav_put_uint8_t(buf, 12, buttons);
    _mav_put_uint8_t(buf, 13, encoder0_direction);
    _mav_put_uint8_t(buf, 14, encoder1_direction);
    _mav_put_uint8_t(buf, 15, encoder2_direction);
    _mav_put_uint8_t(buf, 16, rotation);
    _mav_put_uint8_t(buf, 17, charging);
    _mav_put_uint8_t(buf, 18, time_counter);
    _mav_put_uint16_t_array(buf, 2, analog_in, 4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_BOARD_STATE, buf, MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_CRC);
#else
    mavlink_ihm_board_state_t *packet = (mavlink_ihm_board_state_t *)msgbuf;
    packet->batt_voltage = batt_voltage;
    packet->time_statistics = time_statistics;
    packet->buttons = buttons;
    packet->encoder0_direction = encoder0_direction;
    packet->encoder1_direction = encoder1_direction;
    packet->encoder2_direction = encoder2_direction;
    packet->rotation = rotation;
    packet->charging = charging;
    packet->time_counter = time_counter;
    mav_array_assign_uint16_t(packet->analog_in, analog_in, 4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_BOARD_STATE, (const char *)packet, MAVLINK_MSG_ID_IHM_BOARD_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN, MAVLINK_MSG_ID_IHM_BOARD_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE IHM_BOARD_STATE UNPACKING


/**
 * @brief Get field buttons from ihm_board_state message
 *
 * @return  
        Bitmask of the 7 push-buttons, 1 = pressed. Bits 0-3: btn0-btn3.
        Bits 4-6: encoder0/1/2 push-buttons. Bit 7 unused (BinaryInput's own
        buildButtonMap() hardcodes this bit as a sentinel; we don't carry
        that here, it's just left 0).
      
 */
static inline uint8_t mavlink_msg_ihm_board_state_get_buttons(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field encoder0_direction from ihm_board_state message
 *
 * @return  Encoder 0 movement since last sample.
 */
static inline uint8_t mavlink_msg_ihm_board_state_get_encoder0_direction(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  13);
}

/**
 * @brief Get field encoder1_direction from ihm_board_state message
 *
 * @return  Encoder 1 movement since last sample.
 */
static inline uint8_t mavlink_msg_ihm_board_state_get_encoder1_direction(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  14);
}

/**
 * @brief Get field encoder2_direction from ihm_board_state message
 *
 * @return  Encoder 2 movement since last sample.
 */
static inline uint8_t mavlink_msg_ihm_board_state_get_encoder2_direction(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  15);
}

/**
 * @brief Get field rotation from ihm_board_state message
 *
 * @return  House-keeping orientation/rotation sensor. 0/1.
 */
static inline uint8_t mavlink_msg_ihm_board_state_get_rotation(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  16);
}

/**
 * @brief Get field charging from ihm_board_state message
 *
 * @return  House-keeping charging-status input. 0/1.
 */
static inline uint8_t mavlink_msg_ihm_board_state_get_charging(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  17);
}

/**
 * @brief Get field batt_voltage from ihm_board_state message
 *
 * @return  
        Raw battery-voltage ADC reading, 10-bit (0-1023) -- ATmega2560's
        built-in ADC, no external converter on this input.
      
 */
static inline uint16_t mavlink_msg_ihm_board_state_get_batt_voltage(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field analog_in from ihm_board_state message
 *
 * @return  
        Raw an0_in..an3_in ADC readings, 10-bit each (0-1023).
      
 */
static inline uint16_t mavlink_msg_ihm_board_state_get_analog_in(const mavlink_message_t* msg, uint16_t *analog_in)
{
    return _MAV_RETURN_uint16_t_array(msg, analog_in, 4,  2);
}

/**
 * @brief Get field time_statistics from ihm_board_state message
 *
 * @return  
        Raw value of main.cpp's volatile timeStatistics accumulator
        (accumulated TCNT2 ticks). Send as-is, unaveraged -- pair with
        time_counter below to compute the same ratio the firmware's own
        status bar does.
      
 */
static inline uint16_t mavlink_msg_ihm_board_state_get_time_statistics(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  10);
}

/**
 * @brief Get field time_counter from ihm_board_state message
 *
 * @return  
        Raw value of main.cpp's volatile timeCounter (number of ticks
        accumulated into time_statistics so far, resets at 100).
      
 */
static inline uint8_t mavlink_msg_ihm_board_state_get_time_counter(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  18);
}

/**
 * @brief Decode a ihm_board_state message into a struct
 *
 * @param msg The message to decode
 * @param ihm_board_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_ihm_board_state_decode(const mavlink_message_t* msg, mavlink_ihm_board_state_t* ihm_board_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ihm_board_state->batt_voltage = mavlink_msg_ihm_board_state_get_batt_voltage(msg);
    mavlink_msg_ihm_board_state_get_analog_in(msg, ihm_board_state->analog_in);
    ihm_board_state->time_statistics = mavlink_msg_ihm_board_state_get_time_statistics(msg);
    ihm_board_state->buttons = mavlink_msg_ihm_board_state_get_buttons(msg);
    ihm_board_state->encoder0_direction = mavlink_msg_ihm_board_state_get_encoder0_direction(msg);
    ihm_board_state->encoder1_direction = mavlink_msg_ihm_board_state_get_encoder1_direction(msg);
    ihm_board_state->encoder2_direction = mavlink_msg_ihm_board_state_get_encoder2_direction(msg);
    ihm_board_state->rotation = mavlink_msg_ihm_board_state_get_rotation(msg);
    ihm_board_state->charging = mavlink_msg_ihm_board_state_get_charging(msg);
    ihm_board_state->time_counter = mavlink_msg_ihm_board_state_get_time_counter(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN? msg->len : MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN;
        memset(ihm_board_state, 0, MAVLINK_MSG_ID_IHM_BOARD_STATE_LEN);
    memcpy(ihm_board_state, _MAV_PAYLOAD(msg), len);
#endif
}
