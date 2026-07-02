// Created by k0921 on 2026/3/19.
//

#include "my_wifi.h"
#include <esp_mac.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <lwip/ip4_addr.h>
#include "ap_wifi.h"

#define TAG "my_wifi"

#define WIFI_SSID_MAX_LEN      32
#define WIFI_PASSWORD_MAX_LEN  63

// 鏈€澶ч噸杩炴鏁帮紝瓒呰繃姝ゆ鏁板悗涓嶅啀鑷姩閲嶈繛
#define MAX_CONNECT_COUNT   10
// 褰撳墠閲嶈繛灏濊瘯娆℃暟
static int sta_connected_cnt = 0;

// 鎸囧悜澶栭儴鍥炶皟鍑芥暟鐨勬寚閽堬紝鐢ㄤ簬閫氱煡WiFi鐘舵€佸彉鍖?
static p_wifi_state_cb wifi_cb = NULL;

// WiFi杩炴帴鐘舵€佹爣蹇楋紝true琛ㄧず宸茶繛鎺ュ苟鑾峰緱IP
static bool is_sta_connected = false;
static bool is_wifi_manager_inited = false;
static bool is_ap_config_mode = false;
static bool is_sta_configured = false;

static esp_netif_t *esp_netif_ap_obj;

static SemaphoreHandle_t scan_sem = NULL;

//AP妯″紡涓嬬殑SSID鍚嶇О
static const char * ap_ssid_name = "White_Knob_AP";
//AP妯″紡涓嬬殑瀵嗙爜
static const char * ap_password = "12345678";

static bool wifi_credentials_input_valid(const char *ssid, const char *password) {
    if (ssid == NULL || password == NULL || ssid[0] == '\0') {
        return false;
    }

    size_t ssid_len = strnlen(ssid, WIFI_SSID_MAX_LEN + 1);
    size_t pass_len = strnlen(password, WIFI_PASSWORD_MAX_LEN + 2);
    return ssid_len > 0 && ssid_len <= WIFI_SSID_MAX_LEN && pass_len <= WIFI_PASSWORD_MAX_LEN;
}

/**
 * @brief 浜嬩欢澶勭悊鍑芥暟锛屽鐞哤iFi浜嬩欢鍜孖P浜嬩欢
 * @param arg 鐢ㄦ埛娉ㄥ唽鏃朵紶閫掔殑鍙傛暟锛堟澶勪负NULL锛?
 * @param event_base 浜嬩欢鍩虹ID锛圵IFI_EVENT鎴朓P_EVENT锛?
 * @param event_id 鍏蜂綋浜嬩欢ID
 * @param event_data 浜嬩欢鐩稿叧鏁版嵁锛堟湭浣跨敤锛?
 */
static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                // Station鍚姩鍚庣珛鍗冲皾璇曡繛鎺?
                wifi_mode_t mode;
                esp_wifi_get_mode(&mode);
                if(!is_ap_config_mode && is_sta_configured && mode == WIFI_MODE_STA)
                    esp_wifi_connect();         //鍚姩WIFI杩炴帴
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                wifi_event_sta_disconnected_t *disc = (wifi_event_sta_disconnected_t *)event_data;
                // 鏂紑杩炴帴锛屾洿鏂扮姸鎬佸苟閫氱煡鍥炶皟
                if(is_sta_connected)
                {
                    if(wifi_cb)
                        wifi_cb(WIFI_STATE_DISCONNECTED);
                    is_sta_connected = false;
                }
                // 濡傛灉鏈揪鍒版渶澶ч噸杩炴鏁帮紝鍒欏皾璇曢噸杩?
                if (!is_ap_config_mode && is_sta_configured && sta_connected_cnt < MAX_CONNECT_COUNT) {
                    wifi_mode_t mode;
                    esp_wifi_get_mode(&mode);
                    if(mode == WIFI_MODE_STA)
                        esp_wifi_connect();             //缁х画閲嶈繛
                    sta_connected_cnt++;
                }
                if (!is_ap_config_mode) {
                    ESP_LOGI(TAG,"connect to the AP fail,retry now, reason=%d", disc->reason);
                }
                break;
            case WIFI_EVENT_STA_CONNECTED://WIFI杩炰笂璺敱鍣ㄥ悗锛岃Е鍙戞浜嬩欢
                ESP_LOGI(TAG, "connected to ap");
                break;
            case WIFI_EVENT_AP_STACONNECTED:
                //鏈夎澶囪繛鎺ヤ簡鐑偣锛屾妸瀹冪殑MAC鎵撳嵃鍑烘潵
                wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *) event_data;
                ESP_LOGI(TAG, "Station "MACSTR" joined, AID=%d",
                        MAC2STR(event->mac), event->aid);
                break;
            case WIFI_EVENT_AP_STADISCONNECTED:
                //鏈夎澶囨柇寮€浜嗙儹鐐?
                wifi_event_ap_stadisconnected_t *event_2 = (wifi_event_ap_stadisconnected_t *) event_data;
                ESP_LOGI(TAG, "Station "MACSTR" left, AID=%d",
                        MAC2STR(event_2->mac), event_2->aid);
                break;
            default:
                break;
        }

    }
    else if (event_base == IP_EVENT) {
        if (event_id == IP_EVENT_STA_GOT_IP) {
            // 鑾峰緱IP鍦板潃锛岃〃绀哄畬鍏ㄨ繛鎺ユ垚鍔?
            ESP_LOGI(TAG, "Get wifi IP");
            is_sta_connected = true;
            if (wifi_cb) {
                wifi_cb(WIFI_STATE_CONNECTED);
            }
        }
    }
}

