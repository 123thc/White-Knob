//
// Created by k0921 on 2026/5/7.
//

#include "moving_avg_filter.h"
#include <stdlib.h>

/**
 * 初始化移动平均滤波器
 * @param window_size 窗口大小，必须 > 0
 * @return 滤波器实例指针，失败返回 NULL
 */
MovingAvgFilter* moving_avg_init(int window_size) {
    if (window_size <= 0) return NULL;

    MovingAvgFilter *filter = (MovingAvgFilter*)malloc(sizeof(MovingAvgFilter));
    if (!filter) return NULL;

    filter->buffer = (float*)calloc(window_size, sizeof(float));
    if (!filter->buffer) {
        free(filter);
        return NULL;
    }

    filter->window_size = window_size;
    filter->index = 0;
    filter->count = 0;
    filter->sum = 0.0f;

    return filter;
}

/**
 * 输入一个样本，返回当前移动平均值
 * @param filter 滤波器实例
 * @param new_sample 新的采样值
 * @return 滤波后的输出值
 */
float moving_avg_update(MovingAvgFilter *filter, float new_sample) {
    if (!filter || filter->window_size <= 0) return 0.0f;

    // 从 sum 中减去即将被覆盖的旧值，再加上新值
    float old_value = filter->buffer[filter->index];
    filter->sum = filter->sum - old_value + new_sample;

    // 存入新值并推进索引
    filter->buffer[filter->index] = new_sample;
    filter->index = (filter->index + 1) % filter->window_size;

    // 更新已计数的样本数（最多记录窗口大小）
    if (filter->count < filter->window_size) {
        filter->count++;
    }

    // 返回当前有效平均值
    return filter->sum / filter->count;
}

/**
 * 重置滤波器状态（清空历史数据）
 * @param filter 滤波器实例
 */
void moving_avg_reset(MovingAvgFilter *filter) {
    if (!filter) return;
    for (int i = 0; i < filter->window_size; i++) {
        filter->buffer[i] = 0.0f;
    }
    filter->index = 0;
    filter->count = 0;
    filter->sum = 0.0f;
}

/**
 * 释放滤波器占用的内存
 * @param filter 滤波器实例的指针
 */
void moving_avg_free(MovingAvgFilter *filter) {
    if (filter) {
        free(filter->buffer);
        free(filter);
    }
}