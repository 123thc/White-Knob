//
// Surface Dial (Radial Controller) HID extensions.
//
// 这些定义原本由作者直接加在 TinyUSB 的 class/hid/hid.h 和 hid_device.h 里。
// 由于 managed_components/ 会被 ESP-IDF 组件管理器自动重新下载并覆盖，那些改动会丢失，
// 所以把它们放到项目自己的头文件中，避免再次被覆盖。
//
// 报文布局（与 ble_hid_surfacedial_report* 发送的 2 字节一致）：
//   bit0        : 按钮（Press/Release）
//   bit1..bit15 : 旋转增量（有符号，相对值，逻辑范围 ±3600 = ±360.0°，分辨率 0.1°）
//
// 描述符采用微软 Radial Controller(Surface Dial) 标准格式：
//   Generic Desktop / System Multi-Axis Controller -> Digitizer / Puck
//   -> Button(1) + Dial(15bit, Relative)
//

#ifndef WHITE_KNOB_SURFACE_DIAL_HID_H
#define WHITE_KNOB_SURFACE_DIAL_HID_H

#include "class/hid/hid.h"
#include "class/hid/hid_device.h"

// --- Report ID（同时作为 hid_dev_send_report 的报告标识）---------------------
// 与 hidd_le_prf_int.h 中 HID_RPT_ID_CC_IN(3) / HID_RPT_ID_RADIAL_IN(4) 保持一致。
// TinyUSB 的 hid_interface_protocol_enum_t 只有 NONE/KEYBOARD(1)/MOUSE(2)，
// 这里用宏补上 DIAL 和 MEDIA，避免修改第三方枚举。
#define HID_ITF_PROTOCOL_DIAL    3   // 径向旋钮报告
#define HID_ITF_PROTOCOL_MEDIA   4   // Consumer Control 报告

// --- ble_hid_surfacedial_report() 的 keycode 取值 ---------------------------
// keycode < 2 走按钮分支，report[0] 直接取该值（bit0 即按钮状态）。
#define DIAL_RELEASE   0   // 松开
#define DIAL_PRESS     1   // 按下
// DIAL_R/DIAL_L/DIAL_UNIT 仅用于 ble_hid_surfacedial_report() 的旋转分支；
// 当前旋转走的是 ble_hid_surfacedial_report_custom()，这三者不在主路径上，
// 仅为编译保留。数值含义：逻辑单位 0.1°（发送时再 <<1 放入 bit1..15）。
#define DIAL_R         2   // 右旋
#define DIAL_L         3   // 左旋
#define DIAL_UNIT      100 // 单步旋转量（0.1° 单位，dormant）

// --- Surface Dial 报告描述符模板 -------------------------------------------
// 用法与 TinyUSB 的 TUD_HID_REPORT_DESC_MOUSE 一致：可传入 HID_REPORT_ID(x)。
#define TUD_HID_REPORT_DESC_DIAL(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP   )                      ,\
  HID_USAGE      ( 0x0E /* System Multi-Axis Controller */ )       ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION )                    ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    HID_USAGE_PAGE ( HID_USAGE_PAGE_DIGITIZER )                    ,\
    HID_USAGE      ( 0x21 /* Puck */ )                             ,\
    HID_COLLECTION ( HID_COLLECTION_PHYSICAL )                     ,\
      /* 按钮：1 bit */ \
      HID_USAGE_PAGE  ( HID_USAGE_PAGE_BUTTON )                    ,\
      HID_USAGE       ( 1 )                                        ,\
      HID_REPORT_COUNT( 1 )                                        ,\
      HID_REPORT_SIZE ( 1 )                                        ,\
      HID_LOGICAL_MIN ( 0 )                                        ,\
      HID_LOGICAL_MAX ( 1 )                                        ,\
      HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE )   ,\
      /* 旋转：15 bit，相对值，单位 0.1° */ \
      HID_USAGE_PAGE  ( HID_USAGE_PAGE_DESKTOP )                   ,\
      HID_USAGE       ( 0x37 /* Dial */ )                          ,\
      HID_REPORT_COUNT( 1 )                                        ,\
      HID_REPORT_SIZE ( 15 )                                       ,\
      HID_UNIT_EXPONENT( 0x0F /* -1 */ )                           ,\
      HID_UNIT        ( 0x14 /* Degrees, English Rotation */ )     ,\
      HID_PHYSICAL_MIN_N( -3600, 2 )                               ,\
      HID_PHYSICAL_MAX_N(  3600, 2 )                               ,\
      HID_LOGICAL_MIN_N ( -3600, 2 )                               ,\
      HID_LOGICAL_MAX_N (  3600, 2 )                               ,\
      HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_RELATIVE )   ,\
    HID_COLLECTION_END                                            ,\
  HID_COLLECTION_END \

#endif // WHITE_KNOB_SURFACE_DIAL_HID_H
