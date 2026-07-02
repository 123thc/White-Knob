//
// Created by k0921 on 2026/3/28.
//

#ifndef WHITE_KNOB_LED_DECORATION_H
#define WHITE_KNOB_LED_DECORATION_H
#include "ws2812/ws2812_control.h"

extern ws2812_control_t g_ws2812_control;

void led_task(void *pvParameters);

#endif //WHITE_KNOB_LED_DECORATION_H