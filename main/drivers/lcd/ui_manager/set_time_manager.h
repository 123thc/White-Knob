//
// Created by k0921 on 2026/5/5.
//

#ifndef WHITE_KNOB_SET_TIME_MANAGER_H
#define WHITE_KNOB_SET_TIME_MANAGER_H
#include <stdint.h>

void set_time_register(void);
void set_time_set_mhs(uint16_t *h, uint16_t *m, uint16_t *s);

#endif //WHITE_KNOB_SET_TIME_MANAGER_H