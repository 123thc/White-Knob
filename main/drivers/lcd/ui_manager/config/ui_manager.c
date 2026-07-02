//
// Created by k0921 on 2026/4/23.
//

#include <stddef.h>
#include "ui_manager.h"
#include "lcd/ui_manager/time_2_manager.h"
#include "esp_log.h"
#include "custom.h"
#include "lcd/ui_manager//menu_manager.h"
#include "lcd/ui_manager//time_manager.h"
#include "lcd/ui_manager//dial_manager.h"
#include "lcd/ui_manager//percent_manager.h"
#include "lcd/ui_manager//potato_manager.h"
#include "lcd/ui_manager//light_manager.h"
#include "lcd/ui_manager//wifi_manager.h"
#include "lcd/ui_manager/set_time_manager.h"

#define TAG "ui_manager"

//菜单界面接口
screen_interface_t menu_interface;
//时间界面接口
screen_interface_t time_interface;
//时间界面接口
screen_interface_t time_2_interface;
//SurfaceDial接口
screen_interface_t dial_interface;
//灯光接口
screen_interface_t light_interface;
//旋钮接口
screen_interface_t percent_interface;
//番茄时钟接口
screen_interface_t potatotime_interface;
//设置时间接口
screen_interface_t set_time_interface;
//wifi接口
screen_interface_t wifi_interface;

//全局屏幕活动接口
screen_interface_t *s_active_interface = NULL;

/**
 * 更新显示界面，会依次调用旧界面的退出回调函数和新界面的进入回调函数
 * @param interface 新界面的接口
 */
void screen_manager_set_active(const screen_interface_t *interface) {
    if (interface == NULL || interface->is_registered == false) {
        return;
    }

    // Keep the original behavior: enter the new screen first, then exit the old one.
    if (interface->enter) {
        interface->enter();
    }
    ESP_LOGI(TAG, "==Enter new screen==");

    if (s_active_interface && s_active_interface->exit) {
        s_active_interface->exit();
    }

    s_active_interface = interface;
}
/**
 * 获取当前活跃的屏幕活动接口
 * @return 当前活跃的屏幕活动接口
 */
const screen_interface_t *screen_manager_get_active(void) {
    return s_active_interface;
}


/**
 * 全局界面初始化
 */
void ui_manager_init(void) {
    menu_register();
    time_register();
    time_2_register();
    dial_register();
    percent_register();
    potato_register();
    light_register();
    wifi_register();
    set_time_register();
}
