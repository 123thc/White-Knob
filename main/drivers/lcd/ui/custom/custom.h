/*
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

typedef enum {
    mov_left,
    mov_right,
}mov_dir;

#define BUFFER_NUM      6

//选中app的位置
#define POSx_SELECTED   30
#define POSy_SELECTED   80

//底部app的位置
#define POSx_BOTTOM     95
#define POSy_BOTTOM     185

//顶部app的位置
#define POSx_TOP        95
#define POSy_TOP        -50

//间隔
#define GAP             105

#define ANIMATION_DURATION 300

//屏幕显示大小
#define DIS_WIDTH   240
#define DIS_HEIGHT  240

//图片控件大小
#define IMG_WIDTH   50
#define IMG_HEIGHT  50

//label_induction的进出x坐标
#define POSx_IN_INDUCTION   -105
#define POSx_OUT_INDUCTION   7

typedef enum {
    MAX_TOP = -2,
    TOP = -1,
    SELECTED = 0,
    BOTTOM = 1,
    BOTTOM_ER = 2,
    MAX_BOTTOM = 3
}obj_pos;

typedef struct {
    lv_obj_t *obj;
    int16_t x;
    int16_t y;
}app_obj_t;

typedef struct {
    lv_obj_t *scr;
    bool del;
}screen_info;

typedef enum {
    FANQIESHIZHONG,//番茄时钟
    SHIJIAN,//时间
    SHEZHI,//设置
    XUANNIU,//旋钮
    YINLIANG,//音量
    SURFACEDAIL,//surface—dail
    APP_NUM,//应用数量
}app_index;

typedef struct {
    const lv_img_dsc_t *img_dsc;
    const char *name;//名字
    const char *usage;//功能描述
} app_info_t;


extern app_info_t apps_info[APP_NUM];
extern app_index index_selected;
extern app_obj_t apps_obj[APP_NUM];
extern screen_info my_scerrns[APP_NUM];
extern void (*const my_screen_setup[])(lv_ui *ui);

void custom_init(lv_ui *ui);
void init_menu(void);
void scroll_menu(int16_t step);

#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
