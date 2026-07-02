//
// Created by k0921 on 2026/4/23.
//

#include "set_time_manager.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "config/ui_manager.h"
#include "gui_guider.h"
#include "smart_knob/motor_app.h"
#include "time/get_my_time.h"

#define SET_TIME_HOUR_COUNT 24
#define SET_TIME_MIN_SEC_COUNT 60
#define SET_TIME_ROLLER_ROWS 3

static int roller_focus = 0;

static uint16_t hour, min, sec;
static uint16_t edit_hour, edit_min, edit_sec;

static uint16_t set_time_wrap_value(int value, uint16_t range) {
    while (value < 0) {
        value += range;
    }

    return (uint16_t)(value % range);
}

static uint16_t set_time_focus_range(void) {
    return roller_focus == 0 ? SET_TIME_HOUR_COUNT : SET_TIME_MIN_SEC_COUNT;
}

static void set_time_set_detent_for_focus(void) {
    g_haptic_state.mode = MODE_DETENT;
    g_haptic_state.params.detent.steps = set_time_focus_range();
    g_haptic_state.params.detent.width = 0;
    g_vibratic_next_mode = MODE_DETENT;
}

static void set_time_configure_roller(lv_obj_t *roller) {
    lv_obj_set_style_text_font(roller, &lv_font_AlimamaDaoLiTi_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(roller, &lv_font_AlimamaDaoLiTi_20, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(roller, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(roller, 0, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(roller, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(roller, 0, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_roller_set_visible_row_count(roller, SET_TIME_ROLLER_ROWS);
}

static void set_time_set_three_row_roller(lv_obj_t *roller, uint16_t value, uint16_t range) {
    char opts[32];
    uint16_t prev = set_time_wrap_value((int)value - 1, range);
    uint16_t next = set_time_wrap_value((int)value + 1, range);

    snprintf(opts, sizeof(opts), "%u\n%u\n%u", (unsigned)prev, (unsigned)value, (unsigned)next);
    lv_roller_set_options(roller, opts, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, SET_TIME_ROLLER_ROWS);
    lv_roller_set_selected(roller, 1, LV_ANIM_OFF);
}

static void set_time_apply_edit_values(void) {
    set_time_set_three_row_roller(guider_ui.screen_set_time_roller_1, edit_hour, SET_TIME_HOUR_COUNT);
    set_time_set_three_row_roller(guider_ui.screen_set_time_roller_2, edit_min, SET_TIME_MIN_SEC_COUNT);
    set_time_set_three_row_roller(guider_ui.screen_set_time_roller_3, edit_sec, SET_TIME_MIN_SEC_COUNT);
}

static void set_time_roll_hours(int dir) {
    edit_hour = set_time_wrap_value((int)edit_hour + dir, SET_TIME_HOUR_COUNT);
}

static void set_time_roll_minutes(int dir) {
    edit_min = set_time_wrap_value((int)edit_min + dir, SET_TIME_MIN_SEC_COUNT);
}

static void set_time_roll_seconds(int dir) {
    edit_sec = set_time_wrap_value((int)edit_sec + dir, SET_TIME_MIN_SEC_COUNT);
}

static void set_time_roll_focused_value(int dir) {
    switch (roller_focus) {
        case 0:
            set_time_roll_hours(dir);
            break;
        case 1:
            set_time_roll_minutes(dir);
            break;
        case 2:
            set_time_roll_seconds(dir);
            break;
        default:
            break;
    }
}

void set_time_enter(void) {
    if (guider_ui.screen_set_time_del) {
        setup_scr_screen_set_time(&guider_ui);
        guider_ui.screen_set_time_del = false;
    }

    roller_focus = 0;
    edit_hour = 0;
    edit_min = 0;
    edit_sec = 0;
    set_time_set_detent_for_focus();

    set_time_configure_roller(guider_ui.screen_set_time_roller_1);
    set_time_configure_roller(guider_ui.screen_set_time_roller_2);
    set_time_configure_roller(guider_ui.screen_set_time_roller_3);
    set_time_apply_edit_values();

    lv_scr_load(guider_ui.screen_set_time);
    lv_obj_update_layout(guider_ui.screen_set_time);
    set_time_apply_edit_values();
}

void set_time_exit(void) {
    if (guider_ui.screen_set_time) {
        lv_obj_del(guider_ui.screen_set_time);
        guider_ui.screen_set_time_del = true;
        guider_ui.screen_set_time = NULL;
    }

    roller_focus = 0;
    g_haptic_state.mode = MODE_FREE;
    g_vibratic_next_mode = MODE_FREE;
}

void set_time_set_mhs(uint16_t *h, uint16_t *m, uint16_t *s) {
    *h = hour;
    *m = min;
    *s = sec;
}

void set_time_short_press(void) {
    if (roller_focus < 2) {
        roller_focus++;
        set_time_set_detent_for_focus();
    }
    else {
        hour = edit_hour;
        min = edit_min;
        sec = edit_sec;

        screen_manager_set_active(&potatotime_interface);
    }
}

void set_time_long_press(void) {
    screen_manager_set_active(&menu_interface);
}

void set_time_rotate(float *delta, const float *angle) {
    (void)angle;

    if (delta == NULL) {
        return;
    }

    uint16_t range = set_time_focus_range();
    if (range == 0) {
        return;
    }

    float spacing = 360.0f / (float)range;
    int steps = (int)(*delta / spacing);
    if (steps == 0) {
        return;
    }

    *delta -= steps * spacing;

    int dir = steps > 0 ? 1 : -1;
    int count = abs(steps);
    while (count-- > 0) {
        set_time_roll_focused_value(dir);
    }

    set_time_apply_edit_values();
}

void set_time_register(void) {
    set_time_interface.enter = set_time_enter;
    set_time_interface.exit = set_time_exit;
    set_time_interface.rotate = set_time_rotate;
    set_time_interface.rotate_stop = NULL;
    set_time_interface.short_press = set_time_short_press;
    set_time_interface.release_press = NULL;
    set_time_interface.long_press = set_time_long_press;
    set_time_interface.is_registered = true;
}
