//
// Created by k0921 on 2026/3/28.
//

#ifndef WHITE_KNOB_SMART_KNOB_H
#define WHITE_KNOB_SMART_KNOB_H
#include "freertos/queue.h"

extern QueueHandle_t angle_queue;
extern SemaphoreHandle_t press_sem;
// extern SemaphoreHandle_t knob_to_rotate_sem;

void smart_knob_task(void *pvParameters);

#endif //WHITE_KNOB_SMART_KNOB_H