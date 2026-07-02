//
// Created by k0921 on 2026/4/23.
//

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dial_manager.h"
#include <math.h>
#include "gui_guider.h"
#include "config/ui_manager.h"
#include "class/hid/hid_device.h"
#include "ble_hid/ble_hidd_start.h"
#include "ble_hid/esp_hidd_prf_api.h"
#include "dial/dial_send.h"

//步进阈值
#define STEP_THRESHOLD 10.0f

static bool is_rotating = false;

static bool is_ble_inited = false;

static bool is_last_angle_init = false;

static dial_cmd_t cmd;

//改变方向
static bool is_change_direction = false;

/**
 * 进入SurFaceDial界面前的回调函数
 */
void dial_enter(void) {
    if (!is_ble_inited) {
        //释放信号量，启动蓝牙初始化任务
        xSemaphoreGive(ble_init_sem);
        // bluetooth_hid_start();
        is_ble_inited = true;
    }

    //加载SurFaceDial界面
    if(guider_ui.screen_dial_del) {
        setup_scr_screen_dial(&guider_ui);
        guider_ui.screen_dial_del = false;
    }

    is_last_angle_init = false;

    lv_scr_load(guider_ui.screen_dial);
}

/**
 * 退出SurfaceDial界面的操作
 */
void dial_exit(void) {
    //删除SurfaceDial界面的屏幕
    if (guider_ui.screen_dial) {
        lv_obj_del(guider_ui.screen_dial);      // 彻底删除屏幕及其所有子对象
        guider_ui.screen_dial = NULL;           // 避免野指针
        guider_ui.screen_dial_del = true;       // 标记需要重新创建
    }
}


void dial_rotate(float *delta, const float *angle)
{
    if (dial_cmd_queue == NULL) return;

    is_rotating = true;
    // static bool last_dir = false;
    // float rotation = -*delta;
    //
    // static bool dir_initialized = false;   // 首次旋转时仅初始化方向，不触发换向
    // bool current_dir = (rotation > 0);     // 正方向为 true
    //
    // if (!dir_initialized) {
    //     last_dir = current_dir;
    //     dir_initialized = true;
    // }
    // else {
    //     // 方向确实改变时，标记换向
    //     if (current_dir != last_dir) {
    //         is_change_direction = true;
    //     }
    //     last_dir = current_dir;            // 更新为当前方向
    // }

    // //设置最小旋转值
    // if (rotation > 0) {
    //     if (rotation < STEP_THRESHOLD) {
    //         rotation = STEP_THRESHOLD;
    //     }
    // }
    // else if (rotation < 0) {
    //     if (rotation > -STEP_THRESHOLD) {
    //         rotation = -STEP_THRESHOLD;
    //     }
    // }

    // if (is_change_direction) {
    //     ble_hid_surfacedial_report_custom(hid_conn_id, 0, 0);
    //     is_change_direction = false;
    // }
    //
    // ble_hid_surfacedial_report_custom(hid_conn_id, 0, rotation);

    cmd.type = DIAL_CMD_ROTATE;
    cmd.rotation_angle = -*delta;   // 直接传递原始 delta，任务端再换算

    xQueueSend(dial_cmd_queue, &cmd, 100);
}

/**
 * SurfaceDial旋转停止操作
 */
void dial_release_press(void) {
    if (is_rotating) {
        cmd.type = DIAL_CMD_RELEASE;
        xQueueSend(dial_cmd_queue, &cmd, 100);
        // ble_hid_surfacedial_report(hid_conn_id, DIAL_PRESS);
        is_rotating = false;
    }
}


/**
 * SurfaceDial界面的特殊处理，持续发送按压，在hx711_press_handle中调用
 */
void dial_pressing(void) {
    //处于SurfaceDial界面才触发
    if (lv_scr_act() != guider_ui.screen_dial)
        return;
    cmd.type = DIAL_CMD_PRESS;
    xQueueSend(dial_cmd_queue, &cmd, 100);
    // ble_hid_surfacedial_report(hid_conn_id, DIAL_PRESS);
}

/**
 * 短按触发事件
 */
void dial_short_press(void) {
    cmd.type = DIAL_CMD_SHORT_PRESS;
    xQueueSend(dial_cmd_queue, &cmd, 100);
    // ble_hid_surfacedial_report(hid_conn_id, DIAL_PRESS);
}

/**
 * SurFaceDial界面的特殊处理，增加长按时间返回菜单界面，在hx711_press_handle中调用
 */
void dial_long_long_press(void) {
    //处于SurfaceDial界面才触发
    if (lv_scr_act() != guider_ui.screen_dial)
        return;
    //返回菜单界面
    screen_manager_set_active(&menu_interface);
}

/**
 * 注册菜单界面
 */
void dial_register(void) {
    dial_interface.enter = dial_enter;
    dial_interface.exit = dial_exit;
    dial_interface.rotate = dial_rotate;
    dial_interface.rotate_stop = NULL;
    dial_interface.short_press = dial_short_press;
    dial_interface.release_press = dial_release_press;
    dial_interface.long_press = NULL;
    dial_interface.is_registered = true;
}
