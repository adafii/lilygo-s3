#pragma once

// LCD

#define LCD_WIDTH_PX            240
#define LCD_HEIGHT_PX           240

#define LCD_BL                  GPIO_NUM_45
#define LCD_MISO                -1
#define LCD_MOSI                GPIO_NUM_13
#define LCD_SCLK                GPIO_NUM_18
#define LCD_CS                  GPIO_NUM_12
#define LCD_DC                  GPIO_NUM_38
#define LCD_RST                 -1

#define LCD_CMD_BITS            8
#define LCD_PARAM_BITS          8
#define LCD_PIXEL_CLOCK_HZ      (20 * 1000 * 1000)

#define SPI_MODE                0
#define SPI_TRANS_QUEUE_DEPTH   10

#define LCD_BL_MODE             LEDC_LOW_SPEED_MODE
#define LCD_BL_TIMER            LEDC_TIMER_0
#define LCD_BL_CHANNEL          LEDC_CHANNEL_0
#define LCD_BL_DUTY_RES         LEDC_TIMER_13_BIT
#define LCD_BL_FREQUENCY_HZ     4000
#define LCD_BL_MAX_DUTY         8192  // 2 ^ duty resolution

#define LCD_SPI_HOST            SPI2_HOST

// RTC

#define RTC_CLK_SRC             I2C_CLK_SRC_DEFAULT
#define RTC_I2C_PORT            -1
#define RTC_SDA                 10
#define RTC_SCL                 11
#define RTC_INTERRUPT           17
#define RTC_GLITCH_IGNORE_COUNT 7
#define RTC_I2C_ADDRESS         0x51  // 0xA2 >> 1
#define RTC_I2C_CLK_SPEED_HZ    (100 * 1000)
#define RTC_PROBE_TIMEOUT_MS    1000
#define RTC_RW_TIMEOUT_MS       1000

typedef enum : uint8_t {
    CONTROL1,
    CONTROL2,
    VL_SECONDS,
    MINUTES,
    HOURS,
    DAYS,
    WEEKDAYS,
    CENTURY_MONTHS,
    YEARS,
} rtc_register_address_t;