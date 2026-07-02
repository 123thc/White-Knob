//
// Created by k0921 on 2026/3/28.
//

#ifndef WHITE_KNOB_AS5047P_H
#define WHITE_KNOB_AS5047P_H

#include <stdint.h>

#define AS5047P_DMA_CHANNAL 2

/*内容随外界变化的寄存器 可读不可写*/
#define NOP_ADDR      0x0000 //启动读取过程寄存器地址
#define ERRFL_ADDR    0x0001 //错误寄存器地址
#define PROG_ADDR     0x0003 //编程寄存器地址
#define DIAAGC_ADDR   0x3FFC //诊断和AGC寄存器地址
#define MAG_ADDR      0x3FFD //CORDIC寄存器地址
#define ANGLEUNC_ADDR 0x3FFE //无动态角度误差补偿的测量角度寄存器地址
#define ANGLECOM_ADDR 0x3FFF //带动态角度误差补偿的测量角度寄存器地址

#define SETTINGS1_ADDR 0x0018

/*配置选项寄存器 可读可写*/
#define ZPOSM         0x0016
#define ZPOSL         0x0017
#define SETTINGS1     0x0018
#define SETTINGS2     0x0019


void spi_as5047p_init(uint32_t clk_speed);
float as5047p_to_angle(void);

#endif //WHITE_KNOB_AS5047P_H