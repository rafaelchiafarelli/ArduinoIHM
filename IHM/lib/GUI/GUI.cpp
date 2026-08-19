#include "GUI.h"
#include "RotaryEncoder.h"
#include "ButtonMap.h"

/**
 * This function receives the avereage usage of the CPU. The more it is used, the higher the value in timeStat
 */
void GUI::updateTimeStatistics(double timeStat){
    uint8_t timeStatTrunc = (uint8_t)timeStat;
    statusBar.setTimeSlice(timeStatTrunc);
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
        tft->fillRect(2,47,318,431,BLACK); // clear content area -- each screen's show() only redraws its own widgets, not the background left by the previous tab
        if(tabSelector.getCurrentSelected() == PWM_SELECTED){
            pwmConfig.show();
        }
        if(tabSelector.getCurrentSelected() == SERIAL_SELECTED){
            busStatus.show();
        }
        if(tabSelector.getCurrentSelected() == OUPTUT_SELECTED){
            relayConfig.show();
        }
    }

    if(d0 == CW){
        tabSelector.selectNext();
        tft->fillRect(2,47,318,431,BLACK); // clear content area -- see CCW branch above
        if(tabSelector.getCurrentSelected() == PWM_SELECTED){
            pwmConfig.show();
        }
        if(tabSelector.getCurrentSelected() == SERIAL_SELECTED){
            busStatus.show();
        }
        if(tabSelector.getCurrentSelected() == OUPTUT_SELECTED){
            relayConfig.show();
        }
    }

    if((btnMap & BTN_MASK_ROT0) == 0x00){
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
        if((btnMap & BTN_MASK_ROT1) == 0x00){
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
        // Read-only monitor of the CAN0/CAN1/RS-485 config the PC app has
        // sent -- see BusStatusScreen.h. Nothing to edit from here, so d1/
        // d2/the tab-local button all do nothing on this tab.
        busStatus.update();
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
        if((btnMap & BTN_MASK_ROT1) == 0x00){
            relayConfig.toggleSelected();
        }
        relayConfig.update();
    break;
    default:
        break;
    }

    uint8_t battLevel = (btnMap&0x01)? 128: 30;
    statusBar.setBattLevel(battLevel);
    statusBar.update();
    tabSelector.update();
}