/**
 * @brief 鍒濆鍖朩iFi绠＄悊鍣?
 * @param f 鎸囧悜鐘舵€佸洖璋冨嚱鏁扮殑鎸囬拡锛屽綋WiFi鐘舵€佸彉鍖栨椂璋冪敤
 */
esp_err_t wifi_manager_init(p_wifi_state_cb f) {
    if (is_wifi_manager_inited) {
        wifi_cb = f;
        return ESP_OK;
    }

    esp_err_t err;

    // 鍒濆鍖朤CP/IP缃戠粶鎺ュ彛
    err = esp_netif_init();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "esp_netif_init failed: %s", esp_err_to_name(err));
        return err;
    }

    // 鍒涘缓榛樿浜嬩欢寰幆
    err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "esp_event_loop_create_default failed: %s", esp_err_to_name(err));
        return err;
    }
    // 鍒涘缓榛樿鐨刉iFi Station缃戠粶鎺ュ彛
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    if (sta_netif == NULL) {
        ESP_LOGE(TAG, "create default wifi sta netif failed");
        return ESP_FAIL;
    }
    esp_netif_set_hostname(sta_netif, "White_Knob");   // 鍦ㄦ璁剧疆浣犳兂瑕佺殑鍚嶇О

    esp_netif_ap_obj = esp_netif_create_default_wifi_ap();
    if (esp_netif_ap_obj == NULL) {
        ESP_LOGE(TAG, "create default wifi ap netif failed");
        return ESP_FAIL;
    }

    // 浣跨敤榛樿閰嶇疆鍒濆鍖朩iFi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    err = esp_wifi_init(&cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_init failed: %s", esp_err_to_name(err));
        return err;
    }

    err = esp_wifi_set_storage(WIFI_STORAGE_RAM);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_set_storage RAM failed: %s", esp_err_to_name(err));
        return err;
    }

    // 娉ㄥ唽WiFi浜嬩欢澶勭悊锛堟墍鏈変簨浠讹級
    esp_event_handler_instance_t instance_any_id;
    err = esp_event_handler_instance_register(WIFI_EVENT,
                                              ESP_EVENT_ANY_ID,
                                              &event_handler,
                                              NULL,
                                              &instance_any_id);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "register wifi event failed: %s", esp_err_to_name(err));
        return err;
    }
    // 娉ㄥ唽IP浜嬩欢澶勭悊锛堜粎鍏虫敞鑾峰緱IP浜嬩欢锛?    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_t instance_got_ip;
    err = esp_event_handler_instance_register(IP_EVENT,
                                              IP_EVENT_STA_GOT_IP,
                                              &event_handler,
                                              NULL,
                                              &instance_got_ip);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "register ip event failed: %s", esp_err_to_name(err));
        return err;
    }

    // 淇濆瓨鍥炶皟鍑芥暟鎸囬拡
    wifi_cb = f;

    scan_sem = xSemaphoreCreateBinary();
    if (scan_sem == NULL) {
        ESP_LOGE(TAG, "create scan semaphore failed");
        return ESP_ERR_NO_MEM;
    }
    xSemaphoreGive(scan_sem);

    // 璁剧疆WiFi妯″紡涓篠tation
    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_set_mode STA failed: %s", esp_err_to_name(err));
        return err;
    }

    // 鍚姩WiFi
    err = esp_wifi_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_start failed: %s", esp_err_to_name(err));
        return err;
    }

    is_wifi_manager_inited = true;
    return ESP_OK;
}

/**
 * @brief 杩炴帴鎸囧畾SSID鐨刉iFi缃戠粶
 * @param ssid 瑕佽繛鎺ョ殑WiFi SSID
 * @param password WiFi瀵嗙爜
 */
esp_err_t wifi_manager_connect(const char * ssid, const char * password) {
    if (!wifi_credentials_input_valid(ssid, password)) {
        ESP_LOGE(TAG, "invalid wifi credentials");
        return ESP_ERR_INVALID_ARG;
    }

    sta_connected_cnt = 0;
    is_sta_connected = false;
    is_ap_config_mode = false;

    esp_err_t err = esp_wifi_disconnect();
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "esp_wifi_disconnect before connect: %s", esp_err_to_name(err));
    }

    err = esp_wifi_stop();
    if (err != ESP_OK && err != ESP_ERR_WIFI_NOT_STARTED) {
        ESP_LOGW(TAG, "esp_wifi_stop before connect: %s", esp_err_to_name(err));
    }

    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    size_t ssid_len = strlen(ssid);
    size_t pass_len = strlen(password);
    memcpy(wifi_config.sta.ssid, ssid, ssid_len);
    memcpy(wifi_config.sta.password, password, pass_len);

    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_set_mode STA failed: %s", esp_err_to_name(err));
        return err;
    }

    err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_set_config STA failed: %s", esp_err_to_name(err));
        return err;
    }
    is_sta_configured = true;

    err = esp_wifi_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_start connect failed: %s", esp_err_to_name(err));
    }
    return err;
}

