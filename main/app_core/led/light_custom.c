//
// Created for web-configurable WS2812 light settings.
//

#include "light_custom.h"
#include "led_decoration.h"
#include "nvs.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include <string.h>

#define TAG "light_custom"

#define LIGHT_CUSTOM_NAMESPACE "light_cfg"
#define LIGHT_CUSTOM_KEY "custom"
#define LIGHT_CUSTOM_MAGIC 0x4c435547u
#define LIGHT_CUSTOM_VERSION 1u

typedef struct {
    uint32_t magic;
    uint8_t version;
    uint8_t enabled;
    uint8_t mode;
    uint8_t direction;
    uint8_t brightness;
    uint8_t speed;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} light_custom_storage_t;

static portMUX_TYPE s_light_mux = portMUX_INITIALIZER_UNLOCKED;

static bool light_custom_mode_supported(led_control_mode mode)
{
    return mode == MODE_RAINBOW ||
           mode == MODE_SOLID_COLOR ||
           mode == MODE_GRADIENT ||
           mode == MODE_BREATHING ||
           mode == MODE_COMET;
}

void light_custom_default_config(light_custom_config_t *cfg)
{
    if (cfg == NULL) {
        return;
    }

    cfg->enabled = true;
    cfg->mode = MODE_GRADIENT;
    cfg->direction = LED_FORWARD;
    cfg->brightness = 255;
    cfg->speed = 10;
    cfg->rgb_color[R_COLOR] = 46;
    cfg->rgb_color[G_COLOR] = 197;
    cfg->rgb_color[B_COLOR] = 206;
}

esp_err_t light_custom_validate(const light_custom_config_t *cfg)
{
    if (cfg == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    if (!light_custom_mode_supported(cfg->mode)) {
        return ESP_ERR_INVALID_ARG;
    }
    if (cfg->direction != LED_FORWARD && cfg->direction != LED_BACKWARD) {
        return ESP_ERR_INVALID_ARG;
    }
    if (cfg->speed > WS2812_MAX_SPEED) {
        return ESP_ERR_INVALID_ARG;
    }
    return ESP_OK;
}

esp_err_t light_custom_apply(const light_custom_config_t *cfg)
{
    esp_err_t err = light_custom_validate(cfg);
    if (err != ESP_OK) {
        return err;
    }

    portENTER_CRITICAL(&s_light_mux);
    g_ws2812_control.on_off = cfg->enabled;
    g_ws2812_control.last_mode = cfg->mode;
    g_ws2812_control.direction = cfg->direction;
    g_ws2812_control.brightness = cfg->brightness;
    g_ws2812_control.speed = cfg->speed;
    g_ws2812_control.rgb_color[R_COLOR] = cfg->rgb_color[R_COLOR];
    g_ws2812_control.rgb_color[G_COLOR] = cfg->rgb_color[G_COLOR];
    g_ws2812_control.rgb_color[B_COLOR] = cfg->rgb_color[B_COLOR];

    if (cfg->enabled) {
        g_ws2812_control.mode = cfg->mode;
    }
    else {
        g_ws2812_control.mode = MODE_SOLID_COLOR;
        g_ws2812_control.rgb_color[R_COLOR] = 0;
        g_ws2812_control.rgb_color[G_COLOR] = 0;
        g_ws2812_control.rgb_color[B_COLOR] = 0;
    }
    portEXIT_CRITICAL(&s_light_mux);

    return ESP_OK;
}

esp_err_t light_custom_save(const light_custom_config_t *cfg)
{
    esp_err_t err = light_custom_validate(cfg);
    if (err != ESP_OK) {
        return err;
    }

    light_custom_storage_t store = {
        .magic = LIGHT_CUSTOM_MAGIC,
        .version = LIGHT_CUSTOM_VERSION,
        .enabled = cfg->enabled ? 1 : 0,
        .mode = (uint8_t)cfg->mode,
        .direction = (uint8_t)cfg->direction,
        .brightness = cfg->brightness,
        .speed = cfg->speed,
        .r = cfg->rgb_color[R_COLOR],
        .g = cfg->rgb_color[G_COLOR],
        .b = cfg->rgb_color[B_COLOR],
    };

    nvs_handle_t handle;
    err = nvs_open(LIGHT_CUSTOM_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "nvs_open failed: %s", esp_err_to_name(err));
        return err;
    }

    err = nvs_set_blob(handle, LIGHT_CUSTOM_KEY, &store, sizeof(store));
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }
    nvs_close(handle);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "save failed: %s", esp_err_to_name(err));
    }
    return err;
}

esp_err_t light_custom_load(light_custom_config_t *cfg)
{
    if (cfg == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(LIGHT_CUSTOM_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return err;
    }

    light_custom_storage_t store = {0};
    size_t len = sizeof(store);
    err = nvs_get_blob(handle, LIGHT_CUSTOM_KEY, &store, &len);
    nvs_close(handle);
    if (err != ESP_OK) {
        return err;
    }
    if (len != sizeof(store) ||
        store.magic != LIGHT_CUSTOM_MAGIC ||
        store.version != LIGHT_CUSTOM_VERSION) {
        return ESP_ERR_INVALID_STATE;
    }

    cfg->enabled = store.enabled != 0;
    cfg->mode = (led_control_mode)store.mode;
    cfg->direction = (led_direction)store.direction;
    cfg->brightness = store.brightness;
    cfg->speed = store.speed;
    cfg->rgb_color[R_COLOR] = store.r;
    cfg->rgb_color[G_COLOR] = store.g;
    cfg->rgb_color[B_COLOR] = store.b;

    return light_custom_validate(cfg);
}

esp_err_t light_custom_load_and_apply(void)
{
    light_custom_config_t cfg;
    esp_err_t err = light_custom_load(&cfg);
    if (err != ESP_OK) {
        return err;
    }
    return light_custom_apply(&cfg);
}

void light_custom_get_current(light_custom_config_t *cfg)
{
    if (cfg == NULL) {
        return;
    }

    portENTER_CRITICAL(&s_light_mux);
    cfg->enabled = g_ws2812_control.on_off;
    cfg->mode = g_ws2812_control.on_off ? g_ws2812_control.mode : g_ws2812_control.last_mode;
    cfg->direction = g_ws2812_control.direction;
    cfg->brightness = g_ws2812_control.brightness;
    cfg->speed = g_ws2812_control.speed;
    cfg->rgb_color[R_COLOR] = g_ws2812_control.rgb_color[R_COLOR];
    cfg->rgb_color[G_COLOR] = g_ws2812_control.rgb_color[G_COLOR];
    cfg->rgb_color[B_COLOR] = g_ws2812_control.rgb_color[B_COLOR];
    portEXIT_CRITICAL(&s_light_mux);

    if (!light_custom_mode_supported(cfg->mode)) {
        cfg->mode = MODE_GRADIENT;
    }
}
