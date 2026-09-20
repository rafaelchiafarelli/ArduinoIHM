#pragma once
#include <stdint.h>
#include <HardwareSerial.h>
#include <util/atomic.h>
#include "mavlink.h"

// Max bytes tick() pulls out of the serial RX ring per call. tick() runs in
// the ~1 ms Timer2 ISR (see main.cpp), so this is the hard bound on that
// ISR's added cost. 250000 baud delivers at most ~25 B/ms; 16 B/tick easily
// sustains any real command rate and the core's 64 B ring absorbs bursts.
// Override with -D if a sustained-rate use ever appears -- and re-measure
// (see initiatives/serial_commands/epics/serial_transport/README.md).
#ifndef MAVLINK_RX_BYTES_PER_TICK
#define MAVLINK_RX_BYTES_PER_TICK 16
#endif

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

    // Pending simulated encoder turns, one slot per encoder (hardcoded 3,
    // same as canConfig[2] hardcodes 2 CAN buses -- this class stays
    // decoupled from RotaryEncoder.h/MAX_NUMBER_EMCODERS, see the
    // initiative's layering rule). Raw wire values, not DIRECTION_TYPE.
    uint8_t simulatedEncoderDirection[3];
    bool simulatedEncoderPending[3];

    // Telemetry frames dropped because the TX ring couldn't hold them
    // (saturating). See sendFrame().
    uint8_t txDropped;

    // Non-blocking transmit: HardwareSerial::write() spins when the TX ring
    // is full, which would stall the superloop behind a slow/absent PC.
    // Telemetry is periodic, so dropping a frame is harmless -- the next
    // tick's frame replaces it.
    bool sendFrame(const uint8_t *buf, uint16_t len)
    {
        if (serial->availableForWrite() < (int)len)
        {
            if (txDropped < 255)
                txDropped++;
            return false;
        }
        serial->write(buf, len);
        return true;
    }

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
        default:
            break;
        }
    }

public:
    explicit MavlinkComms(HardwareSerial *serialPort)
        : serial(serialPort), canConfigValid{false, false}, rs485ConfigValid(false),
          simulatedEncoderPending{false, false, false}, txDropped(0)
    {
    }

    // Feeds at most MAVLINK_RX_BYTES_PER_TICK already-received bytes through
    // the MAVLink parser and dispatches any complete message into the
    // storage slots below. Runs from the Timer2 ISR (main.cpp), so it is
    // bounded, never blocks and never waits for data: an empty RX ring
    // returns immediately. It only decodes and sets pending flags -- nothing
    // here (or reachable from here) drives hardware; the superloop does that
    // through the take*() accessors.
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
    void tick()
    {
        for (uint8_t n = 0; n < MAVLINK_RX_BYTES_PER_TICK && serial->available(); n++)
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
        sendFrame(buf, len);
    }

    // Packs and transmits one IHM_RELAY_STATE message. bitmask's bit i is
    // relay i (0-7); caller (main.cpp) builds it from relayState[], this
    // class only packs/sends it -- same split as sendBoardState().
    void sendRelayState(uint8_t relayBitmask)
    {
        mavlink_message_t msg;
        mavlink_msg_ihm_relay_state_pack(1, 1, &msg, relayBitmask);
        uint8_t buf[MAVLINK_MAX_PACKET_LEN];
        uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
        sendFrame(buf, len);
    }

    // Number of telemetry frames dropped so far (saturates at 255).
    uint8_t txDroppedCount() const { return txDropped; }

    // Superloop-side hand-off of one simulated encoder turn that tick()
    // (ISR context) stored. Returns false if encoderIndex is out of range
    // or nothing is pending; otherwise copies the raw wire direction to
    // *direction, clears the pending flag and returns true. The copy+clear is
    // atomic against tick(). One-shot by design, mirroring
    // RotaryEncoder::getDirection()'s own consumed-on-read behavior for real
    // turns. Caller casts *direction to DIRECTION_TYPE.
    bool takeSimulatedEncoderDirection(uint8_t encoderIndex, uint8_t *direction)
    {
        if (encoderIndex >= 3)
            return false;
        bool taken = false;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            if (simulatedEncoderPending[encoderIndex])
            {
                simulatedEncoderPending[encoderIndex] = false;
                *direction = simulatedEncoderDirection[encoderIndex];
                taken = true;
            }
        }
        return taken;
    }

    // NOTE: the two getters below hand out pointers into storage that tick()
    // (ISR) may overwrite, so a read can tear. That is tolerable only while
    // nothing but the display reads them (see main.cpp
    // refreshBusStatusInstance); the task that adds the first real consumer
    // must convert these to take*() atomic copy-outs, like the ones above.
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
