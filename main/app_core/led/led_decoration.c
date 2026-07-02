//
// Created by k0921 on 2026/3/28.
//

#include "led_decoration.h"
#include "light_custom.h"
#include "freertos//FreeRTOS.h"
#include "ws2812/ws2812_control.h"
#include "freertos/queue.h"
#include "smart_knob/smart_knob.h"

//ws2812全局控制变量
ws2812_control_t g_ws2812_control;

void led_task(void *pvParameters) {
    My_WS2812_init(&g_ws2812_control);
    g_ws2812_control.mode = MODE_GRADIENT;
    g_ws2812_control.brightness = 255;
    g_ws2812_control.direction = LED_FORWARD;
    g_ws2812_control.speed = 10;
    g_ws2812_control.rgb_color[R_COLOR] = 46;
    g_ws2812_control.rgb_color[G_COLOR] = 197;
    g_ws2812_control.rgb_color[B_COLOR] = 206;
    g_ws2812_control.last_mode = g_ws2812_control.mode;
    g_ws2812_control.on_off = true;
    light_custom_load_and_apply();
    while (1) {
        My_WS2812_Control(&g_ws2812_control);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
