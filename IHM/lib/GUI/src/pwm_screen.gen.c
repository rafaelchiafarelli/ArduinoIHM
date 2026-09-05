#include "pwm_screen.gen.h"
#include "janus_actions.gen.h"
#include "janus_bindings.gen.h"
static const char pwm_str1[] JANUS_PROGMEM = "Status: TBD";

static const char pwm_str2[] JANUS_PROGMEM = "status_placeholder";

static const janus_widget_desc_t pwm_arr1[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str2, .static_text = pwm_str1, .geometry = {0, 0, 300, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str3[] JANUS_PROGMEM = "status_bar";

static const char pwm_str4[] JANUS_PROGMEM = "PWM";

static const char pwm_str5[] JANUS_PROGMEM = "tab_pwm";

static const char pwm_str6[] JANUS_PROGMEM = "SERIAL";

static const char pwm_str7[] JANUS_PROGMEM = "tab_serial";

static const char pwm_str8[] JANUS_PROGMEM = "Output";

static const char pwm_str9[] JANUS_PROGMEM = "tab_output";

static const janus_widget_desc_t pwm_arr2[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_BUTTON, .id = pwm_str5, .static_text = pwm_str4, .geometry = {0, 22, 100, 28}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = 0, .focus_order = 0, .color = 0x0000, .bg_color = 0xffff, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_BUTTON, .id = pwm_str7, .static_text = pwm_str6, .geometry = {104, 22, 100, 28}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = 1, .focus_order = 1, .color = 0xad55, .bg_color = 0x3186, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_BUTTON, .id = pwm_str9, .static_text = pwm_str8, .geometry = {208, 22, 100, 28}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = 2, .focus_order = 2, .color = 0xad55, .bg_color = 0x3186, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str10[] JANUS_PROGMEM = "tab_bar";

static const char pwm_str11[] JANUS_PROGMEM = "pwm_ch0_enabled_toggle";

static const char pwm_str12[] JANUS_PROGMEM = "Enabled";

static const char pwm_str13[] JANUS_PROGMEM = "pwm_ch0_enabled_label";

static const janus_widget_desc_t pwm_arr3[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str11, .static_text = NULL, .geometry = {0, 78, 28, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch0_enabled), .dirty_offset = offsetof(pwm_dirty_t, ch0_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_PWM_CH0_ENABLED, .navigate_target = -1, .focus_order = 4, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str13, .static_text = pwm_str12, .geometry = {32, 78, 70, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str14[] JANUS_PROGMEM = "";

static const char pwm_str15[] JANUS_PROGMEM = "pwm_ch0_inverting_toggle";

static const char pwm_str16[] JANUS_PROGMEM = "Inverting";

static const char pwm_str17[] JANUS_PROGMEM = "pwm_ch0_inverting_label";

static const janus_widget_desc_t pwm_arr4[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str15, .static_text = NULL, .geometry = {0, 98, 28, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch0_inverting), .dirty_offset = offsetof(pwm_dirty_t, ch0_inverting), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_PWM_CH0_INVERTING, .navigate_target = -1, .focus_order = 5, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str17, .static_text = pwm_str16, .geometry = {32, 98, 70, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str18[] JANUS_PROGMEM = "";

static const janus_widget_desc_t pwm_arr5[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str14, .static_text = NULL, .geometry = {0, 78, 102, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr3, .child_count = 2, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str18, .static_text = NULL, .geometry = {0, 98, 102, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr4, .child_count = 2, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str19[] JANUS_PROGMEM = "pwm_ch0_icon";

static const char pwm_str20[] JANUS_PROGMEM = "pwm_ch0_led";

static const char pwm_str21[] JANUS_PROGMEM = "pwm_ch0_state";

static const char pwm_str22[] JANUS_PROGMEM = "1200Hz";

static const char pwm_str23[] JANUS_PROGMEM = "pwm_ch0_freq";

static const char pwm_str24[] JANUS_PROGMEM = "pwm_ch0_duty";

static const janus_widget_desc_t pwm_arr6[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_IMAGE, .id = pwm_str19, .static_text = NULL, .geometry = {0, 54, 24, 24}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch0_mode_icon), .dirty_offset = offsetof(pwm_dirty_t, ch0_mode_icon), .field_type = JANUS_FIELD_STRING, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LED, .id = pwm_str20, .static_text = NULL, .geometry = {28, 58, 16, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch0_enabled), .dirty_offset = offsetof(pwm_dirty_t, ch0_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str21, .static_text = NULL, .geometry = {48, 58, 50, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch0_state_label), .dirty_offset = offsetof(pwm_dirty_t, ch0_state_label), .field_type = JANUS_FIELD_STRING, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str23, .static_text = pwm_str22, .geometry = {102, 58, 40, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str24, .static_text = NULL, .geometry = {146, 58, 40, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch0_duty_percent), .dirty_offset = offsetof(pwm_dirty_t, ch0_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str25[] JANUS_PROGMEM = "pwm_ch0_box";

static const char pwm_str26[] JANUS_PROGMEM = "pwm_ch1_enabled_toggle";

static const char pwm_str27[] JANUS_PROGMEM = "Enabled";

static const char pwm_str28[] JANUS_PROGMEM = "pwm_ch1_enabled_label";

static const janus_widget_desc_t pwm_arr7[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str26, .static_text = NULL, .geometry = {0, 142, 28, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch1_enabled), .dirty_offset = offsetof(pwm_dirty_t, ch1_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_PWM_CH1_ENABLED, .navigate_target = -1, .focus_order = 7, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str28, .static_text = pwm_str27, .geometry = {32, 142, 70, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str29[] JANUS_PROGMEM = "";

static const char pwm_str30[] JANUS_PROGMEM = "pwm_ch1_inverting_toggle";

static const char pwm_str31[] JANUS_PROGMEM = "Inverting";

static const char pwm_str32[] JANUS_PROGMEM = "pwm_ch1_inverting_label";

static const janus_widget_desc_t pwm_arr8[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str30, .static_text = NULL, .geometry = {0, 162, 28, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch1_inverting), .dirty_offset = offsetof(pwm_dirty_t, ch1_inverting), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_PWM_CH1_INVERTING, .navigate_target = -1, .focus_order = 8, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str32, .static_text = pwm_str31, .geometry = {32, 162, 70, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str33[] JANUS_PROGMEM = "";

static const janus_widget_desc_t pwm_arr9[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str29, .static_text = NULL, .geometry = {0, 142, 102, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr7, .child_count = 2, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str33, .static_text = NULL, .geometry = {0, 162, 102, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr8, .child_count = 2, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str34[] JANUS_PROGMEM = "pwm_ch1_icon";

static const char pwm_str35[] JANUS_PROGMEM = "pwm_ch1_led";

static const char pwm_str36[] JANUS_PROGMEM = "pwm_ch1_state";

static const char pwm_str37[] JANUS_PROGMEM = "2400Hz";

static const char pwm_str38[] JANUS_PROGMEM = "pwm_ch1_freq";

static const char pwm_str39[] JANUS_PROGMEM = "pwm_ch1_duty";

static const janus_widget_desc_t pwm_arr10[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_IMAGE, .id = pwm_str34, .static_text = NULL, .geometry = {0, 118, 24, 24}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch1_mode_icon), .dirty_offset = offsetof(pwm_dirty_t, ch1_mode_icon), .field_type = JANUS_FIELD_STRING, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LED, .id = pwm_str35, .static_text = NULL, .geometry = {28, 122, 16, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch1_enabled), .dirty_offset = offsetof(pwm_dirty_t, ch1_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str36, .static_text = NULL, .geometry = {48, 122, 50, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch1_state_label), .dirty_offset = offsetof(pwm_dirty_t, ch1_state_label), .field_type = JANUS_FIELD_STRING, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str38, .static_text = pwm_str37, .geometry = {102, 122, 40, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str39, .static_text = NULL, .geometry = {146, 122, 40, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch1_duty_percent), .dirty_offset = offsetof(pwm_dirty_t, ch1_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str40[] JANUS_PROGMEM = "pwm_ch1_box";

static const char pwm_str41[] JANUS_PROGMEM = "pwm_ch2_a_enabled_toggle";

static const char pwm_str42[] JANUS_PROGMEM = "A";

static const char pwm_str43[] JANUS_PROGMEM = "pwm_ch2_a_label";

static const char pwm_str44[] JANUS_PROGMEM = "pwm_ch2_a_duty";

static const janus_widget_desc_t pwm_arr11[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str41, .static_text = NULL, .geometry = {0, 206, 28, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_a_enabled), .dirty_offset = offsetof(pwm_dirty_t, ch2_a_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_PWM_CH2_A_ENABLED, .navigate_target = -1, .focus_order = 10, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str43, .static_text = pwm_str42, .geometry = {32, 206, 20, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str44, .static_text = NULL, .geometry = {56, 206, 60, 14}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_a_duty_percent), .dirty_offset = offsetof(pwm_dirty_t, ch2_a_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str45[] JANUS_PROGMEM = "";

static const char pwm_str46[] JANUS_PROGMEM = "pwm_ch2_b_enabled_toggle";

static const char pwm_str47[] JANUS_PROGMEM = "B";

static const char pwm_str48[] JANUS_PROGMEM = "pwm_ch2_b_label";

static const char pwm_str49[] JANUS_PROGMEM = "pwm_ch2_b_duty";

static const janus_widget_desc_t pwm_arr12[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str46, .static_text = NULL, .geometry = {0, 226, 28, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_b_enabled), .dirty_offset = offsetof(pwm_dirty_t, ch2_b_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_PWM_CH2_B_ENABLED, .navigate_target = -1, .focus_order = 11, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str48, .static_text = pwm_str47, .geometry = {32, 226, 20, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str49, .static_text = NULL, .geometry = {56, 226, 60, 14}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_b_duty_percent), .dirty_offset = offsetof(pwm_dirty_t, ch2_b_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str50[] JANUS_PROGMEM = "";

static const char pwm_str51[] JANUS_PROGMEM = "pwm_ch2_c_enabled_toggle";

static const char pwm_str52[] JANUS_PROGMEM = "C";

static const char pwm_str53[] JANUS_PROGMEM = "pwm_ch2_c_label";

static const char pwm_str54[] JANUS_PROGMEM = "pwm_ch2_c_duty";

static const janus_widget_desc_t pwm_arr13[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str51, .static_text = NULL, .geometry = {0, 246, 28, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_c_enabled), .dirty_offset = offsetof(pwm_dirty_t, ch2_c_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_PWM_CH2_C_ENABLED, .navigate_target = -1, .focus_order = 12, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str53, .static_text = pwm_str52, .geometry = {32, 246, 20, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str54, .static_text = NULL, .geometry = {56, 246, 60, 14}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_c_duty_percent), .dirty_offset = offsetof(pwm_dirty_t, ch2_c_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str55[] JANUS_PROGMEM = "";

static const janus_widget_desc_t pwm_arr14[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str45, .static_text = NULL, .geometry = {0, 206, 116, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr11, .child_count = 3, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str50, .static_text = NULL, .geometry = {0, 226, 116, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr12, .child_count = 3, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str55, .static_text = NULL, .geometry = {0, 246, 116, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr13, .child_count = 3, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str56[] JANUS_PROGMEM = "pwm_ch2_icon";

static const char pwm_str57[] JANUS_PROGMEM = "pwm_ch2_state";

static const char pwm_str58[] JANUS_PROGMEM = "500Hz";

static const char pwm_str59[] JANUS_PROGMEM = "pwm_ch2_freq";

static const janus_widget_desc_t pwm_arr15[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_IMAGE, .id = pwm_str56, .static_text = NULL, .geometry = {0, 182, 24, 24}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_mode_icon), .dirty_offset = offsetof(pwm_dirty_t, ch2_mode_icon), .field_type = JANUS_FIELD_STRING, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str57, .static_text = NULL, .geometry = {28, 186, 50, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_state_label), .dirty_offset = offsetof(pwm_dirty_t, ch2_state_label), .field_type = JANUS_FIELD_STRING, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str59, .static_text = pwm_str58, .geometry = {82, 186, 40, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str60[] JANUS_PROGMEM = "pwm_ch2_box";

static const char pwm_str61[] JANUS_PROGMEM = "pwm_ch3_a_enabled_toggle";

static const char pwm_str62[] JANUS_PROGMEM = "A";

static const char pwm_str63[] JANUS_PROGMEM = "pwm_ch3_a_label";

static const char pwm_str64[] JANUS_PROGMEM = "pwm_ch3_a_duty";

static const janus_widget_desc_t pwm_arr16[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str61, .static_text = NULL, .geometry = {0, 290, 28, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_a_enabled), .dirty_offset = offsetof(pwm_dirty_t, ch3_a_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_PWM_CH3_A_ENABLED, .navigate_target = -1, .focus_order = 14, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str63, .static_text = pwm_str62, .geometry = {32, 290, 20, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str64, .static_text = NULL, .geometry = {56, 290, 60, 14}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_a_duty_percent), .dirty_offset = offsetof(pwm_dirty_t, ch3_a_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str65[] JANUS_PROGMEM = "";

static const char pwm_str66[] JANUS_PROGMEM = "pwm_ch3_b_enabled_toggle";

static const char pwm_str67[] JANUS_PROGMEM = "B";

static const char pwm_str68[] JANUS_PROGMEM = "pwm_ch3_b_label";

static const char pwm_str69[] JANUS_PROGMEM = "pwm_ch3_b_duty";

static const janus_widget_desc_t pwm_arr17[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str66, .static_text = NULL, .geometry = {0, 310, 28, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_b_enabled), .dirty_offset = offsetof(pwm_dirty_t, ch3_b_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_PWM_CH3_B_ENABLED, .navigate_target = -1, .focus_order = 15, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str68, .static_text = pwm_str67, .geometry = {32, 310, 20, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str69, .static_text = NULL, .geometry = {56, 310, 60, 14}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_b_duty_percent), .dirty_offset = offsetof(pwm_dirty_t, ch3_b_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str70[] JANUS_PROGMEM = "";

static const char pwm_str71[] JANUS_PROGMEM = "pwm_ch3_c_enabled_toggle";

static const char pwm_str72[] JANUS_PROGMEM = "C";

static const char pwm_str73[] JANUS_PROGMEM = "pwm_ch3_c_label";

static const char pwm_str74[] JANUS_PROGMEM = "pwm_ch3_c_duty";

static const janus_widget_desc_t pwm_arr18[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str71, .static_text = NULL, .geometry = {0, 330, 28, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_c_enabled), .dirty_offset = offsetof(pwm_dirty_t, ch3_c_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_PWM_CH3_C_ENABLED, .navigate_target = -1, .focus_order = 16, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str73, .static_text = pwm_str72, .geometry = {32, 330, 20, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str74, .static_text = NULL, .geometry = {56, 330, 60, 14}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_c_duty_percent), .dirty_offset = offsetof(pwm_dirty_t, ch3_c_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str75[] JANUS_PROGMEM = "";

static const janus_widget_desc_t pwm_arr19[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str65, .static_text = NULL, .geometry = {0, 290, 116, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr16, .child_count = 3, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str70, .static_text = NULL, .geometry = {0, 310, 116, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr17, .child_count = 3, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str75, .static_text = NULL, .geometry = {0, 330, 116, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr18, .child_count = 3, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str76[] JANUS_PROGMEM = "pwm_ch3_icon";

static const char pwm_str77[] JANUS_PROGMEM = "pwm_ch3_state";

static const char pwm_str78[] JANUS_PROGMEM = "8000Hz";

static const char pwm_str79[] JANUS_PROGMEM = "pwm_ch3_freq";

static const janus_widget_desc_t pwm_arr20[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_IMAGE, .id = pwm_str76, .static_text = NULL, .geometry = {0, 266, 24, 24}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_mode_icon), .dirty_offset = offsetof(pwm_dirty_t, ch3_mode_icon), .field_type = JANUS_FIELD_STRING, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str77, .static_text = NULL, .geometry = {28, 270, 50, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_state_label), .dirty_offset = offsetof(pwm_dirty_t, ch3_state_label), .field_type = JANUS_FIELD_STRING, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str79, .static_text = pwm_str78, .geometry = {82, 270, 40, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str80[] JANUS_PROGMEM = "pwm_ch3_box";

static const janus_widget_desc_t pwm_arr21[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_BOX, .id = pwm_str25, .static_text = NULL, .geometry = {0, 54, 186, 60}, .geometry_collapsed = {0, 54, 186, 24}, .initial_expanded = false, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 3, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr5, .child_count = 2, .summary_children = pwm_arr6, .summary_child_count = 5 },
    { .kind = JANUS_WIDGET_BOX, .id = pwm_str40, .static_text = NULL, .geometry = {0, 118, 186, 60}, .geometry_collapsed = {0, 118, 186, 24}, .initial_expanded = false, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 6, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr9, .child_count = 2, .summary_children = pwm_arr10, .summary_child_count = 5 },
    { .kind = JANUS_WIDGET_BOX, .id = pwm_str60, .static_text = NULL, .geometry = {0, 182, 122, 80}, .geometry_collapsed = {0, 182, 122, 24}, .initial_expanded = false, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 9, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr14, .child_count = 3, .summary_children = pwm_arr15, .summary_child_count = 3 },
    { .kind = JANUS_WIDGET_BOX, .id = pwm_str80, .static_text = NULL, .geometry = {0, 266, 122, 80}, .geometry_collapsed = {0, 266, 122, 24}, .initial_expanded = false, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 13, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr19, .child_count = 3, .summary_children = pwm_arr20, .summary_child_count = 3 }
};

static const char pwm_str81[] JANUS_PROGMEM = "pwm_channel_list";

static const janus_widget_desc_t pwm_widgets[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str3, .static_text = NULL, .geometry = {0, 0, 300, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr1, .child_count = 1, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str10, .static_text = NULL, .geometry = {0, 22, 308, 28}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr2, .child_count = 3, .summary_children = NULL, .summary_child_count = 0 },
    { .kind = JANUS_WIDGET_COLUMN, .id = pwm_str81, .static_text = NULL, .geometry = {0, 54, 186, 292}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr21, .child_count = 4, .summary_children = NULL, .summary_child_count = 0 }
};

static const char pwm_str82[] JANUS_PROGMEM = "PWM";

const janus_screen_desc_t pwm_screen JANUS_PROGMEM = {
    .name = pwm_str82,
    .widgets = pwm_widgets,
    .widget_count = 3,
    .bound_struct = &pwm_instance,
    .bound_dirty = &pwm_dirty,
};
