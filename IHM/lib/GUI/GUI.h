#ifndef _GUI_H_
#define _GUI_H_

#include <standardDefinitions.h>
#include <Widgets.h>
#include <PWMScreen.h>
#include <PWM.h>
#include <RelayScreen.h>
#include <Relay.h>
#include <BusStatusScreen.h>
#include <MavlinkComms.h>
#include "RotaryEncoder.h"

class GUI {
    private:
        Display *tft; // Instantiate the display object
        StatusBar statusBar;
        TabSelector tabSelector;
        PWMScreen pwmConfig;
        RelayScreen relayConfig;
        BusStatusScreen busStatus;

    public:
        void update(DIRECTION_TYPE d0,DIRECTION_TYPE d1, DIRECTION_TYPE d2, uint8_t btnMap);
        void setup();
        void updateTimeStatistics(double timeStat);

        GUI(Display *tft, PWM *pwm, Relay *relay, MavlinkComms *mavlinkComms):
            tft(tft), statusBar(tft), tabSelector(2,21,tft), pwmConfig(tft,pwm),
            relayConfig(tft,relay), busStatus(tft, mavlinkComms){

        }
        ~GUI(){}
};

#endif // _GUI_H_
