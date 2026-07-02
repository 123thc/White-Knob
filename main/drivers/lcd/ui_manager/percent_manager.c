//
// Created by k0921 on 2026/4/23.
//

#include "percent_manager.h"

#include <esp_log.h>
#include <math.h>

#include "gui_guider.h"
#include "config/ui_manager.h"
#include "smart_knob/motor_app.h"

static bool is_fist = false;

static uint8_t mode = 0;

static int detent_steps = 0;

static KnobHapticState saved_haptic_state;
static HapticMode saved_vibratic_next_mode = MODE_FREE;
static bool saved_haptic_valid = false;

static uint8_t mode_before_short_press = 0;
static KnobHapticState haptic_state_before_short_press;
static HapticMode vibratic_next_before_short_press = MODE_FREE;
static bool short_press_mode_changed = false;

//在当前界面的全局实时角度
static float angle_now = 0.0f;

lv_obj_t *haptic_label_bottom = NULL;
lv_obj_t *haptic_label_top = NULL;

#define ARC_RANGE   360

//限位范围
#define LimitRange 120

static const char *percent_mode_text(uint8_t value) {
    switch (value) {
        case 1:
            return "Detent Mode";
        case 2:
            return "Damping Mode";
        case 3:
            return "Edge Mode";
        case 0:
        default:
            return "Free Mode";
    }
}

static void percent_build_haptic_state(uint8_t value, KnobHapticState *state, HapticMode *next_mode) {
    *state = g_haptic_state;
    *next_mode = g_vibratic_next_mode;

    if (state->mode == MODE_VIBRATE) {
        state->mode = *next_mode;
    }

    switch (value) {
        case 1:
            state->mode = MODE_DETENT;
            state->params.detent.steps = detent_steps > 0 ? detent_steps : 60;
            state->params.detent.width = 0;
            *next_mode = MODE_DETENT;
            break;
        case 2:
            state->mode = MODE_DAMPING;
            state->params.damping.strength = 3.0f;
            *next_mode = MODE_DAMPING;
            break;
        case 3:
            if (state->mode != MODE_EDGE) {
                state->params.edge.min_angle = angle_now;
                state->params.edge.max_angle = angle_now - LimitRange;
                state->params.edge.side = false;
            }
            state->mode = MODE_EDGE;
            *next_mode = MODE_EDGE;
            break;
        case 0:
        default:
            state->mode = MODE_FREE;
            *next_mode = MODE_FREE;
            break;
    }
}

static void percent_save_haptic_state(void) {
    percent_build_haptic_state(mode, &saved_haptic_state, &saved_vibratic_next_mode);
    saved_haptic_valid = true;
}

static void percent_capture_before_short_press(void) {
    mode_before_short_press = mode;
    percent_build_haptic_state(mode, &haptic_state_before_short_press, &vibratic_next_before_short_press);
    short_press_mode_changed = true;
}

static void percent_restore_before_short_press(void) {
    if (!short_press_mode_changed) {
        return;
    }

    mode = mode_before_short_press;
    saved_haptic_state = haptic_state_before_short_press;
    saved_vibratic_next_mode = vibratic_next_before_short_press;
    saved_haptic_valid = true;

    g_haptic_state = saved_haptic_state;
    g_vibratic_next_mode = saved_vibratic_next_mode;

    if (mode == 1) {
        detent_steps = g_haptic_state.params.detent.steps;
    }

    short_press_mode_changed = false;
}

static void percent_restore_haptic_state(void) {
    if (!saved_haptic_valid) {
        g_haptic_state.mode = MODE_FREE;
        g_vibratic_next_mode = MODE_FREE;
        percent_save_haptic_state();
        return;
    }

    g_haptic_state = saved_haptic_state;
    g_vibratic_next_mode = saved_vibratic_next_mode;

    if (mode == 1) {
        detent_steps = g_haptic_state.params.detent.steps;
    }
}

/**
 * 进入旋钮界面前的回调函数
 */
