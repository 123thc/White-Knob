//
// Created by k0921 on 2026/4/28.
//
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "motor_app.h"
#include <math.h>
#include "lcd/ui_task.h"
#include "filter/moving_avg_filter.h"
#include "motor/svpwm.h"
#include "motor/motor_control.h"
#include "esp_log.h"
#include "esp_timer.h"

#define TAG "motor_app"

//触感反馈状态全局变量
KnobHapticState g_haptic_state;
//震动后的的下一模式
HapticMode g_vibratic_next_mode = MODE_FREE;

void KnobHaptics_Update(float current_angle) {
    switch (g_haptic_state.mode) {
        case MODE_FREE:
            // SVPWM_Controller(current_angle, 0.0f, 0.0f, &M0);
            M0.duty_u = 0.0f;
            M0.duty_v = 0.0f;
            M0.duty_w = 0.0f;
            SVPWM_Set_Duty(&M0);
            break;

        case MODE_DETENT: {
            DetentParams *p = &g_haptic_state.params.detent;
            // 调用你原来的逻辑，但参数从 p 里取
            KnobHaptics_SetDetent(current_angle, p->steps, p->width);
            break;
        }

        case MODE_EDGE: {
            EdgeParams *p = &g_haptic_state.params.edge;
            KnobHaptics_SetEdge(current_angle, p->min_angle, p->max_angle, p->side);
            break;
        }

        case MODE_DAMPING: {
            DampingParams *p = &g_haptic_state.params.damping;
            KnobHaptics_Damping(current_angle, p->strength);
            break;
        }

        case MODE_VIBRATE:
            KnobHaptics_Vibrate(current_angle);
            // 振动执行完后自动回到自由模式（或保持原模式）
            g_haptic_state.mode = g_vibratic_next_mode;   // 可根据需求调整
            break;
        default:break;
    }
}



/**
 * @brief 根据磁编码器角度计算旋转速度
 *
 * @param current_angle  当前角度（0 ~ 360°）
 * @return 角速度，单位 °/s，正值表示顺时针（取决于你的编码器方向），首次调用返回0
 */
float get_rotation_speed(float current_angle)
{
    static float last_angle = 0.0f;
    if (pdTRUE == xSemaphoreTake(rotate_stop_sem, 0)) {
        current_angle = last_angle;
    }
    last_angle = current_angle;

    static float prev_angle = 0.0f;
    static int64_t prev_time_us = 0;
    static bool first_call = true;

    int64_t now_us = esp_timer_get_time();  // 微秒

    if (first_call) {
        prev_angle = current_angle;
        prev_time_us = now_us;
        first_call = false;
        return 0.0f;
    }

    // 计算角度差（最短路径）
    float delta_angle = current_angle - prev_angle;
    if (delta_angle > 180.0f) {
        delta_angle -= 360.0f;
    } else if (delta_angle < -180.0f) {
        delta_angle += 360.0f;
    }

    // 计算时间差（秒）
    float delta_time_s = (float)(now_us - prev_time_us) / 1000000.0f;


    // 防止除零
    if (delta_time_s <= 0.0f) {
        return 0.0f;
    }

    float speed = delta_angle / delta_time_s;   // 单位 °/s

    // 更新历史值
    prev_angle = current_angle;
    prev_time_us = now_us;
    // ESP_LOGI(TAG, "speed: %f", speed);

    return speed;
}

