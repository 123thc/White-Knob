/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen_time;
	bool screen_time_del;
	lv_obj_t *screen_time_img_1;
	lv_obj_t *screen_time_label_1;
	lv_obj_t *screen_time_label_2;
	lv_obj_t *screen_light;
	bool screen_light_del;
	lv_obj_t *screen_light_img_off;
	lv_obj_t *screen_light_img_on;
	lv_obj_t *screen_light_sw_1;
	lv_obj_t *screen_menu;
	bool screen_menu_del;
	lv_obj_t *screen_menu_img_bg;
	lv_obj_t *screen_menu_img_bottom;
	lv_obj_t *screen_menu_arc_1;
	lv_obj_t *screen_menu_cont_induction_main;
	lv_obj_t *screen_menu_label_induction;
	lv_obj_t *screen_menu_cont_move_obj;
	lv_obj_t *screen_menu_cont_cover;
	lv_obj_t *screen_menu_line_1;
	lv_obj_t *screen_menu_cont_label_selected;
	lv_obj_t *screen_menu_label_selected;
	lv_obj_t *screen_menu_img_selected;
	lv_obj_t *screen_menu_cont_top;
	lv_obj_t *screen_menu_label_top;
	lv_obj_t *screen_percent;
	bool screen_percent_del;
	lv_obj_t *screen_percent_label_2;
	lv_obj_t *screen_percent_cont_1;
	lv_obj_t *screen_percent_label_3;
	lv_obj_t *screen_percent_arc_1;
	lv_obj_t *screen_pow_on;
	bool screen_pow_on_del;
	lv_obj_t *screen_pow_on_img_1;
	lv_obj_t *screen_pow_on_img_2;
	lv_obj_t *screen_pow_on_img_3;
	lv_obj_t *screen_pow_on_img_4;
	lv_obj_t *screen_pow_on_img_5;
	lv_obj_t *screen_pow_on_label_1;
	lv_obj_t *screen_pow_on_lottie_1;
	lv_obj_t *screen_pow_on_label_2;
	lv_obj_t *screen_pow_on_label_knobname;
	lv_obj_t *screen_pow_on_bar_1;
	lv_obj_t *screen_pow_on_img_6;
	lv_obj_t *screen_dial;
	bool screen_dial_del;
	lv_obj_t *screen_dial_lottie_1;
	lv_obj_t *screen_dial_label_1;
	lv_obj_t *screen_wifi;
	bool screen_wifi_del;
	lv_obj_t *screen_wifi_roller_1;
	lv_obj_t *screen_wifi_label_1;
	lv_obj_t *screen_wifi_lottie_1;
	lv_obj_t *screen_wifi_cont_1;
	lv_obj_t *screen_wifi_label_2;
	lv_obj_t *screen_potatotime;
	bool screen_potatotime_del;
	lv_obj_t *screen_potatotime_arc_1;
	lv_obj_t *screen_potatotime_lottie_1;
	lv_obj_t *screen_potatotime_label_1;
	lv_obj_t *screen_time_2;
	bool screen_time_2_del;
	lv_obj_t *screen_time_2_img_bg;
	lv_obj_t *screen_time_2_cont_1;
	lv_obj_t *screen_time_2_img_3;
	lv_obj_t *screen_time_2_img_1;
	lv_obj_t *screen_time_2_img_2;
	lv_obj_t *screen_time_2_img_4;
	lv_obj_t *screen_time_2_img_minute_hand;
	lv_obj_t *screen_time_2_img_hour_hand;
	lv_obj_t *screen_set_time;
	bool screen_set_time_del;
	lv_obj_t *screen_set_time_roller_1;
	lv_obj_t *screen_set_time_roller_2;
	lv_obj_t *screen_set_time_roller_3;
	lv_obj_t *screen_set_time_label_1;
	lv_obj_t *screen_set_time_label_2;
	lv_obj_t *screen_set_time_label_3;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen_time(lv_ui *ui);
void setup_scr_screen_light(lv_ui *ui);
void setup_scr_screen_menu(lv_ui *ui);
void setup_scr_screen_percent(lv_ui *ui);
void setup_scr_screen_pow_on(lv_ui *ui);
void setup_scr_screen_dial(lv_ui *ui);
void setup_scr_screen_wifi(lv_ui *ui);
void setup_scr_screen_potatotime(lv_ui *ui);
void setup_scr_screen_time_2(lv_ui *ui);
void setup_scr_screen_set_time(lv_ui *ui);
LV_IMG_DECLARE(_xiaohei_alpha_240x240);
LV_IMG_DECLARE(_light_off_alpha_100x100);
LV_IMG_DECLARE(_light_on_alpha_100x100);
LV_IMG_DECLARE(_whale_alpha_240x240);
LV_IMG_DECLARE(_percent_alpha_50x50);
LV_IMG_DECLARE(_wifi_alpha_50x50);
LV_IMG_DECLARE(_yinliang_alpha_50x50);
LV_IMG_DECLARE(_surfacedial_alpha_50x50);
LV_IMG_DECLARE(_dial_alpha_50x50);
LV_IMG_DECLARE(_shijian_alpha_50x50);
LV_IMG_DECLARE(_potato_alpha_50x50);
#define LV_LOT_DECLARE(array_name) extern const uint8_t array_name[];
LV_LOT_DECLARE(lottie_double_circle);
LV_IMG_DECLARE(_light_alpha_50x50);
#define LV_LOT_DECLARE(array_name) extern const uint8_t array_name[];
LV_LOT_DECLARE(lottie_process);
#define LV_LOT_DECLARE(array_name) extern const uint8_t array_name[];
LV_LOT_DECLARE(lottie_wifi_on);
#define LV_LOT_DECLARE(array_name) extern const uint8_t array_name[];
LV_LOT_DECLARE(lottie_loading);
LV_IMG_DECLARE(_image_bg_alpha_240x240);
LV_IMG_DECLARE(_UI_Img_Horoscope03_alpha_120x120);
LV_IMG_DECLARE(_UI_Img_Horoscope06_alpha_30x30);
LV_IMG_DECLARE(_UI_Img_Horoscope04_alpha_110x110);
LV_IMG_DECLARE(_UI_Img_Horoscope05_alpha_60x60);
LV_IMG_DECLARE(_UI_Clock_HourHand_alpha_240x240);
LV_IMG_DECLARE(_UI_Clock_MinuteHand_alpha_240x240);

LV_FONT_DECLARE(lv_font_AlimamaDaoLiTi_58)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_AlimamaDaoLiTi_27)
LV_FONT_DECLARE(lv_font_AlimamaDaoLiTi_16)
LV_FONT_DECLARE(lv_font_AlimamaDaoLiTi_50)
LV_FONT_DECLARE(lv_font_AlimamaDaoLiTi_19)
LV_FONT_DECLARE(lv_font_AlimamaDaoLiTi_35)
LV_FONT_DECLARE(lv_font_AlimamaDaoLiTi_30)
LV_FONT_DECLARE(lv_font_AlimamaDaoLiTi_12)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_AlimamaDaoLiTi_18)
LV_FONT_DECLARE(lv_font_AlimamaDaoLiTi_22)
LV_FONT_DECLARE(lv_font_AlimamaDaoLiTi_20)


#ifdef __cplusplus
}
#endif
#endif
