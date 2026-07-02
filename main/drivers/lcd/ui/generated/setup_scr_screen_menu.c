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
#include "custom.h"



void setup_scr_screen_menu(lv_ui *ui)
{
    //Write codes screen_menu
    ui->screen_menu = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_menu, 240, 240);
    lv_obj_set_scrollbar_mode(ui->screen_menu, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_menu, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_menu, 248, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_menu, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_menu, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_img_bg
    ui->screen_menu_img_bg = lv_img_create(ui->screen_menu);
    lv_obj_add_flag(ui->screen_menu_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_menu_img_bg, &_whale_alpha_240x240);
    lv_img_set_pivot(ui->screen_menu_img_bg, 50,50);
    lv_img_set_angle(ui->screen_menu_img_bg, 0);
    lv_obj_set_pos(ui->screen_menu_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_menu_img_bg, 240, 240);
    lv_obj_add_flag(ui->screen_menu_img_bg, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_menu_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_menu_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_menu_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_img_bg, 120, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_menu_img_bg, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_img_bottom
    ui->screen_menu_img_bottom = lv_img_create(ui->screen_menu);
    lv_obj_add_flag(ui->screen_menu_img_bottom, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_menu_img_bottom, &_percent_alpha_50x50);
    lv_img_set_pivot(ui->screen_menu_img_bottom, 50,50);
    lv_img_set_angle(ui->screen_menu_img_bottom, 0);
    lv_obj_set_pos(ui->screen_menu_img_bottom, 95, 185);
    lv_obj_set_size(ui->screen_menu_img_bottom, 50, 50);

    //Write style for screen_menu_img_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_menu_img_bottom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_menu_img_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_img_bottom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_menu_img_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_arc_1
    ui->screen_menu_arc_1 = lv_arc_create(ui->screen_menu);
    lv_arc_set_mode(ui->screen_menu_arc_1, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->screen_menu_arc_1, 0, 100);
    lv_arc_set_bg_angles(ui->screen_menu_arc_1, 310, 0);
    lv_arc_set_value(ui->screen_menu_arc_1, 70);
    lv_arc_set_rotation(ui->screen_menu_arc_1, 249);
    lv_obj_set_style_arc_rounded(ui->screen_menu_arc_1, 0,  LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->screen_menu_arc_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_menu_arc_1, 0, 0);
    lv_obj_set_size(ui->screen_menu_arc_1, 240, 240);
    lv_obj_add_flag(ui->screen_menu_arc_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_menu_arc_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_menu_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_menu_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->screen_menu_arc_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_menu_arc_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_menu_arc_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_arc_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_menu_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_menu_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_menu_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_menu_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_menu_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_menu_arc_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->screen_menu_arc_1, 6, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_menu_arc_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_menu_arc_1, lv_color_hex(0xfd761d), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_menu_arc_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_menu_arc_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->screen_menu_arc_1, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_menu_cont_induction_main
    ui->screen_menu_cont_induction_main = lv_obj_create(ui->screen_menu);
    lv_obj_set_pos(ui->screen_menu_cont_induction_main, 100, 68);
    lv_obj_set_size(ui->screen_menu_cont_induction_main, 128, 104);
    lv_obj_set_scrollbar_mode(ui->screen_menu_cont_induction_main, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_menu_cont_induction_main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_menu_cont_induction_main, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_menu_cont_induction_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_menu_cont_induction_main, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_menu_cont_induction_main, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_cont_induction_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_menu_cont_induction_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_menu_cont_induction_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_menu_cont_induction_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_menu_cont_induction_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_menu_cont_induction_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_menu_cont_induction_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_label_induction
    ui->screen_menu_label_induction = lv_label_create(ui->screen_menu_cont_induction_main);
    lv_label_set_text(ui->screen_menu_label_induction, "Usage Wifi\nSettings");
    lv_label_set_long_mode(ui->screen_menu_label_induction, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_menu_label_induction, 11, 37);
    lv_obj_set_size(ui->screen_menu_label_induction, 115, 32);

    //Write style for screen_menu_label_induction, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_menu_label_induction, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_label_induction, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_menu_label_induction, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_menu_label_induction, &lv_font_AlimamaDaoLiTi_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_menu_label_induction, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_menu_label_induction, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_menu_label_induction, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_menu_label_induction, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_menu_label_induction, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_menu_label_induction, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_menu_label_induction, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_menu_label_induction, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_menu_label_induction, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_menu_label_induction, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_cont_move_obj
    ui->screen_menu_cont_move_obj = lv_obj_create(ui->screen_menu_cont_induction_main);
    lv_obj_set_pos(ui->screen_menu_cont_move_obj, -140, 2);
    lv_obj_set_size(ui->screen_menu_cont_move_obj, 144, 93);
    lv_obj_set_scrollbar_mode(ui->screen_menu_cont_move_obj, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_menu_cont_move_obj, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_menu_cont_move_obj, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_menu_cont_move_obj, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_menu_cont_move_obj, lv_color_hex(0xffae1d), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_menu_cont_move_obj, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_cont_move_obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_menu_cont_move_obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_menu_cont_move_obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_menu_cont_move_obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_menu_cont_move_obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_menu_cont_move_obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_menu_cont_move_obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_cont_cover
    ui->screen_menu_cont_cover = lv_obj_create(ui->screen_menu_cont_move_obj);
    lv_obj_set_pos(ui->screen_menu_cont_cover, 13, 9);
    lv_obj_set_size(ui->screen_menu_cont_cover, 123, 73);
    lv_obj_set_scrollbar_mode(ui->screen_menu_cont_cover, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_menu_cont_cover, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_menu_cont_cover, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_menu_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_menu_cont_cover, lv_color_hex(0x1dff62), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_menu_cont_cover, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_menu_cont_cover, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_menu_cont_cover, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_menu_cont_cover, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_menu_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_menu_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_menu_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_menu_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_menu_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_line_1
    ui->screen_menu_line_1 = lv_line_create(ui->screen_menu_cont_move_obj);
    static lv_point_t screen_menu_line_1[] = {{0, 0},{0, 80},};
    lv_line_set_points(ui->screen_menu_line_1, screen_menu_line_1, 2);
    lv_obj_set_pos(ui->screen_menu_line_1, 138, 6);
    lv_obj_set_size(ui->screen_menu_line_1, 5, 80);

    //Write style for screen_menu_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_menu_line_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_menu_line_1, lv_color_hex(0x00cbff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_menu_line_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_menu_line_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_cont_label_selected
    ui->screen_menu_cont_label_selected = lv_obj_create(ui->screen_menu);
    lv_obj_set_pos(ui->screen_menu_cont_label_selected, -12, 154);
    lv_obj_set_size(ui->screen_menu_cont_label_selected, 120, 19);
    lv_obj_set_scrollbar_mode(ui->screen_menu_cont_label_selected, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_menu_cont_label_selected, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_menu_cont_label_selected, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_menu_cont_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_menu_cont_label_selected, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_menu_cont_label_selected, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_cont_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_menu_cont_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_menu_cont_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_menu_cont_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_menu_cont_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_menu_cont_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_menu_cont_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_label_selected
    ui->screen_menu_label_selected = lv_label_create(ui->screen_menu_cont_label_selected);
    lv_label_set_text(ui->screen_menu_label_selected, "配网");
    lv_label_set_long_mode(ui->screen_menu_label_selected, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_menu_label_selected, 0, 0);
    lv_obj_set_size(ui->screen_menu_label_selected, 120, 19);

    //Write style for screen_menu_label_selected, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_menu_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_menu_label_selected, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_menu_label_selected, &lv_font_AlimamaDaoLiTi_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_menu_label_selected, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_menu_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_menu_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_menu_label_selected, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_menu_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_menu_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_menu_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_menu_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_menu_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_menu_label_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_img_selected
    ui->screen_menu_img_selected = lv_img_create(ui->screen_menu);
    lv_obj_add_flag(ui->screen_menu_img_selected, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_menu_img_selected, &_wifi_alpha_50x50);
    lv_img_set_pivot(ui->screen_menu_img_selected, 50,50);
    lv_img_set_angle(ui->screen_menu_img_selected, 0);
    lv_obj_set_pos(ui->screen_menu_img_selected, 25, 87);
    lv_obj_set_size(ui->screen_menu_img_selected, 50, 50);

    //Write style for screen_menu_img_selected, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_menu_img_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_menu_img_selected, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_img_selected, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_menu_img_selected, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_cont_top
    ui->screen_menu_cont_top = lv_obj_create(ui->screen_menu);
    lv_obj_set_pos(ui->screen_menu_cont_top, 60, 30);
    lv_obj_set_size(ui->screen_menu_cont_top, 120, 19);
    lv_obj_set_scrollbar_mode(ui->screen_menu_cont_top, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_menu_cont_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_menu_cont_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_menu_cont_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_menu_cont_top, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_menu_cont_top, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_cont_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_menu_cont_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_menu_cont_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_menu_cont_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_menu_cont_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_menu_cont_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_menu_cont_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_label_top
    ui->screen_menu_label_top = lv_label_create(ui->screen_menu_cont_top);
    lv_label_set_text(ui->screen_menu_label_top, "时间");
    lv_label_set_long_mode(ui->screen_menu_label_top, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_menu_label_top, 0, 0);
    lv_obj_set_size(ui->screen_menu_label_top, 120, 19);

    //Write style for screen_menu_label_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_menu_label_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_label_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_menu_label_top, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_menu_label_top, &lv_font_AlimamaDaoLiTi_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_menu_label_top, 215, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_menu_label_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_menu_label_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_menu_label_top, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_menu_label_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_menu_label_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_menu_label_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_menu_label_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_menu_label_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_menu_label_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_menu.
    init_menu();




    //Update current screen layout.
    lv_obj_update_layout(ui->screen_menu);

    //Init events for screen.
    events_init_screen_menu(ui);
}
