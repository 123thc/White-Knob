//
// Runtime background image for the time screen.
//

#ifndef WHITE_KNOB_TIME_BG_H
#define WHITE_KNOB_TIME_BG_H

#include "esp_err.h"
#include "lvgl.h"
#include <stddef.h>
#include <stdint.h>

#define TIME_BG_WIDTH 240
#define TIME_BG_HEIGHT 240
#define TIME_BG_PIXEL_BYTES 2
#define TIME_BG_IMAGE_SIZE (TIME_BG_WIDTH * TIME_BG_HEIGHT * TIME_BG_PIXEL_BYTES)

esp_err_t time_bg_save_rgb565(const uint8_t *data, size_t len);
const lv_img_dsc_t *time_bg_get_img(void);
void time_bg_apply_to_time_screen(void);
void time_bg_refresh_async(void);

#endif
