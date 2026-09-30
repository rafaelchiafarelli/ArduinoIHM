#pragma once
#include <stdint.h>
#include <util/atomic.h>
#include "mavlink.h"
#include "Uart2.h"
#include "RelayCommand.h"

// Protocol port baud (Serial2/USART2). 250000 is exact at 16 MHz with U2X
// and is what mavlink/scripts/*.py default to.
#ifndef MAVLINK_SERIAL_BAUD
#define MAVLINK_SERIAL_BAUD 250000UL
#endif

// Max bytes fast_handler() parses per Timer2 tick (~1.008 ms). 250000 baud
// is ~25 B/ms, so 32 keeps up with a saturated line; the 64-byte RX ring
// absorbs bursts in between.
#ifndef MAVLINK_RX_BYTES_PER_TICK
#define MAVLINK_RX_BYTES_PER_TICK 32
#endif

// Wraps the board's MAVLink wire protocol (see IHM/mavlink/README.md) --
// unrelated to lib/Comms/SerialCommunication, which is a separate,
// still-broken hand-rolled framing protocol this does not replace or touch.
//
// Runs on Serial2 through lib/Uart2 (never on Serial0, which is debug-only).
// Threading: fast_handler() -- and so dispatch() and every storage slot
// below -- runs in the Timer2 ISR. The superloop only reaches that storage
// through the take*/consume*/get* copy-outs, each an ATOMIC_BLOCK, so a
// frame landing mid-read can never tear a struct. send*() are superloop-only.
class MavlinkComms
{
private:
    mavlink_message_t rxMsg;
    mavlink_status_t rxStatus;

    // Latest received signal-generator configs (301/302), plus a pending
    // flag set by dispatch() and cleared by take*SignalConfig() (superloop),
    // which applies them to its SerialConfig. Last writer wins: a newer
    // frame replaces an un-taken older one. Storage only.
    mavlink_can_signal_config_t canConfig[2];
    bool canConfigDirty[2];
    mavlink_rs485_signal_config_t rs485Config;
    bool rs485ConfigDirty;

    // Pending IHM_SERIAL_SETTING frames, oldest first. Each one is answered
    // (IHM_SERIAL_SETTING_STATE), so they queue instead of replacing each
    // other; a frame arriving with the queue full is dropped.
    static const uint8_t SERIAL_SETTING_QUEUE = 4;
    mavlink_ihm_serial_setting_t settingQueue[SERIAL_SETTING_QUEUE];
    uint8_t settingCount;

    // Pending simulated encoder turns, one slot per encoder (hardcoded 3,
    // same as canConfig[2] hardcodes 2 CAN buses -- this class stays
    // decoupled from RotaryEncoder.h/MAX_NUMBER_EMCODERS, see the
    // initiative's layering rule). Raw wire values, not DIRECTION_TYPE.
    uint8_t simulatedEncoderDirection[3];
    bool simulatedEncoderPending[3];

    // Simulated button presses (IHM_SIMULATE_BUTTON), one bit per button in
    // IHM_BOARD_STATE.buttons layout. Bits accumulate until consumed so two
    // frames arriving in one pass are not lost.
    uint8_t simulatedButtonMask;

    // Latest PWM_CHANNEL_CONFIG per channel, plus a pending flag set by
    // dispatch() (via fast_handler()) and cleared by takePwmChannelConfig()
    // (superloop). Last writer wins: a newer frame for a channel replaces an
    // un-taken older one. Storage only -- this class never touches PWM.
    mavlink_pwm_channel_config_t pwmConfig[4];
    bool pwmConfigDirty[4];

    // Pending IHM_RELAY_COMMAND, not yet taken by the superloop. Frames that
    // arrive before takeRelayCommand() are folded together
    // (relayCommandAccumulate), so none is lost. Storage only -- this class
    // never touches the relays.
    uint8_t relayCmdMask;
    uint8_t relayCmdState;

    // Latest IHM_DAC_COMMAND per DAC, plus a pending flag cleared by
    // takeDacCommand() (superloop). Last writer wins. Storage only -- this
    // class never touches I2C.
    uint16_t dacValue[2];
    bool dacPending[2];

