//
// Created by k0921 on 2026/5/4.
//

#include "get_my_time.h"
#include <stdlib.h>
#include <time.h>

#include "time/sntp_config.h"

void my_timer_init(void) {
    //设置环境变量时区为东八区
    setenv("TZ", "UTC-8", 1);
    tzset();
}

/**
 * 获取Uinx时间戳
 * @return
 */
long long time_get_uinx_time_s(void) {
    long long t;
    t = time(NULL);
    return t;
}

// 获取当前小时、分钟、秒（24小时制）
void time_get_hms(int *hour, int *minute, int *second) {
    time_t now = time(NULL);
    struct tm tm_now;
    localtime_r(&now, &tm_now);   // 线程安全，应用 setenv 设置的时区
    if (hour)   *hour   = tm_now.tm_hour;
    if (minute) *minute = tm_now.tm_min;
    if (second) *second = tm_now.tm_sec;
}

// 获取当前年、月、日
void time_get_ymd(int *year, int *month, int *day) {
    time_t now = time(NULL);
    struct tm tm_now;
    localtime_r(&now, &tm_now);
    if (year)  *year  = tm_now.tm_year + 1900;  // tm_year 是从1900年开始的偏移
    if (month) *month = tm_now.tm_mon + 1;      // tm_mon 范围0-11
    if (day)   *day   = tm_now.tm_mday;
}

// 获取星期几（0=星期日, 1=星期一, ..., 6=星期六）
int time_get_weekday(void) {
    time_t now = time(NULL);
    struct tm tm_now;
    localtime_r(&now, &tm_now);
    return tm_now.tm_wday;  // 直接返回，0表示星期日
}

// 如果你习惯星期一=1，可以：
int time_get_weekday_monday_first(void) {
    int w = time_get_weekday();
    return (w == 0) ? 7 : w;  // 转换为星期一~星期日: 1~7
}