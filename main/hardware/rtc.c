#include "hardware/rtc.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "hardware/config.h"
#include "hardware/rtc_types.h"
#include <stdio.h>
#include <string.h>

static const char* TAG = "rtc";

// Public API

void init_rtc(rtc_handle_t* rtc_handle) {
    ESP_LOGI(TAG, "Initializing RTC");
    i2c_master_bus_config_t rtc_bus_config = {
        .clk_source = RTC_CLK_SRC,
        .i2c_port = RTC_I2C_PORT,
        .scl_io_num = RTC_SCL,
        .sda_io_num = RTC_SDA,
        .glitch_ignore_cnt = RTC_GLITCH_IGNORE_COUNT,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&rtc_bus_config, &rtc_handle->bus_handle));

    ESP_ERROR_CHECK(i2c_master_probe(rtc_handle->bus_handle, RTC_I2C_ADDRESS, RTC_PROBE_TIMEOUT_MS));

    i2c_device_config_t rtc_dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7, .device_address = RTC_I2C_ADDRESS, .scl_speed_hz = RTC_I2C_CLK_SPEED_HZ
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(rtc_handle->bus_handle, &rtc_dev_config, &rtc_handle->dev_handle));

    uint8_t initial_registers[] = {0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    ESP_ERROR_CHECK(i2c_master_transmit(rtc_handle->dev_handle, initial_registers, sizeof(initial_registers), RTC_RW_TIMEOUT_MS));
}

rtc_time_t rtc_get_time(rtc_handle_t* rtc_handle) {
    rtc_register_address_t start_address = CONTROL1;
    uint8_t read_buffer[10];

    ESP_ERROR_CHECK(i2c_master_transmit_receive(
        rtc_handle->dev_handle, &start_address, sizeof(start_address), read_buffer, sizeof(read_buffer), RTC_RW_TIMEOUT_MS
    ));

    for (int i = 0; i < sizeof(read_buffer); ++i) {
        printf("%.2x ", read_buffer[i]);
    }
    printf("\n");

    return (rtc_time_t){};
}