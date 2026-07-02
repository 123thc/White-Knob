//
// Created by k0921 on 2026/3/19.
//

#ifndef ESP32_TEST_MY_WIFI_H
#define ESP32_TEST_MY_WIFI_H
#include <esp_err.h>
#include <esp_wifi_types_generic.h>

typedef enum {
    WIFI_STATE_CONNECTED,
    WIFI_STATE_DISCONNECTED,
}wifi_state;

typedef void (*p_wifi_state_cb)(wifi_state);
typedef void (*p_wifi_scan_cb)(int num, wifi_ap_record_t *ap_records);

esp_err_t wifi_manager_init(p_wifi_state_cb f);
esp_err_t wifi_manager_connect(const char * ssid, const char * password);
esp_err_t wifi_manager_ap(void);
esp_err_t wifi_manager_scan(p_wifi_scan_cb);


#endif //ESP32_TEST_MY_WIFI_H
