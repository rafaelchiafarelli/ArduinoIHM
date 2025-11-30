#ifndef _GUI_H_
#define _GUI_H_

#include <standardDefinitions.h>
#include <Widgets.h>
#include <PWMScreen.h>
#include "RotaryEncoder.h"

typedef enum {
    PORTRAIT,
    LANDSCAPE,
    PORTRAIT_SIDEWAYS,
    LANDSCAPE_SIDEWAYS
}RotateAngle;


typedef enum {
    IDLE_SCREEN,
    CONF_PWM,
    CONF_SERIAL,
    CONF_CAN,
    CONF_ANALOG_OUTPUT,
    CONF_RELAY,
    CONF_SERVO_MOTOR,
    CONF_LINEAR_MOTOR,
    CONF_STEPPER_MOTOR,
    CONF_ANALOG_OUT
}ScreenType;

typedef enum {
    TIMER0_OVERUN,
    TIMER0_WARNING
}WarningType;

class GUI {
    private:
        Display *tft; // Instantiate the display object
        ScreenType currentWork = IDLE_SCREEN;
        StatusBar statusBar;
        TabSelector tabSelector;
        PWMScreen pwmConfig;

    public:
        void update(DIRECTION_TYPE d0,DIRECTION_TYPE d1, DIRECTION_TYPE d2, uint8_t btnMap);
        void setup();
        void rotateGUI(RotateAngle angle);
        void receiveData(uint8_t btnByte, int16_t rot0, int16_t rot1, int16_t rot2, int16_t rot3, uint16_t an0,uint16_t an1,uint16_t an2,uint16_t an3);
        void updateTimeStatistics(double timeStat);
        void showWarning(WarningType type);
        void showEmergency(WarningType type);
        void showNormalOperation(WarningType type);
        void showRegularLoop(WarningType type);

        GUI(Display *tft):tft(tft), statusBar(tft),tabSelector(2,21,tft),pwmConfig(tft){

        }
        ~GUI(){}
};

#endif // _GUI_H_
