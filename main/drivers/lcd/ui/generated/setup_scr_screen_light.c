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



void setup_scr_screen_light(lv_ui *ui)
{
    //Write codes screen_light
    ui->screen_light = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_light, 240, 240);
    lv_obj_set_scrollbar_mode(ui->screen_light, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_light, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_light, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_light, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_light, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_light_img_off
    ui->screen_light_img_off = lv_img_create(ui->screen_light);
    lv_obj_add_flag(ui->screen_light_img_off, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_light_img_off, &_light_off_alpha_100x100);
    lv_img_set_pivot(ui->screen_light_img_off, 50,50);
    lv_img_set_angle(ui->screen_light_img_off, 0);
    lv_obj_set_pos(ui->screen_light_img_off, 70, 41);
    lv_obj_set_size(ui->screen_light_img_off, 100, 100);

    //Write style for screen_light_img_off, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_light_img_off, 22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_light_img_off, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_light_img_off, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_light_img_off, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_light_img_off, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_light_img_on
    ui->screen_light_img_on = lv_img_create(ui->screen_light);
    lv_obj_add_flag(ui->screen_light_img_on, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_light_img_on, &_light_on_alpha_100x100);
    lv_img_set_pivot(ui->screen_light_img_on, 50,50);
    lv_img_set_angle(ui->screen_light_img_on, 0);
    lv_obj_set_pos(ui->screen_light_img_on, 70, 41);
    lv_obj_set_size(ui->screen_light_img_on, 100, 100);
    lv_obj_add_flag(ui->screen_light_img_on, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_light_img_on, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_light_img_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_light_img_on, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_light_img_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_light_img_on, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_light_sw_1
    ui->screen_light_sw_1 = lv_switch_create(ui->screen_light);
    lv_obj_set_pos(ui->screen_light_sw_1, 89, 150);
    lv_obj_set_size(ui->screen_light_sw_1, 61, 35);

    //Write style for screen_light_sw_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_light_sw_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_light_sw_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_light_sw_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_light_sw_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_light_sw_1, 153, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_light_sw_1, lv_color_hex(0x00bdff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_light_sw_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_light_sw_1, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_light_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_light_sw_1, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->screen_light_sw_1, 223, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->screen_light_sw_1, lv_color_hex(0x86ff93), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->screen_light_sw_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->screen_light_sw_1, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for screen_light_sw_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_light_sw_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_light_sw_1, lv_color_hex(0xeb0063), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_light_sw_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_light_sw_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_light_sw_1, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write style for screen_light_sw_1, Part: LV_PART_KNOB, State: LV_STATE_FOCUSED.
    lv_obj_set_style_bg_opa(ui->screen_light_sw_1, 255, LV_PART_KNOB|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(ui->screen_light_sw_1, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(ui->screen_light_sw_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui->screen_light_sw_1, 0, LV_PART_KNOB|LV_STATE_FOCUSED);
    lv_obj_set_style_radius(ui->screen_light_sw_1, 10, LV_PART_KNOB|LV_STATE_FOCUSED);

    //The custom code of screen_light.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_light);

}
