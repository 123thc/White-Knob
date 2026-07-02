//
// Created by k0921 on 2026/5/10.
//

#ifndef WHITE_KNOB_DIAL_SEND_H
#define WHITE_KNOB_DIAL_SEND_H
#include "freertos/FreeRTOS.h"

typedef enum {
    DIAL_CMD_PRESS,
    DIAL_CMD_RELEASE,
    DIAL_CMD_ROTATE,       // 旋转命令，携带角度增量
    DIAL_CMD_SHORT_PRESS,
} dial_cmd_type_t;

typedef struct {
    dial_cmd_type_t type;
    float rotation_angle;   // 旋转增量（仅当 type == DIAL_CMD_ROTATE 时有效）
} dial_cmd_t;

extern QueueHandle_t dial_cmd_queue;

extern SemaphoreHandle_t ble_init_sem;

void dial_send_task(void *pvParameters);

#endif //WHITE_KNOB_DIAL_SEND_H