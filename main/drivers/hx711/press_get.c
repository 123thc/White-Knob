//
// Created by k0921 on 2026/4/6.
//

#include "press_get.h"
#include "esp_timer.h"
#include <stdbool.h>
#include "ble_hid/ble_hidd_start.h"
#include "ble_hid/esp_hidd_prf_api.h"
#include "class/hid/hid_device.h"
#include "lcd/ui_manager/dial_manager.h"
#include "hx711.h"
#include "esp_log.h"

#define TAG "press_get"

//定时中断的周期
#define IT_PERIOD_MS 20    //单位：ms
//按压阈值（越大越难按）
#define THRESHOLD   300000
//长按时间
#define LONG_PRESS_DURATION 750

//定时器句柄
static esp_timer_handle_t hx711_timer_handle = NULL;

//HX711的状态
static hx711_state state = HX711_RELEASE;
//计数器
static uint16_t press_count = 0;

//用于记录初始化传入的回调函数
static hx711_relaeas_cb_t relaes_cb = NULL;
static hx711_short_press_cb_t short_press_cb = NULL;
static hx711_long_press_cb_t long_press_cb = NULL;

/**
 * 检测hx711是否被按下
 * @return 被按下返回true，否则返回false
 */
bool hx711_is_pressed(void) {
    static int last_diff = 0;
    int diff = HX711_getValue(CHANNEL_A_128) - HX711_BASE_VALUE;

    //当通信太快hx711返回值为0时，取上次的diff值
    if (diff == -HX711_BASE_VALUE)
        diff = last_diff;
    last_diff = diff;
    // ESP_LOGI(TAG, "diff: %d", diff);
    return (diff > THRESHOLD) || (diff < -THRESHOLD);
}

/**
 * HX711按压检测相关初始化
 */
void hx711_press_init(hx711_relaeas_cb_t rf, hx711_short_press_cb_t sf, hx711_long_press_cb_t lf) {
    //记录传入的回调函数
    relaes_cb = rf;
    short_press_cb = sf;
    long_press_cb = lf;

    hx711_hw_init();

    state = HX711_RELEASE;
    press_count = 0;
}

//定时回调函数(在lvgl的定时回调中使用)
void hx711_press_handle(void) {
    switch (state) {
        case HX711_RELEASE: //释放状态
            if (hx711_is_pressed()) {
                state = HX711_SHORT_PRESS;
                press_count += IT_PERIOD_MS;
            }
            else {
                press_count = 0;
            }
            break;
        case HX711_SHORT_PRESS: //短按状态
            if (hx711_is_pressed()) {
                press_count += IT_PERIOD_MS;
                if (press_count >= 20) {
                    if (short_press_cb)
                        short_press_cb();
                    state = HX711_LONG_PRESS;
                }
            }
            else {
                if (!relaes_cb)
                    relaes_cb();
                press_count = 0;
                state = HX711_RELEASE;
            }
            break;
        case HX711_LONG_PRESS:  //长按状态
            if (hx711_is_pressed()) {
                dial_pressing();
                press_count += IT_PERIOD_MS;
                if (press_count >= LONG_PRESS_DURATION) {
                    if (long_press_cb)
                        long_press_cb();
                    state = HX711_HOLD_ON;
                }
            }
            else {
                if (relaes_cb)
                    relaes_cb();
                press_count = 0;
                state = HX711_RELEASE;
            }
            break;
        case HX711_HOLD_ON: //持续长按（等待松手）
            if (!hx711_is_pressed()) {
                dial_long_long_press();
                if (relaes_cb)
                    relaes_cb();
                press_count = 0;
                state = HX711_RELEASE;
            }
            break;
        default: break;
    }
}