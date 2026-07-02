//
// Created by k0921 on 2025/10/24.
//

#ifndef ESP_LEARNING_GC9A01_H
#define ESP_LEARNING_GC9A01_H

#include "esp_err.h"
#include "driver/gpio.h"

// 刷新完成回调函数类型
typedef void (*lcd_flush_done_cb)(void* param);

// GC9A01配置结构体
typedef struct {
    int clk;                    // SCLK引脚
    int mosi;                   // MOSI引脚
    int cs;                     // CS引脚
    int dc;                     // DC引脚
    int rst;                    // RESET引脚
    int bl;                     // 背光控制引脚
    int width;                  // 屏幕宽度
    int height;                 // 屏幕高度
    int spi_fre;                // SPI频率
    int spin;                   // 屏幕旋转方向
    lcd_flush_done_cb done_cb;  // 刷新完成回调函数
    void* cb_param;             // 回调函数参数
} gc9a01_cfg_t;

esp_err_t gc9a01_driver_hw_init(gc9a01_cfg_t* cfg);

void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void Circle_LCD_Init(void);//LCD初始化

void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void gc9a01_flush(int x1,int x2,int y1,int y2,void *data);

#endif //ESP_LEARNING_GC9A01_H