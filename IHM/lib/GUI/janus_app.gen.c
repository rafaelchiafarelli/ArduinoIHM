#include "janus_runtime.h"

extern const janus_screen_desc_t pwm_screen;
extern const janus_screen_desc_t busstatus_screen;
extern const janus_screen_desc_t relay_screen;

static const janus_screen_desc_t *const janus_app_screens[] JANUS_PROGMEM = {
    &pwm_screen,
    &busstatus_screen,
    &relay_screen
};

static const char *const janus_app_nav_titles[] JANUS_PROGMEM = {
    "PWM",
    "SERIAL",
    "Output"
};

janus_app_t janus_app = {
    .screens = janus_app_screens,
    .nav_titles = janus_app_nav_titles,
    .screen_count = 3,
    .active_screen = 0,
};
