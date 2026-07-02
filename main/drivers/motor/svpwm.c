//
// Created by k0921 on 2026/3/28.
//

#include "svpwm.h"
#include "encoder/as5047p.h"
#include <math.h>
#include "motor_config.h"
#include "driver/mcpwm.h"

//*****************************UVW三相占空比设置**************************************//
/**
 * 设置电机0的U相占空比，AB相互补
 * @param duty 占空比范围为0~100
 */
void M0_U_Set_Duty(float duty) {
    mcpwm_set_duty(MCPWM_UNIT_M0, MCPWM_TIMER_0, MCPWM_GEN_A, duty);
}

/**
 * 设置电机0的V相占空比，AB相互补
 * @param duty 占空比范围为0~100
 */
void M0_V_Set_Duty(float duty) {
    mcpwm_set_duty(MCPWM_UNIT_M0, MCPWM_TIMER_1, MCPWM_GEN_A, duty);
}

/**
 * 设置电机0的W相占空比，AB相互补
 * @param duty 占空比范围为0~100
 */
void M0_W_Set_Duty(float duty) {
    mcpwm_set_duty(MCPWM_UNIT_M0, MCPWM_TIMER_2, MCPWM_GEN_A, duty);
}
//******************************UVW三相占空比设置**************************************//

/**
 * 限幅函数
 * @param min_val 最小值
 * @param max_val 最大值
 * @param real_val 实际值
 * @return 限幅后的值
 */
float Limiting(float min_val, float max_val, float real_val) {
    if (real_val > max_val)
        real_val = max_val;
    else if (real_val < min_val)
        real_val = min_val;

    return real_val;
}

/**
 * 帕克逆变换，将D、Q轴电压转化为α、β轴电压
 * @param uq Q轴电压分量
 * @param ud D轴电压分量
 * @param motor electric_angle 电角度，单位：rad
 * @param[out] motor u_alpha α轴电压分量
 * @param[out] motor u_beta β轴电压分量
 */
void ParkInverse(float uq, float ud, motor_t *motor) {
    // 获取当前电角度的三角函数值
    float cos_theta = cosf(motor->electric_angle);
    float sin_theta = sinf(motor->electric_angle);

    // 执行反Park变换计算
    motor->u_alpha = ud * cos_theta - uq * sin_theta;
    motor->u_beta  = ud * sin_theta + uq * cos_theta;
}

/**
 * SVPWM设置三相占空比,并输出
 * @param motor duty_u U相占空比
 * @param motor duty_v V相占空比
 * @param motor duty_w W相占空比
 */
void SVPWM_Set_Duty(motor_t* motor) {
    motor->duty_u = Limiting(0, MAXDUTY, motor->duty_u);
    motor->duty_v = Limiting(0, MAXDUTY, motor->duty_v);
    motor->duty_w = Limiting(0, MAXDUTY, motor->duty_w);

    M0_U_Set_Duty(motor->duty_u);
    M0_V_Set_Duty(motor->duty_v);
    M0_W_Set_Duty(motor->duty_w);
}

/**
 * SVPWM核心，将α和β轴电压转化为UVW三相电压
 * @param motor u_alpha α轴电压分量
 * @param motor u_beta β轴电压分量
 * @param[out] motor duty_u U相占空比
 * @param[out] motor duty_v V相站控制
 * @param[out] motor duty_w W相占空比
 */
