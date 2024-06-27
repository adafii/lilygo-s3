#pragma once
#include <time.h>

/**
 * Init RTC by setting local timezone environment variable, TZ
 */
void init_rtc();

time_t rtc_get_time();