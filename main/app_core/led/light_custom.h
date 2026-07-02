//
// Custom light configuration saved from the web page.
//

#ifndef WHITE_KNOB_LIGHT_CUSTOM_H
#define WHITE_KNOB_LIGHT_CUSTOM_H

#include "esp_err.h"
#include "ws2812/ws2812_control.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool enabled;
    led_control_mode mode;
    led_direction direction;
    uint8_t brightness;
    uint8_t speed;
    uint8_t rgb_color[3];
} light_custom_config_t;

void light_custom_default_config(light_custom_config_t *cfg);
esp_err_t light_custom_validate(const light_custom_config_t *cfg);
esp_err_t light_custom_apply(const light_custom_config_t *cfg);
esp_err_t light_custom_save(const light_custom_config_t *cfg);
esp_err_t light_custom_load(light_custom_config_t *cfg);
esp_err_t light_custom_load_and_apply(void);
void light_custom_get_current(light_custom_config_t *cfg);

#endif //WHITE_KNOB_LIGHT_CUSTOM_H
