//
// Created by k0921 on 2026/4/28.
//

#ifndef WHITE_KNOB_MOTOR_APP_H
#define WHITE_KNOB_MOTOR_APP_H

typedef enum {
    MODE_FREE = 0,      // 自由旋转（无力反馈）
    MODE_DAMPING,       // 阻尼
    MODE_DETENT,        // 棘轮
    MODE_EDGE,          // 限位
    MODE_VIBRATE,       // 振动（一次性）
    MODE_SPRING,        // 弹簧回中（预留）
} HapticMode;

// 参数联合体
typedef struct {
    float min_angle;
    float max_angle;
    bool  side;          // false: 允许内区间，true: 允许外区间
} EdgeParams;

typedef struct {
    int   steps;
    float width;         // 卡位宽度
} DetentParams;

typedef struct {
    float strength;      // 阻尼系数
} DampingParams;


// ... 以后还可以加 SpringParams 等

// 触觉状态结构体
typedef struct {
    HapticMode mode;
    union {
        DetentParams  detent;
        EdgeParams    edge;
        DampingParams damping;
        // SpringParams  spring; ...
    } params;
} KnobHapticState;

extern KnobHapticState g_haptic_state;

extern HapticMode g_vibratic_next_mode;


/**
 * @brief 切换当前手感工作模式
 *
 * @param current_angle 当前角度，单位：°
 */
void KnobHaptics_Update(float current_angle);

/**
 * @brief 设置阻尼系数，模拟旋转时的粘滞阻力
 *
 * @param coef 阻尼系数，转矩输出 = -coef * velocity
 */
void KnobHaptics_SetDampingCoef(float coef);

/**
 * @brief 设置惯性及摩擦参数，模拟旋钮的转动惯量和附加动摩擦
 *
 * @param inertia 模拟的转动惯量，数值越高旋转越“重”且不易启停
 * @param friction 附加的库仑/粘性摩擦系数，用于能量衰减
 */
void KnobHaptics_SetInertiaParams(float inertia, float friction);

/**
 * @brief 配置弹簧回中效果，使旋钮趋向于回到指定角度位置
 *
 * @param centerAngle 弹簧平衡角度，即回中目标位置
 * @param stiffness 弹簧刚度，偏离中心越远反力矩越大
 */
void KnobHaptics_SetSpringParams(float centerAngle, float stiffness);

/**
 * @brief 设置多稳态棘轮（卡位）手感参数，旋钮每转过一定角度会产生明显的卡顿力矩
 *
 * @param steps 每旋转一圈的卡位数（如 24 表示 24 档）
 * @param current_angle 当前旋钮位置
 * @param width 卡位的角度宽度（影响卡顿的“锐度”）
 */
void KnobHaptics_SetDetent(float current_angle, int steps, float width);

/**
 * @brief  执行边界限位动作，将当前角度强制拉回允许范围
 *
 * @param  current_angle  旋钮当前的机械角度（°）
 * @param  min_angle          边界角度一（°）
 * @param  max_angle          边界角度二（°）
 * @param  side           边界模式选择：
 *                        - false：角度允许范围为 [min_angle, max_angle] 闭区间（min_angle 为下限，max_angle 为上限），
 *                          超出时拉回对应边界。
 *                        - true ：角度允许范围是 [min_angle, max_angle] 的补集，即旋钮不得停留在该区间内，
 *                          若进入则根据方向拉回最近的边界外侧。
 */
void KnobHaptics_SetEdge(float current_angle, float min_angle, float max_angle, bool side);

/**
 * @brief 设置表面纹理/颗粒感效果参数，叠加高频低幅振动
 *
 * @param amplitude 振动幅值，影响颗粒感的强弱
 * @param frequency 振动频率，单位一般为振动次数/每转或固定频率
 */
void KnobHaptics_SetTextureParams(float amplitude, float frequency);

void KnobHaptics_Vibrate(float current_angle);

float get_rotation_speed(float current_angle);

void KnobHaptics_Damping(float current_angle, float strength);

#endif //WHITE_KNOB_MOT