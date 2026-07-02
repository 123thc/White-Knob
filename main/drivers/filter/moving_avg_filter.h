//
// Created by k0921 on 2026/5/7.
//

#ifndef WHITE_KNOB_MOVING_AVG_FILTER_H
#define WHITE_KNOB_MOVING_AVG_FILTER_H

// 移动平均滤波器结构体
typedef struct {
    int window_size;   // 窗口大小（采样点数）
    float *buffer;     // 环形缓冲区
    int index;         // 当前写入位置
    int count;         // 已累计的样本数（未满窗口时用于计算有效平均）
    float sum;         // 当前缓冲区所有值的和（用于加速计算）
} MovingAvgFilter;

MovingAvgFilter* moving_avg_init(int window_size);
float moving_avg_update(MovingAvgFilter *filter, float new_sample);
void moving_avg_reset(MovingAvgFilter *filter);

#endif //WHITE_KNOB_MOVING_AVG_FILTER_H