/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "ble_hidd_start.h"

#include <class/hid/hid.h>
#include <class/hid/hid_device.h>

#include "esp_hidd_prf_api.h"
#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "driver/gpio.h"
#include "hid_dev.h"
#include "lvgl.h"


/**
 * 简要说明：
 * 本示例实现了BLE HID设备配置文件的相关功能。该HID设备包含4种报告类型：
 * 1：鼠标报告
 * 2：键盘和LED报告
 * 3：消费设备报告
 * 4：供应商自定义设备报告
 * 用户可根据自己的应用场景选择不同的报告类型。
 * BLE HID配置文件继承了USB HID类的设计。
 */

/**
 * 注意事项：
 * 1. Windows 10不支持供应商报告，因此SUPPORT_REPORT_VENDOR始终设为FALSE（定义在hidd_le_prf_int.h中）
 * 2. 在iPhone进行HID加密期间不允许更新连接参数，从设备在加密期间关闭自动更新连接参数的功能
 * 3. 当HID设备连接后，即使HID加密未完成，iPhone也会向报告特征配置描述符写入值1。
 *    实际上这应该在HID加密完成后进行。我们已将报告特征配置描述符的权限修改为
 *    `ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE_ENCRYPTED`。
 *    如果遇到`GATT_INSUF_ENCRYPTION`错误，请忽略。
 */

#define HID_DEMO_TAG "BLE_HID"  // 日志标签

// 全局变量声明
uint16_t hid_conn_id = 0;        // HID连接ID
static bool sec_conn = false;           // 安全连接状态标志
// static bool send_volum_up = false;      // 发送音量增加标志
#define CHAR_DECLARATION_SIZE   (sizeof(uint8_t))  // 特征声明大小

// 函数声明
static void hidd_event_callback(esp_hidd_cb_event_t event, esp_hidd_cb_param_t *param);

// 蓝牙设备名称
#define HIDD_DEVICE_NAME            "白-Knob"

// HID服务UUID（128位）
static uint8_t hidd_service_uuid128[] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    // 第一个UUID，16位，[12]、[13]为实际值
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x12, 0x18, 0x00, 0x00,
};

// 广播数据配置
static esp_ble_adv_data_t hidd_adv_data = {
    .set_scan_rsp = false,              // 不设置扫描响应
    .include_name = true,               // 包含设备名称
    .include_txpower = true,            // 包含发射功率
    .min_interval = 0x0006,             // 从设备最小连接间隔（单位：1.25ms）
    .max_interval = 0x0010,             // 从设备最大连接间隔（单位：1.25ms）
    .appearance = 0x03c0,               // 外观：HID通用设备
    .manufacturer_len = 0,              // 制造商数据长度
    .p_manufacturer_data =  NULL,       // 制造商数据指针
    .service_data_len = 0,              // 服务数据长度
    .p_service_data = NULL,             // 服务数据指针
    .service_uuid_len = sizeof(hidd_service_uuid128),  // 服务UUID长度
    .p_service_uuid = hidd_service_uuid128,            // 服务UUID指针
    .flag = 0x6,                        // 广播标志
};

// 广播参数配置
static esp_ble_adv_params_t hidd_adv_params = {
    .adv_int_min        = 0x20,         // 最小广播间隔
    .adv_int_max        = 0x30,         // 最大广播间隔，单位：0.625ms
    .adv_type           = ADV_TYPE_IND, // 广播类型：可连接的非定向广播
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC, // 自身地址类型：公共地址
    .channel_map        = ADV_CHNL_ALL, // 使用所有通道
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY, // 过滤策略：允许任何扫描和连接
};

/**
 * HID事件回调函数
 * @param event 事件类型
 * @param param 事件参数
 */
static void hidd_event_callback(esp_hidd_cb_event_t event, esp_hidd_cb_param_t *param)
{
    switch(event) {
        case ESP_HIDD_EVENT_REG_FINISH: {  // HID注册完成事件
            if (param->init_finish.state == ESP_HIDD_INIT_OK) {
                // 设置设备名称并配置广播数据
                esp_ble_gap_set_device_name(HIDD_DEVICE_NAME);
                esp_ble_gap_config_adv_data(&hidd_adv_data);
            }
            break;
        }
        case ESP_BAT_EVENT_REG: {  // 电池事件注册
            break;
        }
        case ESP_HIDD_EVENT_DEINIT_FINISH:  // HID反初始化完成
            break;
        case ESP_HIDD_EVENT_BLE_CONNECT: {  // BLE连接事件
            ESP_LOGI(HID_DEMO_TAG, "ESP_HIDD_EVENT_BLE_CONNECT");
            hid_conn_id = param->connect.conn_id;  // 保存连接ID
            break;
        }
        case ESP_HIDD_EVENT_BLE_DISCONNECT: {  // BLE断开连接事件
            sec_conn = false;  // 重置安全连接标志
            ESP_LOGI(HID_DEMO_TAG, "ESP_HIDD_EVENT_BLE_DISCONNECT");
            esp_ble_gap_start_advertising(&hidd_adv_params);  // 重新开始广播
            break;
        }
        case ESP_HIDD_EVENT_BLE_VENDOR_REPORT_WRITE_EVT: {  // 供应商报告写入事件
            ESP_LOGI(HID_DEMO_TAG, "%s, ESP_HIDD_EVENT_BLE_VENDOR_REPORT_WRITE_EVT", __func__);
            // 十六进制打印接收到的数据
            ESP_LOG_BUFFER_HEX(HID_DEMO_TAG, param->vendor_write.data, param->vendor_write.length);
            break;
        }
        case ESP_HIDD_EVENT_BLE_LED_REPORT_WRITE_EVT: {  // LED报告写入事件
            ESP_LOGI(HID_DEMO_TAG, "ESP_HIDD_EVENT_BLE_LED_REPORT_WRITE_EVT");
            // 十六进制打印接收到的数据
            ESP_LOG_BUFFER_HEX(HID_DEMO_TAG, param->led_write.data, param->led_write.length);
            break;
        }
        default:
            break;
    }
    return;
}

