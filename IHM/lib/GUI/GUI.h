#ifndef _GUI_H_
#define _GUI_H_

#include <standardDefinitions.h>
#include <Widgets.h>

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
    CONF_MULTIOUTPUT,
    CONF_ANALOG_OUT
}ScreenType;

typedef enum {
    TIMER0_OVERUN,
    TIMER0_WARNING
}WarningType;

class GUI {
private:
    Display tft; // Instantiate the display object
    ScreenType currentWork = IDLE_SCREEN;

    TabSelector tabSelector = TabSelector(&tft);


    LED led = LED(100,100,2,10,0,(const char *)"LED1",0,&tft);
    Label timeSlot = Label(25,2,2,10,0,(const char *)"TS:----",0,&tft);
    Label label = Label(100,200,2,10,0,(const char *)"LABEL1",0,&tft);

    public:
    void screenMachine(ScreenType screen);
    void update();
    void setup();
    void rotateGUI(RotateAngle angle);
    void receiveData(uint8_t btnByte, int16_t rot0, int16_t rot1, int16_t rot2, int16_t rot3, uint16_t an0,uint16_t an1,uint16_t an2,uint16_t an3);
    void updateTimeStatistics(uint16_t timeStat);
    void showWarning(WarningType type);
    void showEmergency(WarningType type);
    void showNormalOperation(WarningType type);
    void showRegularLoop(WarningType type);
    GUI(){
        

    }
    ~GUI(){}


};

#endif // _GUI_H_
