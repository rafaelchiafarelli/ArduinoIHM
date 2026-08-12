/** @file
 *  @brief MAVLink comm protocol generated from ihm_dialect.xml
 *  @see http://mavlink.org
 */
#pragma once
#ifndef MAVLINK_IHM_DIALECT_H
#define MAVLINK_IHM_DIALECT_H

#ifndef MAVLINK_H
    #error Wrong include order: MAVLINK_IHM_DIALECT.H MUST NOT BE DIRECTLY USED. Include mavlink.h from the same directory instead or set ALL AND EVERY defines from MAVLINK.H manually accordingly, including the #define MAVLINK_H call.
#endif

#define MAVLINK_IHM_DIALECT_XML_HASH -4257345066223267664

#ifdef __cplusplus
extern "C" {
#endif

// MESSAGE LENGTHS AND CRCS

#ifndef MAVLINK_MESSAGE_LENGTHS
#define MAVLINK_MESSAGE_LENGTHS {}
#endif

#ifndef MAVLINK_MESSAGE_CRCS
#define MAVLINK_MESSAGE_CRCS {{300, 157, 19, 19, 0, 0, 0}, {301, 176, 20, 20, 0, 0, 0}, {302, 161, 38, 38, 0, 0, 0}}
#endif

#include "../protocol.h"

#define MAVLINK_ENABLED_IHM_DIALECT

// ENUM DEFINITIONS


/** @brief 
        Rotary encoder movement direction. Mirrors RotaryEncoder.h's
        DIRECTION_TYPE exactly (IHM/lib/RotaryEncoder/RotaryEncoder.h) --
        keep these two in sync if that enum ever changes.
       */
#ifndef HAVE_ENUM_IHM_ENCODER_DIRECTION
#define HAVE_ENUM_IHM_ENCODER_DIRECTION
typedef enum IHM_ENCODER_DIRECTION
{
   IHM_ENCODER_NOT_SUPPORTED=0, /* 
          No movement since this encoder was last read (RotaryEncoder::
          getDirection() resets to this value after every read, so this
          also just means "nothing new since the last sample").
         | */
   IHM_ENCODER_CCW=1, /* Counter-clockwise movement. | */
   IHM_ENCODER_CW=2, /* Clockwise movement. | */
   IHM_ENCODER_DIRECTION_ENUM_END=3, /*  | */
} IHM_ENCODER_DIRECTION;
#endif

// MAVLINK VERSION

#ifndef MAVLINK_VERSION
#define MAVLINK_VERSION 2
#endif

#if (MAVLINK_VERSION == 0)
#undef MAVLINK_VERSION
#define MAVLINK_VERSION 2
#endif

// MESSAGE DEFINITIONS
#include "./mavlink_msg_ihm_board_state.h"
#include "./mavlink_msg_can_signal_config.h"
#include "./mavlink_msg_rs485_signal_config.h"

// base include



#if MAVLINK_IHM_DIALECT_XML_HASH == MAVLINK_PRIMARY_XML_HASH
# define MAVLINK_MESSAGE_INFO {MAVLINK_MESSAGE_INFO_IHM_BOARD_STATE, MAVLINK_MESSAGE_INFO_CAN_SIGNAL_CONFIG, MAVLINK_MESSAGE_INFO_RS485_SIGNAL_CONFIG}
# define MAVLINK_MESSAGE_NAMES {{ "CAN_SIGNAL_CONFIG", 301 }, { "IHM_BOARD_STATE", 300 }, { "RS485_SIGNAL_CONFIG", 302 }}
# if MAVLINK_COMMAND_24BIT
#  include "../mavlink_get_info.h"
# endif
#endif

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MAVLINK_IHM_DIALECT_H
