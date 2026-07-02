//
// Created by k0921 on 2026/3/28.
//

#ifndef WHITE_KNOB_MOTOR_CONTROL_H
#define WHITE_KNOB_MOTOR_CONTROL_H

void motor_set_pos(float target_pos, float current_pos);
void motor_set_vel(float target_vel, float current_vel, float shaft_angle);
void motor_set_pos_with_vel(float target_pos, float target_vel, float current_pos, float current_vel);

#endif //WHITE_KNOB_MOTOR_CONTROL_H