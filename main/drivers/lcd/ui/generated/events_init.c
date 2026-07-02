/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lcd/ui_manager/config/ui_manager.h"
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif


#include "custom.h"

// 定时器回调函数
static void delay_cb(lv_timer_t *timer) {
    // ui_load_scr_animation(&guider_ui, &guider_ui.screen_menu, guider_ui.screen_menu_del, &guider_ui.screen_potatotime_del, setup_scr_screen_menu, LV_SCR_LOAD_ANIM_FADE_OUT, 0, 0, true, true);
    screen_manager_set_active(&menu_interface);
    // 任务执行完毕，删除这个一次性定时器，避免重复执行
    lv_timer_del(timer);
}

// 创建延时任务的函数
static void create_delay_task(lv_anim_t *anim) {
    // 创建定时器：5000毫秒后执行delay_cb，只执行一次
    lv_timer_create(delay_cb, 2000, NULL);
}

static void screen_time_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        switch(dir) {
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_get_act());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_light, guider_ui.screen_light_del, &guider_ui.screen_time_del, setup_scr_screen_light, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 200, false, true);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_time (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_time, screen_time_event_handler, LV_EVENT_ALL, ui);
}

static void screen_menu_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        // lv_img_set_src(apps_obj[index_selected], all_apps[index_selected].img_dsc);
        ui_animation(apps_obj[index_selected].obj, 500, 10, -57, 25, &lv_anim_path_ease_out, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        ui_animation(guider_ui.screen_menu_cont_move_obj, 500, 0, -32, -140, &lv_anim_path_ease_out, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        ui_animation(guider_ui.screen_menu_cont_induction_main, 100, 0, 133, 100, &lv_anim_path_linear, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);

        break;
    }
    default:
        break;
    }
}

void events_init_screen_menu (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_menu, screen_menu_event_handler, LV_EVENT_ALL, ui);
}

static void screen_percent_arc_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        break;
    }
    default:
        break;
    }
}

void events_init_screen_percent (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_percent_arc_1, screen_percent_arc_1_event_handler, LV_EVENT_ALL, ui);
}

static void screen_pow_on_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        ui_animation(guider_ui.screen_pow_on_label_knobname, 500, 0, -60, 49, &lv_anim_path_ease_out, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, create_delay_task, NULL);

        break;
    }
    default:
        break;
    }
}

void events_init_screen_pow_on (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_pow_on, screen_pow_on_event_handler, LV_EVENT_ALL, ui);
}

static void screen_dial_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCROLL:
    {
        break;
    }
    default:
        break;
    }
}

void events_init_screen_dial (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_dial, screen_dial_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
