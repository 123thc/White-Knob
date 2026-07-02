//
// Created by k0921 on 2026/4/11.
//

#include "adsorb.h"
#include "lcd/custom_operation/scroll.h"
#include "custom.h"
#include "gui_guider.h"
#include "esp_log.h"
#include "lcd/ui_manager/menu_manager.h"

/**
 * 界面吸附效果
 */
void interfacial_adsorption(void) {
    encoder_active = false;
    int16_t t_offset_x = 0, t_offset_y = 0;

    for (int i = 0; i < APP_NUM; i++) {
        //使用第一个坐标大于0的对象的坐标对后续进行补偿
        const uint8_t first_obj_pos = 80;

        //找到当前需要偏移到上一个图标位置还是下一个图标位置,产生吸附效果
        if (apps_obj[i].y > first_obj_pos) {//找到大于第一个大于0的对象的坐标的坐标进行求余
            //回到上一位置
            if ((apps_obj[i].y - first_obj_pos) % GAP < (GAP + IMG_HEIGHT) / 2) {
                t_offset_y = -(apps_obj[i].y - first_obj_pos) % GAP;
                // t_offset_x = -(apps_obj[i].x - 30) % 95;
            }
            //前往下一位置
            else {
                t_offset_y = GAP - (apps_obj[i].y - first_obj_pos) % GAP;
                // t_offset_x = 95 - (apps_obj[i].x - 30) % 95;
            }
            break;//找到偏移量即可结束循环
        }
    }

    for (int i = 0; i < APP_NUM; i++) {
        ui_animation(apps_obj[i].obj,
                        t_offset_y > 0 ? 300 + t_offset_y * 5 : 300 - t_offset_y * 5,
                        0,
                        apps_obj[i].y, apps_obj[i].y + t_offset_y,
                        lv_anim_path_ease_out,
                        0, 0, 0, 0,
                        (lv_anim_exec_xcb_t)lv_obj_set_y,
                        NULL, NULL, NULL);

        //更新当前y坐标
        apps_obj[i].y = apps_obj[i].y + t_offset_y;

        if (apps_obj[i].y == POSy_SELECTED) {
            index_selected = i;

            ESP_LOGI("adsorb", "index_selected=%d", i);
            t_offset_x = -(apps_obj[i].x - 30) % 95;
            ui_animation(apps_obj[i].obj,
                        t_offset_x > 0 ? 300 + t_offset_x * 5 : 300 - t_offset_x * 5,
                        0,
                        apps_obj[i].x, apps_obj[i].x + t_offset_x,
                        lv_anim_path_ease_out,
                        0, 0, 0, 0,
                        (lv_anim_exec_xcb_t)lv_obj_set_x,
                        NULL, NULL, NULL);
        }
        else {
            t_offset_x = 95 - (apps_obj[i].x - 30) % 95;
            ui_animation(apps_obj[i].obj,
                        t_offset_x > 0 ? 300 + t_offset_x * 5 : 300 - t_offset_x * 5,
                        0,
                        apps_obj[i].x, (apps_obj[i].x + t_offset_x) > 95 ? 95 : (apps_obj[i].x + t_offset_x),
                        lv_anim_path_ease_out,
                        0, 0, 0, 0,
                        (lv_anim_exec_xcb_t)lv_obj_set_x,
                        NULL, NULL, NULL);
        }
        //更新x坐标
        apps_obj[i].x = (apps_obj[i].x + t_offset_x) > 95 ? 95 : (apps_obj[i].x + t_offset_x);
    }

    move_labels(1, 1);
}