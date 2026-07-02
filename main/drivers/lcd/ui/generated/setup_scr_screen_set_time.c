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
static void build_roller_options(char *buf, size_t buf_size, int max_val) {
    if (buf_size == 0) return;
    buf[0] = '\0';
    size_t remain = buf_size;
    for (int i = 0; i <= max_val; i++) {
        char tmp[16];  // 足够容纳 int 值
        if (i < max_val)
            snprintf(tmp, sizeof(tmp), "%d\n", i);
        else
            snprintf(tmp, sizeof(tmp), "%d", i);

        size_t len = strlen(tmp);
        if (len >= remain) {  // 缓冲区不足，截断
            strncat(buf, tmp, remain - 1);
            break;
        }
        strcat(buf, tmp);
        remain -= len;
    }
}


void setup_scr_screen_set_time(lv_ui *ui)
{
    //Write codes screen_set_time
    ui->screen_set_time = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_set_time, 240, 240);
    lv_obj_set_scrollbar_mode(ui->screen_set_time, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_time, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_time, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_time_roller_1
    ui->screen_set_time_roller_1 = lv_roller_create(ui->screen_set_time);
    lv_roller_set_options(ui->screen_set_time_roller_1, "0\n1\n2\n22\n23", LV_ROLLER_MODE_INFINITE);
    lv_obj_set_pos(ui->screen_set_time_roller_1, 13, 76);
    lv_obj_set_width(ui->screen_set_time_roller_1, 64);

    //Write style for screen_set_time_roller_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->screen_set_time_roller_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_time_roller_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_time_roller_1, lv_color_hex(0xffc720), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_time_roller_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_time_roller_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_time_roller_1, &lv_font_AlimamaDaoLiTi_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_time_roller_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_time_roller_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_time_roller_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_time_roller_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_time_roller_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_time_roller_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_time_roller_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_time_roller_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_time_roller_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_set_time_roller_1, Part: LV_PART_SELECTED, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_time_roller_1, 255, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_time_roller_1, lv_color_hex(0x20ff58), LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_time_roller_1, LV_GRAD_DIR_NONE, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_time_roller_1, lv_color_hex(0x000000), LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_time_roller_1, &lv_font_AlimamaDaoLiTi_20, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_time_roller_1, 255, LV_PART_SELECTED|LV_STATE_DEFAULT);

    lv_roller_set_visible_row_count(ui->screen_set_time_roller_1, 3);
    //Write codes screen_set_time_roller_2
    ui->screen_set_time_roller_2 = lv_roller_create(ui->screen_set_time);
    lv_roller_set_options(ui->screen_set_time_roller_2, "0\n1\n2\n58\n59", LV_ROLLER_MODE_INFINITE);
    lv_obj_set_pos(ui->screen_set_time_roller_2, 88, 76);
    lv_obj_set_width(ui->screen_set_time_roller_2, 64);

    //Write style for screen_set_time_roller_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->screen_set_time_roller_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_time_roller_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_time_roller_2, lv_color_hex(0xffc720), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_time_roller_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_time_roller_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_time_roller_2, &lv_font_AlimamaDaoLiTi_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_time_roller_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_time_roller_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_time_roller_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_time_roller_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_time_roller_2, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_time_roller_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_time_roller_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_time_roller_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_time_roller_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_set_time_roller_2, Part: LV_PART_SELECTED, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_time_roller_2, 255, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_time_roller_2, lv_color_hex(0x20ff58), LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_time_roller_2, LV_GRAD_DIR_NONE, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_time_roller_2, lv_color_hex(0x000000), LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_time_roller_2, &lv_font_AlimamaDaoLiTi_20, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_time_roller_2, 255, LV_PART_SELECTED|LV_STATE_DEFAULT);

    lv_roller_set_visible_row_count(ui->screen_set_time_roller_2, 3);
    //Write codes screen_set_time_roller_3
    ui->screen_set_time_roller_3 = lv_roller_create(ui->screen_set_time);
    lv_roller_set_options(ui->screen_set_time_roller_3, "0\n1\n2\n58\n59", LV_ROLLER_MODE_INFINITE);
    lv_obj_set_pos(ui->screen_set_time_roller_3, 162, 76);
    lv_obj_set_width(ui->screen_set_time_roller_3, 64);

    //Write style for screen_set_time_roller_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->screen_set_time_roller_3, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_time_roller_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_time_roller_3, lv_color_hex(0xffc720), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_time_roller_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_time_roller_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_time_roller_3, &lv_font_AlimamaDaoLiTi_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_time_roller_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_time_roller_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_time_roller_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_time_roller_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_time_roller_3, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_time_roller_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_time_roller_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_time_roller_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_time_roller_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_set_time_roller_3, Part: LV_PART_SELECTED, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_time_roller_3, 255, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_time_roller_3, lv_color_hex(0x20ff58), LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_time_roller_3, LV_GRAD_DIR_NONE, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_time_roller_3, lv_color_hex(0x000000), LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_time_roller_3, &lv_font_AlimamaDaoLiTi_20, LV_PART_SELECTED|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_time_roller_3, 255, LV_PART_SELECTED|LV_STATE_DEFAULT);

    lv_roller_set_visible_row_count(ui->screen_set_time_roller_3, 3);
    //Write codes screen_set_time_label_1
    ui->screen_set_time_label_1 = lv_label_create(ui->screen_set_time);
    lv_label_set_text(ui->screen_set_time_label_1, "时");
    lv_label_set_long_mode(ui->screen_set_time_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_time_label_1, 18, 44);
    lv_obj_set_size(ui->screen_set_time_label_1, 50, 30);

    //Write style for screen_set_time_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_time_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_time_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_time_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_time_label_1, &lv_font_AlimamaDaoLiTi_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_time_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_time_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_time_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_time_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_time_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_time_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_time_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_time_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_time_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_time_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_time_label_2
    ui->screen_set_time_label_2 = lv_label_create(ui->screen_set_time);
    lv_label_set_text(ui->screen_set_time_label_2, "分");
    lv_label_set_long_mode(ui->screen_set_time_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_time_label_2, 94, 44);
    lv_obj_set_size(ui->screen_set_time_label_2, 50, 30);

    //Write style for screen_set_time_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_time_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_time_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_time_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_time_label_2, &lv_font_AlimamaDaoLiTi_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_time_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_time_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_time_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_time_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_time_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_time_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_time_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_time_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_time_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_time_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_time_label_3
    ui->screen_set_time_label_3 = lv_label_create(ui->screen_set_time);
    lv_label_set_text(ui->screen_set_time_label_3, "秒");
    lv_label_set_long_mode(ui->screen_set_time_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_time_label_3, 170, 44);
    lv_obj_set_size(ui->screen_set_time_label_3, 50, 30);

    //Write style for screen_set_time_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_time_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_time_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_time_label_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_time_label_3, &lv_font_AlimamaDaoLiTi_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_time_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_time_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_time_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_time_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_time_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_time_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_time_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_time_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_time_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_time_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_set_time.
    // char opts[512];
// build_roller_options(opts, sizeof(opts), 23);
// lv_roller_set_options(guider_ui.screen_set_time_roller_1, opts, LV_ROLLER_MODE_INFINITE); // 无限模式 ✅

// build_roller_options(opts, sizeof(opts), 40);
// lv_roller_set_options(guider_ui.screen_set_time_roller_2, opts, LV_ROLLER_MODE_INFINITE); // 无限模式 ✅

// build_roller_options(opts, sizeof(opts), 23);
// lv_roller_set_options(guider_ui.screen_set_time_roller_3, opts, LV_ROLLER_MODE_INFINITE); // 无限模式 ✅

// // 全部归零
// lv_roller_set_selected(guider_ui.screen_set_time_roller_1, 0, LV_ANIM_OFF);
// lv_roller_set_selected(guider_ui.screen_set_time_roller_2, 0, LV_ANIM_OFF);
// lv_roller_set_selected(guider_ui.screen_set_time_roller_3, 0, LV_ANIM_OFF);

    //Update current screen layout.
    lv_obj_update_layout(ui->screen_set_time);

}
