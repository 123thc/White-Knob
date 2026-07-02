//
// Created by k0921 on 2026/3/28.
//

#include "ws2812_control.h"
#include "ws2812.h"
#include "pin_config.h"
#include "freertos/FreeRTOS.h"
#include <math.h>

//WS2812控制句柄
ws2812_strip_handle_t ws2812_handle;

//每个LED的rgb颜色值
static  uint16_t r[WS2812_LED_NUM] = {0}, g[WS2812_LED_NUM] = {0}, b[WS2812_LED_NUM] = {0};
static  uint8_t status[WS2812_LED_NUM] = {0};


/**
 * WS2812初始化
 */
void My_WS2812_init(ws2812_control_t *ws2812_control) {
    ws2812_init(WS2812_PIN, WS2812_LED_NUM, &ws2812_handle);

    ws2812_control_t temp = {
        .mode = MODE_BREATHING,
        .brightness = 255,
        .direction = LED_FORWARD,
        .speed = 10,
        .rgb_color = {0, 125, 0} // 对应r=0, g=125, b=0
    };

    *ws2812_control = temp;

    //控制模式为彩虹时，才进行相位初始化
    if (ws2812_control->mode == MODE_RAINBOW) {
        My_WS2812_LED_Phase_Init(ws2812_control, 35);
    }
}

/**
 * ws2812控制函数
 * @param ws2812_control ws2812控制相关结构体
 */
void My_WS2812_Control(ws2812_control_t *ws2812_control) {
    switch (ws2812_control->mode) {
        case MODE_RAINBOW:
            My_WS2812_Rainbow(ws2812_control, 5);
            break;
        case MODE_GRADIENT:
            My_WS2812_Gradient(ws2812_control);
            break;
        case MODE_SOLID_COLOR:
            My_WS2812_Signle_Color(ws2812_control);
            break;
        case MODE_COMET:
            My_WS2812_Comet(ws2812_control);
            break;
        case MODE_BREATHING:
            My_WS2812_Breathing(ws2812_control);
            break;
        case MODE_MARQUEE:
            My_WS2812_Marquee(ws2812_control);
            break;
        case MODE_TRACK:
            My_WS2812_Track(ws2812_control);
            break;
        default:
            break;
    }
}





/**
 * WS2812彩虹模式不同LED灯珠的相位初始化
 * @param ws2812_control ws2812控制相关结构体
 * @param phase_shift 不同LED灯珠相位差
 */
void My_WS2812_LED_Phase_Init(ws2812_control_t *ws2812_control, uint8_t phase_shift) {
    uint16_t red = 0, green = 0, blue = 0;
    uint8_t status = 0;

    for (uint8_t i = 1; i < WS2812_LED_NUM; i++) {
        if (ws2812_control->direction == LED_BACKWARD)
            i = WS2812_LED_NUM - 1 - i;

        switch (status) {
            case 0:
                if (red == ws2812_control->brightness)
                    status++;
                else {
                    red += phase_shift;
                    if (red > ws2812_control->brightness) {
                        red = ws2812_control->brightness;
                        status++;
                    }
                }
                break;
            case 1:
                if (blue == 0)
                    status++;
                else {
                    blue -= phase_shift;
                    if (blue > ws2812_control->brightness) {//因为是uint16_t类型，不能小于0
                        blue = 0;
                        status++;
                    }
                }
                break;
            case 2:
                if (green == ws2812_control->brightness)
                    status++;
                else {
                    green += phase_shift;
                    if (green > ws2812_control->brightness) {
                        green = ws2812_control->brightness;
                        status++;
                    }
                }
                break;
            case 3:
                if (red == 0)
                    status++;
                else {
                    red -= phase_shift;
                    if (red > ws2812_control->brightness) {
                        red = 0;
                        status++;
                    }
                }
                break;
            case 4:
                if (blue == ws2812_control->brightness)
                    status++;
                else {
                    blue += phase_shift;
                    if (blue > ws2812_control->brightness) {
                        blue = ws2812_control->brightness;
                        status++;
                    }
                }
                break;
            case 5:
                if (green == 0)
                    status = 0;
                else {
                    green -= phase_shift;
                    if (green > ws2812_control->brightness) {
                        green = 0;
                        status = 0;
                    }
                }
                break;
            default:
                break;
        }
        r[i] = red;
        g[i] = green;
        b[i] = blue;
    }
}

/**
 * WS2812彩虹控制
 * @param ws2812_control 控制相关结构体
 * @param phase_shift_step 同一个LED灯珠相位变化速率
 */
