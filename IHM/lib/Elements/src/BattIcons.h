#pragma once
#include <Element.h>
#include <standardDefinitions.h>
#include "Icon.h"
#include "Icons/BattCharging.h"
#include "Icons/BattEmpty.h"
#include "Icons/BattFull.h"


 typedef enum{
    BattEmpty,
    BattCharding,
    BattCharged,
    MAX_NUMBER_OF_ICONS
}IconSelection;

// The status bar's battery icon, 3 mutually-exclusive bitmaps (empty/
// charging/full) driven by StatusBar::setBattLevel(). Same shape as
// EdgeSelectionIcons: owns its own IconType_t[] and forwards it to Icon,
// which does the actual dirty-flagged draw -- previously duplicated that
// whole array-loop-drawRGBBitmap logic independently instead of reusing it.
class BattIcons: public Icon {
    private:

    IconType_t battIcons[MAX_NUMBER_OF_ICONS] = {
        {
            .d = batt_full,
            .w = batt_full_w,
            .h = batt_full_h,
            .x = 280,
            .y = 1,
            .state = GONE,
            .isShown = false
        },
        {
            .d = batt_charging,
            .w = batt_charging_w,
            .h = batt_charging_h,
            .x = 280,
            .y = 1,
            .state = GONE,
            .isShown = false
        },
        {
            .d = batt_empty,
            .w = batt_empty_w,
            .h = batt_empty_h,
            .x = 280,
            .y = 1,
            .state = VISIBLE,
            .isShown = false
        }
    };

    public:

        BattIcons(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state, Display *tft):
            Icon(x, y, w, h, state, MAX_NUMBER_OF_ICONS, tft, battIcons)
        {
        }
};
