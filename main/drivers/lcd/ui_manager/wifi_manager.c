//
// Created by k0921 on 2026/4/23.
//

#include "wifi_manager.h"
#include "gui_guider.h"
#include "config/ui_manager.h"
#include "wifi/ap_wifi.h"
#include "time/sntp_config.h"
#include "esp_err.h"
#include "esp_log.h"

#define TAG "wifi_manager"


static bool is_wifi_inited = false;

static bool is_wifi_connected = false;

static bool s_wifi_ap_pending = false;

lv_obj_t *label_connect = NULL;

// 在 WiFi 事件任务中只设置标志，通过 lv_async_call 安全更新 UI
static void wifi_connected_ui_update(void *user_data) {
    if (guider_ui.screen_wifi == NULL) return;  // 屏幕已删除，跳过
    if (label_connect) {
        lv_obj_del(label_connect);
        label_connect = NULL;
    }
    lv_label_set_text(guider_ui.screen_wifi_label_2, "Wifi已连接");
}

static void wifi_disconnected_ui_update(void *user_data) {
    if (guider_ui.screen_wifi == NULL) return;
    lv_label_set_text(guider_ui.screen_wifi_label_2, "Wifi未连接");
}

void wifi_state_callback(wifi_state state) {
    if (state == WIFI_STATE_CONNECTED) {
        is_wifi_connected = true;
        // 通过 lv_async_call 将 UI 更新投递到 LVGL 任务
        lv_async_call(wifi_connected_ui_update, NULL);
        ESP_LOGI(TAG, "WIFI connected.");
        my_nstp_init();
    }
    else if (state == WIFI_STATE_DISCONNECTED) {
        is_wifi_connected = false;
        lv_async_call(wifi_disconnected_ui_update, NULL);
        ESP_LOGI(TAG, "WIFI disconnected.");
    }
}

/**
 * 进入wifi界面前的回调函数
 */
void wifi_enter(void) {
    s_wifi_ap_pending = false;

    //加载wifi界面
    if(guider_ui.screen_wifi_del) {
        setup_scr_screen_wifi(&guider_ui);
        guider_ui.screen_wifi_del = false;
    }
    //配置wifi
    if (!is_wifi_inited) {
        esp_err_t ret = ap_wifi_init(wifi_state_callback);
        if (ret == ESP_OK) {
            is_wifi_inited = true;
        }
        else {
            ESP_LOGE(TAG, "ap wifi init failed: %s", esp_err_to_name(ret));
            lv_label_set_text(guider_ui.screen_wifi_label_2, "Wifi init failed");
        }
    }

    if (is_wifi_connected)
        lv_label_set_text(guider_ui.screen_wifi_label_2, "Wifi已连接");

    lv_scr_load(guider_ui.screen_wifi);
}

/**
 * 退出wifi界面的操作
 */
void wifi_exit(void) {
    s_wifi_ap_pending = false;

    //删除wifi界面的屏幕
    if (guider_ui.screen_wifi) {
        lv_obj_del(guider_ui.screen_wifi);
        guider_ui.screen_wifi_del = true;
        guider_ui.screen_wifi = NULL;
    }
}



static void wifi_start_ap_config(void) {
    if (label_connect) {
        lv_obj_del(label_connect);
        label_connect = NULL;
    }
    label_connect = lv_label_create(lv_scr_act());
    lv_label_set_text(label_connect, "AP config mode ...");
    lv_obj_set_style_text_color(label_connect, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label_connect, &lv_font_AlimamaDaoLiTi_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_align(label_connect, LV_ALIGN_CENTER, 0, 80);

    is_wifi_connected = false;
    esp_err_t ret = ap_wifi_apcfg();
    if (ret == ESP_OK) {
        lv_label_set_text(guider_ui.screen_wifi_label_2, "AP: 192.168.100.1");
    }
    else {
        ESP_LOGE(TAG, "ap wifi config failed: %s", esp_err_to_name(ret));
        lv_label_set_text(guider_ui.screen_wifi_label_2, "Wifi config failed");
    }
}

/**
 * 长按触发的操作
 */
/* Short press only arms AP config; release confirms it. */
void wifi_short_press(void) {
    s_wifi_ap_pending = true;
    lv_label_set_text(guider_ui.screen_wifi_label_2, "Release to enter AP");
}

void wifi_release_press(void) {
    if (!s_wifi_ap_pending) {
        return;
    }

    s_wifi_ap_pending = false;
    wifi_start_ap_config();
}

/* Long press cancels AP config and returns to menu. */
void wifi_long_press(void) {
    s_wifi_ap_pending = false;
    //返回菜单界面
    screen_manager_set_active(&menu_interface);
}

/**
 * 注册wifi界面
 */
void wifi_register(void) {
    wifi_interface.enter = wifi_enter;
    wifi_interface.exit = wifi_exit;
    wifi_interface.rotate = NULL;
    wifi_interface.rotate_stop = NULL;
    wifi_interface.short_press = wifi_short_press;
    wifi_interface.release_press = wifi_release_press;
    wifi_interface.long_press = wifi_long_press;
    wifi_interface.is_registered = true;
}
