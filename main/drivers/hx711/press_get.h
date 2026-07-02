//
// Created by k0921 on 2026/4/6.
//

#ifndef WHITE_KNOB_PRESS_GET_H
#define WHITE_KNOB_PRESS_GET_H

typedef void(* hx711_relaeas_cb_t)(void);
typedef void(* hx711_short_press_cb_t)(void);
typedef void(* hx711_long_press_cb_t)(void);

typedef enum {
    HX711_RELEASE,
    HX711_SHORT_PRESS,
    HX711_LONG_PRESS,
    HX711_HOLD_ON,
}hx711_state;

void hx711_press_init(hx711_relaeas_cb_t rf, hx711_short_press_cb_t sf, hx711_long_press_cb_t lf);
void hx711_press_handle(void);

#endif //WHITE_KNOB_PRESS_GET_H