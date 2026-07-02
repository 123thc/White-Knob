//
// Created by k0921 on 2025/10/9.
//

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "lv_port.h"
#include <math.h>
#include "lcd/custom_operation/adsorb.h"
#include "smart_knob/smart_knob.h"
#include "lcd/custom_operation/scroll.h"
#include <sys/stat.h>
#include "lvgl.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "ST7789.h"
#include "GC9A01.h"
#include "pin_config.h"
#include "lcd/ui_task.h"

#define TAG "lv_port"

#define ON  1
#define OFF 0

#define YES 1
#define NO 0

//显示驱动类型
#define LCD_DRIVER_ST7789  1
#define LCD_DRIVER_GC9A01  2

/************************预编译处理部分****************************/
//LCD屏幕分辨率
#define LCD_WIDTH 240
#define LCD_HEIGHT 240

//LCD的显示驱动选择
#define LCD_DRIVER_NOW          LCD_DRIVER_GC9A01

//LCD外设注册选择
#define LCD_DEVICE_REGISTER     YES

//画面旋转角度
#define LCD_SPIN                0

//SPI通信速率
#define SPI_FREQUENCY           40000000//40M
/************************预编译处理部分****************************/



lv_disp_drv_t disp_drv;

void disp_flush(struct _lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_map) {
#if LCD_DRIVER_NOW == LCD_DRIVER_ST7789
    st7789_flush(area->x1, area->x2 + 1, area->y1, area->y2 + 1, color_map);
#elif LCD_DRIVER_NOW == LCD_DRIVER_GC9A01
    gc9a01_flush(area->x1, area->x2 + 1, area->y1, area->y2 + 1, color_map);
#endif
}

/**
 * @brief ST7789驱动初始化完成调用的回调函数
 * @param param
 */
void lv_flush_done_cb(void* param) {
    lv_disp_flush_ready(&disp_drv);
}

/**
 * @brief LVGL显示初始化
 */
void lv_disp_init(void) {
    static lv_disp_draw_buf_t disp_buf;
    const size_t disp_buf_size = LCD_WIDTH * (LCD_HEIGHT / 4);

    lv_color_t *disp1 = heap_caps_malloc(disp_buf_size * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    lv_color_t *disp2 = heap_caps_malloc(disp_buf_size * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

    if (!disp1 || !disp2) {
        ESP_LOGE(TAG, "disp buff malloc fail!");
        return;
    }
    lv_disp_draw_buf_init(&disp_buf, disp1, disp2, disp_buf_size);

    // 立刻将两块缓冲区清为黑色
    memset(disp1, 0x00, disp_buf_size * sizeof(lv_color_t));
    memset(disp2, 0x00, disp_buf_size * sizeof(lv_color_t));

    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = disp_flush;
    lv_disp_drv_register(&disp_drv);
}


/**
 * @brief LVGL输入设备初始化
 */
void lv_indev_init(void) {
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = indev_read;
    lv_indev_drv_register(&indev_drv);
}


void st7789_hw_init(void) {
    //定义并初始化LCD的驱动ST7789的结构体
    st7789_cfg_t lcd_config = {
        .clk = LCD_CLK, // SPI时钟引脚
        .mosi = LCD_MOSI, // SPI数据引脚
        .dc = LCD_DC, // 数据/命令选择引脚
        .cs = LCD_CS, // 片选引脚
        .rst = LCD_RST, // 复位引脚
        .bl = LCD_BACKLIGHT, // 背光控制引脚
        .width = LCD_WIDTH, // 屏幕宽度
        .height = LCD_HEIGHT, // 屏幕高度
        .spi_fre = SPI_FREQUENCY, // SPI频率40MHz
        .spin = LCD_SPIN, // 旋转方向0度
        .done_cb = lv_flush_done_cb,
        .cb_param = &disp_drv
    };

    // 初始化LCD
    esp_err_t ret = st7789_driver_hw_init(&lcd_config);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "ST7789 has been initialized successfully.");
    } else {
        ESP_LOGE(TAG, "Failed to initialize ST7789.");
        return;
    }

    // 打开背光
    st7789_lcd_backlight(true);
}

void gc9a01_hw_init(void) {
    // 定义并初始化LCD的驱动GC9A01的结构体
    gc9a01_cfg_t lcd_config = {
        .clk = LCD_CLK,         // SPI时钟引脚
        .mosi = LCD_MOSI,       // SPI数据引脚
        .dc = LCD_DC,           // 数据/命令选择引脚
        .cs = LCD_CS,           // 片选引脚
        .rst = LCD_RST,         // 复位引脚
        .bl = LCD_BACKLIGHT,    // 背光控制引脚
        .width = LCD_WIDTH,     // 屏幕宽度 (GC9A01通常为240)
        .height = LCD_HEIGHT,   // 屏幕高度 (GC9A01通常为240)
        .spi_fre = SPI_FREQUENCY,    // SPI频率40MHz
        .spin = LCD_SPIN,              // 旋转方向0度
        .done_cb = lv_flush_done_cb,
        .cb_param = &disp_drv
    };

    // 初始化LCD
    esp_err_t ret = gc9a01_driver_hw_init(&lcd_config);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "GC9A01 has been initialized successfully.");
    } else {
        ESP_LOGE(TAG, "Failed to initialize GC9A01.");
        return;
    }
}

void lv_timer_cb(void* arg) {
    uint32_t tick_interval = *((uint32_t*)arg);
    lv_tick_inc(tick_interval);
}

/**
 * @brief 给LVGL提供的时钟进行初始化
 */
void lv_tick_init(void) {
    static uint32_t tick_interval_ms = 5;
    const esp_timer_create_args_t arg =
    {
        .arg = &tick_interval_ms,
        .callback = lv_timer_cb,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "lv_tick",
        .skip_unhandled_events = true
    };

    esp_timer_handle_t timer_handle;
    esp_timer_create(&arg, &timer_handle);
    esp_timer_start_periodic(timer_handle, tick_interval_ms * 1000);
}

/**
 * @brief LVGL整体初始化
 */
void lv_port_init(void) {
    /*LVGL部件初始化*/
    lv_init();

    /*显示驱动初始化*/
#if LCD_DRIVER_NOW == LCD_DRIVER_ST7789
    st7789_hw_init();
#elif LCD_DRIVER_NOW == LCD_DRIVER_GC9A01
    gc9a01_hw_init();
#endif

    /*LVGL初始化*/
    lv_disp_init();

    //外设是否注册
#if LCD_DEVICE_REGISTER == YES
    lv_indev_init();
#endif

    lv_tick_init();
}
