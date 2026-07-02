//
// Created by k0921 on 2026/3/28.
//

#ifndef WHITE_KNOB_PID_H
#define WHITE_KNOB_PID_H
#include <stdbool.h>

typedef struct {
    float p;//p参数
    float i;//i参数
    float d;//d参数
    float error_p;
    float error_i;
    float max_limit_val_i;//i限幅值
    float error_d;
    float error_pre;
    bool is_first;//是否进行初始化
    float ret_val;//返回值
}pid_cfg_t;

void pid_caculate(float target_val, float current_val, pid_cfg_t * pid);

#endif //WHITE_KNOB_PID_H