void SVPWM_Core(motor_t *motor) {
    // 计算关键常数
    const float HALF = 0.5f;

    // ========= 扇区判断 ==========
    float Vref1 = motor->u_beta;
    float Vref2 = (SQRT3 * motor->u_alpha - motor->u_beta) * HALF;
    float Vref3 = (-SQRT3 * motor->u_alpha - motor->u_beta) * HALF;

    uint8_t A = (Vref1 > 0) ? 1 : 0;
    uint8_t B = (Vref2 > 0) ? 1 : 0;
    uint8_t C = (Vref3 > 0) ? 1 : 0;

    uint8_t N = 4 * C + 2 * B + A;
    uint8_t sector = 0;

    const uint8_t sector_map[7] = {0, 2, 6, 1, 4, 3, 5}; // 优化扇区映射
    if (N >= 1 && N <= 6)
    {
        sector = sector_map[N];
    }

    // ========= 矢量作用时间计算 ==========
    float T1 = 0, T2 = 0; // 两个有效矢量作用时间
    float T0 = 0;         // 零矢量作用时间

    switch (sector)
    {
        case 1:
            T2 = K * Vref1;
            T1 = K * Vref2;
            break;
        case 2:
            T2 = -K * Vref2;
            T1 = -K * Vref3;
            break;
        case 3:
            T1 = K * Vref1;
            T2 = K * Vref3;
            break;
        case 4:
            T2 = -K * Vref1;
            T1 = -K * Vref2;
            break;
        case 5:
            T2 = K * Vref2;
            T1 = K * Vref3;
            break;
        case 6:
            T1 = -K * Vref1;
            T2 = -K * Vref3;
            break;
        default:
            // 处理错误或默认值
            T1 = 0;
            T2 = 0;
            break;
    }

    // 过调制处理
    if (T1 + T2 > TS)
    {
        float scale = TS / (T1 + T2);
        T1 *= scale;
        T2 *= scale;
        T0 = 0;
    }
    else
    {
        T0 = TS - T1 - T2;
    }

    float T0_half = T0 * HALF;

    // ========= 计算各相导通时间 ==========
    float Ta, Tb, Tc;

    switch (sector)
    {
        case 1:
            Ta = T1 + T2 + T0_half;
            Tb = T2 + T0_half;
            Tc = T0_half;
            break;
        case 2:
            Ta = T1 + T0_half;
            Tb = T1 + T2 + T0_half;
            Tc = T0_half;
            break;
        case 3:
            Ta = T0_half;
            Tb = T1 + T2 + T0_half;
            Tc = T2 + T0_half;
            break;
        case 4:
            Ta = T0_half;
            Tb = T1 + T0_half;
            Tc = T1 + T2 + T0_half;
            break;
        case 5:
            Ta = T2 + T0_half;
            Tb = T0_half;
            Tc = T1 + T2 + T0_half;
            break;
        case 6:
            Ta = T1 + T2 + T0_half;
            Tb = T0_half;
            Tc = T1 + T0_half;
            break;
        default:
            // 默认50%占空比（零矢量）
            Ta = Tb = Tc = TS * HALF;
            break;
    }

    // ========= 转换为PWM比较值 ==========
    // 使用乘法代替除法提高性能
    const float inv_TS = 1.0f / TS;
    motor->duty_u = (uint16_t)(Ta * inv_TS * MAXDUTY);
    motor->duty_v = (uint16_t)(Tb * inv_TS * MAXDUTY);
    motor->duty_w = (uint16_t)(Tc * inv_TS * MAXDUTY);
}


/**
 * 将角度归一化到0~360度并转换为弧度
 * @param angle_deg 输入角度，单位：度
 * @return 归一化后的角度，单位：弧度 [0, 2π)
 */
float NormalizeAngleDegToRad(float angle_deg)
{
    float a = fmod(angle_deg, 360.0f);
    float b = ((a >= 0) ? a : (a + 360));
    return b / 180 * PI;
}

/**
 * 获取电机的电角度
 * @param motor zero_bias_shaft_angle 电机零偏机械角度，单位：度
 * @param motor motor_pp 电机极对数
 * @param motor dir 电机旋转方向
 * @param[out] motor electric_angle 单位：rad
 */
void Motor_Get_Electric_Angle(motor_t *motor, float shaft_angle)
{
    motor->electric_angle = NormalizeAngleDegToRad((shaft_angle - motor->zero_bias_shaft_angle) * motor->motor_pp * motor->dir);
}

/**
 * SVPWM控制器，通过给定Q轴电压和D轴电压生成三相马鞍波并输出
 * @param uq Q轴电压分量
 * @param ud D轴电压分量
 * @param motor 电机相关结构体
 * @note 使用前需对电机相关结构体motor进行初始化
 */
void SVPWM_Controller(float shaft_angle, float uq, float ud, motor_t *motor) {
    Motor_Get_Electric_Angle(motor, shaft_angle);
    ParkInverse(uq, ud, motor);
    SVPWM_Core(motor);
    SVPWM_Set_Duty(motor);
}