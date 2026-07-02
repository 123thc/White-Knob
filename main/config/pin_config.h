//引脚配置文件

#ifndef WHITE_KNOB_PIN_CONFIG_H
#define WHITE_KNOB_PIN_CONFIG_H

// ================== SPI主机选择 ==================
#define LCD_SPI_HOST        SPI2_HOST
#define AS5047P_SPI_HOST    SPI3_HOST

// ================== LCD显示屏 ==================
#define LCD_CLK         GPIO_NUM_11
#define LCD_MOSI        GPIO_NUM_12
#define LCD_DC          GPIO_NUM_10
#define LCD_CS          GPIO_NUM_9 //硬件直接接地
#define LCD_RST         GPIO_NUM_13
#define LCD_BACKLIGHT   GPIO_NUM_14

// ================== AS5047P编码器 ==================
#define AS5047P_CLK     GPIO_NUM_39
#define AS5047P_MOSI    GPIO_NUM_48
#define AS5047P_MISO    GPIO_NUM_45
#define AS5047P_SS      GPIO_NUM_40

// ================== WS2812 灯带 ==================
#define WS2812_PIN      GPIO_NUM_1
#define WS2812_LED_NUM  16

// ================== TMC6300电机驱动 ==================
#define M0_IN1  GPIO_NUM_17
#define M0_IN2  GPIO_NUM_16
#define M0_IN3  GPIO_NUM_15

// ================== HX711压力传感器 ==================
#define STRAIN_SCK  GPIO_NUM_47
#define STRAIN_DO   GPIO_NUM_21

// ================== 电源检测按键 ==================
#define KEY_POWER    GPIO_NUM_5

// ================== 电池ADC ==================
#define POWER_ADC   GPIO_NUM_4


#endif //WHITE_KNOB_PIN_CONFIG_H