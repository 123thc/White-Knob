//
// Created by k0921 on 2026/4/23.
//

#include "gui_guider.h"
#include "lcd/custom_operation/adsorb.h"
#include "lcd/custom_operation/scroll.h"
#include "menu_manager.h"
#include "config/ui_manager.h"
#include "custom.h"
#include "esp_log.h"

#define TAG "menu_manager"

//步进阈值
#define STEP_THRESHOLD 5.0f

static bool is_rotating = false;

//进入菜单界面的回调函数
void menu_enter(void) {
    if(guider_ui.screen_menu_del) {
        setup_scr_screen_menu(&guider_ui);
        guider_ui.screen_menu_del = false;
    }
    lv_scr_load(guider_ui.screen_menu);
}

/**
 * 菜单旋转操作
 * @param detal 角度变化量
 */
void menu_rotate(float *delta, const float *angle) {
    is_rotating = true;
    encoder_scroll_move(*delta * (5));
    int steps = (int)(*delta / STEP_THRESHOLD);

    if (steps != 0) {
        *delta -= steps * STEP_THRESHOLD; // 减去已处理的部分
    }
}

/**
 * 旋转停止时的操作
 */
void menu_rotate_stop(void) {
    //只触发一次界面吸附
    if (is_rotating) {
        interfacial_adsorption();
        is_rotating = false;
    }
}

/**
 * 短按时的操作
 */
void menu_short_press(void) {
    switch (index_selected) {
        case SHIJIAN:
            screen_manager_set_active(&time_interface);
            break;
        case SHEZHI:
            screen_manager_set_active(&wifi_interface);
            break;
        case XUANNIU:
            screen_manager_set_active(&percent_interface);
            break;
        case YINLIANG:
            screen_manager_set_active(&light_interface);
            break;
        case SURFACEDAIL:
            screen_manager_set_active(&dial_interface);
            break;
        case FANQIESHIZHONG:
            screen_manager_set_active(&potatotime_interface);
        default:break;
    }

}

/**
 * 注册菜单界面
 */
void menu_register(void) {
    menu_interface.enter = menu_enter;
    menu_interface.exit = NULL;
    menu_interface.rotate = menu_rotate;
    menu_interface.rotate_stop = menu_rotate_stop;
    menu_interface.short_press = menu_short_press;
    menu_interface.release_press = NULL;
    menu_interface.long_press = NULL;
    menu_interface.is_registered = true;
}