    void dispatch()
    {
        switch (rxMsg.msgid)
        {
        case MAVLINK_MSG_ID_CAN_SIGNAL_CONFIG:
        {
            mavlink_can_signal_config_t cfg;
            mavlink_msg_can_signal_config_decode(&rxMsg, &cfg);
            if (cfg.bus_id < 2)
            {
                canConfig[cfg.bus_id] = cfg;
                canConfigDirty[cfg.bus_id] = true;
            }
            break;
        }
        case MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG:
            mavlink_msg_rs485_signal_config_decode(&rxMsg, &rs485Config);
            rs485ConfigDirty = true;
            break;
        case MAVLINK_MSG_ID_IHM_SERIAL_SETTING:
            if (settingCount < SERIAL_SETTING_QUEUE)
            {
                mavlink_msg_ihm_serial_setting_decode(&rxMsg, &settingQueue[settingCount]);
                settingCount++;
            }
            break;
        case MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER:
        {
            mavlink_ihm_simulate_encoder_t cmd;
            mavlink_msg_ihm_simulate_encoder_decode(&rxMsg, &cmd);
            if (cmd.encoder < 3)
            {
                simulatedEncoderDirection[cmd.encoder] = cmd.direction;
                simulatedEncoderPending[cmd.encoder] = true;
            }
            break;
        }
        case MAVLINK_MSG_ID_IHM_SIMULATE_BUTTON:
        {
            mavlink_ihm_simulate_button_t cmd;
            mavlink_msg_ihm_simulate_button_decode(&rxMsg, &cmd);
            simulatedButtonMask |= (cmd.button_mask & 0x7F);
            break;
        }
        case MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG:
        {
            mavlink_pwm_channel_config_t cfg;
            mavlink_msg_pwm_channel_config_decode(&rxMsg, &cfg);
            if (cfg.channel < 4)
            {
                pwmConfig[cfg.channel] = cfg;
                pwmConfigDirty[cfg.channel] = true;
            }
            break;
        }
        case MAVLINK_MSG_ID_IHM_RELAY_COMMAND:
        {
            mavlink_ihm_relay_command_t cmd;
            mavlink_msg_ihm_relay_command_decode(&rxMsg, &cmd);
            relayCommandAccumulate(relayCmdMask, relayCmdState, cmd.mask, cmd.state);
            break;
        }
        case MAVLINK_MSG_ID_IHM_DAC_COMMAND:
        {
            mavlink_ihm_dac_command_t cmd;
            mavlink_msg_ihm_dac_command_decode(&rxMsg, &cmd);
            if (cmd.channel < 2 && cmd.value <= 4095)
            {
                dacValue[cmd.channel] = cmd.value;
                dacPending[cmd.channel] = true;
            }
            break;
        }
        default:
            break;
        }
    }

    // All-or-nothing hand-off to the TX ring; the UDRE ISR does the rest.
    // A frame that doesn't fit is dropped (uart2::txDroppedCount()) --
    // telemetry is best-effort, the next ~100 ms frame replaces it.
    void sendMessage(const mavlink_message_t *msg)
    {
        uint8_t buf[MAVLINK_MAX_PACKET_LEN];
        uint16_t len = mavlink_msg_to_send_buffer(buf, msg);
        uart2::writeFrame(buf, len);
    }

public:
    MavlinkComms()
        : canConfigDirty{false, false}, rs485ConfigDirty(false), settingCount(0),
          simulatedEncoderPending{false, false, false}, simulatedButtonMask(0), pwmConfigDirty{false, false, false, false},
          relayCmdMask(0), relayCmdState(0), dacValue{0, 0}, dacPending{false, false}
    {
    }

    void begin(uint32_t baud)
    {
        uart2::begin(baud);
    }

    // Parses at most MAVLINK_RX_BYTES_PER_TICK bytes from the Serial2 RX ring
    // and dispatches any complete message into the storage slots. Called from
    // the Timer2 tick (see main.cpp): bounded, no Serial, no heap.
    //
    // Uses mavlink_frame_char_buffer(), not the more commonly-shown
    // mavlink_parse_char() -- the latter's error-handling path references
    // mavlink_get_channel_buffer()/mavlink_get_channel_status(), which pulls
    // in the library's own internal static mavlink_message_t[4] (~400 bytes
    // on this build's 64-byte payload cap) regardless of the rxMsg/rxStatus
    // we already own. mavlink_frame_char_buffer() is the library's own
    // documented "no global variables" variant -- passing NULL for the
    // r_message/r_mavlink_status output params is fine, since a complete
    // message is already sitting in rxMsg itself once this returns
    // MAVLINK_FRAMING_OK; no separate output copy is needed.
    void fast_handler()
    {
        uint8_t c;
        for (uint8_t n = 0; n < MAVLINK_RX_BYTES_PER_TICK && uart2::read(&c); n++)
        {
            if (mavlink_frame_char_buffer(&rxMsg, &rxStatus, c, NULL, NULL) == MAVLINK_FRAMING_OK)
            {
                dispatch();
            }
        }
    }

