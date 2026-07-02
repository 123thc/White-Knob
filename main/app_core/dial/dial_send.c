//
// Created by k0921 on 2026/5/10.
//

#include "dial_send.h"
#include "lcd/ui_manager/dial_manager.h"
#include <math.h>
#include <class/hid/hid_device.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "ble_hid/ble_hidd_start.h"
#include "ble_hid/esp_hidd_prf_api.h"

SemaphoreHandle_t ble_init_sem = NULL;

QueueHandle_t dial_cmd_queue = NULL;

void dial_send_task(void *pvParameters) {
    ble_init_sem = xSemaphoreCreateBinary();
    dial_cmd_queue = xQueueCreate(50, sizeof(dial_cmd_t));
    if (ble_init_sem != NULL) {
        /* 进入Dial界面后才开始初始化 */
        if (xSemaphoreTake(ble_init_sem, portMAX_DELAY) == pdTRUE) {
            bluetooth_hid_start();
        }
    }

    if (dial_cmd_queue == NULL) {
        // 处理错误，可以删除任务或记录日志
        vTaskDelete(NULL);
        return;
    }

    dial_cmd_t cmd;
    while (1) {
        // 阻塞等待指令
        if (xQueueReceive(dial_cmd_queue, &cmd, portMAX_DELAY) == pdTRUE) {
            switch (cmd.type) {
                case DIAL_CMD_PRESS:
                    ble_hid_surfacedial_report(hid_conn_id, DIAL_PRESS);
                    break;
                case DIAL_CMD_RELEASE:
                    ble_hid_surfacedial_report(hid_conn_id, DIAL_RELEASE);
                    break;
                case DIAL_CMD_ROTATE:
                    ble_hid_surfacedial_report_custom(hid_conn_id, 0, cmd.rotation_angle);
                    break;
                case DIAL_CMD_SHORT_PRESS:
                    ble_hid_surfacedial_report(hid_conn_id, DIAL_PRESS);
                    break;
                default:
                    break;
            }
        }
    }
}
