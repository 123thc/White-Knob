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
// lv_timer_t *gear_timer = NULL;

// static void gears_rotate_timer(lv_timer_t *timer)
// {
//     // 每次回调让齿轮旋转 +3°
//     static int16_t angle = 0;
//     angle += 10;  // 3.0°
//     if(angle >= 3600) angle = 0;

//     lv_img_set_angle(guider_ui.screen_time_2_img_1, angle);
//     lv_img_set_angle(guider_ui.screen_time_2_img_2, angle);
//     lv_img_set_angle(guider_ui.screen_time_2_img_3, angle);
//     lv_img_set_angle(guider_ui.screen_time_2_img_4, -angle);
// }


void setup_scr_screen_time_2(lv_ui *ui)
{
    //Write codes screen_time_2
    ui->screen_time_2 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_time_2, 240, 240);
    lv_obj_set_scrollbar_mode(ui->screen_time_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_time_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_time_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_time_2_img_bg
    ui->screen_time_2_img_bg = lv_img_create(ui->screen_time_2);
    lv_obj_add_flag(ui->screen_time_2_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_time_2_img_bg, &_image_bg_alpha_240x240);
    lv_img_set_pivot(ui->screen_time_2_img_bg, 50,50);
    lv_img_set_angle(ui->screen_time_2_img_bg, 0);
    lv_obj_set_pos(ui->screen_time_2_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_time_2_img_bg, 240, 240);

    //Write style for screen_time_2_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_time_2_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_time_2_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_time_2_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_time_2_img_bg, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_time_2_cont_1
    ui->screen_time_2_cont_1 = lv_obj_create(ui->screen_time_2);
    lv_obj_set_pos(ui->screen_time_2_cont_1, 60, 60);
    lv_obj_set_size(ui->screen_time_2_cont_1, 120, 120);
    lv_obj_set_scrollbar_mode(ui->screen_time_2_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_time_2_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_time_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_time_2_cont_1, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_time_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_time_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_time_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_time_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_time_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_time_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_time_2_img_3
    ui->screen_time_2_img_3 = lv_img_create(ui->screen_time_2_cont_1);
    lv_obj_add_flag(ui->screen_time_2_img_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_time_2_img_3, &_UI_Img_Horoscope03_alpha_120x120);
    lv_img_set_pivot(ui->screen_time_2_img_3, 60,60);
    lv_img_set_angle(ui->screen_time_2_img_3, 0);
    lv_obj_set_pos(ui->screen_time_2_img_3, 6, -11);
    lv_obj_set_size(ui->screen_time_2_img_3, 120, 120);

    //Write style for screen_time_2_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_time_2_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_time_2_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_time_2_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_time_2_img_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_time_2_img_1
    ui->screen_time_2_img_1 = lv_img_create(ui->screen_time_2_cont_1);
    lv_obj_add_flag(ui->screen_time_2_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_time_2_img_1, &_UI_Img_Horoscope06_alpha_30x30);
    lv_img_set_pivot(ui->screen_time_2_img_1, 15,15);
    lv_img_set_angle(ui->screen_time_2_img_1, 0);
    lv_obj_set_pos(ui->screen_time_2_img_1, 69, -15);
    lv_obj_set_size(ui->screen_time_2_img_1, 30, 30);

    //Write style for screen_time_2_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_time_2_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_time_2_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_time_2_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_time_2_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_time_2_img_2
    ui->screen_time_2_img_2 = lv_img_create(ui->screen_time_2_cont_1);
    lv_obj_add_flag(ui->screen_time_2_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_time_2_img_2, &_UI_Img_Horoscope04_alpha_110x110);
    lv_img_set_pivot(ui->screen_time_2_img_2, 55,55);
    lv_img_set_angle(ui->screen_time_2_img_2, 0);
    lv_obj_set_pos(ui->screen_time_2_img_2, -19, 11);
    lv_obj_set_size(ui->screen_time_2_img_2, 110, 110);

    //Write style for screen_time_2_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_time_2_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_time_2_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_time_2_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_time_2_img_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_time_2_img_4
    ui->screen_time_2_img_4 = lv_img_create(ui->screen_time_2_cont_1);
    lv_obj_add_flag(ui->screen_time_2_img_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_time_2_img_4, &_UI_Img_Horoscope05_alpha_60x60);
    lv_img_set_pivot(ui->screen_time_2_img_4, 30,30);
    lv_img_set_angle(ui->screen_time_2_img_4, 0);
    lv_obj_set_pos(ui->screen_time_2_img_4, 82, 63);
    lv_obj_set_size(ui->screen_time_2_img_4, 60, 60);

    //Write style for screen_time_2_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_time_2_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_time_2_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_time_2_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_time_2_img_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_time_2_img_minute_hand
    ui->screen_time_2_img_minute_hand = lv_img_create(ui->screen_time_2);
    lv_obj_add_flag(ui->screen_time_2_img_minute_hand, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_time_2_img_minute_hand, &_UI_Clock_HourHand_alpha_240x240);
    lv_img_set_pivot(ui->screen_time_2_img_minute_hand, 120,120);
    lv_img_set_angle(ui->screen_time_2_img_minute_hand, 400);
    lv_obj_set_pos(ui->screen_time_2_img_minute_hand, 0, 0);
    lv_obj_set_size(ui->screen_time_2_img_minute_hand, 240, 240);

    //Write style for screen_time_2_img_minute_hand, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_time_2_img_minute_hand, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_time_2_img_minute_hand, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_time_2_img_minute_hand, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_time_2_img_minute_hand, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_time_2_img_hour_hand
    ui->screen_time_2_img_hour_hand = lv_img_create(ui->screen_time_2);
    lv_obj_add_flag(ui->screen_time_2_img_hour_hand, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_time_2_img_hour_hand, &_UI_Clock_MinuteHand_alpha_240x240);
    lv_img_set_pivot(ui->screen_time_2_img_hour_hand, 120,120);
    lv_img_set_angle(ui->screen_time_2_img_hour_hand, 0);
    lv_obj_set_pos(ui->screen_time_2_img_hour_hand, 0, 0);
    lv_obj_set_size(ui->screen_time_2_img_hour_hand, 240, 240);

    //Write style for screen_time_2_img_hour_hand, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_time_2_img_hour_hand, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_time_2_img_hour_hand, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_time_2_img_hour_hand, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_time_2_img_hour_hand, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_time_2.
    //设置容器为圆形
    lv_obj_set_style_radius(guider_ui.screen_time_2_cont_1, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(guider_ui.screen_time_2_cont_1, true, LV_PART_MAIN);

// gear_timer = lv_timer_create(gears_rotate_timer, 50, NULL);

    //Update current screen layout.
    lv_obj_update_layout(ui->screen_time_2);

}