void My_WS2812_Rainbow(ws2812_control_t *ws2812_control, uint8_t phase_shift_step) {
    // 根据 speed 动态调整步长：speed 越大，步长越大，变化越快
    uint8_t step = 1 + (ws2812_control->speed / 10);
    if (step < 1) step = 1;
    if (step > 20) step = 20;  // 避免跳变太剧烈

    for (uint8_t i = 0; i < WS2812_LED_NUM; i++) {
        uint8_t idx = (ws2812_control->direction == LED_BACKWARD)
                        ? (WS2812_LED_NUM - 1 - i) : i;

        switch (status[idx]) {
            case 0:
                if (r[idx] == ws2812_control->brightness) status[idx]++;
                else {
                    r[idx] += step;
                    if (r[idx] > ws2812_control->brightness) {
                        r[idx] = ws2812_control->brightness;
                        status[idx]++;
                    }
                }
                break;
            case 1:
                if (b[idx] == 0) status[idx]++;
                else {
                    b[idx] -= step;
                    if (b[idx] > ws2812_control->brightness) { // 下溢保护
                        b[idx] = 0;
                        status[idx]++;
                    }
                }
                break;
            case 2:
                if (g[idx] == ws2812_control->brightness) status[idx]++;
                else {
                    g[idx] += step;
                    if (g[idx] > ws2812_control->brightness) {
                        g[idx] = ws2812_control->brightness;
                        status[idx]++;
                    }
                }
                break;
            case 3:
                if (r[idx] == 0) status[idx]++;
                else {
                    r[idx] -= step;
                    if (r[idx] > ws2812_control->brightness) {
                        r[idx] = 0;
                        status[idx]++;
                    }
                }
                break;
            case 4:
                if (b[idx] == ws2812_control->brightness) status[idx]++;
                else {
                    b[idx] += step;
                    if (b[idx] > ws2812_control->brightness) {
                        b[idx] = ws2812_control->brightness;
                        status[idx]++;
                    }
                }
                break;
            case 5:
                if (g[idx] == 0) status[idx] = 0;
                else {
                    g[idx] -= step;
                    if (g[idx] > ws2812_control->brightness) {
                        g[idx] = 0;
                        status[idx] = 0;
                    }
                }
                break;
            default:
                break;
        }
        // 更新 LED 颜色（不再逐个添加大的延迟）
        ws2812_write(ws2812_handle, idx, r[idx], g[idx], b[idx]);
    }
    // 不再需要整个循环内的 vTaskDelay，调用频率由 led_task 中的 10ms 控制
}

/**
 * ws2812单色控制
 * @param ws2812_control ws2812控制相关结构体
 */
void My_WS2812_Signle_Color(ws2812_control_t *ws2812_control) {
    uint8_t r = ws2812_control->rgb_color[R_COLOR];
    uint8_t g = ws2812_control->rgb_color[G_COLOR];
    uint8_t b = ws2812_control->rgb_color[B_COLOR];

    if (r > ws2812_control->brightness)
        r = ws2812_control->brightness;
    if (g > ws2812_control->brightness)
        g = ws2812_control->brightness;
    if (b > ws2812_control->brightness)
        b = ws2812_control->brightness;

    for (uint8_t i = 0; i < WS2812_LED_NUM; i++) {
        ws2812_write(ws2812_handle, i, r, g, b);
    }
}

/* ---------------- 工具：HSV 转 RGB ---------------- */
static void hsv2rgb(uint16_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b) {
    // h: 0~360, s: 0~255, v: 0~255
    uint16_t region;
    uint8_t p, q, t;
    if (s == 0) {
        *r = *g = *b = v;
        return;
    }
    region = h / 60;
    uint16_t remainder = (h - region * 60) * 255 / 60;
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;
    switch (region) {
        case 0: *r = v; *g = t; *b = p; break;
        case 1: *r = q; *g = v; *b = p; break;
        case 2: *r = p; *g = v; *b = t; break;
        case 3: *r = p; *g = q; *b = v; break;
        case 4: *r = t; *g = p; *b = v; break;
        default: *r = v; *g = p; *b = q; break;
    }
}

/* ---------------- 1. 渐变模式 ---------------- */
void My_WS2812_Gradient(ws2812_control_t *ws2812_control) {
    static uint16_t hue_offset = 0;   // 色相偏移
    uint8_t speed_step = (ws2812_control->speed >> 2) + 1; // 将 speed 映射为步长

    for (uint8_t i = 0; i < WS2812_LED_NUM; i++) {
        uint8_t idx = (ws2812_control->direction == LED_FORWARD) ? i : (WS2812_LED_NUM - 1 - i);
        // 为每个 LED 计算色相，形成渐变条
        uint16_t hue = (hue_offset + i * 360 / WS2812_LED_NUM) % 360;
        uint8_t rr, gg, bb;
        hsv2rgb(hue, 255, ws2812_control->brightness, &rr, &gg, &bb);
        ws2812_write(ws2812_handle, idx, rr, gg, bb);
    }
    hue_offset = (hue_offset + speed_step) % 360;
}

