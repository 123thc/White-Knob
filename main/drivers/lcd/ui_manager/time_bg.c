//
// Runtime background image for the time screen.
//

#include "time_bg.h"

#include "esp_crc.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "gui_guider.h"
#include "src/draw/lv_img_cache.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TAG "time_bg"

#define TIME_BG_PARTITION_LABEL "others"
#define TIME_BG_MAGIC 0x47425457u
#define TIME_BG_VERSION 1u
#define TIME_BG_FORMAT_RGB565_SWAPPED 0x4c563136u
#define TIME_BG_HEADER_OFFSET 0
#define TIME_BG_DATA_OFFSET 0x1000

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t header_size;
    uint16_t width;
    uint16_t height;
    uint32_t format;
    uint32_t data_size;
    uint32_t crc32;
    uint32_t reserved[8];
} time_bg_header_t;

static uint8_t *s_image_data = NULL;
static lv_img_dsc_t s_img_dsc;
static bool s_img_dsc_ready = false;
static bool s_load_checked = false;
static bool s_loaded = false;

static const esp_partition_t *time_bg_find_partition(void)
{
    const esp_partition_t *part = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, TIME_BG_PARTITION_LABEL);
    if (part == NULL) {
        ESP_LOGE(TAG, "partition %s not found", TIME_BG_PARTITION_LABEL);
        return NULL;
    }

    if (part->size < TIME_BG_DATA_OFFSET + TIME_BG_IMAGE_SIZE) {
        ESP_LOGE(TAG, "partition %s too small: %lu bytes", TIME_BG_PARTITION_LABEL,
                 (unsigned long)part->size);
        return NULL;
    }

    return part;
}

static bool time_bg_header_valid(const time_bg_header_t *header)
{
    return header->magic == TIME_BG_MAGIC &&
           header->version == TIME_BG_VERSION &&
           header->header_size == (uint16_t)sizeof(time_bg_header_t) &&
           header->width == TIME_BG_WIDTH &&
           header->height == TIME_BG_HEIGHT &&
           header->format == TIME_BG_FORMAT_RGB565_SWAPPED &&
           header->data_size == TIME_BG_IMAGE_SIZE;
}

static void time_bg_prepare_dsc(void)
{
    memset(&s_img_dsc, 0, sizeof(s_img_dsc));
    s_img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;
    s_img_dsc.header.always_zero = 0;
    s_img_dsc.header.reserved = 0;
    s_img_dsc.header.w = TIME_BG_WIDTH;
    s_img_dsc.header.h = TIME_BG_HEIGHT;
    s_img_dsc.data_size = TIME_BG_IMAGE_SIZE;
    s_img_dsc.data = s_image_data;
    s_img_dsc_ready = true;
    lv_img_cache_invalidate_src(&s_img_dsc);
}

static esp_err_t time_bg_read_flash_image(uint8_t **out_data)
{
    if (out_data == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    *out_data = NULL;

    const esp_partition_t *part = time_bg_find_partition();
    if (part == NULL) {
        return ESP_ERR_NOT_FOUND;
    }

    time_bg_header_t header = {0};
    esp_err_t err = esp_partition_read(part, TIME_BG_HEADER_OFFSET, &header, sizeof(header));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "read time bg header failed: %s", esp_err_to_name(err));
        return err;
    }

    if (!time_bg_header_valid(&header)) {
        return ESP_ERR_NOT_FOUND;
    }

    uint8_t *data = (uint8_t *)malloc(TIME_BG_IMAGE_SIZE);
    if (data == NULL) {
        ESP_LOGE(TAG, "malloc time bg failed");
        return ESP_ERR_NO_MEM;
    }

    err = esp_partition_read(part, TIME_BG_DATA_OFFSET, data, TIME_BG_IMAGE_SIZE);
    if (err != ESP_OK) {
        free(data);
        ESP_LOGE(TAG, "read time bg data failed: %s", esp_err_to_name(err));
        return err;
    }

    uint32_t crc = esp_crc32_le(0, data, TIME_BG_IMAGE_SIZE);
    if (crc != header.crc32) {
        free(data);
        ESP_LOGW(TAG, "time bg crc mismatch: flash=%08lx calc=%08lx",
                 (unsigned long)header.crc32, (unsigned long)crc);
        return ESP_ERR_INVALID_CRC;
    }

    *out_data = data;
    return ESP_OK;
}