//棘轮模式
void KnobHaptics_SetDetent(float current_angle, int steps, float width) {
    /* --- 参数合法性处理 --- */
    if (steps < 1) {
        steps = 1;  /* 至少保留一个卡位 */
    }

    /* 计算相邻卡位中心的角度间距 */
    float spacing = 360.0f / (float)steps;

    /* width 不得为负，且不应超过相邻间距的一半（否则死区重叠，无反馈力） */
    if (width < 0.0f) {
        width = 0.0f;
    }
    else if (width > spacing * 0.5f) {
        width = spacing * 0.5f;
    }

    // // 用直角坐标矢量进行低通滤波，完美处理 0°↔360° 环绕
    // static float filt_x = 0.0f;      // 滤波后的 x = cos(angle)
    // static float filt_y = 0.0f;      // 滤波后的 y = sin(angle)
    // static bool first_call = true;
    //
    // float rad = current_angle * (M_PI / 180.0f);
    // float raw_x = cosf(rad);
    // float raw_y = sinf(rad);
    //
    // if (first_call) {
    //     filt_x = raw_x;
    //     filt_y = raw_y;
    //     first_call = false;
    // } else {
    //     filt_x = 0.6f * filt_x + 0.4f * raw_x;
    //     filt_y = 0.6f * filt_y + 0.4f * raw_y;
    // }

    static float filt_x = 0.0f, filt_y = 0.0f;
    static bool first_call = true;

    // 计算当前转速（°/s）
    float speed = get_rotation_speed(current_angle);
    float abs_speed = fabsf(speed);

    // 自适应滤波系数：转速越高 alpha 越大（越接近原始值）
    float alpha = 0.05f;                     // 基础强滤波（静止时）
    if (abs_speed > 10.0f) {                 // 超过 10°/s 时逐渐减弱滤波
        alpha = 0.05f + 0.5f * fminf(1.0f, (abs_speed - 20.0f) / 200.0f);
    }
    // alpha 确保在 0.05 ~ 0.55 之间
    float one_minus_alpha = 1.0f - alpha;

    float rad = current_angle * (M_PI / 180.0f);
    float raw_x = cosf(rad);
    float raw_y = sinf(rad);

    if (first_call) {
        filt_x = raw_x;
        filt_y = raw_y;
        first_call = false;
    } else {
        filt_x = one_minus_alpha * filt_x + alpha * raw_x;
        filt_y = one_minus_alpha * filt_y + alpha * raw_y;
    }

    // 从滤波后的矢量取回角度
    current_angle = atan2f(filt_y, filt_x) * (180.0f / M_PI);
    if (current_angle < 0.0f) {
        current_angle += 360.0f;
    }

    float target_pos = roundf(current_angle / spacing) * spacing;

    //旋转停止时消除误差
    if (pdTRUE == xSemaphoreTake(rotate_stop_sem, 0)) {
        current_angle = target_pos;
    }
    motor_set_pos(target_pos, current_angle);
}

//限位模式
void KnobHaptics_SetEdge(float current_angle, float min_angle, float max_angle, bool side) {
    /* 小角 > 大角，并且差值大于180 */
    if (min_angle - max_angle > 180.0f) {
        /* 将角制度换成[-180，180]范围 */
        if (current_angle > 180.0f)
            current_angle -= 360.0f;
        min_angle -= 360.0f;//此时小角更小
    }
    /* 小角 < 大角，并且差值小于-180 */
    else if (min_angle - max_angle < -180.0f) {
        /* 将角制度换成[-180，180]范围 */
        if (current_angle > 180.0f)
            current_angle -= 360.0f;
        max_angle -= 360.0f;//此时大角会更小
        //交换小角和大角，使小角 < 大角
        float temp = min_angle;
        min_angle = max_angle;
        max_angle = temp;
    }

    //使小角始终小于大角
    if (min_angle > max_angle) {
        float temp = min_angle;
        min_angle = max_angle;
        max_angle = temp;
    }

    float angle_diff_min = min_angle - current_angle;
    float angle_diff_max = max_angle - current_angle;

    /* 当差值绝对值大于180时，将当前的角制度换成另一角制度，如[-180, 180]换成[0, 360]，用于处理各自的边界值 */
    if (angle_diff_min > 180.0f) angle_diff_min -= 360.0f;
    else if (angle_diff_min < -180.0f) angle_diff_min += 360.0f;
    if (angle_diff_max > 180.0f) angle_diff_max -= 360.0f;
    else if (angle_diff_max < -180.0f) angle_diff_max += 360.0f;

    // 用直角坐标矢量进行低通滤波，完美处理 0°↔360° 环绕
    static float filt_x = 0.0f;      // 滤波后的 x = cos(angle)
    static float filt_y = 0.0f;      // 滤波后的 y = sin(angle)
    static bool first_call = true;

    float rad = current_angle * (M_PI / 180.0f);
    float raw_x = cosf(rad);
    float raw_y = sinf(rad);

    if (first_call) {
        filt_x = raw_x;
        filt_y = raw_y;
        first_call = false;
    } else {
        filt_x = 0.8f * filt_x + 0.2f * raw_x;
        filt_y = 0.8f * filt_y + 0.2f * raw_y;
    }

    // 从滤波后的矢量取回角度
    current_angle = atan2f(filt_y, filt_x) * (180.0f / M_PI);
    if (current_angle < 0.0f) {
        current_angle += 360.0f;
    }

    /* 为1时，旋钮能在[min_angle , max_angle]范围 “外” 转动 */
    if (side) {
        if (current_angle > min_angle && current_angle < max_angle) {
            if (fabs(angle_diff_min) > fabs(angle_diff_max)) {
                //距离大角位置近，吸附到大角
                motor_set_pos(max_angle, current_angle);
            }
            else {
                //距离小角位置近，吸附到小角
                motor_set_pos(min_angle, current_angle);
            }
        }
        else {
            //使旋钮可以自由旋转（可修改成其他模式）
            SVPWM_Controller(current_angle, 0.0f, 0.0f, &M0);
        }
    }
    /* 为0时，旋钮能在 [min_angle , max_angle]范围 “内” 转动 */
    else {
        if (current_angle < min_angle || current_angle > max_angle) {
            if (fabs(angle_diff_min) > fabs(angle_diff_max)) {
                //距离大角位置近，吸附到大角
                motor_set_pos(max_angle, current_angle);
            }
            else {
                //距离小角位置近，吸附到小角
                motor_set_pos(min_angle, current_angle);
            }
        }
        else {
            //使旋钮可以自由旋转（可修改成其他模式）
            SVPWM_Controller(current_angle, 0.0f, 0.0f, &M0);
        }
    }
}

