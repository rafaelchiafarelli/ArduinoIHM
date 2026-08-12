#pragma once
#include <stdint.h>
#include <HardwareSerial.h>
#include "mavlink.h"

// Wraps the board's MAVLink wire protocol (see IHM/mavlink/README.md) --
// unrelated to lib/Comms/SerialCommunication, which is a separate,
// still-broken hand-rolled framing protocol this does not replace or touch.
class MavlinkComms
{
private:
    HardwareSerial *serial;
    mavlink_message_t rxMsg;
    mavlink_status_t rxStatus;

    // Latest received signal-generator configs, storage only. No CAN or
    // RS-485 transmit driver exists in this codebase yet (see
    // IHM/ARCHITECTURE.md's known gaps) -- these are decoded and held here
    // for whoever wires up the actual bus hardware next; nothing currently
    // acts on them.
    mavlink_can_signal_config_t canConfig[2];
    bool canConfigValid[2];
    mavlink_rs485_signal_config_t rs485Config;
    bool rs485ConfigValid;

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
                canConfigValid[cfg.bus_id] = true;
            }
            break;
        }
        case MAVLINK_MSG_ID_RS485_SIGNAL_CONFIG:
            mavlink_msg_rs485_signal_config_decode(&rxMsg, &rs485Config);
            rs485ConfigValid = true;
            break;
        default:
            break;
        }
    }

public:
    explicit MavlinkComms(HardwareSerial *serialPort)
        : serial(serialPort), canConfigValid{false, false}, rs485ConfigValid(false)
    {
    }

    // Feeds all currently-available bytes through the MAVLink parser and
    // dispatches any complete message. Call once per superloop pass; not
    // ISR-driven (see main.cpp), no fixed cadence requirement.
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
    void poll()
    {
        while (serial->available())
        {
            uint8_t c = (uint8_t)serial->read();
            if (mavlink_frame_char_buffer(&rxMsg, &rxStatus, c, NULL, NULL) == MAVLINK_FRAMING_OK)
            {
                dispatch();
            }
        }
    }

    // Packs and transmits one IHM_BOARD_STATE message. Field values are the
    // caller's responsibility to gather (see main.cpp) -- this class owns
    // the protocol, not the input/ADC reads.
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
        uint8_t buf[MAVLINK_MAX_PACKET_LEN];
        uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
        serial->write(buf, len);
    }

    // Returns nullptr if no config has been received yet for that bus.
    const mavlink_can_signal_config_t *getCanSignalConfig(uint8_t bus) const
    {
        if (bus >= 2 || !canConfigValid[bus])
            return nullptr;
        return &canConfig[bus];
    }

    const mavlink_rs485_signal_config_t *getRs485SignalConfig() const
    {
        return rs485ConfigValid ? &rs485Config : nullptr;
    }
};
