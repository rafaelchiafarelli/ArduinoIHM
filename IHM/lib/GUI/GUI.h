#ifndef _GUI_H_
#define _GUI_H_
#include <standardDefinitions.h>

#include <LED.h>
#include <Label.h>
typedef enum {
    WORK1,
    WORK2,
    WORK3,
    WORK4,
    WORK5,
    WORK6,
    WORK7,
    WORK8
}WorkType;

typedef enum {
    TIMER0_OVERUN,
    TIMER0_WARNING
}WarningType;

class GUI {
private:
    Display tft; // Instantiate the display object
    WorkType currentWork;
    LED led = LED(100,100,2,10,0,(const char *)"LED1",0,&tft);
    Label label = Label(100,150,2,10,0,(const char *)"LED1",0,&tft);
    public:
    void stateMachine();
    void update();
    void setup();
    void receiveData(uint16_t *data);
    void updateTimeStatistics(uint16_t timeStat);
    void showWarning(WarningType type);
    void showEmergency(WarningType type);
    GUI(){
        

    }
    ~GUI(){}


};

#endif // _GUI_H_
