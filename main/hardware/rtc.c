#include "driver/i2c_master.h"
#include "esp_log.h"
#include "hardware/config.h"

static const char* TAG = "rtc";

void init_rtc() {
    ESP_LOGI(TAG, "Initializing RTC");
    i2c_master_bus_handle_t rtc_bus_handle = NULL;
    i2c_master_bus_config_t rtc_bus_config = {
        .clk_source = RTC_CLK_SRC,
        .i2c_port = RTC_I2C_PORT,
        .scl_io_num = RTC_SCL,
        .sda_io_num = RTC_SDA,
        .glitch_ignore_cnt = RTC_GLITCH_IGNORE_COUNT,
        .flags.enable_internal_pullup = true
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&rtc_bus_config, &rtc_bus_handle));

    ESP_ERROR_CHECK(i2c_master_probe(rtc_bus_handle, RTC_I2C_ADDRESS, RTC_PROBE_TIMEOUT_MS));

    i2c_master_dev_handle_t rtc_dev_handle = NULL;
    i2c_device_config_t rtc_dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7, .device_address = RTC_I2C_ADDRESS, .scl_speed_hz = RTC_I2C_CLK_SPEED_HZ
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(rtc_bus_handle, &rtc_dev_config, &rtc_dev_handle));
}