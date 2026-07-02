//
// Created by k0921 on 2026/3/21.
//

#include "ap_wifi.h"
#include <string.h>
#include <cJSON.h>
#include "my_wifi.h"
#include "esp_spiffs.h"
#include "web_sever.h"
#include "esp_log.h"
#include <sys/stat.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "ap_wifi"

#define SPIFFS_MOUNT    "/spiffs"   //鎸傝浇鐐?
#define HTML_PATH       "/spiffs/AP_Connected.html"

#define APCFG_BIT   (BIT0)

#define AP_WIFI_SSID_BUF_LEN      33
#define AP_WIFI_PASSWORD_BUF_LEN  65

static char current_ssid[AP_WIFI_SSID_BUF_LEN];
static char current_password[AP_WIFI_PASSWORD_BUF_LEN];

static const char * html_code = NULL;
static EventGroupHandle_t apcfg_ev;
static TaskHandle_t ap_wifi_task_handle = NULL;

static char *init_web_page_buffer(void);

static const char *fallback_html =
    "<!doctype html><html><head><meta charset=\"utf-8\">"
    "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
    "<title>White Knob 配网</title>"
    "<style>body{font-family:sans-serif;margin:28px;background:#111;color:#eee}"
    "input,button{box-sizing:border-box;width:100%;padding:12px;margin:8px 0;font-size:16px}"
    "button{background:#19a7ce;color:white;border:0;border-radius:6px}</style>"
    "</head><body><h2>White Knob 配网</h2>"
    "<input id=\"ssid\" placeholder=\"WiFi名称（SSID）\">"
    "<input id=\"password\" placeholder=\"WiFi密码\" type=\"password\">"
    "<button onclick=\"send()\">连接并配置</button><p id=\"msg\"></p>"
    "<p><a style=\"color:#9bdcff\" href=\"/photo\">照片和灯光设置</a></p>"
    "<script>"
    "let ws;"
    "function openws(){ws=new WebSocket('ws://'+location.host+'/ws');"
    "ws.onopen=()=>msg.textContent='已连接设备';"
    "ws.onerror=()=>msg.textContent='WebSocket连接错误';}"
    "function send(){if(!ws||ws.readyState!==1)openws();"
    "setTimeout(()=>{ws.send(JSON.stringify({ssid:ssid.value,password:password.value}));"
    "msg.textContent='配置已发送，设备正在连接...';},200);}"
    "openws();"
    "</script></body></html>";

static const char *get_web_page(void) {
    if (html_code == NULL) {
        html_code = init_web_page_buffer();
        if (html_code == NULL) {
            ESP_LOGW(TAG, "AP config page is unavailable, use fallback page");
            html_code = fallback_html;
        }
    }
    return html_code;
}

static char *init_web_page_buffer(void) {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = SPIFFS_MOUNT,
        .format_if_mount_failed = false,
        .max_files = 5,
        .partition_label = "html",  //鍒嗗尯琛ㄥ悕瀛?
    };
    esp_err_t err = esp_vfs_spiffs_register(&conf);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "spiffs register failed: %s", esp_err_to_name(err));
        return NULL;
    }

    struct stat st;
    if (stat(HTML_PATH, &st)) {
        ESP_LOGE(TAG, "apcfg.html not found");
        return NULL;
    }

    char *buf = (char *)malloc(st.st_size + 1);
    if (!buf) {
        return NULL;
    }
    memset(buf, 0, st.st_size + 1);

    FILE *fp = fopen(HTML_PATH, "r");
    if (!fp) {
        free(buf);
        ESP_LOGE(TAG, "open apcfg.html failed");
        return NULL;
    }
    if (fread(buf, st.st_size, 1, fp) == 0)
    {
        free(buf);
        buf = NULL;
        ESP_LOGE(TAG, "fread failed");
    }
    fclose(fp);
    return buf;
}


static void ap_wifi_task(void * param) {
    EventBits_t ev;
    while (1) {
        ev = xEventGroupWaitBits(apcfg_ev, APCFG_BIT, pdTRUE, pdFALSE, pdMS_TO_TICKS(10 * 1000));
        if (ev & APCFG_BIT) {
            web_ws_stop();
            ESP_LOGI(TAG, "AP stopped and STA wifi connecting...");
            esp_err_t err = wifi_manager_connect(current_ssid, current_password);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "connect received WiFi failed: %s", esp_err_to_name(err));
            }
            // vTaskDelete(NULL);
        }
    }
}


