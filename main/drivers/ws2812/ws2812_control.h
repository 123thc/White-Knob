//
// Created by k0921 on 2026/3/28.
//

#ifndef WHITE_KNOB_WS2812_CONTROL_H
#define WHITE_KNOB_WS2812_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

#define WS2812_MAX_SPEED 100//最大变化速度

#define R_COLOR 0
#define G_COLOR 1
#define B_COLOR 2

typedef enum {
    MODE_RAINBOW = 0,     // 彩虹
    MODE_SOLID_COLOR,     // 单色
    MODE_GRADIENT,        // 渐变
    MODE_MARQUEE,         // 跑马灯
    MODE_BREATHING,       // 呼吸
    MODE_COMET,           // 彗星
    MODE_TRACK,           // 追踪
}led_control_mode;

typedef enum {
    LED_FORWARD = 0,
    LED_BACKWARD,
}led_direction;

typedef struct {
    led_control_mode mode;//控制模式选择
    led_control_mode last_mode;//上一次的模式
    led_direction direction;//流动方向
    uint16_t speed;//变化速度
    uint8_t brightness;//亮度0~255
    uint8_t rgb_color[3];//对有颜色需要的指定
    bool on_off;
    float angle_value;
}ws2812_control_t;

void My_WS2812_init(ws2812_control_t *ws2812_control);
void My_WS2812_Control(ws2812_control_t *ws2812_control);


void My_WS2812_LED_Phase_Init(ws2812_control_t *ws2812_control, uint8_t phase_shift);
void My_WS2812_Rainbow(ws2812_control_t *ws2812_control, uint8_t phase_shift_step);
void My_WS2812_Signle_Color(ws2812_control_t *ws2812_control);

void My_WS2812_Gradient(ws2812_control_t *ws2812_control);
void My_WS2812_Comet(ws2812_control_t *ws2812_control);
void My_WS2812_Breathing(ws2812_control_t *ws2812_control);
void My_WS2812_Marquee(ws2812_control_t *ws2812_control);
void My_WS2812_Track(ws2812_control_t *ws2812_control);

#endif //WHITE_KNOB_WS2812_CONTROL_H