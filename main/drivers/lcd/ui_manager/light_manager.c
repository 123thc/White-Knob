//
// Created by k0921 on 2026/4/23.
//

#include "light_manager.h"
#include "esp_log.h"
#include "gui_guider.h"
#include "config/ui_manager.h"
#include "led/led_decoration.h"
#include "led/light_custom.h"

#define TAG "light_manager"

#define STEP_THRESHOLD 5.0f

//保存灯的状态
static bool light_status = true;
static uint8_t light_saved_rgb[3] = {46, 197, 206};

static void light_save_current_rgb(void)
{
    light_saved_rgb[R_COLOR] = g_ws2812_control.rgb_color[R_COLOR];
    light_saved_rgb[G_COLOR] = g_ws2812_control.rgb_color[G_COLOR];
    light_saved_rgb[B_COLOR] = g_ws2812_control.rgb_color[B_COLOR];
}

static void light_restore_saved_rgb(void)
{
    g_ws2812_control.rgb_color[R_COLOR] = light_saved_rgb[R_COLOR];
    g_ws2812_control.rgb_color[G_COLOR] = light_saved_rgb[G_COLOR];
    g_ws2812_control.rgb_color[B_COLOR] = light_saved_rgb[B_COLOR];
}

static void light_sync_saved_rgb_from_config(void)
{
    light_custom_config_t cfg;
    if (light_custom_load(&cfg) == ESP_OK) {
        light_saved_rgb[R_COLOR] = cfg.rgb_color[R_COLOR];
        light_saved_rgb[G_COLOR] = cfg.rgb_color[G_COLOR];
        light_saved_rgb[B_COLOR] = cfg.rgb_color[B_COLOR];
    }
    else if (g_ws2812_control.on_off) {
        light_save_current_rgb();
    }
}

static uint8_t light_mode_to_index(led_control_mode mode)
{
    switch (mode) {
        case MODE_SOLID_COLOR:
            return 0;
        case MODE_GRADIENT:
            return 1;
        case MODE_BREATHING:
            return 2;
        case MODE_COMET:
            return 3;
        case MODE_RAINBOW:
            return 4;
        default:
            return 0;
    }
}

/**
 * 进入灯控界面前的回调函数
 */
void light_enter(void) {
    //加载灯控界面
    if(guider_ui.screen_light_del) {
        setup_scr_screen_light(&guider_ui);
        guider_ui.screen_light_del = false;
    }
    lv_scr_load(guider_ui.screen_light);
    light_status = g_ws2812_control.on_off;
    light_sync_saved_rgb_from_config();

    if (light_status) {
        lv_obj_add_state(guider_ui.screen_light_sw_1, LV_STATE_CHECKED);
        lv_img_set_src(guider_ui.screen_light_img_off, &_light_on_alpha_100x100);
    }
    else {
        lv_obj_clear_state(guider_ui.screen_light_sw_1, LV_STATE_CHECKED);
        lv_img_set_src(guider_ui.screen_light_img_off, &_light_off_alpha_100x100);
    }
}

void light_short_press(void) {
    if (g_ws2812_control.on_off == true) {
        light_save_current_rgb();
        g_ws2812_control.last_mode = g_ws2812_control.mode;
        g_ws2812_control.mode = MODE_SOLID_COLOR;
        g_ws2812_control.rgb_color[R_COLOR] = 250;
        g_ws2812_control.rgb_color[G_COLOR] = 250;
        g_ws2812_control.rgb_color[B_COLOR] = 250;
    }
}

void light_release_press(void) {
    light_status = g_ws2812_control.on_off;
    if (!light_status) {
        return;
    }

    //在开关状态为开的时候才允许切换模式
    uint8_t mode = light_mode_to_index(g_ws2812_control.last_mode);

    mode = (mode + 1) % 5;

    switch (mode) {
        case 0:
            g_ws2812_control.mode = MODE_SOLID_COLOR;
            light_restore_saved_rgb();
            break;
        case 1:
            g_ws2812_control.mode = MODE_GRADIENT;
            light_restore_saved_rgb();
            break;
        case 2:
            g_ws2812_control.mode = MODE_BREATHING;
            light_restore_saved_rgb();
            break;
        case 3:
            g_ws2812_control.mode = MODE_COMET;
            light_restore_saved_rgb();
            break;
        case 4:
            g_ws2812_control.mode = MODE_RAINBOW;
            light_restore_saved_rgb();
            break;
        default:break;
    }

    g_ws2812_control.last_mode = g_ws2812_control.mode;
}


/**
 * 退出灯控界面的操作
 */
void light_exit(void) {
    //删除灯控界面的屏幕
    if (guider_ui.screen_light) {
        lv_obj_del(guider_ui.screen_light);
        guider_ui.screen_light_del = true;
        guider_ui.screen_light = NULL;
    }
}


/**
 * 灯控旋转操作
 * @param delta 旋转角度差值
 * @param angle 当前角度
 */
void light_rotate(float *delta, const float *angle) {
    //开
    if (*delta < 0) {
        light_status = true;
        g_ws2812_control.on_off = true;
        lv_obj_add_state(guider_ui.screen_light_sw_1, LV_STATE_CHECKED);
        lv_img_set_src(guider_ui.screen_light_img_off, &_light_on_alpha_100x100);

        g_ws2812_control.mode = g_ws2812_control.last_mode;
        light_restore_saved_rgb();
    }
    //关
    else if (*delta > 0) {
        light_status = false;
        light_save_current_rgb();
        g_ws2812_control.on_off = false;
        lv_obj_clear_state(guider_ui.screen_light_sw_1, LV_STATE_CHECKED);
        lv_img_set_src(guider_ui.screen_light_img_off, &_light_off_alpha_100x100);

        g_ws2812_control.mode = MODE_SOLID_COLOR;
        g_ws2812_control.rgb_color[R_COLOR] = 0;
        g_ws2812_control.rgb_color[G_COLOR] = 0;
        g_ws2812_control.rgb_color[B_COLOR] = 0;
    }

    int steps = (int)(*delta / STEP_THRESHOLD);

    if (steps != 0) {
        *delta -= steps * STEP_THRESHOLD; // 减去已处理的部分
    }
}

/**
 * 长按触发的操作
 */
void light_long_press(void) {
    if (g_ws2812_control.on_off == true) {
        //返回上一状态
        g_ws2812_control.mode = g_ws2812_control.last_mode;
        light_restore_saved_rgb();
    }
    //返回菜单界面
    screen_manager_set_active(&menu_interface);
}

/**
 * 注册灯控界面
 */
void light_register(void) {
    light_interface.enter = light_enter;
    light_interface.exit = light_exit;
    light_interface.rotate = light_rotate;
    light_interface.rotate_stop = NULL;
    light_interface.short_press = light_short_press;
    light_interface.release_press = light_release_press;
    light_interface.long_press = light_long_press;
    light_interface.is_registered = true;
}
