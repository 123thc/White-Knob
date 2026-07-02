#include <custom.h>
#include <gui_guider.h>
#include <stdio.h>
#include "encoder/as5047p.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led/led_decoration.h"
#include "lcd/ui_task.h"
#include "smart_knob/smart_knob.h"
#include "lcd/ui_manager/config/ui_manager.h"
#include "lcd/lvgl_port/lv_port.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "time/get_my_time.h"
#include "lv_conf_template.h"
#include "dial/dial_send.h"

#define TAG "White_Knob"

SemaphoreHandle_t knob_to_rotate_sem = NULL;


void angle_monitor_task(void *pvParameter) {
    float angle = 0.0f;
    float angle_filtered = 0.0f;
    while (1) {
        if (pdTRUE == xQueuePeek(angle_queue, &angle, portMAX_DELAY)) {
            // angle_filtered = 0.8f * angle_filtered + 0.2f * angle;
            ESP_LOGI(TAG, "angle = %.2f", angle);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}



void app_main(void)
{
    /* 初始化nvs */
    esp_err_t ret;
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    my_timer_init();
    ui_manager_init();
    xTaskCreatePinnedToCore(ui_task, "ui_task", 65536, NULL, 6, NULL, 0);
    xTaskCreatePinnedToCore(smart_knob_task, "smart_knob_task", 8192, NULL, 6, NULL, 1);
    xTaskCreatePinnedToCore(led_task, "led_task", 4096, NULL, 4, NULL, 0);
    xTaskCreatePinnedToCore(dial_send_task, "dial_task", 4096, NULL, 7, NULL, 0);
    // xTaskCreatePinnedToCore(angle_monitor_task, "angle_task", 4096, NULL, 2, NULL, 0);
}
