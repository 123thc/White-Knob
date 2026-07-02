//
// Created by k0921 on 2026/4/23.
//

#ifndef WHITE_KNOB_UI_MANAGER_H
#define WHITE_KNOB_UI_MANAGER_H
#include <stdbool.h>

/* 回调函数类型定义 */
typedef void (*screen_enter_cb_t)(void);
typedef void (*screen_exit_cb_t)(void);
typedef void (*screen_rotate_cb_t)(float *delta, const float *angle);      // 旋转过程中持续触发
typedef void (*screen_rotate_stop_cb_t)(void);        // 旋转停止时触发
typedef void (*screen_release_press_cb_t)(void);
typedef void (*screen_short_press_cb_t)(void);
typedef void (*screen_long_press_cb_t)(void);

/* 屏幕交互接口结构体 */
typedef struct {
    screen_enter_cb_t           enter;           // 进入屏幕时调用
    screen_exit_cb_t            exit;            // 离开屏幕时调用
    screen_rotate_cb_t          rotate;          // 旋转编码器时调用
    screen_rotate_stop_cb_t     rotate_stop;     // 旋转停止时调用（可选）
    screen_release_press_cb_t   release_press;   // 释放（抬起）时调用
    screen_short_press_cb_t     short_press;     // 短按时调用
    screen_long_press_cb_t      long_press;      // 长按时调用
    bool is_registered; //是否已经注册
} screen_interface_t;

//全局接口
extern screen_interface_t *s_active_interface;

//菜单界面接口
extern screen_interface_t menu_interface;
//时间界面接口
extern screen_interface_t time_interface;
extern screen_interface_t time_2_interface;
//SurfaceDial接口
extern screen_interface_t dial_interface;
//灯光接口
extern screen_interface_t light_interface;
//旋钮接口
extern screen_interface_t percent_interface;
//番茄时钟接口
extern screen_interface_t potatotime_interface;
//设置时间接口
extern screen_interface_t set_time_interface;
//wifi接口
extern screen_interface_t wifi_interface;

void screen_manager_set_active(const screen_interface_t *interface);
const screen_interface_t *screen_manager_get_active(void);
void ui_manager_init(void);

#endif //WHITE_KNOB_UI_MANAGER_H