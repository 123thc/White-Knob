//
// Created by k0921 on 2026/3/28.
//

#include "freertos/FreeRTOS.h"
#include "motor_control.h"
#include <esp_log.h>
#include <math.h>
#include "motor_config.h"
#include "pid.h"
#include "svpwm.h"

static float angle_error_normalized(float target, float current) {
    float error = target - current;
    if (error > 180.0f) error -= 360.0f;
    if (error < -180.0f) error += 360.0f;
    return error;
}

/**
 * 电机控制位置环
 * @param target_pos 目标位置，单位：°（度）
 * @apram current_pos 当前位置：单位：°（度）
 */
void motor_set_pos(float target_pos, float current_pos) {
    float error = angle_error_normalized(target_pos, current_pos);

    // 死区：误差小于阈值时强制零力矩
    if (fabsf(error) < 1.0f) {
        SVPWM_Controller(current_pos, 0.0f, 0.0f, &M0);
        // 同时清空 PID 积分，防止滞环
        pid_pos.error_i = 0.0f;
        return;
    }

    // 目标值为 0，当前误差为 error
    pid_caculate(0, error, &pid_pos);
    float uq = pid_pos.ret_val;

    //限幅
    if (uq > 5.0f) uq = 5.0f;
    else if (uq < -5.0f) uq = -5.0f;

    if (fabs(uq) < 0.2f) uq = 0.0f;

    //uq根据旋转时角度的增减方向的不同来决定正负
    SVPWM_Controller(current_pos, uq, 0, &M0);
}

/**
 * 电机控制速度环
 * @param target_vel 目标速度，单位：°/s （度/秒）
 * @param current_vel 当前速度，单位：°/s （度/秒）
 */
void motor_set_vel(float target_vel, float current_vel, float shaft_angle) {
    pid_caculate(target_vel, current_vel, &pid_vel);
    SVPWM_Controller(shaft_angle, pid_vel.ret_val, 0, &M0);
}

/**
 * 电机控制速度位置环
 * @param target_pos 目标位置，单位：°（度）
 * @param target_vel 目标速度，单位：°/s （度/秒）
 * @apram current_pos 当前位置：单位：°（度）
 * @param current_vel 当前速度，单位：°/s （度/秒）
 */
void motor_set_pos_with_vel(float target_pos, float target_vel, float current_pos, float current_vel) {

}