/**
 * GAP事件处理函数
 * @param event GAP事件类型
 * @param param GAP事件参数
 */
static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:  // 广播数据设置完成
        esp_ble_gap_start_advertising(&hidd_adv_params);  // 开始广播
        break;
    case ESP_GAP_BLE_SEC_REQ_EVT:  // 安全请求事件
        // 打印对端设备地址
        for(int i = 0; i < ESP_BD_ADDR_LEN; i++) {
            ESP_LOGD(HID_DEMO_TAG, "%x:", param->ble_security.ble_req.bd_addr[i]);
        }
        esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);  // 响应安全请求
        break;
    case ESP_GAP_BLE_AUTH_CMPL_EVT:  // 认证完成事件
        sec_conn = true;  // 设置安全连接标志
        esp_bd_addr_t bd_addr;
        memcpy(bd_addr, param->ble_security.auth_cmpl.bd_addr, sizeof(esp_bd_addr_t));
        // 打印对端蓝牙地址
        ESP_LOGI(HID_DEMO_TAG, "远程设备地址: %08x%04x",
                (bd_addr[0] << 24) + (bd_addr[1] << 16) + (bd_addr[2] << 8) + bd_addr[3],
                (bd_addr[4] << 8) + bd_addr[5]);
        ESP_LOGI(HID_DEMO_TAG, "地址类型 = %d", param->ble_security.auth_cmpl.addr_type);
        ESP_LOGI(HID_DEMO_TAG, "配对状态 = %s",
                param->ble_security.auth_cmpl.success ? "成功" : "失败");
        if(!param->ble_security.auth_cmpl.success) {
            ESP_LOGE(HID_DEMO_TAG, "失败原因 = 0x%x", param->ble_security.auth_cmpl.fail_reason);
        }
        break;
    default:
        break;
    }
}


/**
 * 蓝牙HID启动函数
 * 初始化整个蓝牙HID设备
 */
void bluetooth_hid_start(void)
{
    esp_err_t ret;
    //
    // // 初始化NVS（非易失性存储）
    // ret = nvs_flash_init();
    // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     ESP_ERROR_CHECK(nvs_flash_erase());  // 擦除NVS分区
    //     ret = nvs_flash_init();  // 重新初始化
    // }
    // ESP_ERROR_CHECK(ret);

    // 初始化蓝牙控制器（释放经典蓝牙内存，仅使用BLE）
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    // 配置并初始化蓝牙控制器
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(HID_DEMO_TAG, "%s 控制器初始化失败", __func__);
        return;
    }

    // 启用蓝牙控制器（BLE模式）
    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        ESP_LOGE(HID_DEMO_TAG, "%s 启用控制器失败", __func__);
        return;
    }

    // 初始化蓝牙协议栈
    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(HID_DEMO_TAG, "%s 初始化Bluedroid失败", __func__);
        return;
    }

    // 启用蓝牙协议栈
    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(HID_DEMO_TAG, "%s 启用Bluedroid失败", __func__);
        return;
    }

    // 初始化HID配置文件
    if((ret = esp_hidd_profile_init()) != ESP_OK) {
        ESP_LOGE(HID_DEMO_TAG, "%s HID配置文件初始化失败", __func__);
    }

    // 注册GAP事件回调函数
    esp_ble_gap_register_callback(gap_event_handler);
    // 注册HID事件回调函数
    esp_hidd_register_callbacks(hidd_event_callback);

    /* 设置安全参数：认证要求、I/O能力、密钥大小和初始密钥响应参数 */
    esp_ble_auth_req_t auth_req = ESP_LE_AUTH_BOND;     // 认证后与对端设备绑定
    esp_ble_io_cap_t iocap = ESP_IO_CAP_NONE;           // I/O能力：无输入输出
    uint8_t key_size = 16;                              // 密钥大小：16字节（7~16字节之间）
    uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;  // 初始密钥
    uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;   // 响应密钥

    // 设置各项安全参数
    esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));

    /*
     * 如果BLE设备作为从设备：
     * init_key表示你希望主设备分发哪些类型的密钥给你
     * rsp_key表示你可以向主设备分发哪些密钥
     *
     * 如果BLE设备作为主设备：
     * rsp_key表示你希望从设备分发哪些类型的密钥给你
     * init_key表示你可以向从设备分发哪些密钥
     */
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));
}

