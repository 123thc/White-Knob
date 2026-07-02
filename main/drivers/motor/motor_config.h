//
// Created by k0921 on 2026/3/28.
//

#ifndef WHITE_KNOB_MOTOR_HARDWARE_H
#define WHITE_KNOB_MOTOR_HARDWARE_H

#include "pid.h"

#define MCPWM_UNIT_M0           MCPWM_UNIT_0                            //选择MCPWM单元
#define MCPWM_DEADTIME_MODE_M0  MCPWM_ACTIVE_HIGH_COMPLIMENT_MODE       //选择死区模式，A相与输入同相，AB相互补
#define MCPWM_FREQUENCYM_M0     30000                                   //PWM的频率
#define DEAD_TIME_NS            200

typedef enum {
    FORWARD = 1,
    BACKWARD = -1,
}motor_dir;

typedef struct {
    float duty_u, duty_v, duty_w;//UVW相电压
    float u_alpha, u_beta;//α和β轴电压分量
    float electric_angle;//电角度，单位：rad
    float zero_bias_shaft_angle;//零偏机械角度，单位：度
    int motor_pp;//电机极对数
    motor_dir dir;//电机磁铁方向，根据三相线的接线顺序不同设
}motor_t;

extern pid_cfg_t pid_pos;
extern pid_cfg_t pid_vel;

extern motor_t M0;

void motor_init(float shaft_angle);

#endif //WHITE_KNOB_MOTOR_HARDWARE_H