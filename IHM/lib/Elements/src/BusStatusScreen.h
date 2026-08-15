#pragma once

#include <PWMLabelFormat.h> // PWM_LABEL_BUFFER_SIZE -- same 15-byte cap as LABEL_STRING_SIZE
#include <MavlinkComms.h>
#include "BusStatusRow.h"

#define BUSSTATUSSCREEN_X0 4
#define BUSSTATUSSCREEN_Y0 47
#define BUSSTATUSSCREEN_ROW_H 53

/**
 * The SERIAL tab: a read-only monitor of the CAN0/CAN1/RS-485
 * signal-generator config the board has received from the PC app
 * (CAN_SIGNAL_CONFIG/RS485_SIGNAL_CONFIG -- see mavlink/README.md and
 * MavlinkComms::getCanSignalConfig()/getRs485SignalConfig(),
 * lib/MavlinkComms/src/MavlinkComms.h). Nothing here is editable from the
 * board -- the PC app (IHM-PCApp) is the intended editor now that it
 * exists -- so unlike PWMScreen/RelayScreen there's no selection cursor or
 * encoder-driven interaction, just three static rows.
 *
 * Each row keeps its own last-seen copy of its config to detect "this
 * changed since the last render" and pulse its LED briefly (see
 * BusStatusRow::setSummary's justChanged param). That comparison lives
 * entirely here, UI-side -- MavlinkComms itself is untouched, it already
 * stores everything needed.
 */
class BusStatusScreen {
    private:
        BusStatusRow can0Row;
        BusStatusRow can1Row;
        BusStatusRow rs485Row;
        MavlinkComms *mavlinkComms;

        bool can0EverValid = false, can1EverValid = false, rs485EverValid = false;
        mavlink_can_signal_config_t can0Last{}, can1Last{};
        mavlink_rs485_signal_config_t rs485Last{};

        static void formatCanSummary(char *buf, const mavlink_can_signal_config_t *cfg){
            if(!cfg){ snprintf(buf, PWM_LABEL_BUFFER_SIZE, "no config"); return; }
            snprintf(buf, PWM_LABEL_BUFFER_SIZE, "%lX D%u %s", (unsigned long)cfg->can_id, cfg->dlc, cfg->extended_id ? "EXT" : "STD");
        }
        static void formatRs485Summary(char *buf, const mavlink_rs485_signal_config_t *cfg){
            if(!cfg){ snprintf(buf, PWM_LABEL_BUFFER_SIZE, "no config"); return; }
            snprintf(buf, PWM_LABEL_BUFFER_SIZE, "LEN%u P%ums", cfg->length, cfg->period_ms);
        }

    public:
        BusStatusScreen(Display *tft, MavlinkComms *mavlinkComms):
            can0Row(BUSSTATUSSCREEN_X0, BUSSTATUSSCREEN_Y0 + 0 * BUSSTATUSSCREEN_ROW_H, BUSSTATUSSCREEN_ROW_H, "CAN0", tft),
            can1Row(BUSSTATUSSCREEN_X0, BUSSTATUSSCREEN_Y0 + 1 * BUSSTATUSSCREEN_ROW_H, BUSSTATUSSCREEN_ROW_H, "CAN1", tft),
            rs485Row(BUSSTATUSSCREEN_X0, BUSSTATUSSCREEN_Y0 + 2 * BUSSTATUSSCREEN_ROW_H, BUSSTATUSSCREEN_ROW_H, "RS485", tft),
            mavlinkComms(mavlinkComms)
        {
        }

        void update(){
            char buf[PWM_LABEL_BUFFER_SIZE];

            const mavlink_can_signal_config_t *can0 = mavlinkComms->getCanSignalConfig(0);
            bool can0Changed = can0 && (!can0EverValid || memcmp(can0, &can0Last, sizeof(*can0)) != 0);
            if(can0){ can0Last = *can0; can0EverValid = true; }
            formatCanSummary(buf, can0);
            can0Row.setSummary(buf, can0 && can0->enable, can0Changed);

            const mavlink_can_signal_config_t *can1 = mavlinkComms->getCanSignalConfig(1);
            bool can1Changed = can1 && (!can1EverValid || memcmp(can1, &can1Last, sizeof(*can1)) != 0);
            if(can1){ can1Last = *can1; can1EverValid = true; }
            formatCanSummary(buf, can1);
            can1Row.setSummary(buf, can1 && can1->enable, can1Changed);

            const mavlink_rs485_signal_config_t *rs485 = mavlinkComms->getRs485SignalConfig();
            bool rs485Changed = rs485 && (!rs485EverValid || memcmp(rs485, &rs485Last, sizeof(*rs485)) != 0);
            if(rs485){ rs485Last = *rs485; rs485EverValid = true; }
            formatRs485Summary(buf, rs485);
            rs485Row.setSummary(buf, rs485 && rs485->enable, rs485Changed);

            can0Row.update();
            can1Row.update();
            rs485Row.update();
        }

        void show(){
            can0Row.show();
            can1Row.show();
            rs485Row.show();
        }
};
