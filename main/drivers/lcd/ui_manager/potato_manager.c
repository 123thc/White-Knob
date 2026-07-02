#include "gui_guider.h"
#include "set_time_manager.h"
#include "config/ui_manager.h"
#include <time.h>
#include "esp_timer.h"       // ESP32 底层高精度定时器
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//arc的范围值
#define ARC_RANGE   3600

static uint32_t total_seconds = 0;      // 总倒计时秒数
static uint32_t remaining_seconds = 0;  // 剩余秒数
static esp_timer_handle_t potato_timer = NULL;

// LVGL 异步更新 UI 的回调（C 函数）
static void potato_update_ui(void *user_data) {
    uint32_t rem = remaining_seconds;
    uint16_t h = rem / 3600;
    uint16_t m = (rem % 3600) / 60;
    uint16_t s = rem % 60;

    lv_label_set_text_fmt(guider_ui.screen_potatotime_label_1,
                          "%02d : %02d : %02d", h, m, s);

    if (total_seconds > 0) {
        uint32_t percent = (remaining_seconds * ARC_RANGE) / total_seconds;
        lv_arc_set_value(guider_ui.screen_potatotime_arc_1, (int16_t)percent);
    }

    //计时结束
    if (remaining_seconds == 0) {
        lv_label_set_text(guider_ui.screen_potatotime_label_1, "Time Is Over");
    }
}

// ESP32 高精度定时器回调（1 秒）
static void potato_timer_callback(void *arg) {
    if (remaining_seconds > 0) {
        remaining_seconds--;

        // 投递到 LVGL 任务更新界面
        lv_async_call(potato_update_ui, NULL);

        // 倒计时结束：停止并删除定时器
        if (remaining_seconds == 0) {
            if (potato_timer) {
                esp_timer_stop(potato_timer);
                esp_timer_delete(potato_timer);
                potato_timer = NULL;
            }
        }
    }
}

/**
 * 进入番茄时钟界面
 */
void potato_enter(void) {
    if (guider_ui.screen_potatotime_del) {
        setup_scr_screen_potatotime(&guider_ui);
        guider_ui.screen_potatotime_del = false;
    }

    // 获取设置的时分秒
    uint16_t h = 0, m = 0, s = 0;
    set_time_set_mhs(&h, &m, &s);
    total_seconds = h * 3600 + m * 60 + s;
    remaining_seconds = total_seconds;

    // 初始化标签显示
    lv_label_set_text_fmt(guider_ui.screen_potatotime_label_1,
                          "%02d : %02d : %02d", h, m, s);

    // 设置弧形范围 0~100（百分比），初始值 100
    lv_arc_set_range(guider_ui.screen_potatotime_arc_1, 0, ARC_RANGE);
    lv_arc_set_value(guider_ui.screen_potatotime_arc_1, ARC_RANGE);

    // 创建 ESP32 底层定时器，周期 1 秒
    if (potato_timer == NULL) {
        const esp_timer_create_args_t timer_args = {
            .callback = &potato_timer_callback,
            .arg = NULL,
            .dispatch_method = ESP_TIMER_TASK,   // 在定时器任务中回调
            .name = "potato_timer"
        };
        esp_timer_create(&timer_args, &potato_timer);
        esp_timer_start_periodic(potato_timer, 1000000); // 1,000,000 μs = 1s
    }

    lv_scr_load(guider_ui.screen_potatotime);
}

/**
 * 退出番茄时钟界面
 */
void potato_exit(void) {
    // 停止并删除底层定时器
    if (potato_timer) {
        esp_timer_stop(potato_timer);
        esp_timer_delete(potato_timer);
        potato_timer = NULL;
    }

    // 删除屏幕
    if (guider_ui.screen_potatotime) {
        lv_obj_del(guider_ui.screen_potatotime);
        guider_ui.screen_potatotime_del = true;
        guider_ui.screen_potatotime = NULL;
    }
}

void potato_short_press(void) {
    screen_manager_set_active(&set_time_interface);
}

void potato_long_press(void) {
    screen_manager_set_active(&menu_interface);
}

/**
 * 注册番茄时钟界面
 */
void potato_register(void) {
    potatotime_interface.enter = potato_enter;
    potatotime_interface.exit = potato_exit;
    potatotime_interface.rotate = NULL;
    potatotime_interface.rotate_stop = NULL;
    potatotime_interface.short_press = potato_short_press;
    potatotime_interface.release_press = NULL;
    potatotime_interface.long_press = potato_long_press;
    potatotime_interface.is_registered = true;
}