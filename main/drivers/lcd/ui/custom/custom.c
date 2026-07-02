/*
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
#include "gui_guider.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */

screen_info my_scerrns[APP_NUM];

app_info_t apps_info[APP_NUM] = {
    {&_potato_alpha_50x50, "番茄时钟",   "Focus timer\nwith breaks"},
    {&_shijian_alpha_50x50,  "时间",     "Display current\ntime"},
    {&_wifi_alpha_50x50,   "配网",     "Usage Wifi\nSettings"},
    {&_percent_alpha_50x50,  "旋钮",     "Virtual rotary\nknob"},
    {&_light_alpha_50x50, "灯光",     "Adjust light\nstatus"},
    {&_dial_alpha_50x50,     "surfacedial", "Surface Dial\ncontroller"},
};

void (*const my_screen_setup[])(lv_ui *ui) = {
    setup_scr_screen_potatotime,
    setup_scr_screen_time,
    setup_scr_screen_wifi,
    setup_scr_screen_percent,
    setup_scr_screen_light,
    setup_scr_screen_dial,
};

app_obj_t apps_obj[APP_NUM];
obj_pos offset[APP_NUM] = {MAX_TOP, TOP, SELECTED, BOTTOM, BOTTOM_ER, MAX_BOTTOM};
app_index index_selected = 0;

/**
 * 菜单界面初始化
 */
void init_menu(void) {
    //清除屏幕的可滚动标志
    lv_obj_clear_flag(guider_ui.screen_menu, LV_OBJ_FLAG_SCROLLABLE);

    //初始化图片控件
    index_selected = SHEZHI;
    apps_obj[SHEZHI].obj = guider_ui.screen_menu_img_selected;
    apps_obj[XUANNIU].obj = guider_ui.screen_menu_img_bottom;
    for (int i = 0; i < APP_NUM; i++) {
        if (i != SHEZHI && i != XUANNIU) {
            apps_obj[i].obj = lv_img_create(guider_ui.screen_menu);
            lv_img_set_src(apps_obj[i].obj, apps_info[i].img_dsc);
        }
    }

    //设置控件初始位置
    for (int i = 0; i < APP_NUM; i++) {
        lv_obj_clear_flag(apps_obj[i].obj, LV_OBJ_FLAG_SCROLLABLE);
        //计算y坐标
        apps_obj[i].y = (DIS_HEIGHT / 2 - 40) + offset[i] * GAP;

        //计算x坐标
        if (i == index_selected)
            apps_obj[i].x = POSx_SELECTED;
        else
            apps_obj[i].x = (DIS_WIDTH - IMG_WIDTH) / 2;

        //设置应用位置
        lv_obj_set_pos(apps_obj[i].obj, apps_obj[i].x, apps_obj[i].y);
    }
}


void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    //初始化屏幕数组
    my_scerrns[SHEZHI].scr = guider_ui.screen_wifi;
    my_scerrns[SHIJIAN].scr = guider_ui.screen_time;
    my_scerrns[XUANNIU].scr = guider_ui.screen_percent;
    my_scerrns[SURFACEDAIL].scr = guider_ui.screen_dial;
    my_scerrns[FANQIESHIZHONG].scr = guider_ui.screen_potatotime;
    my_scerrns[YINLIANG].scr = guider_ui.screen_light;

    my_scerrns[SHEZHI].del = guider_ui.screen_wifi_del;
    my_scerrns[SHIJIAN].del = guider_ui.screen_time_del;
    my_scerrns[XUANNIU].del = guider_ui.screen_percent_del;
    my_scerrns[SURFACEDAIL].del = guider_ui.screen_dial_del;
    my_scerrns[FANQIESHIZHONG].del = guider_ui.screen_potatotime_del;
    my_scerrns[YINLIANG].del = guider_ui.screen_light_del;

}

