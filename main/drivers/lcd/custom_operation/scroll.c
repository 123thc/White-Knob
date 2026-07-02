//
// Created by k0921 on 2026/4/11.
//

#include "scroll.h"
#include "gui_guider.h"
#include "custom.h"
#include <stdlib.h>

//函数声明
void move_labels(uint8_t dir, bool back);

bool encoder_active  = false;

void encoder_scroll_move(int16_t delta_pixels) {
    if (delta_pixels == 0 || lv_scr_act() != guider_ui.screen_menu) return;

    //若当前不处于滚动状态，则删除相关动画
    if (encoder_active == false) {
        for (int i = 0; i < APP_NUM; i++) {
            lv_anim_del(apps_obj[i].obj, NULL);
        }

        if (delta_pixels > 0)
            move_labels(0, false);
        else
            move_labels(1, false);

        encoder_active = true;
    }
    // else {
    //     // 如果已经处于触摸状态，获取当前点击点坐标
    //     lv_indev_get_point(lv_indev_get_act(), &click_point_now);
    // }

    int16_t t_offset_x = 10;

    for (int i = 0; i < APP_NUM; i++) {
        //更新图标的y轴坐标
        apps_obj[i].y = apps_obj[i].y + delta_pixels;

        //处理边界问题，实现循环滚动
        if (apps_obj[i].y < (DIS_HEIGHT / 2 - 40) + MAX_TOP * GAP) {
            apps_obj[i].y += APP_NUM * GAP;
        }
        else if (apps_obj[i].y > (DIS_HEIGHT / 2 - 40) + MAX_BOTTOM * GAP) {
            apps_obj[i].y -= APP_NUM * GAP;
        }

        //滚动过程中，让当前选中的应用归到其它应用的x轴一起滚动
        if (apps_obj[i].x <= 95) {
            apps_obj[i].x += t_offset_x;
            if (apps_obj[i].x > 95)
                apps_obj[i].x = 95;
            lv_obj_set_x(apps_obj[i].obj, apps_obj[i].x);
        }

        //设置y轴坐标
        lv_obj_set_y(apps_obj[i].obj, apps_obj[i].y);
    }
}



