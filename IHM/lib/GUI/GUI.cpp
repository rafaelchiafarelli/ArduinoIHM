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
        if(tabSelector.getCurrentSelected() == PWM_SELECTED){
            pwmConfig.show();
        }
        if(tabSelector.getCurrentSelected() == OUPTUT_SELECTED){
            relayConfig.show();
        }
    }

    if(d0 == CW){
        tabSelector.selectNext();
        if(tabSelector.getCurrentSelected() == PWM_SELECTED){
            pwmConfig.show();
        }
        if(tabSelector.getCurrentSelected() == OUPTUT_SELECTED){
            relayConfig.show();
        }
    }

    if((btnMap & 0b01000000) == 0x00){
        tabSelector.selectCurrTab();
    }

    switch (tabSelector.getCurrentSelected())
    {
    case PWM_SELECTED:
        /**
         * rot1 (d1/its button) drives two navigation levels so a third
         * encoder isn't needed: its button toggles between selecting which
         * of the 4 channels is highlighted, and editing that channel's
         * fields. While editing, d1 moves between fields and d2 adjusts the
         * highlighted field's value -- applied to the real timer
         * immediately by PWMScreen (see PWMScreen.h / PWMSimplex::editField
         * / PWMComplex::editField).
         */
        if((btnMap & 0b00100000) == 0x00){
            pwmConfig.toggleEditMode();
        }

        if(!pwmConfig.isEditingField()){
            if(d1 == CCW) pwmConfig.selectPreviousChannel();
            if(d1 == CW) pwmConfig.selectNextChannel();
        } else {
            if(d1 == CCW) pwmConfig.selectPreviousField();
            if(d1 == CW) pwmConfig.selectNextField();
            if(d2 != not_supported) pwmConfig.adjustSelectedField(d2);
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
        /**
         * rot1 (d1) moves the highlighted relay row; its button (same bit
         * PWM_SELECTED uses for its own tab-local meaning, safe to reuse
         * since it's scoped to this case) toggles that relay on/off --
         * applied to the real relay immediately by RelayScreen (see
         * RelayScreen.h / RelayElement::toggle).
         */
        if(d1 == CCW) relayConfig.selectPrevious();
        if(d1 == CW) relayConfig.selectNext();
        if((btnMap & 0b00100000) == 0x00){
            relayConfig.toggleSelected();
        }
        relayConfig.update();
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
