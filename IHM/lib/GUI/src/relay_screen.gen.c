#include "relay_screen.gen.h"
#include "janus_actions.gen.h"
#include "janus_bindings.gen.h"
static const char relay_str1[] JANUS_PROGMEM = "Status: TBD";

static const char relay_str2[] JANUS_PROGMEM = "status_placeholder";

static const janus_widget_desc_t relay_arr1[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str2, .static_text = relay_str1, .text_is_format = false, .geometry = {0, 28, 140, 20}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str3[] JANUS_PROGMEM = "status_bar";

static const char relay_str4[] JANUS_PROGMEM = "relay_0";

static const char relay_str5[] JANUS_PROGMEM = "Relay 0";

static const char relay_str6[] JANUS_PROGMEM = "relay_0_label";

static const janus_widget_desc_t relay_arr2[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str4, .static_text = NULL, .text_is_format = false, .geometry = {0, 52, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_0), .dirty_offset = offsetof(relay_dirty_t, relay_0), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_0, .navigate_target = -1, .focus_order = 0, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str6, .static_text = relay_str5, .text_is_format = false, .geometry = {28, 52, 292, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str7[] JANUS_PROGMEM = "";

static const char relay_str8[] JANUS_PROGMEM = "relay_1";

static const char relay_str9[] JANUS_PROGMEM = "Relay 1";

static const char relay_str10[] JANUS_PROGMEM = "relay_1_label";

static const janus_widget_desc_t relay_arr3[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str8, .static_text = NULL, .text_is_format = false, .geometry = {0, 88, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_1), .dirty_offset = offsetof(relay_dirty_t, relay_1), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_1, .navigate_target = -1, .focus_order = 1, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str10, .static_text = relay_str9, .text_is_format = false, .geometry = {28, 88, 292, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str11[] JANUS_PROGMEM = "";

static const char relay_str12[] JANUS_PROGMEM = "relay_2";

static const char relay_str13[] JANUS_PROGMEM = "Relay 2";

static const char relay_str14[] JANUS_PROGMEM = "relay_2_label";

static const janus_widget_desc_t relay_arr4[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str12, .static_text = NULL, .text_is_format = false, .geometry = {0, 124, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_2), .dirty_offset = offsetof(relay_dirty_t, relay_2), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_2, .navigate_target = -1, .focus_order = 2, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str14, .static_text = relay_str13, .text_is_format = false, .geometry = {28, 124, 292, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str15[] JANUS_PROGMEM = "";

static const char relay_str16[] JANUS_PROGMEM = "relay_3";

static const char relay_str17[] JANUS_PROGMEM = "Relay 3";

static const char relay_str18[] JANUS_PROGMEM = "relay_3_label";

static const janus_widget_desc_t relay_arr5[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str16, .static_text = NULL, .text_is_format = false, .geometry = {0, 160, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_3), .dirty_offset = offsetof(relay_dirty_t, relay_3), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_3, .navigate_target = -1, .focus_order = 3, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str18, .static_text = relay_str17, .text_is_format = false, .geometry = {28, 160, 292, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str19[] JANUS_PROGMEM = "";

static const char relay_str20[] JANUS_PROGMEM = "relay_4";

static const char relay_str21[] JANUS_PROGMEM = "Relay 4";

static const char relay_str22[] JANUS_PROGMEM = "relay_4_label";

static const janus_widget_desc_t relay_arr6[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str20, .static_text = NULL, .text_is_format = false, .geometry = {0, 196, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_4), .dirty_offset = offsetof(relay_dirty_t, relay_4), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_4, .navigate_target = -1, .focus_order = 4, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str22, .static_text = relay_str21, .text_is_format = false, .geometry = {28, 196, 292, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str23[] JANUS_PROGMEM = "";

static const char relay_str24[] JANUS_PROGMEM = "relay_5";

static const char relay_str25[] JANUS_PROGMEM = "Relay 5";

static const char relay_str26[] JANUS_PROGMEM = "relay_5_label";

static const janus_widget_desc_t relay_arr7[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str24, .static_text = NULL, .text_is_format = false, .geometry = {0, 232, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_5), .dirty_offset = offsetof(relay_dirty_t, relay_5), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_5, .navigate_target = -1, .focus_order = 5, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str26, .static_text = relay_str25, .text_is_format = false, .geometry = {28, 232, 292, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str27[] JANUS_PROGMEM = "";

static const char relay_str28[] JANUS_PROGMEM = "relay_6";

static const char relay_str29[] JANUS_PROGMEM = "Relay 6";

static const char relay_str30[] JANUS_PROGMEM = "relay_6_label";

static const janus_widget_desc_t relay_arr8[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str28, .static_text = NULL, .text_is_format = false, .geometry = {0, 268, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_6), .dirty_offset = offsetof(relay_dirty_t, relay_6), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_6, .navigate_target = -1, .focus_order = 6, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str30, .static_text = relay_str29, .text_is_format = false, .geometry = {28, 268, 292, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str31[] JANUS_PROGMEM = "";

static const char relay_str32[] JANUS_PROGMEM = "relay_7";

static const char relay_str33[] JANUS_PROGMEM = "Relay 7";

static const char relay_str34[] JANUS_PROGMEM = "relay_7_label";

static const janus_widget_desc_t relay_arr9[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str32, .static_text = NULL, .text_is_format = false, .geometry = {0, 304, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_7), .dirty_offset = offsetof(relay_dirty_t, relay_7), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_7, .navigate_target = -1, .focus_order = 7, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str34, .static_text = relay_str33, .text_is_format = false, .geometry = {28, 304, 292, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str35[] JANUS_PROGMEM = "";

static const janus_widget_desc_t relay_arr10[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = relay_str7, .static_text = NULL, .text_is_format = false, .geometry = {0, 52, 320, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr2, .child_count = 2, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str11, .static_text = NULL, .text_is_format = false, .geometry = {0, 88, 320, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr3, .child_count = 2, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str15, .static_text = NULL, .text_is_format = false, .geometry = {0, 124, 320, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr4, .child_count = 2, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str19, .static_text = NULL, .text_is_format = false, .geometry = {0, 160, 320, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr5, .child_count = 2, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str23, .static_text = NULL, .text_is_format = false, .geometry = {0, 196, 320, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr6, .child_count = 2, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str27, .static_text = NULL, .text_is_format = false, .geometry = {0, 232, 320, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr7, .child_count = 2, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str31, .static_text = NULL, .text_is_format = false, .geometry = {0, 268, 320, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr8, .child_count = 2, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str35, .static_text = NULL, .text_is_format = false, .geometry = {0, 304, 320, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr9, .child_count = 2, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str36[] JANUS_PROGMEM = "relay_list";

static const janus_widget_desc_t relay_widgets[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = relay_str3, .static_text = NULL, .text_is_format = false, .geometry = {0, 28, 140, 20}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr1, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_COLUMN, .id = relay_str36, .static_text = NULL, .text_is_format = false, .geometry = {0, 52, 320, 428}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr10, .child_count = 8, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str37[] JANUS_PROGMEM = "Relay";

const janus_screen_desc_t relay_screen JANUS_PROGMEM = {
    .name = relay_str37,
    .widgets = relay_widgets,
    .widget_count = 2,
    .bound_struct = &relay_instance,
    .bound_dirty = &relay_dirty,
    .resolve_images = NULL,
    .image_far = NULL,
};