    // Packs and queues one IHM_BOARD_STATE message. Field values are the
    // caller's responsibility to gather (see main.cpp) -- this class owns
    // the protocol, not the input/ADC reads. Superloop-only.
    void sendBoardState(uint8_t buttons,
                         uint8_t encoder0Direction, uint8_t encoder1Direction, uint8_t encoder2Direction,
                         uint8_t rotation, uint8_t charging, uint16_t battVoltage,
                         const uint16_t analogIn[4],
                         uint16_t timeStatistics, uint8_t timeCounter)
    {
        mavlink_message_t msg;
        mavlink_msg_ihm_board_state_pack(1, 1, &msg,
                                          buttons, encoder0Direction, encoder1Direction, encoder2Direction,
                                          rotation, charging, battVoltage, analogIn,
                                          timeStatistics, timeCounter);
        sendMessage(&msg);
    }

    // Packs and queues one IHM_RELAY_STATE message. bitmask's bit i is
    // relay i (0-7); caller (main.cpp) builds it from relayState[], this
    // class only packs/sends it -- same split as sendBoardState().
    void sendRelayState(uint8_t relayBitmask)
    {
        mavlink_message_t msg;
        mavlink_msg_ihm_relay_state_pack(1, 1, &msg, relayBitmask);
        sendMessage(&msg);
    }

    // Packs and queues one IHM_DAC_STATE message: the code last written to
    // each DAC and the bitmask of DACs that ACKed. Caller (main.cpp) owns
    // both; this class only packs/sends them.
    void sendDacState(const uint16_t value[2], uint8_t presentMask)
    {
        mavlink_message_t msg;
        mavlink_msg_ihm_dac_state_pack(1, 1, &msg, value, presentMask);
        sendMessage(&msg);
    }

    // Packs and queues one IHM_PWM_STATE message: the config currently
    // applied to one PWM channel, same field layout as PWM_CHANNEL_CONFIG.
    // Caller (main.cpp) passes its last-applied config for that channel; this
    // class only packs/sends it -- same split as sendRelayState().
    void sendPwmState(const mavlink_ihm_pwm_state_t &state)
    {
        mavlink_message_t msg;
        mavlink_msg_ihm_pwm_state_encode(1, 1, &msg, &state);
        sendMessage(&msg);
    }

    // Packs and queues one IHM_UI_STATE message: Janus's remote UI state
    // (janus_remote_state_t) as four scalars, so this class stays free of
    // Janus headers. Caller (main.cpp) fills them from
    // janus_remote_state_get() -- same split as sendRelayState().
    void sendUiState(uint16_t screen, int16_t focus, int16_t navFocus, uint16_t boxesExpanded)
    {
        mavlink_message_t msg;
        mavlink_msg_ihm_ui_state_pack(1, 1, &msg, screen, focus, navFocus, boxesExpanded);
        sendMessage(&msg);
    }

    // Packs and queues one IHM_CAN_SIGNAL_STATE / IHM_RS485_SIGNAL_STATE: the
    // generator config main.cpp holds for that bus (its SerialConfig), same
    // fields as 301/302. Caller fills it; this class only packs/sends it.
    void sendCanSignalState(const mavlink_ihm_can_signal_state_t &state)
    {
        mavlink_message_t msg;
        mavlink_msg_ihm_can_signal_state_encode(1, 1, &msg, &state);
        sendMessage(&msg);
    }

    void sendRs485SignalState(const mavlink_ihm_rs485_signal_state_t &state)
    {
        mavlink_message_t msg;
        mavlink_msg_ihm_rs485_signal_state_encode(1, 1, &msg, &state);
        sendMessage(&msg);
    }

    // Packs and queues one IHM_SERIAL_SETTING_STATE (the answer to an
    // IHM_SERIAL_SETTING, or a keyed setting's periodic readback).
    void sendSerialSettingState(uint8_t bus, uint16_t key, int32_t value, uint8_t status)
    {
        mavlink_message_t msg;
        mavlink_msg_ihm_serial_setting_state_pack(1, 1, &msg, bus, key, value, status);
        sendMessage(&msg);
    }

