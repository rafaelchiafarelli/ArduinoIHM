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

class GUI {
private:
    Display tft; // Instantiate the display object
    WorkType currentWork;
    LED led = LED(100,100,2,10,0,(const char *)"LED1",0,&tft);
    Label label = Label(100,150,2,10,0,(const char *)"LED1",0,&tft);
    public:
    GUI(){
        

    }
    void setup(){
        // Any additional setup can be done here
                uint16_t ID = tft.readID(); // Read display ID
        tft.begin(ID); // Initialize the display
        tft.setRotation(0); // Adjust rotation as needed
        tft.fillScreen(BLACK); // Clear the screen
        
    }
    void someWork(){
        // Example: Draw a rectangle
        switch(currentWork){
            case WORK1:
                led.setState(0); // Set LED to blinking state
                led.setLabel("top");
                led.setLocation(1);
                label.setState(1);
                label.setLabel("Hello World");
                currentWork = WORK2;
                break;
            case WORK2:
                led.setState(1); // Set LED to blinking state
                led.setLocation(0);
                label.setState(0);
                
                currentWork = WORK3;
                break;
            case WORK3:
                led.setState(2); // Set LED to blinking state
                led.setLabel("bottom");
                led.setLocation(2);
                label.setState(2);
                label.setLabel("Start!");
                currentWork = WORK4;
                break;
            case WORK4:
                led.setState(3); // Set LED to blinking state
                led.setLocation(0);
                label.setState(0);
                
                currentWork = WORK5;
                break;
            case WORK5:
                led.setState(4); // Set LED to blinking state
                led.setLabel("left");
                led.setLocation(3);
                label.setState(3);
                label.setLabel("Begin!");
                currentWork = WORK6;
                break;  
            case WORK6:
                led.setState(4); // Set LED to blinking state
                led.setLocation(0);
                label.setState(0);
                
                currentWork = WORK7;
                break;                                 
            case WORK7:
                led.setState(4); // Set LED to blinking state
                led.setLabel("right");
                led.setLocation(4);
                label.setState(4);
                label.setLabel("Move!");                
                currentWork = WORK8;
                break;           
            case WORK8:
                led.setState(4); // Set LED to blinking state
                led.setLocation(0);
                currentWork = WORK1;
                break;                                                                       
        }
        

    }
    void update(){
        // Example: Draw a moving rectangle
        led.update();
        label.update();

    }
    ~GUI(){}


};

#endif // _GUI_H_
