//
// Created by k0921 on 2026/4/11.
//

#ifndef WHITE_KNOB_SCROLL_H
#define WHITE_KNOB_SCROLL_H
#include <stdbool.h>
#include <stdint.h>

extern bool encoder_active;

void encoder_scroll_move(int16_t delta_pixels);
void move_labels(uint8_t dir, bool back);

#endif //WHITE_KNOB_SCROLL_H