void move_labels(uint8_t dir, bool back) {
    //返回到各自的位置
    if (back) {
        //========================================设置相关标签向容器下移动进行隐藏========================================
        //设置选中应用的标签
        lv_label_set_text(guider_ui.screen_menu_label_selected, apps_info[index_selected].name);

        ui_animation(guider_ui.screen_menu_label_selected,
                ANIMATION_DURATION,
                0,
                -lv_obj_get_y(guider_ui.screen_menu_label_selected), 0,
                lv_anim_path_ease_out,
                0, 0, 0, 0,
                (lv_anim_exec_xcb_t)lv_obj_set_y,
                NULL, NULL, NULL);

        //设置顶部位置应用的标签
        lv_label_set_text(guider_ui.screen_menu_label_top, apps_info[(index_selected - 1) % APP_NUM].name);

        ui_animation(guider_ui.screen_menu_label_top,
                ANIMATION_DURATION,
                0,
                -lv_obj_get_y(guider_ui.screen_menu_label_top), 0,
                lv_anim_path_ease_out,
                0, 0, 0, 0,
                (lv_anim_exec_xcb_t)lv_obj_set_y,
                NULL, NULL, NULL);
        //========================================设置相关标签向容器下移动进行隐藏END========================================

        //===================================设置功能标签===================================
        lv_label_set_text(guider_ui.screen_menu_label_induction, apps_info[index_selected].usage);

        ui_animation(guider_ui.screen_menu_cont_induction_main,
            ANIMATION_DURATION + 150,
            0,
            lv_obj_get_x(guider_ui.screen_menu_cont_induction_main), 100,
            lv_anim_path_ease_out,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)lv_obj_set_x,
            NULL, NULL, NULL);
        ui_animation(guider_ui.screen_menu_cont_induction_main,
            ANIMATION_DURATION,
            0,
            lv_obj_get_y(guider_ui.screen_menu_cont_induction_main), 68,
            lv_anim_path_ease_out,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)lv_obj_set_y,
            NULL, NULL, NULL);
        ui_animation(guider_ui.screen_menu_cont_move_obj,
                    ANIMATION_DURATION,
                    0,
                    lv_obj_get_x(guider_ui.screen_menu_cont_move_obj), -140,
                    lv_anim_path_ease_out,
                    0, 0, 0, 0,
                    (lv_anim_exec_xcb_t)lv_obj_set_x,
                    NULL, NULL, NULL);
        //===================================设置功能标签END===================================
    }
    //离开各自的位置
    else {
        //向下滚动
        if (dir) {
            //========================================设置相关标签向容器下移动进行隐藏========================================
            //设置选中应用的标签
            ui_animation(guider_ui.screen_menu_label_selected,
                    ANIMATION_DURATION,
                    0,
                    lv_obj_get_y(guider_ui.screen_menu_label_selected), -19,
                    lv_anim_path_ease_out,
                    0, 0, 0, 0,
                    (lv_anim_exec_xcb_t)lv_obj_set_y,
                    NULL, NULL, NULL);

            //设置顶部位置应用的标签
            ui_animation(guider_ui.screen_menu_label_top,
                    ANIMATION_DURATION,
                    0,
                    lv_obj_get_y(guider_ui.screen_menu_label_top), -19,
                    lv_anim_path_ease_out,
                    0, 0, 0, 0,
                    (lv_anim_exec_xcb_t)lv_obj_set_y,
                    NULL, NULL, NULL);
            //========================================设置相关标签向容器下移动进行隐藏END========================================

            //===================================设置功能标签===================================
            ui_animation(guider_ui.screen_menu_cont_induction_main,
                ANIMATION_DURATION,
                0,
                lv_obj_get_x(guider_ui.screen_menu_cont_induction_main), DIS_WIDTH,
                lv_anim_path_ease_out,
                0, 0, 0, 0,
                (lv_anim_exec_xcb_t)lv_obj_set_x,
                NULL, NULL, NULL);
            ui_animation(guider_ui.screen_menu_cont_induction_main,
                ANIMATION_DURATION,
                0,
                lv_obj_get_y(guider_ui.screen_menu_cont_induction_main), -33,
                lv_anim_path_ease_out,
                0, 0, 0, 0,
                (lv_anim_exec_xcb_t)lv_obj_set_y,
                NULL, NULL, NULL);

            ui_animation(guider_ui.screen_menu_cont_move_obj,
                        ANIMATION_DURATION,
                        0,
                        lv_obj_get_x(guider_ui.screen_menu_cont_move_obj), -33,
                        lv_anim_path_ease_out,
                        0, 0, 0, 0,
                        (lv_anim_exec_xcb_t)lv_obj_set_x,
                        NULL, NULL, NULL);
            //===================================设置功能标签END===================================
        }
        //向上移动
        else {
            //========================================设置相关标签向容器下移动进行隐藏========================================
            //设置选中应用的标签
            ui_animation(guider_ui.screen_menu_label_selected,
                    ANIMATION_DURATION,
                    0,
                    lv_obj_get_y(guider_ui.screen_menu_label_selected), 19,
                    lv_anim_path_ease_out,
                    0, 0, 0, 0,
                    (lv_anim_exec_xcb_t)lv_obj_set_y,
                    NULL, NULL, NULL);

            //设置顶部位置应用的标签
            ui_animation(guider_ui.screen_menu_label_top,
                    ANIMATION_DURATION,
                    0,
                    lv_obj_get_y(guider_ui.screen_menu_label_top), 19,
                    lv_anim_path_ease_out,
                    0, 0, 0, 0,
                    (lv_anim_exec_xcb_t)lv_obj_set_y,
                    NULL, NULL, NULL);
            //========================================设置相关标签向容器下移动进行隐藏END========================================

            //===================================设置功能标签===================================
            ui_animation(guider_ui.screen_menu_cont_induction_main,
                ANIMATION_DURATION,
                0,
                lv_obj_get_x(guider_ui.screen_menu_cont_induction_main), DIS_WIDTH,
                lv_anim_path_ease_out,
                0, 0, 0, 0,
                (lv_anim_exec_xcb_t)lv_obj_set_x,
                NULL, NULL, NULL);
            ui_animation(guider_ui.screen_menu_cont_induction_main,
                ANIMATION_DURATION,
                0,
                lv_obj_get_y(guider_ui.screen_menu_cont_induction_main), 165,
                lv_anim_path_ease_out,
                0, 0, 0, 0,
                (lv_anim_exec_xcb_t)lv_obj_set_y,
                NULL, NULL, NULL);

            ui_animation(guider_ui.screen_menu_cont_move_obj,
                        ANIMATION_DURATION,
                        0,
                        lv_obj_get_x(guider_ui.screen_menu_cont_move_obj), -33,
                        lv_anim_path_ease_out,
                        0, 0, 0, 0,
                        (lv_anim_exec_xcb_t)lv_obj_set_x,
                        NULL, NULL, NULL);
            //===================================设置功能标签END===================================
        }
    }
}