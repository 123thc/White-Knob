//
// Created by k0921 on 2026/3/28.
//

#ifndef WHITE_KNOB_SVPWM_H
#define WHITE_KNOB_SVPWM_H
#include "motor_config.h"

#define PI 3.14159265f
#define SQRT3 1.73205080757f//根号3
#define SQRT3_2 0.86602540378f//根号3除2

#define DCBUSVALUE 5.0f//母线电压
#define MAXDUTY 100//最大占空比

#define TS (1.0f / 30000.0f)//10kHz,与PWM周期一致，设定范围小于等于PWM周期
#define K (SQRT3 * TS / DCBUSVALUE)//根号3 * Ts / Udc

void SVPWM_Core(motor_t *motor);
void SVPWM_Set_Duty(motor_t* motor);
void SVPWM_Controller(float shaft_angle, float uq, float ud, motor_t *motor);

#endif //WHITE_KNOB_SVPWM_H