    // Superloop-side hand-off of the latest PWM_CHANNEL_CONFIG for channel
    // `ch` that fast_handler() stored. Returns false if ch >= 4 or nothing
    // new has arrived for it; otherwise copies it to *out, clears the
    // pending flag and returns true -- atomically, since fast_handler()
    // writes the slot from the Timer2 ISR. No validation beyond the channel
    // bound here -- main.cpp validates with pwmWireConfigValid() before
    // touching hardware.
    bool takePwmChannelConfig(uint8_t ch, mavlink_pwm_channel_config_t *out)
    {
        if (ch >= 4)
            return false;
        bool taken = false;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            if (pwmConfigDirty[ch])
            {
                pwmConfigDirty[ch] = false;
                *out = pwmConfig[ch];
                taken = true;
            }
        }
        return taken;
    }

    // Superloop-side hand-off of the pending IHM_RELAY_COMMAND: false if none
    // arrived since the last call; otherwise copies mask/state out, clears
    // them and returns true -- atomically, as dispatch() writes them from the
    // Timer2 ISR. main.cpp applies it (relayCommandApply semantics).
    bool takeRelayCommand(uint8_t *mask, uint8_t *state)
    {
        bool taken = false;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            if (relayCmdMask)
            {
                *mask = relayCmdMask;
                *state = relayCmdState;
                relayCmdMask = 0;
                relayCmdState = 0;
                taken = true;
            }
        }
        return taken;
    }

    // Superloop-side hand-off of the latest IHM_DAC_COMMAND for DAC `ch`:
    // false if ch >= 2 or nothing new arrived; otherwise copies the code out
    // and clears the pending flag, atomically (same pattern as
    // takePwmChannelConfig). The code is already range-checked (0-4095).
    bool takeDacCommand(uint8_t ch, uint16_t *value)
    {
        if (ch >= 2)
            return false;
        bool taken = false;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            if (dacPending[ch])
            {
                dacPending[ch] = false;
                *value = dacValue[ch];
                taken = true;
            }
        }
        return taken;
    }

    // Returns 0 (not_supported) if encoderIndex is out of range or nothing
    // is pending for it; otherwise clears the pending flag and returns the
    // stored direction (raw wire value -- caller casts to DIRECTION_TYPE).
    // One-shot by design, mirroring RotaryEncoder::getDirection()'s own
    // consumed-on-read behavior for real turns.
    uint8_t consumeSimulatedEncoderDirection(uint8_t encoderIndex)
    {
        if (encoderIndex >= 3)
            return 0;
        uint8_t dir = 0;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            if (simulatedEncoderPending[encoderIndex])
            {
                simulatedEncoderPending[encoderIndex] = false;
                dir = simulatedEncoderDirection[encoderIndex];
            }
        }
        return dir;
    }

    // Returns the bitmask of simulated buttons pressed since the last call
    // (bit layout as IHM_BOARD_STATE.buttons) and clears it, so each
    // IHM_SIMULATE_BUTTON is one press for exactly one superloop pass.
    uint8_t consumeSimulatedButtons()
    {
        uint8_t m;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            m = simulatedButtonMask;
            simulatedButtonMask = 0;
        }
        return m;
    }

    // Superloop-side hand-off of the latest CAN_SIGNAL_CONFIG for `bus`:
    // false if bus >= 2 or nothing new arrived; otherwise copies it out and
    // clears the pending flag, atomically (same pattern as
    // takePwmChannelConfig). main.cpp validates it (canGeneratorFromWire).
    bool takeCanSignalConfig(uint8_t bus, mavlink_can_signal_config_t *out)
    {
        if (bus >= 2)
            return false;
        bool taken = false;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            if (canConfigDirty[bus])
            {
                canConfigDirty[bus] = false;
                *out = canConfig[bus];
                taken = true;
            }
        }
        return taken;
    }

    bool takeRs485SignalConfig(mavlink_rs485_signal_config_t *out)
    {
        bool taken = false;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            if (rs485ConfigDirty)
            {
                rs485ConfigDirty = false;
                *out = rs485Config;
                taken = true;
            }
        }
        return taken;
    }

    // Superloop-side hand-off of the oldest pending IHM_SERIAL_SETTING;
    // false if none. main.cpp dispatches it by key and answers it.
    bool takeSerialSetting(mavlink_ihm_serial_setting_t *out)
    {
        bool taken = false;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            if (settingCount > 0)
            {
                *out = settingQueue[0];
                for (uint8_t i = 1; i < settingCount; i++)
                    settingQueue[i - 1] = settingQueue[i];
                settingCount--;
                taken = true;
            }
        }
        return taken;
    }
};
