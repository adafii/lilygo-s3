#include "hardware/rtc.h"
#include "esp_log.h"
#include "hardware/config.h"

static const char* TAG = "rtc";

// Public API
void init_rtc() {
    ESP_LOGI(TAG, "Initializing RTC");
    setenv("TZ", RTC_TIMEZONE, 1);
    tzset();
}

time_t rtc_get_time() {
    ESP_LOGD(TAG, "Reading current time from RTC");
    time_t rtc_time = time(NULL);
    return rtc_time;
}