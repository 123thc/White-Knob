//
// Created by k0921 on 2026/4/5.
//

#ifndef WHITE_KNOB_HX711_H
#define WHITE_KNOB_HX711_H
#include <stdint.h>

typedef enum {
    CHANNEL_A_128,
    CHANNEL_B_64,
    CHANNEL_C_32,
}gain_hx711;

extern uint32_t HX711_BASE_VALUE;

void hx711_hw_init(void);
uint32_t HX711_getValue(gain_hx711 gian);

#endif //WHITE_KNOB_HX711_H