void percent_enter(void) {
    //加载旋钮界面
    if(guider_ui.screen_percent_del) {
        setup_scr_screen_percent(&guider_ui);
        guider_ui.screen_percent_del = false;
    }
    is_fist = true;

    lv_arc_set_range(guider_ui.screen_percent_arc_1, 0, ARC_RANGE);

    /* 创建模式文字 */
    haptic_label_bottom = lv_label_create(guider_ui.screen_percent);
    lv_obj_move_foreground(guider_ui.screen_percent_cont_1);
    lv_obj_move_foreground(guider_ui.screen_percent_arc_1);
    haptic_label_top = lv_label_create(guider_ui.screen_percent_cont_1);
    lv_obj_set_style_text_color(haptic_label_bottom, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(haptic_label_bottom, &lv_font_AlimamaDaoLiTi_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(haptic_label_top, lv_color_hex(0xff0027), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(haptic_label_top, &lv_font_AlimamaDaoLiTi_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    percent_restore_haptic_state();
    lv_label_set_text(haptic_label_bottom, percent_mode_text(mode));
    lv_label_set_text(haptic_label_top, percent_mode_text(mode));
    lv_obj_align(haptic_label_bottom, LV_ALIGN_CENTER, 0, 30);
    lv_obj_align_to(haptic_label_top, haptic_label_bottom, LV_ALIGN_DEFAULT, 0, 0);

    lv_scr_load(guider_ui.screen_percent);
}

/**
 * 退出旋钮界面的操作
 */
void percent_exit(void) {
    //删除旋钮界面的屏幕
    if (guider_ui.screen_percent) {
        lv_obj_del(guider_ui.screen_percent);
        guider_ui.screen_percent_del = true;
        guider_ui.screen_percent = NULL;
    }

    percent_save_haptic_state();
}


/**
 * 旋钮旋转操作
 * @param delta 旋转角度差值
 * @param angle 当前角度
 */
void percent_rotate(float *delta, const float *angle) {
    //进入旋转状态时的初始偏移角度
    static int16_t angle_offset = 0;
    static float last_pos = 0.0f;
    static int remind_steps = 0;
    if (is_fist) {
        angle_offset = *angle;
        remind_steps = 0;
        if (detent_steps != 0) {
            float spacing = 360.0f / detent_steps;
            last_pos = roundf(*angle / spacing) * spacing;
        }
        is_fist = false;
    }

    lv_obj_t *acr_target = guider_ui.screen_percent_arc_1;

    /* 棘轮模式 */
    if (mode == 1) {
        float spacing = 360.0f / detent_steps;

        // 计算当前卡位中心
        float target_pos = roundf(*angle / spacing) * spacing;

        // 累加卡位变化步数
        int step_change = (int)roundf((target_pos - last_pos) / spacing);
        remind_steps -= step_change;

        // 限幅 0 ~ detent_steps
        if (remind_steps < 0) remind_steps += detent_steps;
        else if (remind_steps > detent_steps) remind_steps -= detent_steps;

        // 转换为百分比 0~100
        int percent = remind_steps * 100 / detent_steps;

        lv_arc_set_value(acr_target, ARC_RANGE * percent / 100.0f);
        lv_label_set_text_fmt(guider_ui.screen_percent_label_2, "%d%%", percent);
        lv_label_set_text_fmt(guider_ui.screen_percent_label_3, "%d%%", percent);

        // 颜色填充高度（基于百分比，容器总高240对应100%）
        lv_obj_set_height(guider_ui.screen_percent_cont_1, percent * 240 / 100.0f);
        lv_obj_align(guider_ui.screen_percent_cont_1, LV_ALIGN_BOTTOM_MID, 0, 0);
        lv_obj_align_to(guider_ui.screen_percent_label_3, guider_ui.screen_percent_label_2, LV_ALIGN_DEFAULT, 0, 0);
        lv_obj_align_to(haptic_label_top, haptic_label_bottom, LV_ALIGN_DEFAULT, 0, 0);

        last_pos = target_pos;  // 更新参考位置
    }
    /* 限位模式 */
    else if (mode == 3) {
        int16_t percent_angle = angle_offset - *angle;
        if (percent_angle < -180) percent_angle += 360;
        else if (percent_angle > 180) percent_angle -= 360;

        if (percent_angle < 0) {
            lv_arc_set_value(acr_target, percent_angle / 360.0f * ARC_RANGE + LimitRange);
            lv_label_set_text_fmt(guider_ui.screen_percent_label_2, "%d%%", (lv_arc_get_value(acr_target) - LimitRange) * 100 / LimitRange);
            lv_obj_set_style_text_color(guider_ui.screen_percent_label_2, lv_color_black(), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xff0000), 0);
            lv_obj_set_style_text_color(haptic_label_bottom, lv_color_black(), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
        else if (percent_angle > LimitRange){
            lv_arc_set_value(acr_target, percent_angle / 360.0f * ARC_RANGE + LimitRange);
            lv_label_set_text_fmt(guider_ui.screen_percent_label_2, "%d%%", (lv_arc_get_value(acr_target) - LimitRange) * 100 / LimitRange);
            lv_obj_set_style_text_color(guider_ui.screen_percent_label_2, lv_color_black(), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xff0000), 0);
            lv_obj_set_style_text_color(haptic_label_bottom, lv_color_black(), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
        else {
            //转换成0~360范围
            if (percent_angle < 0) percent_angle += 360;
            else if (percent_angle > 360) percent_angle -= 360;

            lv_arc_set_value(acr_target, percent_angle / 360.0f * ARC_RANGE + LimitRange);
            lv_label_set_text_fmt(guider_ui.screen_percent_label_2, "%d%%", (lv_arc_get_value(acr_target) - LimitRange) * 100 / LimitRange);
            lv_obj_set_style_text_color(guider_ui.screen_percent_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
            lv_obj_set_style_text_color(haptic_label_bottom, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        }

    }
    else {
        //根据角度方向来调整
        int16_t percent_angle = angle_offset - *angle;
        if (percent_angle > 360)
            percent_angle -= 360;
        else if (percent_angle < 0)
            percent_angle += 360;


        lv_arc_set_value(acr_target, percent_angle / 360.0f * ARC_RANGE);

        lv_label_set_text_fmt(guider_ui.screen_percent_label_2, "%d%%", lv_arc_get_value(acr_target) * 100 / ARC_RANGE );
        lv_label_set_text_fmt(guider_ui.screen_percent_label_3, "%d%%", lv_arc_get_value(acr_target) * 100 / ARC_RANGE);

        //设置颜色填充和标签位置对齐
        lv_obj_set_height(guider_ui.screen_percent_cont_1, lv_arc_get_value(acr_target) * 240 / ARC_RANGE);
        lv_obj_align(guider_ui.screen_percent_cont_1, LV_ALIGN_BOTTOM_MID, 0, 0);  // 重新定位，使底边对齐
        lv_obj_align_to(guider_ui.screen_percent_label_3, guider_ui.screen_percent_label_2, LV_ALIGN_DEFAULT, 0, 0);
        lv_obj_align_to(haptic_label_top, haptic_label_bottom, LV_ALIGN_DEFAULT, 0, 0);
    }
    angle_now = *angle;
}

void percent_short_press(void) {
    //标签及填充的复位
    bool reset = true;

    percent_capture_before_short_press();

    //触感反馈模式更新
    mode++;
    if (mode == 4)
        mode = 0;
    //角度参数重置
    is_fist = true;

    //设置默认背景颜色和字体颜色（在限位模式时会被修改）
    lv_obj_set_style_text_color(guider_ui.screen_percent_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_set_style_text_color(haptic_label_bottom, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);

    if (mode == 0) {
        g_haptic_state.mode = MODE_FREE;
        g_vibratic_next_mode = MODE_FREE;
        lv_label_set_text(haptic_label_bottom, "Free Mode");
        lv_label_set_text(haptic_label_top, "Free Mode");

        reset = true;
    }
    else if (mode == 1) {
        g_haptic_state.mode = MODE_DETENT;
        g_haptic_state.params.detent.steps = 60;
        g_haptic_state.params.detent.width = 0;
        g_vibratic_next_mode = MODE_DETENT;

        detent_steps = g_haptic_state.params.detent.steps;
        lv_label_set_text(haptic_label_bottom, "Detent Mode");
        lv_label_set_text(haptic_label_top, "Detent Mode");

        reset = true;
    }
    else if (mode == 2) {
        g_haptic_state.mode = MODE_DAMPING;
        g_haptic_state.params.damping.strength = 3.0f;
        g_vibratic_next_mode = MODE_DAMPING;

        lv_label_set_text(haptic_label_bottom, "Damping Mode");
        lv_label_set_text(haptic_label_top, "Damping Mode");

        reset = true;
    }
    else if (mode == 3) {
        g_haptic_state.mode = MODE_EDGE;
        g_haptic_state.params.edge.min_angle = angle_now;
        int16_t max_angle = angle_now - LimitRange;
        // if (max_angle > 360)
        //     max_angle -= 360;
        // else if (max_angle < 0)
        //     max_angle += 360;
        g_haptic_state.params.edge.max_angle = max_angle;
        g_haptic_state.params.edge.side = false;
        g_vibratic_next_mode = MODE_EDGE;

        lv_label_set_text(haptic_label_bottom, "Edge Mode");
        lv_label_set_text(haptic_label_top, "Edge Mode");

        lv_arc_set_value(guider_ui.screen_percent_arc_1, 120);

        lv_label_set_text_fmt(guider_ui.screen_percent_label_2, "%d%%", 0);
        lv_label_set_text_fmt(guider_ui.screen_percent_label_3, "%d%%", 0);

        //设置颜色填充和标签位置对齐
        lv_obj_set_height(guider_ui.screen_percent_cont_1, 0);
        lv_obj_align(guider_ui.screen_percent_cont_1, LV_ALIGN_BOTTOM_MID, 0, 0);  // 重新定位，使底边对齐
        lv_obj_align_to(guider_ui.screen_percent_label_3, guider_ui.screen_percent_label_2, LV_ALIGN_DEFAULT, 0, 0);
        lv_obj_align_to(haptic_label_top, haptic_label_bottom, LV_ALIGN_DEFAULT, 0, 0);

        reset = false;
    }

    //是否对标签及填充进行重置
    if (reset) {
        lv_arc_set_value(guider_ui.screen_percent_arc_1, 0);

        lv_label_set_text_fmt(guider_ui.screen_percent_label_2, "%d%%", 0);
        lv_label_set_text_fmt(guider_ui.screen_percent_label_3, "%d%%", 0);

        //设置颜色填充和标签位置对齐
        lv_obj_set_height(guider_ui.screen_percent_cont_1, 0);
        lv_obj_align(guider_ui.screen_percent_cont_1, LV_ALIGN_BOTTOM_MID, 0, 0);  // 重新定位，使底边对齐
        lv_obj_align_to(guider_ui.screen_percent_label_3, guider_ui.screen_percent_label_2, LV_ALIGN_DEFAULT, 0, 0);
        lv_obj_align_to(haptic_label_top, haptic_label_bottom, LV_ALIGN_DEFAULT, 0, 0);
    }

    percent_save_haptic_state();
}

/**
 * 长按触发的操作
 */
void percent_release_press(void) {
    short_press_mode_changed = false;
}

void percent_long_press(void) {
    percent_restore_before_short_press();
    //返回菜单界面
    screen_manager_set_active(&menu_interface);
}

/**
 * 注册旋钮界面
 */
void percent_register(void) {
    percent_interface.enter = percent_enter;
    percent_interface.exit = percent_exit;
    percent_interface.rotate = percent_rotate;
    percent_interface.rotate_stop = NULL;
    percent_interface.short_press = percent_short_press;
    percent_interface.release_press = percent_release_press;
    percent_interface.long_press = percent_long_press;
    percent_interface.is_registered = true;
}
