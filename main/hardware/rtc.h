#pragma once
#include "rtc_types.h"

/**
 * Initialize Real Time Clock
 * @param rtc_handle RTC handle
 */
void init_rtc(rtc_handle_t* rtc_handle);

rtc_time_t rtc_get_time(rtc_handle_t* rtc_handle);