/* ---------------- 2. 彗星模式 ---------------- */
void My_WS2812_Comet(ws2812_control_t *ws2812_control) {
    static float comet_pos = 0.0f;           // 彗星头部位置（浮点，支持平滑移动）
    #define COMET_TAIL 8                     // 彗尾长度（LED 个数）
    float step = ws2812_control->speed / 10.0f;
    if (step < 0.5f) step = 0.5f;
    if (ws2812_control->direction == LED_BACKWARD) step = -step;

    comet_pos += step;
    // 循环限制位置范围
    if (comet_pos >= WS2812_LED_NUM) comet_pos -= WS2812_LED_NUM;
    if (comet_pos < 0) comet_pos += WS2812_LED_NUM;

    // 先熄灭所有 LED
    for (uint8_t i = 0; i < WS2812_LED_NUM; i++) {
        ws2812_write(ws2812_handle, i, 0, 0, 0);
    }

    // 绘制彗星：头部最亮，尾部逐渐变暗
    for (int t = 0; t < COMET_TAIL; t++) {
        float led_pos_f = comet_pos - t * step / fabs(step); // 向尾部方向偏移
        if (led_pos_f < 0) led_pos_f += WS2812_LED_NUM;
        if (led_pos_f >= WS2812_LED_NUM) led_pos_f -= WS2812_LED_NUM;
        uint8_t idx = (uint8_t) led_pos_f;

        uint8_t brightness = ws2812_control->brightness * (COMET_TAIL - t) / COMET_TAIL;
        uint8_t r = ws2812_control->rgb_color[R_COLOR] * brightness / 255;
        uint8_t g = ws2812_control->rgb_color[G_COLOR] * brightness / 255;
        uint8_t b = ws2812_control->rgb_color[B_COLOR] * brightness / 255;
        ws2812_write(ws2812_handle, idx, r, g, b);
    }
}

/* ---------------- 3. 呼吸模式 ---------------- */
void My_WS2812_Breathing(ws2812_control_t *ws2812_control) {
    static uint16_t breath_phase = 0;            // 0~360 度
    uint8_t speed_step = (ws2812_control->speed >> 3) + 1;
    breath_phase = (breath_phase + speed_step) % 360;

    // 用正弦波生成当前亮度因子 (0.0 ~ 1.0)
    float rad = breath_phase * M_PI / 180.0f;
    float factor = (sin(rad) + 1.0f) / 2.0f;     // 0~1

    uint8_t base_r = ws2812_control->rgb_color[R_COLOR];
    uint8_t base_g = ws2812_control->rgb_color[G_COLOR];
    uint8_t base_b = ws2812_control->rgb_color[B_COLOR];

    uint8_t r = (uint8_t)(base_r * factor);
    uint8_t g = (uint8_t)(base_g * factor);
    uint8_t b = (uint8_t)(base_b * factor);

    // 应用整体亮度限制
    if (r > ws2812_control->brightness) r = ws2812_control->brightness;
    if (g > ws2812_control->brightness) g = ws2812_control->brightness;
    if (b > ws2812_control->brightness) b = ws2812_control->brightness;

    for (uint8_t i = 0; i < WS2812_LED_NUM; i++) {
        ws2812_write(ws2812_handle, i, r, g, b);
    }
}

/* ---------------- 4. 跑马灯模式 ---------------- */
void My_WS2812_Marquee(ws2812_control_t *ws2812_control) {
    static uint8_t marquee_pos = 0;
    #define MARQUEE_ON  3    // 同时点亮的 LED 个数

    uint8_t step = (ws2812_control->speed >> 5) + 1;
    if (step < 1) step = 1;

    if (ws2812_control->direction == LED_FORWARD) {
        marquee_pos = (marquee_pos + step) % WS2812_LED_NUM;
    } else {
        marquee_pos = (marquee_pos - step + WS2812_LED_NUM) % WS2812_LED_NUM;
    }

    // 熄灭所有 LED
    for (uint8_t i = 0; i < WS2812_LED_NUM; i++) {
        ws2812_write(ws2812_handle, i, 0, 0, 0);
    }

    // 点亮连续的几个 LED
    for (uint8_t j = 0; j < MARQUEE_ON; j++) {
        uint8_t idx = (marquee_pos + j) % WS2812_LED_NUM;
        uint8_t r = ws2812_control->rgb_color[R_COLOR];
        uint8_t g = ws2812_control->rgb_color[G_COLOR];
        uint8_t b = ws2812_control->rgb_color[B_COLOR];
        if (r > ws2812_control->brightness) r = ws2812_control->brightness;
        if (g > ws2812_control->brightness) g = ws2812_control->brightness;
        if (b > ws2812_control->brightness) b = ws2812_control->brightness;
        ws2812_write(ws2812_handle, idx, r, g, b);
    }
}

//跟踪模式
void My_WS2812_Track(ws2812_control_t *ws2812_control) {
    float spacing = 360.0f / WS2812_LED_NUM;

    uint8_t index = roundf(ws2812_control->angle_value / spacing);

    uint8_t r = ws2812_control->rgb_color[R_COLOR];
    uint8_t g = ws2812_control->rgb_color[G_COLOR];
    uint8_t b = ws2812_control->rgb_color[B_COLOR];
    for (uint8_t i = 0; i < WS2812_LED_NUM; i++) {
        if (i == index) {
            ws2812_write(ws2812_handle, i, r, g, b);
        }
        else
            ws2812_write(ws2812_handle, i, 0, 0, 0);
    }
}