//
// Created by k0921 on 2026/3/20.
//

#ifndef ESP32_TEST_WEB_SEVER_H
#define ESP32_TEST_WEB_SEVER_H
#include <esp_err.h>
#include <stdint.h>

typedef void(* ws_receive_cb)(uint8_t * payload, int len);

typedef struct {
    const char * html_code;
    ws_receive_cb receive_fn;
}ws_cfg_t;

esp_err_t web_ws_start(ws_cfg_t * cfg);
esp_err_t web_ws_stop(void);
esp_err_t web_ws_send(uint8_t *data, int len);

#endif //ESP32_TEST_WEB_SEVER_H