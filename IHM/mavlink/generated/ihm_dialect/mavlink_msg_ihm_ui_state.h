#pragma once
// MESSAGE IHM_UI_STATE PACKING

#define MAVLINK_MSG_ID_IHM_UI_STATE 308


typedef struct __mavlink_ihm_ui_state_t {
 uint16_t screen; /*<  Active screen index (janus_app.active_screen).*/
 int16_t focus; /*<  Focused widget's index among the active screen's reachable focusable widgets, in Janus focus-traversal order. -1 = no widget focused.*/
 int16_t nav_focus; /*<  Previewed nav-strip tab index. -1 = none. At most one of focus / nav_focus is >= 0.*/
 uint16_t boxes_expanded; /*<  Bit i = the i-th box of the active screen's widget tree (depth-first, tree order) is expanded. 16 boxes max.*/
} mavlink_ihm_ui_state_t;

#define MAVLINK_MSG_ID_IHM_UI_STATE_LEN 8
#define MAVLINK_MSG_ID_IHM_UI_STATE_MIN_LEN 8
#define MAVLINK_MSG_ID_308_LEN 8
#define MAVLINK_MSG_ID_308_MIN_LEN 8

#define MAVLINK_MSG_ID_IHM_UI_STATE_CRC 3
#define MAVLINK_MSG_ID_308_CRC 3



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IHM_UI_STATE { \
    308, \
    "IHM_UI_STATE", \
    4, \
    {  { "screen", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_ihm_ui_state_t, screen) }, \
         { "focus", NULL, MAVLINK_TYPE_INT16_T, 0, 2, offsetof(mavlink_ihm_ui_state_t, focus) }, \
         { "nav_focus", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_ihm_ui_state_t, nav_focus) }, \
         { "boxes_expanded", NULL, MAVLINK_TYPE_UINT16_T, 0, 6, offsetof(mavlink_ihm_ui_state_t, boxes_expanded) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IHM_UI_STATE { \
    "IHM_UI_STATE", \
    4, \
    {  { "screen", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_ihm_ui_state_t, screen) }, \
         { "focus", NULL, MAVLINK_TYPE_INT16_T, 0, 2, offsetof(mavlink_ihm_ui_state_t, focus) }, \
         { "nav_focus", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_ihm_ui_state_t, nav_focus) }, \
         { "boxes_expanded", NULL, MAVLINK_TYPE_UINT16_T, 0, 6, offsetof(mavlink_ihm_ui_state_t, boxes_expanded) }, \
         } \
}
#endif

