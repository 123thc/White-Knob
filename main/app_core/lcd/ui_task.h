//
// Created by k0921 on 2026/4/10.
//

#ifndef WHITE_KNOB_UI_TASK_H
#define WHITE_KNOB_UI_TASK_H
#include "lvgl/lvgl.h"
#include "freertos/FreeRTOS.h"

extern SemaphoreHandle_t rotate_stop_sem;

void ui_task(void *pvParameters);
void IRAM_ATTR indev_read(struct _lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

#endif //WHITE_KNOB_UI_TASK_H