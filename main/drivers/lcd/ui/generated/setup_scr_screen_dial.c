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



void setup_scr_screen_dial(lv_ui *ui)
{
    //Write codes screen_dial
    ui->screen_dial = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_dial, 240, 240);
    lv_obj_set_scrollbar_mode(ui->screen_dial, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_dial, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_dial, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_dial, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_dial, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_dial_lottie_1
    ui->screen_dial_lottie_1 = lv_obj_create(ui->screen_dial);
    lv_obj_set_pos(ui->screen_dial_lottie_1, 20, 20);
    lv_obj_set_size(ui->screen_dial_lottie_1, 200, 200);
    lv_obj_clear_flag(ui->screen_dial_lottie_1, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for screen_dial_lottie_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_dial_lottie_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_dial_lottie_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_dial_lottie_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_dial_lottie_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->screen_dial_lottie_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(ui->screen_dial_lottie_1, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *dial_pulse_outer = lv_obj_create(ui->screen_dial_lottie_1);
    lv_obj_set_size(dial_pulse_outer, 188, 188);
    lv_obj_center(dial_pulse_outer);
    lv_obj_clear_flag(dial_pulse_outer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(dial_pulse_outer, LV_RADIUS_CIRCLE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(dial_pulse_outer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dial_pulse_outer, lv_color_hex(0x5359FD), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dial_pulse_outer, 38, LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_obj_t *dial_pulse_mid = lv_obj_create(ui->screen_dial_lottie_1);
    lv_obj_set_size(dial_pulse_mid, 132, 132);
    lv_obj_center(dial_pulse_mid);
    lv_obj_clear_flag(dial_pulse_mid, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(dial_pulse_mid, LV_RADIUS_CIRCLE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(dial_pulse_mid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dial_pulse_mid, lv_color_hex(0x5359FD), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dial_pulse_mid, 82, LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_obj_t *dial_pulse_inner = lv_obj_create(ui->screen_dial_lottie_1);
    lv_obj_set_size(dial_pulse_inner, 74, 74);
    lv_obj_center(dial_pulse_inner);
    lv_obj_clear_flag(dial_pulse_inner, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(dial_pulse_inner, LV_RADIUS_CIRCLE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(dial_pulse_inner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dial_pulse_inner, lv_color_hex(0x5359FD), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dial_pulse_inner, 150, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_dial_label_1
    ui->screen_dial_label_1 = lv_label_create(ui->screen_dial);
    lv_label_set_text(ui->screen_dial_label_1, "Dial");
    lv_label_set_long_mode(ui->screen_dial_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_dial_label_1, 70, 104);
    lv_obj_set_size(ui->screen_dial_label_1, 100, 32);

    //Write style for screen_dial_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_dial_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_dial_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_dial_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_dial_label_1, &lv_font_AlimamaDaoLiTi_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_dial_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_dial_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_dial_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_dial_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_dial_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_dial_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_dial_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_dial_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_dial_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_dial_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_dial.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_dial);

    //Init events for screen.
    events_init_screen_dial(ui);
}
