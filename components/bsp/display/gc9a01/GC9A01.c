//
// Created by k0921 on 2025/10/24.
//

//
// Created by k0921 on 2025/10/7.
//

#include "GC9A01.h"

#include <string.h>
#include "pin_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"

#define TAG "gc9a01"

//******************************硬件********************************************
//lcd操作句柄
static esp_lcd_panel_io_handle_t lcd_io_handle = NULL;

//刷新完成回调函数
static lcd_flush_done_cb    s_flush_done_cb = NULL;

//背光GPIO
static gpio_num_t   s_bl_gpio = -1;

// GC9A01 控制器指令定义（需放在代码头部，与其他宏定义同级）
#define GC9A01_CMD_CASET    0x2A  // 列地址设置（同ST7789）
#define GC9A01_CMD_RASET    0x2B  // 行地址设置（同ST7789）
#define GC9A01_CMD_RAMWR    0x2C  // 内存写入（同ST7789）
#define GC9A01_X_OFFSET     0    // GC9A01 X轴偏移量（关键：屏幕实际显示区域偏移20像素）
#define GC9A01_Y_OFFSET     0     // GC9A01 Y轴偏移量（多数型号为0，需根据实际屏幕调整）

static bool notify_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
	if(s_flush_done_cb)
		s_flush_done_cb(user_ctx);
	return false;
}

/** 控制背光
 * @param enable 是否使能背光
 * @return 无
*/
void gc9a01_lcd_backlight(bool enable)
{
	if(enable)
	{
		gpio_set_level(s_bl_gpio,1);
	}
	else
	{
		gpio_set_level(s_bl_gpio,0);
	}
}

/** gc9a01初始化
 * @param gc9a01_cfg_t  接口参数
 * @return 成功或失败
*/
esp_err_t gc9a01_driver_hw_init(gc9a01_cfg_t* cfg)
{
    //初始化SPI
    spi_bus_config_t buscfg = {
        .sclk_io_num = cfg->clk,        //SCLK引脚
        .mosi_io_num = cfg->mosi,       //MOSI引脚
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .flags = SPICOMMON_BUSFLAG_MASTER , //SPI主模式
        .max_transfer_sz = cfg->width * 40 * sizeof(uint16_t),  //DMA单次传输最大字节，最大32768
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

    s_flush_done_cb = cfg->done_cb; //设置刷新完成回调函数

    s_bl_gpio = cfg->bl;    //设置背光GPIO
    //初始化GPIO(BL)
    gpio_config_t bl_gpio_cfg =
    {
        .pull_up_en = GPIO_PULLUP_DISABLE,          //禁止上拉
        .pull_down_en = GPIO_PULLDOWN_DISABLE,      //禁止下拉
        .mode = GPIO_MODE_OUTPUT,                   //输出模式
        .intr_type = GPIO_INTR_DISABLE,             //禁止中断
        .pin_bit_mask = (1ULL<<cfg->bl)                //GPIO脚
    };
    gpio_config(&bl_gpio_cfg);

    //初始化复位脚
    if(cfg->rst > 0)
    {
        gpio_config_t rst_gpio_cfg =
        {
            .pull_up_en = GPIO_PULLUP_DISABLE,          //禁止上拉
            .pull_down_en = GPIO_PULLDOWN_DISABLE,      //禁止下拉
            .mode = GPIO_MODE_OUTPUT,                   //输出模式
            .intr_type = GPIO_INTR_DISABLE,             //禁止中断
            .pin_bit_mask = (1ULL<<cfg->rst)                //GPIO脚
        };
        gpio_config(&rst_gpio_cfg);
    }

    //创建基于spi的lcd操作句柄
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = cfg->dc,         //DC引脚
        .cs_gpio_num = cfg->cs,         //CS引脚
        .pclk_hz = cfg->spi_fre,        //SPI时钟频率
        .lcd_cmd_bits = 8,              //命令长度
        .lcd_param_bits = 8,            //参数长度
        .spi_mode = 0,                  //使用SPI0模式
        .trans_queue_depth = 10,        //表示可以缓存的spi传输事务队列深度
        .on_color_trans_done = notify_flush_ready,   //刷新完成回调函数
        .user_ctx = cfg->cb_param,                                    //回调函数参数
        .flags = {    // 以下为 SPI 时序的相关参数，需根据 LCD 驱动 IC 的数据手册以及硬件的配置确定
            .sio_mode = 0,    // 通过一根数据线（MOSI）读写数据，0: Interface I 型，1: Interface II 型
        },
    };
    // Attach the LCD to the SPI bus
    ESP_LOGI(TAG,"create esp_lcd_new_panel");
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_HOST, &io_config, &lcd_io_handle));

    //硬件复位
    if(cfg->rst > 0)
    {
        gpio_set_level(cfg->rst,0);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_set_level(cfg->rst,1);
        vTaskDelay(pdMS_TO_TICKS(100));
    }

	//打开背光
	gc9a01_lcd_backlight(1);
	vTaskDelay(pdMS_TO_TICKS(100));

    /*向LCD写入初始化命令*/
    esp_lcd_panel_io_tx_param(lcd_io_handle,0xEF, NULL,0);
	esp_lcd_panel_io_tx_param(lcd_io_handle,0xEB, (uint8_t[]) {0x14,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xFE, NULL,0);
	esp_lcd_panel_io_tx_param(lcd_io_handle,0xEF, NULL,0);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xEB, (uint8_t[]) {0x14,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x84, (uint8_t[]) {0x40,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x85, (uint8_t[]) {0xFF,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x86, (uint8_t[]) {0xFF,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x87, (uint8_t[]) {0xFF,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x88, (uint8_t[]) {0x0A,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x89, (uint8_t[]) {0x21,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x8A, (uint8_t[]) {0x00,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x8B, (uint8_t[]) {0x80,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x8C, (uint8_t[]) {0x01,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x8D, (uint8_t[]) {0x01,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x8E, (uint8_t[]) {0xFF,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x8F, (uint8_t[]) {0xFF,},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xB6, (uint8_t[]) {0x00, 0x20},2);

	//控制旋转角度
	switch(cfg->spin)
	{
		case 0:
			esp_lcd_panel_io_tx_param(lcd_io_handle,0x36, (uint8_t[]) {0x08},1); // RGB顺序
			break;
		case 1:
			esp_lcd_panel_io_tx_param(lcd_io_handle,0x36, (uint8_t[]) {0xC8},1);
			break;
		case 2:
			esp_lcd_panel_io_tx_param(lcd_io_handle,0x36, (uint8_t[]) {0x68},1);
			break;
		case 3:
			esp_lcd_panel_io_tx_param(lcd_io_handle,0x36, (uint8_t[]) {0xA8},1);
			break;
		default:break;
	}

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x3A, (uint8_t[]) {0x65},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x90, (uint8_t[]) {0x08, 0x08, 0x08, 0x08},4);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xBD, (uint8_t[]) {0x06},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xBC, (uint8_t[]) {0x00},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xFF, (uint8_t[]) {0x60, 0x01, 0x04},3);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xC3, (uint8_t[]) {0x13},1);
	esp_lcd_panel_io_tx_param(lcd_io_handle,0xC4, (uint8_t[]) {0x13},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xC9, (uint8_t[]) {0x22},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xBE, (uint8_t[]) {0x11},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xE1, (uint8_t[]) {0x10, 0x0E},2);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xDF, (uint8_t[]) {0x21, 0x0c, 0x02},3);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xF0, (uint8_t[]) {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A},6);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xF1, (uint8_t[]) {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F},6);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xF2, (uint8_t[]) {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A},6);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xF3, (uint8_t[]) {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F},6);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xED, (uint8_t[]) {0x1B, 0x0B},2);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xAE, (uint8_t[]) {0x77},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xCD, (uint8_t[]) {0x63},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x70, (uint8_t[]) {0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08,
	0x03},9);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0xE8, (uint8_t[]) {0x34},1);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x62, (uint8_t[]) {0x18, 0x0D, 0x71, 0xED, 0x70, 0x70, 0x18, 0x0F,
	0x71, 0xEF, 0x70, 0x70},12);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x63, (uint8_t[]) {0x18, 0x11, 0x71, 0xF1, 0x70, 0x70, 0x18, 0x13,
	0x71, 0xF3, 0x70, 0x70},12);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x64, (uint8_t[]) {0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07},7);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x66, (uint8_t[]) {0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00,
	0x00, 0x00},10);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x67, (uint8_t[]) {0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10,
	0x32, 0x98},10);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x74, (uint8_t[]) {0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00},
	7);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x98, (uint8_t[]) {0x3e, 0x07}, 2);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x35, NULL,0);
	esp_lcd_panel_io_tx_param(lcd_io_handle,0x21, NULL,0);

	esp_lcd_panel_io_tx_param(lcd_io_handle,0x11, NULL,0);
	vTaskDelay(pdMS_TO_TICKS(120));
	esp_lcd_panel_io_tx_param(lcd_io_handle,0x29, NULL,0);
	vTaskDelay(pdMS_TO_TICKS(20));

    return ESP_OK;
}

