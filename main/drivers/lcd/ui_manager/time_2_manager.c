//
// Created by k0921 on 2026/5/4.
//

#include "time_2_manager.h"
#include "gui_guider.h"
#include "config/ui_manager.h"
#include "time/get_my_time.h"

lv_timer_t *gear_timer = NULL;

static void gears_rotate_timer(lv_timer_t *timer)
{
    // 每次回调让齿轮旋转 +3°
    static int16_t angle = 0;
    angle += 10;  // 3.0°
    if(angle >= 3600) angle = 0;

    lv_img_set_angle(guider_ui.screen_time_2_img_1, angle);
    lv_img_set_angle(guider_ui.screen_time_2_img_2, angle);
    lv_img_set_angle(guider_ui.screen_time_2_img_3, angle);
    lv_img_set_angle(guider_ui.screen_time_2_img_4, -angle);

    static uint8_t s_flag = 0;
    int h, m, s;
    //在50ms的定时回调里，20次为1s
    if (s_flag != 20) {
        s_flag++;
    }
    else {
        s_flag = 0;
        time_get_hms(&h, &m, &s);
        h = h > 12 ? h - 12 : h;
        int16_t m_angle = 360 / 60 * m;
        m_angle = m_angle > 360 ? m_angle - 360 : m_angle;
        lv_img_set_angle(guider_ui.screen_time_2_img_minute_hand, m_angle * 10);
        lv_img_set_angle(guider_ui.screen_time_2_img_hour_hand, 360 / 12 * h * 10 + 1800);
    }
}

/**
 * 进入时间界面前的回调函数
 */
void time_2_enter(void) {
    //加载时间界面
    if(guider_ui.screen_time_2_del) {
        setup_scr_screen_time_2(&guider_ui);
        guider_ui.screen_time_2_del = false;
    }
    lv_scr_load(guider_ui.screen_time_2);

    if (gear_timer == NULL) {
        gear_timer = lv_timer_create(gears_rotate_timer, 50, NULL);
    }

    int h, m, s;
    time_get_hms(&h, &m, &s);
    h = h > 12 ? h - 12 : h;
    int16_t m_angle = 360 / 60 * m;
    m_angle = m_angle > 360 ? m_angle - 360 : m_angle;
    lv_img_set_angle(guider_ui.screen_time_2_img_minute_hand, m_angle * 10);
    lv_img_set_angle(guider_ui.screen_time_2_img_hour_hand, 360 / 12 * h * 10 + 1800);
}


void time_2_short_press(void) {
    screen_manager_set_active(&time_interface);
}

/**
 * 退出时间界面的操作
 */
void time_2_exit(void) {
    //删除时间界面的屏幕
    if (guider_ui.screen_time_2) {
        lv_obj_del(guider_ui.screen_time_2);
        guider_ui.screen_time_2_del = true;
        guider_ui.screen_time_2 = NULL;
    }
    //删除相关定时器
    if (gear_timer) {
        lv_timer_del(gear_timer);
        gear_timer = NULL;
    }
}

/**
 * 长按触发的操作
 */
void time_2_long_press(void) {
    //返回菜单界面
    screen_manager_set_active(&menu_interface);
}

/**
 * 注册时间界面
 */
void time_2_register(void) {
    time_2_interface.enter = time_2_enter;
    time_2_interface.exit = time_2_exit;
    time_2_interface.rotate = NULL;
    time_2_interface.rotate_stop = NULL;
    time_2_interface.short_press = time_2_short_press;
    time_2_interface.release_press = NULL;
    time_2_interface.long_press = time_2_long_press;
    time_2_interface.is_registered = true;
}