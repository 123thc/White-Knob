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



void setup_scr_screen_pow_on(lv_ui *ui)
{
    //Write codes screen_pow_on
    ui->screen_pow_on = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_pow_on, 240, 240);
    lv_obj_set_scrollbar_mode(ui->screen_pow_on, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_pow_on, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_pow_on, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_pow_on, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_pow_on, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_pow_on_img_1
    ui->screen_pow_on_img_1 = lv_img_create(ui->screen_pow_on);
    lv_obj_add_flag(ui->screen_pow_on_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_pow_on_img_1, &_yinliang_alpha_50x50);
    lv_img_set_pivot(ui->screen_pow_on_img_1, 50,50);
    lv_img_set_angle(ui->screen_pow_on_img_1, 0);
    lv_obj_set_pos(ui->screen_pow_on_img_1, 20, 33);
    lv_obj_set_size(ui->screen_pow_on_img_1, 50, 50);
    lv_obj_add_flag(ui->screen_pow_on_img_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_pow_on_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_pow_on_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_pow_on_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pow_on_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_pow_on_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_pow_on_img_2
    ui->screen_pow_on_img_2 = lv_img_create(ui->screen_pow_on);
    lv_obj_add_flag(ui->screen_pow_on_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_pow_on_img_2, &_surfacedial_alpha_50x50);
    lv_img_set_pivot(ui->screen_pow_on_img_2, 50,50);
    lv_img_set_angle(ui->screen_pow_on_img_2, 0);
    lv_obj_set_pos(ui->screen_pow_on_img_2, 107, 33);
    lv_obj_set_size(ui->screen_pow_on_img_2, 50, 50);
    lv_obj_add_flag(ui->screen_pow_on_img_2, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_pow_on_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_pow_on_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_pow_on_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pow_on_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_pow_on_img_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_pow_on_img_3
    ui->screen_pow_on_img_3 = lv_img_create(ui->screen_pow_on);
    lv_obj_add_flag(ui->screen_pow_on_img_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_pow_on_img_3, &_dial_alpha_50x50);
    lv_img_set_pivot(ui->screen_pow_on_img_3, 50,50);
    lv_img_set_angle(ui->screen_pow_on_img_3, 0);
    lv_obj_set_pos(ui->screen_pow_on_img_3, 91, 109);
    lv_obj_set_size(ui->screen_pow_on_img_3, 50, 50);
    lv_obj_add_flag(ui->screen_pow_on_img_3, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_pow_on_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_pow_on_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_pow_on_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pow_on_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_pow_on_img_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_pow_on_img_4
    ui->screen_pow_on_img_4 = lv_img_create(ui->screen_pow_on);
    lv_obj_add_flag(ui->screen_pow_on_img_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_pow_on_img_4, &_shijian_alpha_50x50);
    lv_img_set_pivot(ui->screen_pow_on_img_4, 50,50);
    lv_img_set_angle(ui->screen_pow_on_img_4, 0);
    lv_obj_set_pos(ui->screen_pow_on_img_4, 171, 104);
    lv_obj_set_size(ui->screen_pow_on_img_4, 50, 50);
    lv_obj_add_flag(ui->screen_pow_on_img_4, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_pow_on_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_pow_on_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_pow_on_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pow_on_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_pow_on_img_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_pow_on_img_5
    ui->screen_pow_on_img_5 = lv_img_create(ui->screen_pow_on);
    lv_obj_add_flag(ui->screen_pow_on_img_5, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_pow_on_img_5, &_potato_alpha_50x50);
    lv_img_set_pivot(ui->screen_pow_on_img_5, 50,50);
    lv_img_set_angle(ui->screen_pow_on_img_5, 0);
    lv_obj_set_pos(ui->screen_pow_on_img_5, 16, 135);
    lv_obj_set_size(ui->screen_pow_on_img_5, 50, 50);
    lv_obj_add_flag(ui->screen_pow_on_img_5, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_pow_on_img_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_pow_on_img_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_pow_on_img_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pow_on_img_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_pow_on_img_5, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_pow_on_label_1
    ui->screen_pow_on_label_1 = lv_label_create(ui->screen_pow_on);
    lv_label_set_text(ui->screen_pow_on_label_1, "灯光旋钮番茄时钟已");
    lv_label_set_long_mode(ui->screen_pow_on_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_pow_on_label_1, 83, 180);
    lv_obj_set_size(ui->screen_pow_on_label_1, 100, 32);
    lv_obj_add_flag(ui->screen_pow_on_label_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_pow_on_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_pow_on_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pow_on_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_pow_on_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_pow_on_label_1, &lv_font_AlimamaDaoLiTi_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_pow_on_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_pow_on_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_pow_on_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_pow_on_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_pow_on_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_pow_on_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_pow_on_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_pow_on_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_pow_on_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_pow_on_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_pow_on_lottie_1
    ui->screen_pow_on_lottie_1 = lv_rlottie_create_from_raw(ui->screen_pow_on, 50, 50, (const void *)lottie_double_circle);
    lv_obj_set_pos(ui->screen_pow_on_lottie_1, 42, 142);
    lv_obj_set_size(ui->screen_pow_on_lottie_1, 50, 50);

    //Write style for screen_pow_on_lottie_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_pow_on_lottie_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_pow_on_label_2
    ui->screen_pow_on_label_2 = lv_label_create(ui->screen_pow_on);
    lv_label_set_text(ui->screen_pow_on_label_2, "Loading...");
    lv_label_set_long_mode(ui->screen_pow_on_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_pow_on_label_2, 100, 154);
    lv_obj_set_size(ui->screen_pow_on_label_2, 100, 32);

    //Write style for screen_pow_on_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_pow_on_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pow_on_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_pow_on_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_pow_on_label_2, &lv_font_AlimamaDaoLiTi_19, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_pow_on_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_pow_on_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_pow_on_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_pow_on_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_pow_on_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_pow_on_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_pow_on_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_pow_on_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_pow_on_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_pow_on_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_pow_on_label_knobname
    ui->screen_pow_on_label_knobname = lv_label_create(ui->screen_pow_on);
    lv_label_set_text(ui->screen_pow_on_label_knobname, "白-Knob");
    lv_label_set_long_mode(ui->screen_pow_on_label_knobname, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_pow_on_label_knobname, 49, 75);
    lv_obj_set_size(ui->screen_pow_on_label_knobname, 137, 45);

    //Write style for screen_pow_on_label_knobname, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_pow_on_label_knobname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pow_on_label_knobname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_pow_on_label_knobname, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_pow_on_label_knobname, &lv_font_AlimamaDaoLiTi_35, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_pow_on_label_knobname, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_pow_on_label_knobname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_pow_on_label_knobname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_pow_on_label_knobname, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_pow_on_label_knobname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_pow_on_label_knobname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_pow_on_label_knobname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_pow_on_label_knobname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_pow_on_label_knobname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_pow_on_label_knobname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_pow_on_bar_1
    ui->screen_pow_on_bar_1 = lv_bar_create(ui->screen_pow_on);
    lv_obj_set_style_anim_time(ui->screen_pow_on_bar_1, 1000, 0);
    lv_bar_set_mode(ui->screen_pow_on_bar_1, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_pow_on_bar_1, 0, 100);
    lv_bar_set_value(ui->screen_pow_on_bar_1, 50, LV_ANIM_ON);
    lv_obj_set_pos(ui->screen_pow_on_bar_1, 66, 120);
    lv_obj_set_size(ui->screen_pow_on_bar_1, 90, 4);

    //Write style for screen_pow_on_bar_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_pow_on_bar_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pow_on_bar_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_pow_on_bar_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_pow_on_bar_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_pow_on_bar_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_pow_on_bar_1, lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_pow_on_bar_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pow_on_bar_1, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_pow_on_img_6
    ui->screen_pow_on_img_6 = lv_img_create(ui->screen_pow_on);
    lv_obj_add_flag(ui->screen_pow_on_img_6, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_pow_on_img_6, &_light_alpha_50x50);
    lv_img_set_pivot(ui->screen_pow_on_img_6, 50,50);
    lv_img_set_angle(ui->screen_pow_on_img_6, 0);
    lv_obj_set_pos(ui->screen_pow_on_img_6, 70, 185);
    lv_obj_set_size(ui->screen_pow_on_img_6, 50, 50);
    lv_obj_add_flag(ui->screen_pow_on_img_6, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_pow_on_img_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_pow_on_img_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_pow_on_img_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_pow_on_img_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_pow_on_img_6, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_pow_on.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_pow_on);

    //Init events for screen.
    events_init_screen_pow_on(ui);
}
