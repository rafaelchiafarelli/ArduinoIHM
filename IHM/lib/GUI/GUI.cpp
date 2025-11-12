#include "GUI.h"

void GUI::receiveData(uint16_t *data){
    // Process incoming data and update GUI elements accordingly
    // For example, update the LED state based on data[0]
    if(data[0] == 1){
        led.setState(1); // Turn LED on
    }else{
        led.setState(0); // Turn LED off
    }
    // Similarly, update label based on data[1]
    if(data[1] == 1){
        label.setLabel("Active");
    }else{
        label.setLabel("Inactive");
    }
}
void GUI::updateTimeStatistics(uint16_t timeStat){
    // Update time statistics display on the GUI
    label.setLabel("Time: ms");
}
void GUI::showWarning(WarningType type){
    // Display warning message on the GUI
    tft.fillScreen(BLACK);
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.setCursor(50, 100);
    tft.println("WARNING: TIMER0 OVERUN");
    _delay_ms(1000); // Display for 1 second
    tft.fillScreen(BLACK); // Clear screen after warning
}
void GUI::showEmergency(WarningType type){
    // Display emergency message on the GUI
    tft.fillScreen(BLACK);
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.setCursor(50, 100);
    tft.println("EMERGENCY: TIMER0 OVERUN");
    _delay_ms(2000); // Display for 2 seconds
    tft.fillScreen(BLACK); // Clear screen after emergency
}

void GUI::setup(){
    Serial.println("Display ID: 0x");
    // Any additional setup can be done here
    _delay_ms(500);  
    uint16_t ID = tft.readID(); // Read display ID

    Serial.println(ID, HEX);  
    _delay_ms(500);  
    tft.begin(ID); // Initialize the display
    tft.setRotation(0); // Adjust rotation as needed
    tft.fillScreen(BLACK); // Clear the screen
    
}
void GUI::stateMachine(){
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
void GUI::update(){
    // Example: Draw a moving rectangle
    led.update();
    label.update();
}