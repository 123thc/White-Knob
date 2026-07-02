//
// Created by k0921 on 2026/4/23.
//

#include "time_manager.h"
#include "gui_guider.h"
#include "config/ui_manager.h"
#include "time/get_my_time.h"
#include "time_bg.h"

lv_timer_t *clock_timer;

void time_callback(lv_timer_t *timer) {
    int h, m, s;
    time_get_hms(&h, &m, &s);
    lv_label_set_text_fmt(guider_ui.screen_time_label_1, "%02d:\n%02d", h, m);
}

/**
 * 进入时间界面前的回调函数
 */
void time_enter(void) {
    //加载时间界面
    if(guider_ui.screen_time_del) {
        setup_scr_screen_time(&guider_ui);
        guider_ui.screen_time_del = false;
    }
    time_bg_apply_to_time_screen();
    lv_scr_load(guider_ui.screen_time);

    if (clock_timer == NULL) {
        clock_timer = lv_timer_create(time_callback, 1000, NULL);
    }

    int h, m, s;
    time_get_hms(&h, &m, &s);
    lv_label_set_text_fmt(guider_ui.screen_time_label_1, "%02d:", h);
    lv_label_set_text_fmt(guider_ui.screen_time_label_2, "%02d", m);
}


void time_short_press(void) {
    screen_manager_set_active(&time_2_interface);
}

/**
 * 退出时间界面的操作
 */
void time_exit(void) {
    //删除时间界面的屏幕
    if (guider_ui.screen_time) {
        lv_obj_del(guider_ui.screen_time);
        guider_ui.screen_time_del = true;
        guider_ui.screen_time = NULL;
    }
    if (clock_timer) {
        lv_timer_del(clock_timer);
        clock_timer = NULL;
    }

}

/**
 * 长按触发的操作
 */
void time_long_press(void) {
    //返回菜单界面
    screen_manager_set_active(&menu_interface);
}

/**
 * 注册时间界面
 */
void time_register(void) {
    time_interface.enter = time_enter;
    time_interface.exit = time_exit;
    time_interface.rotate = NULL;
    time_interface.rotate_stop = NULL;
    time_interface.short_press = time_short_press;
    time_interface.release_press = NULL;
    time_interface.long_press = time_long_press;
    time_interface.is_registered = true;
}
