//
// Created by k0921 on 2026/4/6.
//

#include "power_key.h"
#include "driver/gpio.h"
#include "pin_config.h"

void power_key_init(void) {
    gpio_config_t enio_cfg = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << KEY_POWER),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&enio_cfg);
}

uint8_t power_key_read(void) {
    return gpio_get_level(KEY_POWER);
}