/** GC9A01写入显示数据
 * @param x1,x2,y1,y2:显示区域（原始坐标，未含偏移）
 * @param data: 显示数据缓冲区（16位RGB565格式）
 * @return 无
*/
void gc9a01_flush(int x1,int x2,int y1,int y2,void *data)
{
	// 过滤无效区域
	if(x2 <= x1 || y2 <= y1)
	{
		if(s_flush_done_cb)
			s_flush_done_cb(NULL);
		return;
	}

	// 关键：添加GC9A01坐标偏移（屏幕物理区域与控制器内存区域不一致）
	int gc_x1 = x1 + GC9A01_X_OFFSET;
	int gc_x2 = x2 + GC9A01_X_OFFSET;
	int gc_y1 = y1 + GC9A01_Y_OFFSET;
	int gc_y2 = y2 + GC9A01_Y_OFFSET;

	// 发送列地址设置指令（CASET）
	esp_lcd_panel_io_tx_param(lcd_io_handle, GC9A01_CMD_CASET, (uint8_t[]) {
		(gc_x1 >> 8) & 0xFF,
		gc_x1 & 0xFF,
		((gc_x2 - 1) >> 8) & 0xFF,
		(gc_x2 - 1) & 0xFF,
	}, 4);

	// 发送行地址设置指令（RASET）
	esp_lcd_panel_io_tx_param(lcd_io_handle, GC9A01_CMD_RASET, (uint8_t[]) {
		(gc_y1 >> 8) & 0xFF,
		gc_y1 & 0xFF,
		((gc_y2 - 1) >> 8) & 0xFF,
		(gc_y2 - 1) & 0xFF,
	}, 4);

	// 批量传输显示数据（保持DMA高速传输，数据格式与ST7789一致为RGB565）
	size_t len = (x2 - x1) * (y2 - y1) * 2;  // 总字节数=像素数×2（16位色）
	esp_lcd_panel_io_tx_color(lcd_io_handle, GC9A01_CMD_RAMWR, data, len);

	return ;
}
