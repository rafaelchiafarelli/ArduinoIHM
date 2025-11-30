#include "GUI.h"
#include "RotaryEncoder.h"
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
void GUI::updateTimeStatistics(double timeStat){
    uint8_t timeStatTrunc = (uint8_t)timeStat;
    statusBar.setTimeSlice(timeStatTrunc);
}
/**
 * Show a warning Icon at the top of the screen
 * 
 */
void GUI::showNormalOperation(WarningType type){
    // Display warning message on the GUI
    
    tft->setTextSize(2);
    tft->setTextColor(RED);
    tft->setCursor(50, 150);
    tft->println("normal operation");
    
    //tft->fillScreen(BLACK); // Clear screen after warning
}
void GUI::showRegularLoop(WarningType type){
    tft->setTextSize(2);
    tft->setTextColor(RED);
    tft->setCursor(50, 250);
    tft->println("normal loop");
}
/**
 * Show a warning Icon at the top of the screen
 * 
 */
void GUI::showWarning(WarningType type){
    // Display warning message on the GUI
    
    tft->setTextSize(2);
    tft->setTextColor(RED);
    tft->setCursor(50, 100);
    tft->println("WARNING: TIMER0 OVERUN");
    
    //tft->fillScreen(BLACK); // Clear screen after warning
}
/**
 * show a emergency icon in the screen (above the screen) that serves to tell the user this is no longer a proper device to be used. 
 */
void GUI::showEmergency(WarningType type){
    // Display emergency message on the GUI
    
    tft->setTextSize(2);
    tft->setTextColor(RED);
    tft->setCursor(50, 50);
    tft->println("EMERGENCY: TIMER0 OVERUN");
    
    //tft->fillScreen(BLACK); // Clear screen after emergency
}
/**
 * function to start the devices attached to the gui.
 */
void GUI::setup(){
    uint16_t ID = tft->readID(); // Read display ID
    tft->begin(ID); // Initialize the display
    tft->setRotation(0); // Adjust rotation as needed
    tft->fillScreen(BLACK); // Clear the screen
}


void GUI::update(DIRECTION_TYPE d0,DIRECTION_TYPE d1, DIRECTION_TYPE d2, uint8_t btnMap){
    if(d0 != not_supported){
        tabSelector.selectNext();
    }
    tabSelector.update();
    switch (tabSelector.getCurrentSelected())
    {
    case PWM_SELECTED:
        /* code */
        pwmConfig.update();
        break;
    case SERIAL_SELECTED:
        
        break;
    default:
    tft->fillRect(2,47,318,431,BLACK);
        break;
    }
    uint8_t battLevel = (btnMap&0x01)? 128: 30;
    statusBar.setBattLevel(battLevel);
    statusBar.update();

}
