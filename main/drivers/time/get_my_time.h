//
// Created by k0921 on 2026/5/4.
//

#ifndef WHITE_KNOB_GET_MY_TIME_H
#define WHITE_KNOB_GET_MY_TIME_H

void my_timer_init(void);
long long time_get_uinx_time_s(void);
void time_get_hms(int *hour, int *minute, int *second);
void time_get_ymd(int *year, int *month, int *day);
int  time_get_weekday(void);
int  time_get_weekday_monday_first(void);

#endif //WHITE_KNOB_GET_MY_TIME_H