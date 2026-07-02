//
// Created by k0921 on 2026/5/4.
//

#include "sntp_config.h"
#include "esp_sntp.h"
#include "esp_log.h"

#define TAG "sntp_config"

static void sntp_finish_cb (struct timeval *tv) {

}

void my_nstp_init(void) {
    if (!esp_sntp_enabled()) {
        esp_sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
        esp_sntp_setservername(0, "ntp.aliyun.com");
        esp_sntp_setservername(1, "time.asia.apple.com");
        esp_sntp_setservername(2, "pool.ntp.org");
        esp_sntp_set_time_sync_notification_cb(sntp_finish_cb);
        esp_sntp_init();
        ESP_LOGI(TAG, "SNTP Init successfully.");
    }
}
