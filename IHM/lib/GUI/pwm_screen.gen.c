#include "pwm_screen.gen.h"
#include "janus_actions.gen.h"
#include "janus_bindings.gen.h"
static const char pwm_str1[] JANUS_PROGMEM = "PWM";

static const char pwm_str2[] JANUS_PROGMEM = "pwm_header";

static const char pwm_str3[] JANUS_PROGMEM = "pwm_ch0_enabled";

static const char pwm_str4[] JANUS_PROGMEM = "En";

static const char pwm_str5[] JANUS_PROGMEM = "pwm_ch0_enabled_label";

static const char pwm_str6[] JANUS_PROGMEM = "pwm_ch0_inverting";

static const char pwm_str7[] JANUS_PROGMEM = "Inv";

static const char pwm_str8[] JANUS_PROGMEM = "pwm_ch0_inverting_label";

static const janus_widget_desc_t pwm_arr1[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str3, .static_text = NULL, .geometry = {0, 34, 20, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch0_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str5, .static_text = pwm_str4, .geometry = {24, 34, 18, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str6, .static_text = NULL, .geometry = {46, 34, 20, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch0_inverting), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str8, .static_text = pwm_str7, .geometry = {70, 34, 22, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str9[] JANUS_PROGMEM = "";

static const char pwm_str10[] JANUS_PROGMEM = "F";

static const char pwm_str11[] JANUS_PROGMEM = "pwm_ch0_freq_label";

static const char pwm_str12[] JANUS_PROGMEM = "pwm_ch0_freq";

static const char pwm_str13[] JANUS_PROGMEM = "T";

static const char pwm_str14[] JANUS_PROGMEM = "pwm_ch0_top_label";

static const char pwm_str15[] JANUS_PROGMEM = "pwm_ch0_top";

static const janus_widget_desc_t pwm_arr2[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str11, .static_text = pwm_str10, .geometry = {0, 50, 10, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str12, .static_text = NULL, .geometry = {14, 50, 30, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch0_frequency), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str14, .static_text = pwm_str13, .geometry = {48, 50, 10, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str15, .static_text = NULL, .geometry = {62, 50, 30, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch0_variable_top), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str16[] JANUS_PROGMEM = "";

static const char pwm_str17[] JANUS_PROGMEM = "pwm_ch0_duty";

static const janus_widget_desc_t pwm_arr3[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str9, .static_text = NULL, .geometry = {0, 34, 92, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr1, .child_count = 4 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str16, .static_text = NULL, .geometry = {0, 50, 92, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr2, .child_count = 4 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str17, .static_text = NULL, .geometry = {0, 66, 92, 10}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch0_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str18[] JANUS_PROGMEM = "pwm_ch0_box";

static const char pwm_str19[] JANUS_PROGMEM = "pwm_ch1_enabled";

static const char pwm_str20[] JANUS_PROGMEM = "En";

static const char pwm_str21[] JANUS_PROGMEM = "pwm_ch1_enabled_label";

static const char pwm_str22[] JANUS_PROGMEM = "pwm_ch1_inverting";

static const char pwm_str23[] JANUS_PROGMEM = "Inv";

static const char pwm_str24[] JANUS_PROGMEM = "pwm_ch1_inverting_label";

static const janus_widget_desc_t pwm_arr4[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str19, .static_text = NULL, .geometry = {0, 96, 20, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch1_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str21, .static_text = pwm_str20, .geometry = {24, 96, 18, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str22, .static_text = NULL, .geometry = {46, 96, 20, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch1_inverting), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str24, .static_text = pwm_str23, .geometry = {70, 96, 22, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str25[] JANUS_PROGMEM = "";

static const char pwm_str26[] JANUS_PROGMEM = "F";

static const char pwm_str27[] JANUS_PROGMEM = "pwm_ch1_freq_label";

static const char pwm_str28[] JANUS_PROGMEM = "pwm_ch1_freq";

static const char pwm_str29[] JANUS_PROGMEM = "T";

static const char pwm_str30[] JANUS_PROGMEM = "pwm_ch1_top_label";

static const char pwm_str31[] JANUS_PROGMEM = "pwm_ch1_top";

static const janus_widget_desc_t pwm_arr5[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str27, .static_text = pwm_str26, .geometry = {0, 112, 10, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str28, .static_text = NULL, .geometry = {14, 112, 30, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch1_frequency), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str30, .static_text = pwm_str29, .geometry = {48, 112, 10, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str31, .static_text = NULL, .geometry = {62, 112, 30, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch1_variable_top), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str32[] JANUS_PROGMEM = "";

static const char pwm_str33[] JANUS_PROGMEM = "pwm_ch1_duty";

static const janus_widget_desc_t pwm_arr6[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str25, .static_text = NULL, .geometry = {0, 96, 92, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr4, .child_count = 4 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str32, .static_text = NULL, .geometry = {0, 112, 92, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr5, .child_count = 4 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str33, .static_text = NULL, .geometry = {0, 128, 92, 10}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch1_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str34[] JANUS_PROGMEM = "pwm_ch1_box";

static const janus_widget_desc_t pwm_arr7[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_BOX, .id = pwm_str18, .static_text = NULL, .geometry = {0, 18, 92, 58}, .geometry_collapsed = {0, 18, 92, 16}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 0, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr3, .child_count = 3 },
    { .kind = JANUS_WIDGET_BOX, .id = pwm_str34, .static_text = NULL, .geometry = {0, 80, 92, 58}, .geometry_collapsed = {0, 80, 92, 16}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 1, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr6, .child_count = 3 }
};

static const char pwm_str35[] JANUS_PROGMEM = "pwm_simplex_col";

static const char pwm_str36[] JANUS_PROGMEM = "F";

static const char pwm_str37[] JANUS_PROGMEM = "pwm_ch2_freq_label";

static const char pwm_str38[] JANUS_PROGMEM = "pwm_ch2_freq";

static const char pwm_str39[] JANUS_PROGMEM = "T";

static const char pwm_str40[] JANUS_PROGMEM = "pwm_ch2_top_label";

static const char pwm_str41[] JANUS_PROGMEM = "pwm_ch2_top";

static const janus_widget_desc_t pwm_arr8[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str37, .static_text = pwm_str36, .geometry = {96, 34, 10, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str38, .static_text = NULL, .geometry = {110, 34, 30, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_frequency), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str40, .static_text = pwm_str39, .geometry = {144, 34, 10, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str41, .static_text = NULL, .geometry = {158, 34, 30, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_variable_top), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str42[] JANUS_PROGMEM = "";

static const char pwm_str43[] JANUS_PROGMEM = "pwm_ch2_a_enabled";

static const char pwm_str44[] JANUS_PROGMEM = "A";

static const char pwm_str45[] JANUS_PROGMEM = "pwm_ch2_a_label";

static const char pwm_str46[] JANUS_PROGMEM = "pwm_ch2_a_duty";

static const janus_widget_desc_t pwm_arr9[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str43, .static_text = NULL, .geometry = {96, 50, 20, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_a_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str45, .static_text = pwm_str44, .geometry = {120, 50, 14, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str46, .static_text = NULL, .geometry = {138, 50, 46, 10}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_a_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str47[] JANUS_PROGMEM = "";

static const char pwm_str48[] JANUS_PROGMEM = "pwm_ch2_b_enabled";

static const char pwm_str49[] JANUS_PROGMEM = "B";

static const char pwm_str50[] JANUS_PROGMEM = "pwm_ch2_b_label";

static const char pwm_str51[] JANUS_PROGMEM = "pwm_ch2_b_duty";

static const janus_widget_desc_t pwm_arr10[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str48, .static_text = NULL, .geometry = {96, 66, 20, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_b_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str50, .static_text = pwm_str49, .geometry = {120, 66, 14, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str51, .static_text = NULL, .geometry = {138, 66, 46, 10}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_b_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str52[] JANUS_PROGMEM = "";

static const char pwm_str53[] JANUS_PROGMEM = "pwm_ch2_c_enabled";

static const char pwm_str54[] JANUS_PROGMEM = "C";

static const char pwm_str55[] JANUS_PROGMEM = "pwm_ch2_c_label";

static const char pwm_str56[] JANUS_PROGMEM = "pwm_ch2_c_duty";

static const janus_widget_desc_t pwm_arr11[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str53, .static_text = NULL, .geometry = {96, 82, 20, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_c_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str55, .static_text = pwm_str54, .geometry = {120, 82, 14, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str56, .static_text = NULL, .geometry = {138, 82, 46, 10}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch2_c_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str57[] JANUS_PROGMEM = "";

static const janus_widget_desc_t pwm_arr12[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str42, .static_text = NULL, .geometry = {96, 34, 92, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr8, .child_count = 4 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str47, .static_text = NULL, .geometry = {96, 50, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr9, .child_count = 3 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str52, .static_text = NULL, .geometry = {96, 66, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr10, .child_count = 3 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str57, .static_text = NULL, .geometry = {96, 82, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr11, .child_count = 3 }
};

static const char pwm_str58[] JANUS_PROGMEM = "pwm_ch2_box";

static const char pwm_str59[] JANUS_PROGMEM = "F";

static const char pwm_str60[] JANUS_PROGMEM = "pwm_ch3_freq_label";

static const char pwm_str61[] JANUS_PROGMEM = "pwm_ch3_freq";

static const char pwm_str62[] JANUS_PROGMEM = "T";

static const char pwm_str63[] JANUS_PROGMEM = "pwm_ch3_top_label";

static const char pwm_str64[] JANUS_PROGMEM = "pwm_ch3_top";

static const janus_widget_desc_t pwm_arr13[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str60, .static_text = pwm_str59, .geometry = {96, 114, 10, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str61, .static_text = NULL, .geometry = {110, 114, 30, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_frequency), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str63, .static_text = pwm_str62, .geometry = {144, 114, 10, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str64, .static_text = NULL, .geometry = {158, 114, 30, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_variable_top), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str65[] JANUS_PROGMEM = "";

static const char pwm_str66[] JANUS_PROGMEM = "pwm_ch3_a_enabled";

static const char pwm_str67[] JANUS_PROGMEM = "A";

static const char pwm_str68[] JANUS_PROGMEM = "pwm_ch3_a_label";

static const char pwm_str69[] JANUS_PROGMEM = "pwm_ch3_a_duty";

static const janus_widget_desc_t pwm_arr14[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str66, .static_text = NULL, .geometry = {96, 130, 20, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_a_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str68, .static_text = pwm_str67, .geometry = {120, 130, 14, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str69, .static_text = NULL, .geometry = {138, 130, 46, 10}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_a_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str70[] JANUS_PROGMEM = "";

static const char pwm_str71[] JANUS_PROGMEM = "pwm_ch3_b_enabled";

static const char pwm_str72[] JANUS_PROGMEM = "B";

static const char pwm_str73[] JANUS_PROGMEM = "pwm_ch3_b_label";

static const char pwm_str74[] JANUS_PROGMEM = "pwm_ch3_b_duty";

static const janus_widget_desc_t pwm_arr15[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str71, .static_text = NULL, .geometry = {96, 146, 20, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_b_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str73, .static_text = pwm_str72, .geometry = {120, 146, 14, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str74, .static_text = NULL, .geometry = {138, 146, 46, 10}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_b_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str75[] JANUS_PROGMEM = "";

static const char pwm_str76[] JANUS_PROGMEM = "pwm_ch3_c_enabled";

static const char pwm_str77[] JANUS_PROGMEM = "C";

static const char pwm_str78[] JANUS_PROGMEM = "pwm_ch3_c_label";

static const char pwm_str79[] JANUS_PROGMEM = "pwm_ch3_c_duty";

static const janus_widget_desc_t pwm_arr16[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = pwm_str76, .static_text = NULL, .geometry = {96, 162, 20, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_c_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = pwm_str78, .static_text = pwm_str77, .geometry = {120, 162, 14, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_PROGRESS, .id = pwm_str79, .static_text = NULL, .geometry = {138, 162, 46, 10}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(pwm_t, ch3_c_duty_percent), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 100 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char pwm_str80[] JANUS_PROGMEM = "";

static const janus_widget_desc_t pwm_arr17[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str65, .static_text = NULL, .geometry = {96, 114, 92, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr13, .child_count = 4 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str70, .static_text = NULL, .geometry = {96, 130, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr14, .child_count = 3 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str75, .static_text = NULL, .geometry = {96, 146, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr15, .child_count = 3 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str80, .static_text = NULL, .geometry = {96, 162, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr16, .child_count = 3 }
};

static const char pwm_str81[] JANUS_PROGMEM = "pwm_ch3_box";

static const janus_widget_desc_t pwm_arr18[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_BOX, .id = pwm_str58, .static_text = NULL, .geometry = {96, 18, 92, 76}, .geometry_collapsed = {96, 18, 92, 16}, .initial_expanded = false, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 2, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr12, .child_count = 4 },
    { .kind = JANUS_WIDGET_BOX, .id = pwm_str81, .static_text = NULL, .geometry = {96, 98, 92, 76}, .geometry_collapsed = {96, 98, 92, 16}, .initial_expanded = false, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 3, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr17, .child_count = 4 }
};

static const char pwm_str82[] JANUS_PROGMEM = "pwm_complex_col";

static const janus_widget_desc_t pwm_arr19[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_COLUMN, .id = pwm_str35, .static_text = NULL, .geometry = {0, 18, 92, 120}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr7, .child_count = 2 },
    { .kind = JANUS_WIDGET_COLUMN, .id = pwm_str82, .static_text = NULL, .geometry = {96, 18, 92, 156}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr18, .child_count = 2 }
};

static const char pwm_str83[] JANUS_PROGMEM = "";

static const janus_widget_desc_t pwm_widgets[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_HEADER, .id = pwm_str2, .static_text = pwm_str1, .geometry = {0, 0, 180, 14}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_ROW, .id = pwm_str83, .static_text = NULL, .geometry = {0, 18, 188, 156}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = pwm_arr19, .child_count = 2 }
};

static const char pwm_str84[] JANUS_PROGMEM = "PWM";

const janus_screen_desc_t pwm_screen JANUS_PROGMEM = {
    .name = pwm_str84,
    .widgets = pwm_widgets,
    .widget_count = 2,
    .bound_struct = &pwm_instance,
};
