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
    
    if(d0 == CCW){
        tabSelector.selectBefore();
    }

    if(d0 == CW){
        tabSelector.selectNext();
        if(tabSelector.getCurrentSelected() == PWM_SELECTED){
            pwmConfig.show();
        }
    }    

    if((btnMap & 0b01000000) == 0x00){
        tabSelector.selectCurrTab();
    }

    switch (tabSelector.getCurrentSelected())
    {
    case PWM_SELECTED:
        /* code */
        if((btnMap&0b01000000) == 0x00){
            pwmConfig.setCurrSelected(0,true);
        }

        if(d1 == CCW){
            pwmConfig.show();
            pwmConfig.setBeforeSelected();
        }

        if(d1 == CW){
            pwmConfig.show();
            pwmConfig.setNextSelected();
        }

        pwmConfig.update();
        break;
    case SERIAL_SELECTED:
        
        tft->drawFastHLine(0,200,50,WHITE);
        tft->drawFastHLine(0,220,100,WHITE);
        tft->drawFastHLine(0,240,150,WHITE);
        tft->drawFastHLine(0,260,200,WHITE);
        tft->drawFastHLine(0,280,250,WHITE);
        tft->drawFastHLine(0,300,300,WHITE);
        break;
    case OUPTUT_SELECTED:
        tft->fillRect(2,47,318,431,BLACK);
    break;
    default:
        tft->fillRect(2,47,318,431,WHITE);
        break;
    }

    uint8_t battLevel = (btnMap&0x01)? 128: 30;
    statusBar.setBattLevel(battLevel);
    statusBar.update();
    tabSelector.update();
}