/**
 * @brief Pack a ihm_ui_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param screen  Active screen index (janus_app.active_screen).
 * @param focus  Focused widget's index among the active screen's reachable focusable widgets, in Janus focus-traversal order. -1 = no widget focused.
 * @param nav_focus  Previewed nav-strip tab index. -1 = none. At most one of focus / nav_focus is >= 0.
 * @param boxes_expanded  Bit i = the i-th box of the active screen's widget tree (depth-first, tree order) is expanded. 16 boxes max.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_ui_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint16_t screen, int16_t focus, int16_t nav_focus, uint16_t boxes_expanded)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_UI_STATE_LEN];
    _mav_put_uint16_t(buf, 0, screen);
    _mav_put_int16_t(buf, 2, focus);
    _mav_put_int16_t(buf, 4, nav_focus);
    _mav_put_uint16_t(buf, 6, boxes_expanded);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_UI_STATE_LEN);
#else
    mavlink_ihm_ui_state_t packet;
    packet.screen = screen;
    packet.focus = focus;
    packet.nav_focus = nav_focus;
    packet.boxes_expanded = boxes_expanded;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_UI_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_UI_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IHM_UI_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_CRC);
}

/**
 * @brief Pack a ihm_ui_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param screen  Active screen index (janus_app.active_screen).
 * @param focus  Focused widget's index among the active screen's reachable focusable widgets, in Janus focus-traversal order. -1 = no widget focused.
 * @param nav_focus  Previewed nav-strip tab index. -1 = none. At most one of focus / nav_focus is >= 0.
 * @param boxes_expanded  Bit i = the i-th box of the active screen's widget tree (depth-first, tree order) is expanded. 16 boxes max.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_ui_state_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint16_t screen, int16_t focus, int16_t nav_focus, uint16_t boxes_expanded)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_UI_STATE_LEN];
    _mav_put_uint16_t(buf, 0, screen);
    _mav_put_int16_t(buf, 2, focus);
    _mav_put_int16_t(buf, 4, nav_focus);
    _mav_put_uint16_t(buf, 6, boxes_expanded);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_UI_STATE_LEN);
#else
    mavlink_ihm_ui_state_t packet;
    packet.screen = screen;
    packet.focus = focus;
    packet.nav_focus = nav_focus;
    packet.boxes_expanded = boxes_expanded;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_UI_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_UI_STATE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_UI_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IHM_UI_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_LEN);
#endif
}

/**
 * @brief Pack a ihm_ui_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param screen  Active screen index (janus_app.active_screen).
 * @param focus  Focused widget's index among the active screen's reachable focusable widgets, in Janus focus-traversal order. -1 = no widget focused.
 * @param nav_focus  Previewed nav-strip tab index. -1 = none. At most one of focus / nav_focus is >= 0.
 * @param boxes_expanded  Bit i = the i-th box of the active screen's widget tree (depth-first, tree order) is expanded. 16 boxes max.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ihm_ui_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint16_t screen,int16_t focus,int16_t nav_focus,uint16_t boxes_expanded)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_UI_STATE_LEN];
    _mav_put_uint16_t(buf, 0, screen);
    _mav_put_int16_t(buf, 2, focus);
    _mav_put_int16_t(buf, 4, nav_focus);
    _mav_put_uint16_t(buf, 6, boxes_expanded);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IHM_UI_STATE_LEN);
#else
    mavlink_ihm_ui_state_t packet;
    packet.screen = screen;
    packet.focus = focus;
    packet.nav_focus = nav_focus;
    packet.boxes_expanded = boxes_expanded;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IHM_UI_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IHM_UI_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IHM_UI_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_CRC);
}

/**
 * @brief Encode a ihm_ui_state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ihm_ui_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_ui_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ihm_ui_state_t* ihm_ui_state)
{
    return mavlink_msg_ihm_ui_state_pack(system_id, component_id, msg, ihm_ui_state->screen, ihm_ui_state->focus, ihm_ui_state->nav_focus, ihm_ui_state->boxes_expanded);
}

/**
 * @brief Encode a ihm_ui_state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ihm_ui_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_ui_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ihm_ui_state_t* ihm_ui_state)
{
    return mavlink_msg_ihm_ui_state_pack_chan(system_id, component_id, chan, msg, ihm_ui_state->screen, ihm_ui_state->focus, ihm_ui_state->nav_focus, ihm_ui_state->boxes_expanded);
}

/**
 * @brief Encode a ihm_ui_state struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ihm_ui_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ihm_ui_state_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ihm_ui_state_t* ihm_ui_state)
{
    return mavlink_msg_ihm_ui_state_pack_status(system_id, component_id, _status, msg,  ihm_ui_state->screen, ihm_ui_state->focus, ihm_ui_state->nav_focus, ihm_ui_state->boxes_expanded);
}

/**
 * @brief Send a ihm_ui_state message
 * @param chan MAVLink channel to send the message
 *
 * @param screen  Active screen index (janus_app.active_screen).
 * @param focus  Focused widget's index among the active screen's reachable focusable widgets, in Janus focus-traversal order. -1 = no widget focused.
 * @param nav_focus  Previewed nav-strip tab index. -1 = none. At most one of focus / nav_focus is >= 0.
 * @param boxes_expanded  Bit i = the i-th box of the active screen's widget tree (depth-first, tree order) is expanded. 16 boxes max.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ihm_ui_state_send(mavlink_channel_t chan, uint16_t screen, int16_t focus, int16_t nav_focus, uint16_t boxes_expanded)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IHM_UI_STATE_LEN];
    _mav_put_uint16_t(buf, 0, screen);
    _mav_put_int16_t(buf, 2, focus);
    _mav_put_int16_t(buf, 4, nav_focus);
    _mav_put_uint16_t(buf, 6, boxes_expanded);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_UI_STATE, buf, MAVLINK_MSG_ID_IHM_UI_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_CRC);
#else
    mavlink_ihm_ui_state_t packet;
    packet.screen = screen;
    packet.focus = focus;
    packet.nav_focus = nav_focus;
    packet.boxes_expanded = boxes_expanded;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_UI_STATE, (const char *)&packet, MAVLINK_MSG_ID_IHM_UI_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_CRC);
#endif
}

/**
 * @brief Send a ihm_ui_state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ihm_ui_state_send_struct(mavlink_channel_t chan, const mavlink_ihm_ui_state_t* ihm_ui_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ihm_ui_state_send(chan, ihm_ui_state->screen, ihm_ui_state->focus, ihm_ui_state->nav_focus, ihm_ui_state->boxes_expanded);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_UI_STATE, (const char *)ihm_ui_state, MAVLINK_MSG_ID_IHM_UI_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_IHM_UI_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ihm_ui_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint16_t screen, int16_t focus, int16_t nav_focus, uint16_t boxes_expanded)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, screen);
    _mav_put_int16_t(buf, 2, focus);
    _mav_put_int16_t(buf, 4, nav_focus);
    _mav_put_uint16_t(buf, 6, boxes_expanded);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_UI_STATE, buf, MAVLINK_MSG_ID_IHM_UI_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_CRC);
#else
    mavlink_ihm_ui_state_t *packet = (mavlink_ihm_ui_state_t *)msgbuf;
    packet->screen = screen;
    packet->focus = focus;
    packet->nav_focus = nav_focus;
    packet->boxes_expanded = boxes_expanded;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IHM_UI_STATE, (const char *)packet, MAVLINK_MSG_ID_IHM_UI_STATE_MIN_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_LEN, MAVLINK_MSG_ID_IHM_UI_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE IHM_UI_STATE UNPACKING


/**
 * @brief Get field screen from ihm_ui_state message
 *
 * @return  Active screen index (janus_app.active_screen).
 */
