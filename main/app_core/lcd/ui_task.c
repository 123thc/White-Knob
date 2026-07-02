//
// Created by k0921 on 2026/4/10.
//

#include "ui_task.h"
#include "lvgl/lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lcd/lvgl_port/lv_port.h"
#include "gui_guider.h"
#include "custom.h"
#include "esp_log.h"
#include "lcd/custom_operation/adsorb.h"
#include "lcd/custom_operation/scroll.h"
#include <math.h>
#include <led/led_decoration.h>
#include <wifi/ap_wifi.h>

#include "hx711/press_get.h"
#include "smart_knob/smart_knob.h"
#include "lcd/ui_manager/config/ui_manager.h"

#define TAG "ui_task"

//累计清零时间
#define CLEAR_THRESHOLD  100 //单位：ms
//定时中断的周期
#define IT_PERIOD_MS 20    //单位：ms
//电机静止阈值
#define STATIONARY_THRESHOLD 0.5 //单位：度


//计数器
static uint16_t counter = 0;    //单位：ms

//停止旋转的信号量
SemaphoreHandle_t rotate_stop_sem = NULL;

lv_ui guider_ui;

static bool s_led_press_feedback_active = false;
static led_control_mode s_led_press_mode;
static led_control_mode s_led_press_last_mode;
static led_direction s_led_press_direction;
static uint8_t s_led_press_speed;
static uint8_t s_led_press_brightness;
static uint8_t s_led_press_rgb[3];
static bool s_skip_next_release = false;

static void led_press_backup_state(void)
{
    if (s_led_press_feedback_active) {
        return;
    }

    s_led_press_mode = g_ws2812_control.mode;
    s_led_press_last_mode = g_ws2812_control.last_mode;
    s_led_press_direction = g_ws2812_control.direction;
    s_led_press_speed = g_ws2812_control.speed;
    s_led_press_brightness = g_ws2812_control.brightness;
    s_led_press_rgb[R_COLOR] = g_ws2812_control.rgb_color[R_COLOR];
    s_led_press_rgb[G_COLOR] = g_ws2812_control.rgb_color[G_COLOR];
    s_led_press_rgb[B_COLOR] = g_ws2812_control.rgb_color[B_COLOR];
    s_led_press_feedback_active = true;
}

static void led_press_restore_state(void)
{
    if (!s_led_press_feedback_active) {
        g_ws2812_control.mode = g_ws2812_control.last_mode;
        return;
    }

    g_ws2812_control.mode = s_led_press_mode;
    g_ws2812_control.last_mode = s_led_press_last_mode;
    g_ws2812_control.direction = s_led_press_direction;
    g_ws2812_control.speed = s_led_press_speed;
    g_ws2812_control.brightness = s_led_press_brightness;
    g_ws2812_control.rgb_color[R_COLOR] = s_led_press_rgb[R_COLOR];
    g_ws2812_control.rgb_color[G_COLOR] = s_led_press_rgb[G_COLOR];
    g_ws2812_control.rgb_color[B_COLOR] = s_led_press_rgb[B_COLOR];
    s_led_press_feedback_active = false;
}

//释放回调
void hx711_relaeas_cb(void) {
    ESP_LOGI(TAG, "HX711 RELAEAS CALLBACK");
    const bool was_light_interface = s_active_interface == &light_interface;
    const bool skip_release = s_skip_next_release;
    s_skip_next_release = false;

    if (!skip_release && s_active_interface && s_active_interface->release_press)
        s_active_interface->release_press();

    if (!was_light_interface && g_ws2812_control.on_off == true) {
        led_press_restore_state();
    }
}

//短按回调
void hx711_short_press_cb(void) {
    ESP_LOGI(TAG, "HX711 SHORTPRESS CALLBACK");
    screen_interface_t *previous_interface = s_active_interface;
    const bool was_light_interface = s_active_interface == &light_interface;
    if (s_active_interface && s_active_interface->short_press)
        s_active_interface->short_press();
    if (previous_interface != s_active_interface) {
        s_skip_next_release = true;
    }

    if (!was_light_interface && s_active_interface != &light_interface && g_ws2812_control.on_off == true) {
        led_press_backup_state();
        g_ws2812_control.mode = MODE_SOLID_COLOR;
        g_ws2812_control.rgb_color[R_COLOR] = 250;
        g_ws2812_control.rgb_color[G_COLOR] = 250;
        g_ws2812_control.rgb_color[B_COLOR] = 250;
    }
    //发送按压的信号量触发震动
    xSemaphoreGive(press_sem);
}

//长按回调
void hx711_long_press_cb(void) {
    ESP_LOGI(TAG, "HX711 LONGPRESS CALLBACK");
    screen_interface_t *previous_interface = s_active_interface;
    const bool was_light_interface = s_active_interface == &light_interface;
    if (s_active_interface && s_active_interface->long_press)
        s_active_interface->long_press();
    if (previous_interface != s_active_interface) {
        s_skip_next_release = true;
    }

    if (!was_light_interface && s_active_interface != &light_interface && g_ws2812_control.on_off == true) {
        led_press_backup_state();
        g_ws2812_control.mode = MODE_SOLID_COLOR;
        g_ws2812_control.rgb_color[R_COLOR] = 255;
        g_ws2812_control.rgb_color[G_COLOR] = 215;
        g_ws2812_control.rgb_color[B_COLOR] = 0;
    }
}

//lvgl任务线程
void ui_task(void *pvParameters) {
    rotate_stop_sem = xSemaphoreCreateBinary();
    //注册hx711模拟按键回调
    hx711_press_init(hx711_relaeas_cb, hx711_short_press_cb, hx711_long_press_cb);
    static bool first_ble_connected = true;
    lv_port_init();
    setup_ui(&guider_ui);
    custom_init(&guider_ui);

    while (1) {
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

//LVGL输入设备回调（lvgl中的定时回调）
void IRAM_ATTR indev_read(struct _lv_indev_drv_t * indev_drv, lv_indev_data_t * data) {
    //当前电机角度（从队列中获取）
    static float current_angle = 0.0f;
    static float last_angle = 0.0f;
    static float angle_accum = 0.0f;   // 累积未处理的微小角度变化
    static bool first_read = true;
    // static float angle_filtered = 0.0f;

    if (pdTRUE == xQueuePeek(angle_queue, &current_angle, 0)) {
        if (!first_read) {
            // 计算角度变化，处理 0°/360° 边界跳变（最短路径）
            float delta = current_angle - last_angle;
            if (delta > 180.0f) {
                delta -= 360.0f;
            } else if (delta < -180.0f) {
                delta += 360.0f;
            }

            //角度变化小于阈值的时候开始计时
            if (fabs(delta) < STATIONARY_THRESHOLD) {
                counter += IT_PERIOD_MS;
                //达到一定时间后判定电机静止，对角度累计值进行一次清零
                if (counter >= CLEAR_THRESHOLD) {
                    //旋转停止
                    if (s_active_interface && s_active_interface->rotate_stop)
                        s_active_interface->rotate_stop();
                    //释放停止旋转的信号量
                    xSemaphoreGive(rotate_stop_sem);
                    angle_accum = 0.0f;
                    counter = 0;
                }
            }
            else {
                counter = 0;
                // 累积角度变化
                angle_accum += delta;
                if (s_active_interface && s_active_interface->rotate)
                    s_active_interface->rotate(&angle_accum, &current_angle);
                // ESP_LOGI(TAG, "rotating....");
            }
        }
        last_angle = current_angle;
        first_read = false;
    }

    //旋转时不误触按压
    if (counter != 0)
        //触发模拟按键回调
        hx711_press_handle();
}
