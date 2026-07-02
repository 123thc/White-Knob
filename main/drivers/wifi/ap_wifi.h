//
// Created by k0921 on 2026/3/21.
//

#ifndef ESP32_TEST_AP_WIFI_H
#define ESP32_TEST_AP_WIFI_H
#include "my_wifi.h"
#include "freertos/FreeRTOS.h"

// extern SemaphoreHandle_t wifi_sem;

esp_err_t ap_wifi_init(p_wifi_state_cb f);
esp_err_t ap_wifi_apcfg(void);

#endif //ESP32_TEST_AP_WIFI_H