//震动反馈
void KnobHaptics_Vibrate(float current_angle) {
    SVPWM_Controller(current_angle, 0.0f, 0.0f, &M0);
    for (uint16_t i = 0; i < 3; i++) {
        SVPWM_Controller(current_angle, 0.5f, 0.0f, &M0);
        vTaskDelay(pdMS_TO_TICKS(10));
        SVPWM_Controller(current_angle, -0.5f, 0.0f, &M0);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    SVPWM_Controller(current_angle, 0.0f, 0.0f, &M0);
}

//阻尼效果
void KnobHaptics_Damping(float current_angle, float strength) {
    static float filtered_speed = 0.0f;          // 速度的一阶低通滤波状态
    // static MovingAvgFilter* uq_filter = NULL;    // uq 的移动平均滤波器（静态，只初始化一次）

    // // 初始化移动平均滤波器（窗口大小可调）
    // if (uq_filter == NULL) {
    //     uq_filter = moving_avg_init(10);
    //     // 若初始化失败，后续将简单跳过滤波，保证程序不会崩溃
    // }

    // 计算原始转速（将角度变化率归一化到 rps 或类似单位）
    float raw_speed = get_rotation_speed(current_angle) / 360.0f;

    // 一阶低通滤波得到平滑速度
    filtered_speed = filtered_speed * 0.97f + raw_speed * 0.03f;

    // 计算原始力矩 uq
    float uq = strength * filtered_speed;

    // 限幅到 ±5
    if (uq > 5.0f) uq = 5.0f;
    else if (uq < -5.0f) uq = -5.0f;

    static float uq_filter = 0.0f;
    uq_filter = 0.8f * uq_filter + 0.2f * uq;
    uq = uq_filter;

    // 对限幅后的 uq 使用移动平均滤波，进一步平滑力矩输出
    // if (uq_filter != NULL) {
    //     uq = moving_avg_update(uq_filter, uq);
    //     // 滤波后可能因边界值平均而略微超出限幅，再次钳位（可选）
    //     if (uq > 5.0f) uq = 5.0f;
    //     else if (uq < -5.0f) uq = -5.0f;
    // }

    if (fabs(uq) < 0.2f) uq = 0.0f;

    // 用直角坐标矢量进行低通滤波，完美处理 0°↔360° 环绕
    static float filt_x = 0.0f;      // 滤波后的 x = cos(angle)
    static float filt_y = 0.0f;      // 滤波后的 y = sin(angle)
    static bool first_call = true;

    float rad = current_angle * (M_PI / 180.0f);
    float raw_x = cosf(rad);
    float raw_y = sinf(rad);

    if (first_call) {
        filt_x = raw_x;
        filt_y = raw_y;
        first_call = false;
    } else {
        filt_x = 0.8f * filt_x + 0.2f * raw_x;
        filt_y = 0.8f * filt_y + 0.2f * raw_y;
    }

    // 从滤波后的矢量取回角度
    current_angle = atan2f(filt_y, filt_x) * (180.0f / M_PI);
    if (current_angle < 0.0f) {
        current_angle += 360.0f;
    }

    // 输出给 SVPWM 控制器
    SVPWM_Controller(current_angle, uq, 0.0f, &M0);
}