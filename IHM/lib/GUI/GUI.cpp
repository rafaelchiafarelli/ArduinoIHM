#include "GUI.h"

/**
 * This function will allow the user to use the device in 1 of 4 different angles
 */

void GUI::rotateGUI(RotateAngle angle){

}
/**
 * This function is the connection to the outside world, it receives commands and configurations for other modules.
*/
void GUI::receiveData(uint8_t btnByte, int16_t rot0, int16_t rot1, int16_t rot2, int16_t rot3, uint16_t an0,uint16_t an1,uint16_t an2,uint16_t an3){

}

/**
 * This function receives the avereage usage of the CPU. The more it is used, the higher the value in timeStat
 */
void GUI::updateTimeStatistics(uint16_t timeStat){
    // Update time statistics display on the GUI
    timeSlot.setState(1);
    timeSlot.setLabel("TS:----");
}
/**
 * Show a warning Icon at the top of the screen
 * 
 */
void GUI::showNormalOperation(WarningType type){
    // Display warning message on the GUI
    
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.setCursor(50, 150);
    tft.println("normal operation");
    
    //tft.fillScreen(BLACK); // Clear screen after warning
}
void GUI::showRegularLoop(WarningType type){
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.setCursor(50, 250);
    tft.println("normal loop");
}
/**
 * Show a warning Icon at the top of the screen
 * 
 */
void GUI::showWarning(WarningType type){
    // Display warning message on the GUI
    
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.setCursor(50, 100);
    tft.println("WARNING: TIMER0 OVERUN");
    
    //tft.fillScreen(BLACK); // Clear screen after warning
}
/**
 * show a emergency icon in the screen (above the screen) that serves to tell the user this is no longer a proper device to be used. 
 */
void GUI::showEmergency(WarningType type){
    // Display emergency message on the GUI
    
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.setCursor(50, 50);
    tft.println("EMERGENCY: TIMER0 OVERUN");
    
    //tft.fillScreen(BLACK); // Clear screen after emergency
}
/**
 * function to start the devices attached to the gui.
 */
void GUI::setup(){
    uint16_t ID = tft.readID(); // Read display ID
    tft.begin(ID); // Initialize the display
    tft.setRotation(0); // Adjust rotation as needed
    tft.fillScreen(BLACK); // Clear the screen
}

/**
 * function to change the screen of the state machine.
 */
void GUI::screenMachine(ScreenType screen){
    // Example: Draw a rectangle
    currentWork = screen;
    switch(currentWork){
        case IDLE_SCREEN:
            led.setState(0); // Set LED to blinking state
            led.setLabel("top");
            led.setLocation(1);
            label.setState(1);
            label.setLabel("IDLE_SCREEN");
            
            break;
        case CONF_PWM:
            led.setState(1); // Set LED to blinking state
            led.setLocation(0);
            label.setState(1);
            label.setLabel("CONF_PWM");
            
            break;
        case CONF_SERIAL:
            led.setState(2); // Set LED to blinking state
            led.setLabel("bottom");
            led.setLocation(2);
            label.setState(2);
            label.setLabel("CONF_SERIAL");
            
            break;
        case CONF_CAN:
            led.setState(3); // Set LED to blinking state
            led.setLocation(0);
            label.setState(2);
            label.setLabel("CONF_SERIAL");
            
            break;
        case CONF_RELAY:
            led.setState(4); // Set LED to blinking state
            led.setLabel("left");
            led.setLocation(3);
            label.setState(3);
            label.setLabel("CONF_RELAY");
            
            break;  
        case CONF_ANALOG_OUTPUT:
            led.setState(4); // Set LED to blinking state
            led.setLocation(0);
            label.setState(3);
            label.setLabel("CONF_ANALOG_OUTPUT");
            
            break;                                 
        case CONF_MULTIOUTPUT:
            led.setState(4); // Set LED to blinking state
            led.setLabel("right");
            led.setLocation(4);
            label.setState(4);
            label.setLabel("CONF_MULTIOUTPUT");                
            
            break;           
        case CONF_ANALOG_OUT:
            led.setState(4); // Set LED to blinking state
            led.setLocation(0);
            label.setState(4);
            label.setLabel("CONF_ANALOG_OUT");                
            
            break;                                                                       
    }
}
void GUI::update(){
    led.update();
    label.update();
    timeSlot.update();
}
