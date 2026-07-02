//
// Created by k0921 on 2026/3/28.
//

#include "pid.h"

/**
 * PID计算函数，输入目标值和当前值，返回pid->ret_val
 * @param target_val 目标值
 * @param current_val 当前实际值
 * @param pid
 */
void pid_caculate(float target_val, float current_val, pid_cfg_t * pid) {
    float error = target_val - current_val;
    pid->error_p = error;
    pid->error_i += error;

    //i值限幅
    if (pid->error_i > 0 && pid->error_i > pid->max_limit_val_i)
        pid->error_i = pid->max_limit_val_i;
    else if (pid->error_i < 0 && pid->error_i < -pid->max_limit_val_i)
        pid->error_i = -pid->max_limit_val_i;

    //error先前值初始化
    if (pid->is_first) {
        pid->error_pre = error;
        pid->is_first = false;
    }
    pid->error_d = error - pid->error_pre;

    pid->ret_val = pid->p * pid->error_p + pid->i * pid->error_i + pid->d * pid->error_d;
    pid->error_pre = error;
}