// SemaphoreHandle_t wifi_sem = NULL;

esp_err_t ap_wifi_init(p_wifi_state_cb f) {
    esp_err_t err = wifi_manager_init(f);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "wifi manager init failed: %s", esp_err_to_name(err));
        return err;
    }

    get_web_page();

    if (apcfg_ev == NULL) {
        apcfg_ev = xEventGroupCreate();
        if (apcfg_ev == NULL) {
            ESP_LOGE(TAG, "create ap config event group failed");
            return ESP_ERR_NO_MEM;
        }
    }

    if (ap_wifi_task_handle == NULL) {
        BaseType_t ret = xTaskCreatePinnedToCore(ap_wifi_task, "ap_wifi_task", 4096, NULL, 2, &ap_wifi_task_handle, 1);
        if (ret != pdPASS) {
            ap_wifi_task_handle = NULL;
            ESP_LOGE(TAG, "create ap wifi task failed");
            return ESP_ERR_NO_MEM;
        }
    }
    // wifi_sem = xSemaphoreCreateBinary();

    return ESP_OK;
}


void wifi_scan_handle(int num, wifi_ap_record_t * ap_recoeds) {
    cJSON * root = cJSON_CreateArray();
    // cJSON_AddArrayToObject(root, "wifi_list");
    for (int i = 0; i < num; i++) {
        cJSON * wifi_js = cJSON_CreateObject();
        cJSON_AddStringToObject(wifi_js, "ssid", (char *)ap_recoeds[i].ssid);
        cJSON_AddNumberToObject(wifi_js, "rssi", ap_recoeds[i].rssi);
        cJSON_AddItemToArray(root, wifi_js);
    }
    char * data = cJSON_Print(root);
    ESP_LOGI(TAG, "ws send:%s", data);
    web_ws_send((uint8_t *)data, strlen(data));
    cJSON_free(data);
    cJSON_Delete(root);
}

static void ws_receive_handle(uint8_t *payload, int len) {
    cJSON * root = cJSON_Parse((char *)payload);
    if (root) {
        cJSON * scan_js = cJSON_GetObjectItem(root, "scan");
        cJSON * ssid_js = cJSON_GetObjectItem(root, "ssid");
        cJSON * password_js = cJSON_GetObjectItem(root, "password");

        if (scan_js) {
            char * scan_value = cJSON_GetStringValue(scan_js);
            if (scan_value && strcmp(scan_value, "start") == 0) {
                wifi_manager_scan(wifi_scan_handle);
            }
        }
        else if (ssid_js && password_js) {
            char * ssid_value = cJSON_GetStringValue(ssid_js);
            char * password_value = cJSON_GetStringValue(password_js);
            if (ssid_value == NULL || password_value == NULL) {
                ESP_LOGE(TAG, "Invalid WiFi config payload");
                cJSON_Delete(root);
                return;
            }
            snprintf(current_ssid, sizeof(current_ssid), "%s", ssid_value);
            snprintf(current_password, sizeof(current_password), "%s", password_value);
            ESP_LOGI(TAG, "Receive ssid:%s, now stop http server", current_ssid);
            xEventGroupSetBits(apcfg_ev, APCFG_BIT);
        }
        else {
            ESP_LOGI(TAG, "Receive unknown message: %s", payload);
        }
        cJSON_Delete(root);
    } else {
        ESP_LOGE(TAG, "JSON parse failed");
    }
}

//杩涘叆AP閰嶇綉妯″紡
esp_err_t ap_wifi_apcfg(void) {
    const char *page = get_web_page();
    if (page == NULL) {
        ESP_LOGE(TAG, "AP config html is not loaded");
        return ESP_FAIL;
    }

    esp_err_t err = wifi_manager_ap();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "start wifi ap failed: %s", esp_err_to_name(err));
        return err;
    }

    ws_cfg_t ws_cfg = {
        .html_code = page,
        .receive_fn = ws_receive_handle,
    };
    web_ws_stop();
    err = web_ws_start(&ws_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "start web server failed: %s", esp_err_to_name(err));
    }
    return err;
}
