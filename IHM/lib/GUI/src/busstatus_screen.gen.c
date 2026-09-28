#include "busstatus_screen.gen.h"
#include "janus_actions.gen.h"
#include "janus_bindings.gen.h"
static const char busstatus_str1[] JANUS_PROGMEM = "can0_enabled_toggle";

static const janus_widget_desc_t busstatus_arr1[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = busstatus_str1, .static_text = NULL, .text_is_format = false, .geometry = {6, 54, 28, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_enabled), .dirty_offset = offsetof(bus_status_dirty_t, can0_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_CAN0_ENABLED, .navigate_target = -1, .focus_order = 0, .focus_ring = 1, .knob_off_color = 0xf800, .knob_on_color = 0x07e0, .knob_flags = JANUS_KNOB_OFF_SET | JANUS_KNOB_ON_SET, .color = 0xd69a, .bg_color = 0xd69a, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str2[] JANUS_PROGMEM = "";

static const char busstatus_str3[] JANUS_PROGMEM = "CAN0";

static const char busstatus_str4[] JANUS_PROGMEM = "can0_label";

static const char busstatus_str5[] JANUS_PROGMEM = "ID %x";

static const char busstatus_str6[] JANUS_PROGMEM = "can0_id";

static const janus_widget_desc_t busstatus_arr2[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str6, .static_text = busstatus_str5, .text_is_format = true, .geometry = {98, 54, 121, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_id), .dirty_offset = offsetof(bus_status_dirty_t, can0_id), .field_type = JANUS_FIELD_INT64, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN0_ID, .navigate_target = -1, .focus_order = 1, .focus_ring = 2, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str7[] JANUS_PROGMEM = "";

static const char busstatus_str8[] JANUS_PROGMEM = "DLC %d";

static const char busstatus_str9[] JANUS_PROGMEM = "can0_dlc";

static const janus_widget_desc_t busstatus_arr3[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str9, .static_text = busstatus_str8, .text_is_format = true, .geometry = {235, 54, 66, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_dlc), .dirty_offset = offsetof(bus_status_dirty_t, can0_dlc), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN0_DLC, .navigate_target = -1, .focus_order = 2, .focus_ring = 3, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str10[] JANUS_PROGMEM = "";

static const janus_widget_desc_t busstatus_arr4[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str2, .static_text = NULL, .text_is_format = false, .geometry = {0, 48, 40, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr1, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str4, .static_text = busstatus_str3, .text_is_format = false, .geometry = {44, 54, 44, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str7, .static_text = NULL, .text_is_format = false, .geometry = {92, 48, 133, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr2, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str10, .static_text = NULL, .text_is_format = false, .geometry = {229, 48, 78, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr3, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str11[] JANUS_PROGMEM = "can0_row";

static const char busstatus_str12[] JANUS_PROGMEM = "EXT%d";

static const char busstatus_str13[] JANUS_PROGMEM = "can0_extended";

static const janus_widget_desc_t busstatus_arr5[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str13, .static_text = busstatus_str12, .text_is_format = true, .geometry = {6, 88, 44, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_extended), .dirty_offset = offsetof(bus_status_dirty_t, can0_extended), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN0_EXTENDED, .navigate_target = -1, .focus_order = 3, .focus_ring = 4, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str14[] JANUS_PROGMEM = "";

static const char busstatus_str15[] JANUS_PROGMEM = "%dms";

static const char busstatus_str16[] JANUS_PROGMEM = "can0_period_ms";

static const janus_widget_desc_t busstatus_arr6[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str16, .static_text = busstatus_str15, .text_is_format = true, .geometry = {66, 88, 77, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_period_ms), .dirty_offset = offsetof(bus_status_dirty_t, can0_period_ms), .field_type = JANUS_FIELD_INT64, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN0_PERIOD, .navigate_target = -1, .focus_order = 4, .focus_ring = 5, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str17[] JANUS_PROGMEM = "";

static const char busstatus_str18[] JANUS_PROGMEM = "%s";

static const char busstatus_str19[] JANUS_PROGMEM = "can0_repeat";

static const janus_widget_desc_t busstatus_arr7[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str19, .static_text = busstatus_str18, .text_is_format = true, .geometry = {159, 88, 66, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_repeat_label), .dirty_offset = offsetof(bus_status_dirty_t, can0_repeat_label), .field_type = JANUS_FIELD_STRING, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN0_REPEAT, .navigate_target = -1, .focus_order = 5, .focus_ring = 6, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str20[] JANUS_PROGMEM = "";

static const char busstatus_str21[] JANUS_PROGMEM = "B%d";

static const char busstatus_str22[] JANUS_PROGMEM = "can0_byte_index";

static const janus_widget_desc_t busstatus_arr8[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str22, .static_text = busstatus_str21, .text_is_format = true, .geometry = {241, 88, 22, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_byte_index), .dirty_offset = offsetof(bus_status_dirty_t, can0_byte_index), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN0_BYTE_INDEX, .navigate_target = -1, .focus_order = 6, .focus_ring = 7, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str23[] JANUS_PROGMEM = "";

static const char busstatus_str24[] JANUS_PROGMEM = "=%x";

static const char busstatus_str25[] JANUS_PROGMEM = "can0_byte_value";

static const janus_widget_desc_t busstatus_arr9[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str25, .static_text = busstatus_str24, .text_is_format = true, .geometry = {279, 88, 33, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_byte_value), .dirty_offset = offsetof(bus_status_dirty_t, can0_byte_value), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN0_BYTE_VALUE, .navigate_target = -1, .focus_order = 7, .focus_ring = 8, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str26[] JANUS_PROGMEM = "";

static const janus_widget_desc_t busstatus_arr10[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str14, .static_text = NULL, .text_is_format = false, .geometry = {0, 82, 56, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr5, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str17, .static_text = NULL, .text_is_format = false, .geometry = {60, 82, 89, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr6, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str20, .static_text = NULL, .text_is_format = false, .geometry = {153, 82, 78, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr7, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str23, .static_text = NULL, .text_is_format = false, .geometry = {235, 82, 34, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr8, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str26, .static_text = NULL, .text_is_format = false, .geometry = {273, 82, 45, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr9, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str27[] JANUS_PROGMEM = "can0_row2";

static const char busstatus_str28[] JANUS_PROGMEM = "can1_enabled_toggle";

static const janus_widget_desc_t busstatus_arr11[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = busstatus_str28, .static_text = NULL, .text_is_format = false, .geometry = {6, 122, 28, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_enabled), .dirty_offset = offsetof(bus_status_dirty_t, can1_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_CAN1_ENABLED, .navigate_target = -1, .focus_order = 8, .focus_ring = 9, .knob_off_color = 0xf800, .knob_on_color = 0x07e0, .knob_flags = JANUS_KNOB_OFF_SET | JANUS_KNOB_ON_SET, .color = 0xd69a, .bg_color = 0xd69a, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str29[] JANUS_PROGMEM = "";

static const char busstatus_str30[] JANUS_PROGMEM = "CAN1";

static const char busstatus_str31[] JANUS_PROGMEM = "can1_label";

static const char busstatus_str32[] JANUS_PROGMEM = "ID %x";

static const char busstatus_str33[] JANUS_PROGMEM = "can1_id";

static const janus_widget_desc_t busstatus_arr12[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str33, .static_text = busstatus_str32, .text_is_format = true, .geometry = {98, 122, 121, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_id), .dirty_offset = offsetof(bus_status_dirty_t, can1_id), .field_type = JANUS_FIELD_INT64, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN1_ID, .navigate_target = -1, .focus_order = 9, .focus_ring = 10, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str34[] JANUS_PROGMEM = "";

static const char busstatus_str35[] JANUS_PROGMEM = "DLC %d";

static const char busstatus_str36[] JANUS_PROGMEM = "can1_dlc";

static const janus_widget_desc_t busstatus_arr13[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str36, .static_text = busstatus_str35, .text_is_format = true, .geometry = {235, 122, 66, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_dlc), .dirty_offset = offsetof(bus_status_dirty_t, can1_dlc), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN1_DLC, .navigate_target = -1, .focus_order = 10, .focus_ring = 11, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str37[] JANUS_PROGMEM = "";

static const janus_widget_desc_t busstatus_arr14[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str29, .static_text = NULL, .text_is_format = false, .geometry = {0, 116, 40, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr11, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str31, .static_text = busstatus_str30, .text_is_format = false, .geometry = {44, 122, 44, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str34, .static_text = NULL, .text_is_format = false, .geometry = {92, 116, 133, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr12, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str37, .static_text = NULL, .text_is_format = false, .geometry = {229, 116, 78, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr13, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str38[] JANUS_PROGMEM = "can1_row";

static const char busstatus_str39[] JANUS_PROGMEM = "EXT%d";

static const char busstatus_str40[] JANUS_PROGMEM = "can1_extended";

static const janus_widget_desc_t busstatus_arr15[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str40, .static_text = busstatus_str39, .text_is_format = true, .geometry = {6, 156, 44, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_extended), .dirty_offset = offsetof(bus_status_dirty_t, can1_extended), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN1_EXTENDED, .navigate_target = -1, .focus_order = 11, .focus_ring = 12, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str41[] JANUS_PROGMEM = "";

static const char busstatus_str42[] JANUS_PROGMEM = "%dms";

static const char busstatus_str43[] JANUS_PROGMEM = "can1_period_ms";

static const janus_widget_desc_t busstatus_arr16[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str43, .static_text = busstatus_str42, .text_is_format = true, .geometry = {66, 156, 77, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_period_ms), .dirty_offset = offsetof(bus_status_dirty_t, can1_period_ms), .field_type = JANUS_FIELD_INT64, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN1_PERIOD, .navigate_target = -1, .focus_order = 12, .focus_ring = 13, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str44[] JANUS_PROGMEM = "";

static const char busstatus_str45[] JANUS_PROGMEM = "%s";

static const char busstatus_str46[] JANUS_PROGMEM = "can1_repeat";

static const janus_widget_desc_t busstatus_arr17[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str46, .static_text = busstatus_str45, .text_is_format = true, .geometry = {159, 156, 66, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_repeat_label), .dirty_offset = offsetof(bus_status_dirty_t, can1_repeat_label), .field_type = JANUS_FIELD_STRING, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN1_REPEAT, .navigate_target = -1, .focus_order = 13, .focus_ring = 14, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str47[] JANUS_PROGMEM = "";

static const char busstatus_str48[] JANUS_PROGMEM = "B%d";

static const char busstatus_str49[] JANUS_PROGMEM = "can1_byte_index";

static const janus_widget_desc_t busstatus_arr18[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str49, .static_text = busstatus_str48, .text_is_format = true, .geometry = {241, 156, 22, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_byte_index), .dirty_offset = offsetof(bus_status_dirty_t, can1_byte_index), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN1_BYTE_INDEX, .navigate_target = -1, .focus_order = 14, .focus_ring = 15, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str50[] JANUS_PROGMEM = "";

static const char busstatus_str51[] JANUS_PROGMEM = "=%x";

static const char busstatus_str52[] JANUS_PROGMEM = "can1_byte_value";

static const janus_widget_desc_t busstatus_arr19[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str52, .static_text = busstatus_str51, .text_is_format = true, .geometry = {279, 156, 33, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_byte_value), .dirty_offset = offsetof(bus_status_dirty_t, can1_byte_value), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_CAN1_BYTE_VALUE, .navigate_target = -1, .focus_order = 15, .focus_ring = 16, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str53[] JANUS_PROGMEM = "";

static const janus_widget_desc_t busstatus_arr20[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str41, .static_text = NULL, .text_is_format = false, .geometry = {0, 150, 56, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr15, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str44, .static_text = NULL, .text_is_format = false, .geometry = {60, 150, 89, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr16, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str47, .static_text = NULL, .text_is_format = false, .geometry = {153, 150, 78, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr17, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str50, .static_text = NULL, .text_is_format = false, .geometry = {235, 150, 34, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr18, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str53, .static_text = NULL, .text_is_format = false, .geometry = {273, 150, 45, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr19, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str54[] JANUS_PROGMEM = "can1_row2";

static const char busstatus_str55[] JANUS_PROGMEM = "rs485_enabled_toggle";

static const janus_widget_desc_t busstatus_arr21[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = busstatus_str55, .static_text = NULL, .text_is_format = false, .geometry = {6, 190, 28, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, rs485_enabled), .dirty_offset = offsetof(bus_status_dirty_t, rs485_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RS485_ENABLED, .navigate_target = -1, .focus_order = 16, .focus_ring = 17, .knob_off_color = 0xf800, .knob_on_color = 0x07e0, .knob_flags = JANUS_KNOB_OFF_SET | JANUS_KNOB_ON_SET, .color = 0xd69a, .bg_color = 0xd69a, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str56[] JANUS_PROGMEM = "";

static const char busstatus_str57[] JANUS_PROGMEM = "RS485";

static const char busstatus_str58[] JANUS_PROGMEM = "rs485_label";

static const char busstatus_str59[] JANUS_PROGMEM = "LEN %d";

static const char busstatus_str60[] JANUS_PROGMEM = "rs485_length";

static const janus_widget_desc_t busstatus_arr22[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str60, .static_text = busstatus_str59, .text_is_format = true, .geometry = {114, 190, 66, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, rs485_length), .dirty_offset = offsetof(bus_status_dirty_t, rs485_length), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_RS485_LENGTH, .navigate_target = -1, .focus_order = 17, .focus_ring = 18, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str61[] JANUS_PROGMEM = "";

static const janus_widget_desc_t busstatus_arr23[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str56, .static_text = NULL, .text_is_format = false, .geometry = {0, 184, 40, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr21, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str58, .static_text = busstatus_str57, .text_is_format = false, .geometry = {44, 190, 60, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str61, .static_text = NULL, .text_is_format = false, .geometry = {108, 184, 78, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr22, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str62[] JANUS_PROGMEM = "rs485_row";

static const char busstatus_str63[] JANUS_PROGMEM = "%dms";

static const char busstatus_str64[] JANUS_PROGMEM = "rs485_period_ms";

static const janus_widget_desc_t busstatus_arr24[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str64, .static_text = busstatus_str63, .text_is_format = true, .geometry = {6, 224, 77, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, rs485_period_ms), .dirty_offset = offsetof(bus_status_dirty_t, rs485_period_ms), .field_type = JANUS_FIELD_INT64, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_RS485_PERIOD, .navigate_target = -1, .focus_order = 18, .focus_ring = 19, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str65[] JANUS_PROGMEM = "";

static const char busstatus_str66[] JANUS_PROGMEM = "%s";

static const char busstatus_str67[] JANUS_PROGMEM = "rs485_repeat";

static const janus_widget_desc_t busstatus_arr25[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str67, .static_text = busstatus_str66, .text_is_format = true, .geometry = {99, 224, 66, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, rs485_repeat_label), .dirty_offset = offsetof(bus_status_dirty_t, rs485_repeat_label), .field_type = JANUS_FIELD_STRING, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_RS485_REPEAT, .navigate_target = -1, .focus_order = 19, .focus_ring = 20, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str68[] JANUS_PROGMEM = "";

static const char busstatus_str69[] JANUS_PROGMEM = "B%d";

static const char busstatus_str70[] JANUS_PROGMEM = "rs485_byte_index";

static const janus_widget_desc_t busstatus_arr26[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str70, .static_text = busstatus_str69, .text_is_format = true, .geometry = {181, 224, 33, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, rs485_byte_index), .dirty_offset = offsetof(bus_status_dirty_t, rs485_byte_index), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_RS485_BYTE_INDEX, .navigate_target = -1, .focus_order = 20, .focus_ring = 21, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str71[] JANUS_PROGMEM = "";

static const char busstatus_str72[] JANUS_PROGMEM = "=%x";

static const char busstatus_str73[] JANUS_PROGMEM = "rs485_byte_value";

static const janus_widget_desc_t busstatus_arr27[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str73, .static_text = busstatus_str72, .text_is_format = true, .geometry = {230, 224, 33, 18}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, rs485_byte_value), .dirty_offset = offsetof(bus_status_dirty_t, rs485_byte_value), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_EDIT_RS485_BYTE_VALUE, .navigate_target = -1, .focus_order = 21, .focus_ring = 22, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_MEDIUM, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str74[] JANUS_PROGMEM = "";

static const janus_widget_desc_t busstatus_arr28[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str65, .static_text = NULL, .text_is_format = false, .geometry = {0, 218, 89, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr24, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str68, .static_text = NULL, .text_is_format = false, .geometry = {93, 218, 78, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr25, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str71, .static_text = NULL, .text_is_format = false, .geometry = {175, 218, 45, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr26, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str74, .static_text = NULL, .text_is_format = false, .geometry = {224, 218, 45, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr27, .child_count = 1, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char busstatus_str75[] JANUS_PROGMEM = "rs485_row2";

static const janus_widget_desc_t busstatus_widgets[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str11, .static_text = NULL, .text_is_format = false, .geometry = {0, 48, 307, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr4, .child_count = 4, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str27, .static_text = NULL, .text_is_format = false, .geometry = {0, 82, 318, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr10, .child_count = 5, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str38, .static_text = NULL, .text_is_format = false, .geometry = {0, 116, 307, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr14, .child_count = 4, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str54, .static_text = NULL, .text_is_format = false, .geometry = {0, 150, 318, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr20, .child_count = 5, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str62, .static_text = NULL, .text_is_format = false, .geometry = {0, 184, 186, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr23, .child_count = 3, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = busstatus_str75, .static_text = NULL, .text_is_format = false, .geometry = {0, 218, 269, 30}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = busstatus_arr28, .child_count = 4, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const janus_focus_ring_t busstatus_focus_rings[] JANUS_PROGMEM = {
    { .rect = {0, 48, 40, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {92, 48, 133, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {229, 48, 78, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 82, 56, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {60, 82, 89, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {153, 82, 78, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {235, 82, 34, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {273, 82, 45, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 116, 40, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {92, 116, 133, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {229, 116, 78, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 150, 56, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {60, 150, 89, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {153, 150, 78, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {235, 150, 34, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {273, 150, 45, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 184, 40, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {108, 184, 78, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 218, 89, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {93, 218, 78, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {175, 218, 45, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {224, 218, 45, 30}, .bg_color = JANUS_COLOR_DEFAULT_BG }
};

static const char busstatus_str76[] JANUS_PROGMEM = "BusStatus";

const janus_screen_desc_t busstatus_screen JANUS_PROGMEM = {
    .name = busstatus_str76,
    .widgets = busstatus_widgets,
    .widget_count = 6,
    .bound_struct = &bus_status_instance,
    .bound_dirty = &bus_status_dirty,
    .resolve_images = NULL,
    .image_far = NULL,
    .focus_rings = busstatus_focus_rings,
};
