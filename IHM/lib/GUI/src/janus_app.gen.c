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

static const char janus_app_nav_tab_str1[] JANUS_PROGMEM = "PWM";

static const char janus_app_nav_tab_str2[] JANUS_PROGMEM = "SERIAL";

static const char janus_app_nav_tab_str3[] JANUS_PROGMEM = "Output";

static const janus_nav_tab_t janus_app_nav_tabs[] JANUS_PROGMEM = {
    { {0, 0, 106, 28}, janus_app_nav_tab_str1, 0 },
    { {106, 0, 106, 28}, janus_app_nav_tab_str2, 1 },
    { {212, 0, 108, 28}, janus_app_nav_tab_str3, 2 }
};

janus_app_t janus_app = {
    .screens = janus_app_screens,
    .nav_titles = janus_app_nav_titles,
    .nav_tabs = janus_app_nav_tabs,
    .nav_tab_count = 3,
    .screen_count = 3,
    .active_screen = 0,
};
