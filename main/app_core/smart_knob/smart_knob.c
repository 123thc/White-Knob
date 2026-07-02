//
// Created by k0921 on 2026/3/28.
//

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gptimer.h"
#include "smart_knob.h"

#include <lcd/ui_task.h>
#include <led/led_decoration.h>

#include "encoder/as5047p.h"
#include "motor/svpwm.h"
#include "motor/motor_control.h"
#include "smart_knob/motor_app.h"

#define TAG "smart_knob_task"

static SemaphoreHandle_t run_sem = NULL;
SemaphoreHandle_t press_sem = NULL;
QueueHandle_t angle_queue = NULL;
static float angle_value = 0;

// 定时器中断回调函数（在ISR上下文中执行，必须短小精悍）
static bool IRAM_ATTR timer_alarm_cb(gptimer_handle_t timer,
                                     const gptimer_alarm_event_data_t *edata,
                                     void *user_ctx)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(run_sem, &xHigherPriorityTaskWoken);
    return xHigherPriorityTaskWoken == pdTRUE;
}

void gptimer_init(void) {
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,   // 选择默认时钟源
        .direction = GPTIMER_COUNT_UP,        // 向上计数
        .resolution_hz = 1 * 10 * 1000,     // 10kHz
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    // 3. 配置报警动作：每1,000,000个tick（即1秒）触发一次，并自动重载
    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,                    // 触发后计数器重置为0
        .alarm_count = 10,               // 报警阈值：2毫秒
        .flags.auto_reload_on_alarm = true,   // 启用自动重载，实现周期性
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

    // 4. 注册中断回调函数
    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_alarm_cb,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));

    // 5. 使能定时器并启动
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));

    // ESP_LOGI(TAG, "定时器已启动");
}



void smart_knob_task(void *pvParameters) {
    //创建用于传输磁编码器角度值的队列
    angle_queue = xQueueCreate(1, sizeof(angle_value));
    spi_as5047p_init(10000);
    motor_init(as5047p_to_angle());
    run_sem = xSemaphoreCreateBinary();
    press_sem = xSemaphoreCreateBinary();
    gptimer_init();

    //记录旋转停止时的角度
    static float last_angle_value = 0;

    // ESP_LOGI(TAG, "电机初始化完成");
    while (1) {
        if (pdTRUE == xSemaphoreTake(run_sem, portMAX_DELAY)) {
            angle_value = as5047p_to_angle();
            xQueueOverwrite(angle_queue, &angle_value);

            //触发震动反馈
            if (pdTRUE == xSemaphoreTake(press_sem, 0)) {
                g_haptic_state.mode = MODE_VIBRATE;
            }

            // //旋转停止，一直使用停止前的角度，防止静态抖动
            // if (pdTRUE == xSemaphoreTake(rotate_stop_sem, 0)) {
            //     angle_value = last_angle_value;
            // }
            // //旋转时记录，不旋转的时候不记录
            // last_angle_value = angle_value;

            KnobHaptics_Update(angle_value);
        }
    }
}