static inline uint16_t mavlink_msg_ihm_ui_state_get_screen(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field focus from ihm_ui_state message
 *
 * @return  Focused widget's index among the active screen's reachable focusable widgets, in Janus focus-traversal order. -1 = no widget focused.
 */
static inline int16_t mavlink_msg_ihm_ui_state_get_focus(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  2);
}

/**
 * @brief Get field nav_focus from ihm_ui_state message
 *
 * @return  Previewed nav-strip tab index. -1 = none. At most one of focus / nav_focus is >= 0.
 */
static inline int16_t mavlink_msg_ihm_ui_state_get_nav_focus(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  4);
}

/**
 * @brief Get field boxes_expanded from ihm_ui_state message
 *
 * @return  Bit i = the i-th box of the active screen's widget tree (depth-first, tree order) is expanded. 16 boxes max.
 */
static inline uint16_t mavlink_msg_ihm_ui_state_get_boxes_expanded(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  6);
}

/**
 * @brief Decode a ihm_ui_state message into a struct
 *
 * @param msg The message to decode
 * @param ihm_ui_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_ihm_ui_state_decode(const mavlink_message_t* msg, mavlink_ihm_ui_state_t* ihm_ui_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ihm_ui_state->screen = mavlink_msg_ihm_ui_state_get_screen(msg);
    ihm_ui_state->focus = mavlink_msg_ihm_ui_state_get_focus(msg);
    ihm_ui_state->nav_focus = mavlink_msg_ihm_ui_state_get_nav_focus(msg);
    ihm_ui_state->boxes_expanded = mavlink_msg_ihm_ui_state_get_boxes_expanded(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IHM_UI_STATE_LEN? msg->len : MAVLINK_MSG_ID_IHM_UI_STATE_LEN;
        memset(ihm_ui_state, 0, MAVLINK_MSG_ID_IHM_UI_STATE_LEN);
    memcpy(ihm_ui_state, _MAV_PAYLOAD(msg), len);
#endif
}