static esp_err_t time_bg_load_from_flash(bool force)
{
    if (!force && s_load_checked) {
        return s_loaded ? ESP_OK : ESP_ERR_NOT_FOUND;
    }

    uint8_t *new_data = NULL;
    esp_err_t err = time_bg_read_flash_image(&new_data);
    if (err != ESP_OK) {
        s_loaded = false;
        s_load_checked = (err == ESP_ERR_NOT_FOUND || err == ESP_ERR_INVALID_CRC);
        return err;
    }

    if (s_img_dsc_ready) {
        lv_img_cache_invalidate_src(&s_img_dsc);
    }

    uint8_t *old_data = s_image_data;
    s_image_data = new_data;
    s_load_checked = true;
    s_loaded = true;
    time_bg_prepare_dsc();

    if (old_data != NULL) {
        free(old_data);
    }

    ESP_LOGI(TAG, "custom time bg loaded");
    return ESP_OK;
}

esp_err_t time_bg_save_rgb565(const uint8_t *data, size_t len)
{
    if (data == NULL || len != TIME_BG_IMAGE_SIZE) {
        return ESP_ERR_INVALID_ARG;
    }

    const esp_partition_t *part = time_bg_find_partition();
    if (part == NULL) {
        return ESP_ERR_NOT_FOUND;
    }

    time_bg_header_t header = {
        .magic = TIME_BG_MAGIC,
        .version = TIME_BG_VERSION,
        .header_size = (uint16_t)sizeof(time_bg_header_t),
        .width = TIME_BG_WIDTH,
        .height = TIME_BG_HEIGHT,
        .format = TIME_BG_FORMAT_RGB565_SWAPPED,
        .data_size = TIME_BG_IMAGE_SIZE,
        .crc32 = esp_crc32_le(0, data, TIME_BG_IMAGE_SIZE),
    };

    esp_err_t err = esp_partition_erase_range(part, 0, part->size);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "erase time bg partition failed: %s", esp_err_to_name(err));
        return err;
    }

    err = esp_partition_write(part, TIME_BG_DATA_OFFSET, data, TIME_BG_IMAGE_SIZE);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "write time bg data failed: %s", esp_err_to_name(err));
        return err;
    }

    err = esp_partition_write(part, TIME_BG_HEADER_OFFSET, &header, sizeof(header));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "write time bg header failed: %s", esp_err_to_name(err));
        return err;
    }

    s_load_checked = false;
    s_loaded = false;
    ESP_LOGI(TAG, "custom time bg saved");
    return ESP_OK;
}

const lv_img_dsc_t *time_bg_get_img(void)
{
    if (time_bg_load_from_flash(false) != ESP_OK) {
        return NULL;
    }

    return s_img_dsc_ready ? &s_img_dsc : NULL;
}

void time_bg_apply_to_time_screen(void)
{
    if (guider_ui.screen_time == NULL || guider_ui.screen_time_img_1 == NULL) {
        return;
    }

    const lv_img_dsc_t *img = time_bg_get_img();
    if (img == NULL) {
        return;
    }

    lv_img_set_src(guider_ui.screen_time_img_1, img);
    lv_obj_invalidate(guider_ui.screen_time_img_1);
}

static void time_bg_refresh_cb(void *user_data)
{
    (void)user_data;
    if (time_bg_load_from_flash(true) == ESP_OK) {
        time_bg_apply_to_time_screen();
    }
}

void time_bg_refresh_async(void)
{
    lv_async_call(time_bg_refresh_cb, NULL);
}