esp_err_t wifi_manager_ap(void) {
    is_ap_config_mode = true;
    wifi_mode_t wifi_mode;
    esp_err_t err = esp_wifi_get_mode(&wifi_mode);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_get_mode failed: %s", esp_err_to_name(err));
        return err;
    }
    if (wifi_mode == WIFI_MODE_AP) {
        return ESP_OK;
    }
    else {
        ESP_LOGW(TAG, "Switch WiFi to AP config mode");
    }
    err = esp_wifi_disconnect();
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "esp_wifi_disconnect before AP: %s", esp_err_to_name(err));
    }
    err = esp_wifi_stop();
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "esp_wifi_stop before AP: %s", esp_err_to_name(err));
    }
    err = esp_wifi_set_mode(WIFI_MODE_AP);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_set_mode AP failed: %s", esp_err_to_name(err));
        return err;
    }
    wifi_config_t wifi_config = {
        .ap = {
            .channel = 5,
            .max_connection = 2,
            .authmode = WIFI_AUTH_WPA2_PSK,
        }
    };
    snprintf((char *)wifi_config.ap.ssid, 32, "%s", ap_ssid_name);
    wifi_config.ap.ssid_len = strlen(ap_ssid_name);
    snprintf((char *)wifi_config.ap.password, 64, "%s", ap_password);

    err = esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_set_config AP failed: %s", esp_err_to_name(err));
        return err;
    }

    if (esp_netif_ap_obj == NULL) {
        ESP_LOGE(TAG, "AP netif is NULL");
        return ESP_FAIL;
    }

    esp_netif_ip_info_t ip_info;
    IP4_ADDR(&ip_info.ip, 192, 168, 100, 1);    //IP鍦板潃
    IP4_ADDR(&ip_info.gw, 192, 168, 100, 1);    //缃戝叧
    IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0);//瀛愮綉鎺╃爜

    err = esp_netif_dhcps_stop(esp_netif_ap_obj);//璁剧疆IP涔嬪墠鍋滅敤DHCP
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "esp_netif_dhcps_stop: %s", esp_err_to_name(err));
    }
    err = esp_netif_set_ip_info(esp_netif_ap_obj, &ip_info);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_netif_set_ip_info failed: %s", esp_err_to_name(err));
        return err;
    }
    err = esp_netif_dhcps_start(esp_netif_ap_obj);//璁剧疆IP鎴愬姛涔嬪悗鍋滅敤DHCP
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_netif_dhcps_start failed: %s", esp_err_to_name(err));
        return err;
    }

    return esp_wifi_start();
}

//鎵弿浠诲姟
static void scan_task(void * param) {
    p_wifi_scan_cb callback = (p_wifi_scan_cb)param;
    uint16_t ap_count = 0;
    uint16_t ap_num = 20;
    wifi_ap_record_t * ap_list = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * ap_num);
    if (ap_list == NULL) {
        ESP_LOGE(TAG, "malloc ap list failed");
        if (callback) {
            callback(0, NULL);
        }
        xSemaphoreGive(scan_sem);
        vTaskDelete(NULL);
        return;
    }

    wifi_mode_t mode;
    esp_err_t err = esp_wifi_get_mode(&mode);
    if (err == ESP_OK && is_ap_config_mode && mode == WIFI_MODE_AP) {
        err = esp_wifi_set_mode(WIFI_MODE_APSTA);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "switch APSTA for scan failed: %s", esp_err_to_name(err));
        }
    }

    wifi_scan_config_t scan_config = {
        .show_hidden = true,
    };
    err = esp_wifi_scan_start(&scan_config, true);
    if (err == ESP_OK) {
        err = esp_wifi_scan_get_ap_num(&ap_count);
    }
    if (err == ESP_OK) {
        ap_num = ap_count < ap_num ? ap_count : ap_num;
        err = esp_wifi_scan_get_ap_records(&ap_num, ap_list);
    }
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "wifi scan failed: %s", esp_err_to_name(err));
        ap_num = 0;
    }

    ESP_LOGI(TAG, "Total ap count: %d, actual ap number:%d", ap_count, ap_num);
    if (callback) {
        callback(ap_num, ap_list);
    }
    free(ap_list);
    xSemaphoreGive(scan_sem);//閲婃斁淇″彿閲?
    vTaskDelete(NULL);
}

esp_err_t wifi_manager_scan(p_wifi_scan_cb f) {
    if (pdTRUE == xSemaphoreTake(scan_sem, 0)) {
        esp_wifi_clear_ap_list();//娓呴櫎涓婃鎵弿鐨勪俊鎭?
        return xTaskCreatePinnedToCore(scan_task, "scan", 8192, f, 3, NULL, 1);
    }
    return ESP_FAIL;
}
