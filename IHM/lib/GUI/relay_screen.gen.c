#include "relay_screen.gen.h"
#include "janus_actions.gen.h"
#include "janus_bindings.gen.h"
static const char relay_str1[] JANUS_PROGMEM = "Output";

static const char relay_str2[] JANUS_PROGMEM = "relay_header";

static const char relay_str3[] JANUS_PROGMEM = "relay_0";

static const char relay_str4[] JANUS_PROGMEM = "Relay 0";

static const char relay_str5[] JANUS_PROGMEM = "relay_0_label";

static const janus_widget_desc_t relay_arr1[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str3, .static_text = NULL, .geometry = {0, 20, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_0), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_0, .navigate_target = -1, .focus_order = 0, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str5, .static_text = relay_str4, .geometry = {28, 20, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char relay_str6[] JANUS_PROGMEM = "";

static const char relay_str7[] JANUS_PROGMEM = "relay_1";

static const char relay_str8[] JANUS_PROGMEM = "Relay 1";

static const char relay_str9[] JANUS_PROGMEM = "relay_1_label";

static const janus_widget_desc_t relay_arr2[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str7, .static_text = NULL, .geometry = {0, 36, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_1), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_1, .navigate_target = -1, .focus_order = 1, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str9, .static_text = relay_str8, .geometry = {28, 36, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char relay_str10[] JANUS_PROGMEM = "";

static const char relay_str11[] JANUS_PROGMEM = "relay_2";

static const char relay_str12[] JANUS_PROGMEM = "Relay 2";

static const char relay_str13[] JANUS_PROGMEM = "relay_2_label";

static const janus_widget_desc_t relay_arr3[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str11, .static_text = NULL, .geometry = {0, 52, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_2), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_2, .navigate_target = -1, .focus_order = 2, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str13, .static_text = relay_str12, .geometry = {28, 52, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char relay_str14[] JANUS_PROGMEM = "";

static const char relay_str15[] JANUS_PROGMEM = "relay_3";

static const char relay_str16[] JANUS_PROGMEM = "Relay 3";

static const char relay_str17[] JANUS_PROGMEM = "relay_3_label";

static const janus_widget_desc_t relay_arr4[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str15, .static_text = NULL, .geometry = {0, 68, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_3), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_3, .navigate_target = -1, .focus_order = 3, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str17, .static_text = relay_str16, .geometry = {28, 68, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char relay_str18[] JANUS_PROGMEM = "";

static const char relay_str19[] JANUS_PROGMEM = "relay_4";

static const char relay_str20[] JANUS_PROGMEM = "Relay 4";

static const char relay_str21[] JANUS_PROGMEM = "relay_4_label";

static const janus_widget_desc_t relay_arr5[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str19, .static_text = NULL, .geometry = {0, 84, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_4), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_4, .navigate_target = -1, .focus_order = 4, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str21, .static_text = relay_str20, .geometry = {28, 84, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char relay_str22[] JANUS_PROGMEM = "";

static const char relay_str23[] JANUS_PROGMEM = "relay_5";

static const char relay_str24[] JANUS_PROGMEM = "Relay 5";

static const char relay_str25[] JANUS_PROGMEM = "relay_5_label";

static const janus_widget_desc_t relay_arr6[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str23, .static_text = NULL, .geometry = {0, 100, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_5), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_5, .navigate_target = -1, .focus_order = 5, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str25, .static_text = relay_str24, .geometry = {28, 100, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char relay_str26[] JANUS_PROGMEM = "";

static const char relay_str27[] JANUS_PROGMEM = "relay_6";

static const char relay_str28[] JANUS_PROGMEM = "Relay 6";

static const char relay_str29[] JANUS_PROGMEM = "relay_6_label";

static const janus_widget_desc_t relay_arr7[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str27, .static_text = NULL, .geometry = {0, 116, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_6), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_6, .navigate_target = -1, .focus_order = 6, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str29, .static_text = relay_str28, .geometry = {28, 116, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char relay_str30[] JANUS_PROGMEM = "";

static const char relay_str31[] JANUS_PROGMEM = "relay_7";

static const char relay_str32[] JANUS_PROGMEM = "Relay 7";

static const char relay_str33[] JANUS_PROGMEM = "relay_7_label";

static const janus_widget_desc_t relay_arr8[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str31, .static_text = NULL, .geometry = {0, 132, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_7), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_7, .navigate_target = -1, .focus_order = 7, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str33, .static_text = relay_str32, .geometry = {28, 132, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char relay_str34[] JANUS_PROGMEM = "";

static const janus_widget_desc_t relay_arr9[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = relay_str6, .static_text = NULL, .geometry = {0, 20, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = relay_arr1, .child_count = 2 },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str10, .static_text = NULL, .geometry = {0, 36, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = relay_arr2, .child_count = 2 },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str14, .static_text = NULL, .geometry = {0, 52, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = relay_arr3, .child_count = 2 },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str18, .static_text = NULL, .geometry = {0, 68, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = relay_arr4, .child_count = 2 },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str22, .static_text = NULL, .geometry = {0, 84, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = relay_arr5, .child_count = 2 },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str26, .static_text = NULL, .geometry = {0, 100, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = relay_arr6, .child_count = 2 },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str30, .static_text = NULL, .geometry = {0, 116, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = relay_arr7, .child_count = 2 },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str34, .static_text = NULL, .geometry = {0, 132, 88, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = relay_arr8, .child_count = 2 }
};

static const char relay_str35[] JANUS_PROGMEM = "relay_list";

static const janus_widget_desc_t relay_widgets[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_HEADER, .id = relay_str2, .static_text = relay_str1, .geometry = {0, 0, 80, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_COLUMN, .id = relay_str35, .static_text = NULL, .geometry = {0, 20, 88, 124}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = relay_arr9, .child_count = 8 }
};

static const char relay_str36[] JANUS_PROGMEM = "Relay";

const janus_screen_desc_t relay_screen JANUS_PROGMEM = {
    .name = relay_str36,
    .widgets = relay_widgets,
    .widget_count = 2,
    .bound_struct = &relay_instance,
};
