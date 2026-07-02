/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen_potatotime(lv_ui *ui)
{
    //Write codes screen_potatotime
    ui->screen_potatotime = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_potatotime, 240, 240);
    lv_obj_set_scrollbar_mode(ui->screen_potatotime, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_potatotime, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_potatotime, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_potatotime, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_potatotime, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_potatotime_arc_1
    ui->screen_potatotime_arc_1 = lv_arc_create(ui->screen_potatotime);
    lv_arc_set_mode(ui->screen_potatotime_arc_1, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->screen_potatotime_arc_1, 0, 100);
    lv_arc_set_bg_angles(ui->screen_potatotime_arc_1, 135, 45);
    lv_arc_set_value(ui->screen_potatotime_arc_1, 70);
    lv_arc_set_rotation(ui->screen_potatotime_arc_1, 0);
    lv_obj_set_pos(ui->screen_potatotime_arc_1, 0, 0);
    lv_obj_set_size(ui->screen_potatotime_arc_1, 240, 240);

    //Write style for screen_potatotime_arc_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_potatotime_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_potatotime_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->screen_potatotime_arc_1, 12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_potatotime_arc_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_potatotime_arc_1, lv_color_hex(0xfcff8d), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_potatotime_arc_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_potatotime_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_potatotime_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_potatotime_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_potatotime_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_potatotime_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_potatotime_arc_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->screen_potatotime_arc_1, 12, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_potatotime_arc_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_potatotime_arc_1, lv_color_hex(0x2FDAAE), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_potatotime_arc_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_potatotime_arc_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->screen_potatotime_arc_1, 5, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_potatotime_lottie_1
    ui->screen_potatotime_lottie_1 = lv_rlottie_create_from_raw(ui->screen_potatotime, 100, 100, (const void *)lottie_loading);
    lv_obj_set_pos(ui->screen_potatotime_lottie_1, 70, 70);
    lv_obj_set_size(ui->screen_potatotime_lottie_1, 100, 100);

    //Write style for screen_potatotime_lottie_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_potatotime_lottie_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_potatotime_label_1
    ui->screen_potatotime_label_1 = lv_label_create(ui->screen_potatotime);
    lv_label_set_text(ui->screen_potatotime_label_1, "00 : 00 : 00");
    lv_label_set_long_mode(ui->screen_potatotime_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_potatotime_label_1, 61, 154);
    lv_obj_set_size(ui->screen_potatotime_label_1, 118, 32);

    //Write style for screen_potatotime_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_potatotime_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_potatotime_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_potatotime_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_potatotime_label_1, &lv_font_AlimamaDaoLiTi_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_potatotime_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_potatotime_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_potatotime_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_potatotime_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_potatotime_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_potatotime_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_potatotime_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_potatotime_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_potatotime_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_potatotime_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_potatotime.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_potatotime);

}
