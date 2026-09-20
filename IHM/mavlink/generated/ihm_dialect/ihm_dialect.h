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

#define MAVLINK_IHM_DIALECT_XML_HASH 252923084419477378

#ifdef __cplusplus
extern "C" {
#endif

// MESSAGE LENGTHS AND CRCS

#ifndef MAVLINK_MESSAGE_LENGTHS
#define MAVLINK_MESSAGE_LENGTHS {}
#endif

#ifndef MAVLINK_MESSAGE_CRCS
#define MAVLINK_MESSAGE_CRCS {{300, 157, 19, 19, 0, 0, 0}, {301, 176, 20, 20, 0, 0, 0}, {302, 161, 38, 38, 0, 0, 0}, {303, 93, 13, 13, 0, 0, 0}, {304, 163, 1, 1, 0, 0, 0}, {305, 19, 2, 2, 0, 0, 0}}
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

/** @brief 
        PWM channel frequency selector. Mirrors PWMFrequency in
        IHM/lib/MultiOutput/src/PWMTiming.h exactly (same order, same
        0-based values) -- keep the two in sync if that enum ever
        changes. The trailing NUMBER_OF_PWM_FREQUENCIES count in the C
        enum is deliberately not mirrored here (it's a bound, not a
        value). Entry names carry the truncated integer Hz; the C enum's
        own names carry the exact fractional Hz.
       */
#ifndef HAVE_ENUM_PWM_FREQUENCY
#define HAVE_ENUM_PWM_FREQUENCY
typedef enum PWM_FREQUENCY
{
   PWM_FREQUENCY_62500_HZ=0, /* frequency_62_500HZ. | */
   PWM_FREQUENCY_31250_HZ=1, /* frequency_31_250HZ. | */
   PWM_FREQUENCY_15625_HZ=2, /* frequency_15_625HZ. | */
   PWM_FREQUENCY_7812_HZ=3, /* frequency_7812_5HZ. | */
   PWM_FREQUENCY_3906_HZ=4, /* frequency_3906_25HZ. | */
   PWM_FREQUENCY_1953_HZ=5, /* frequency_1953_125HZ. | */
   PWM_FREQUENCY_976_HZ=6, /* frequency_976_5625HZ. | */
   PWM_FREQUENCY_488_HZ=7, /* frequency_488_28125HZ. | */
   PWM_FREQUENCY_244_HZ=8, /* frequency_244_140625HZ. | */
   PWM_FREQUENCY_122_HZ=9, /* frequency_122_0703125HZ. | */
   PWM_FREQUENCY_61_HZ=10, /* frequency_61_03515625HZ. | */
   PWM_FREQUENCY_30_HZ=11, /* frequency_30_517578125HZ. | */
   PWM_FREQUENCY_15_HZ=12, /* frequency_15_2587890625HZ. | */
   PWM_FREQUENCY_VARIABLE=13, /* 
          frequency_variable -- Fast PWM mode 14, TOP = ICRn. The
          PWM_CHANNEL_CONFIG.frequency field supplies that raw TOP; it is
          ignored for every other selector.
         | */
   PWM_FREQUENCY_ENUM_END=14, /*  | */
} PWM_FREQUENCY;
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
#include "./mavlink_msg_pwm_channel_config.h"
#include "./mavlink_msg_ihm_relay_state.h"
#include "./mavlink_msg_ihm_simulate_encoder.h"

// base include



#if MAVLINK_IHM_DIALECT_XML_HASH == MAVLINK_PRIMARY_XML_HASH
# define MAVLINK_MESSAGE_INFO {MAVLINK_MESSAGE_INFO_IHM_BOARD_STATE, MAVLINK_MESSAGE_INFO_CAN_SIGNAL_CONFIG, MAVLINK_MESSAGE_INFO_RS485_SIGNAL_CONFIG, MAVLINK_MESSAGE_INFO_PWM_CHANNEL_CONFIG, MAVLINK_MESSAGE_INFO_IHM_RELAY_STATE, MAVLINK_MESSAGE_INFO_IHM_SIMULATE_ENCODER}
# define MAVLINK_MESSAGE_NAMES {{ "CAN_SIGNAL_CONFIG", 301 }, { "IHM_BOARD_STATE", 300 }, { "IHM_RELAY_STATE", 304 }, { "IHM_SIMULATE_ENCODER", 305 }, { "PWM_CHANNEL_CONFIG", 303 }, { "RS485_SIGNAL_CONFIG", 302 }}
# if MAVLINK_COMMAND_24BIT
#  include "../mavlink_get_info.h"
# endif
#endif

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MAVLINK_IHM_DIALECT_H
