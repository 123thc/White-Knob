//
// Created by k0921 on 2026/4/5.
//

#include "hx711.h"
#include "pin_config.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define TAG "hx711"

//HX711的基准值
uint32_t HX711_BASE_VALUE = 0;

/**
 * HX711硬件相关初始化
 */
void hx711_hw_init(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ull << STRAIN_SCK),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    io_conf = (gpio_config_t){
        .pin_bit_mask = (1ull << STRAIN_DO),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
    };
    gpio_config(&io_conf);

    gpio_set_level(STRAIN_SCK, 0);//默认设为低电平

    ESP_LOGI(TAG, "waiting for hx711 initialing......");
    //获取hx711的基础值
    for (uint8_t i = 0; i < 100; i++) {
        uint32_t temp = HX711_getValue(CHANNEL_A_128);
        while (temp == 0) {
            temp = HX711_getValue(CHANNEL_A_128);
        }
        HX711_BASE_VALUE += (temp / 100);
    }
    ESP_LOGI(TAG, "hx711 has initialed successfully");
    ESP_LOGI(TAG, "The base sensor value is %ld", HX711_BASE_VALUE);
}

/**
 * 读取HX711数据（无符号）
 * @param gian 增益通道
 * @return 24位无符号重量数据
 */
uint32_t HX711_getValue(gain_hx711 gian) {
    uint32_t value = 0;
    gpio_set_level(STRAIN_SCK, 0);
    if (gpio_get_level(STRAIN_DO) == 1)
        return 0;

    for (uint8_t i = 0; i < 24; i++) {
        gpio_set_level(STRAIN_SCK, 1);
        value = value << 1;
        gpio_set_level(STRAIN_SCK, 0);
        if (gpio_get_level(STRAIN_DO) == 1)
            value++;
    }

    switch (gian) {
        case CHANNEL_A_128:
            gpio_set_level(STRAIN_SCK, 1);
            value = value ^ 0x800000;
            gpio_set_level(STRAIN_SCK, 0);
            break;
        case CHANNEL_B_64:
            gpio_set_level(STRAIN_SCK, 1);
            value = value ^ 0x800000;
            gpio_set_level(STRAIN_SCK, 0);
            gpio_set_level(STRAIN_SCK, 1);
            gpio_set_level(STRAIN_SCK, 0);
            break;
        case CHANNEL_C_32:
            gpio_set_level(STRAIN_SCK, 1);
            value = value ^ 0x800000;
            gpio_set_level(STRAIN_SCK, 0);
            gpio_set_level(STRAIN_SCK, 1);
            gpio_set_level(STRAIN_SCK, 0);
            gpio_set_level(STRAIN_SCK, 1);
            gpio_set_level(STRAIN_SCK, 0);
            break;
        default:
            break;
    }

    return value;
}