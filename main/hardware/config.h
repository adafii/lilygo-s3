#pragma once

#define LCD_WIDTH_PX          240
#define LCD_HEIGHT_PX         240

#define LCD_BL                GPIO_NUM_45
#define LCD_MISO              -1
#define LCD_MOSI              GPIO_NUM_13
#define LCD_SCLK              GPIO_NUM_18
#define LCD_CS                GPIO_NUM_12
#define LCD_DC                GPIO_NUM_38
#define LCD_RST               -1

#define LCD_CMD_BITS          8
#define LCD_PARAM_BITS        8
#define LCD_PIXEL_CLOCK_HZ    (20 * 1000 * 1000)

#define SPI_MODE              0
#define SPI_TRANS_QUEUE_DEPTH 10

#define LCD_BL_MODE           LEDC_LOW_SPEED_MODE
#define LCD_BL_TIMER          LEDC_TIMER_0
#define LCD_BL_CHANNEL        LEDC_CHANNEL_0
#define LCD_BL_DUTY_RES       LEDC_TIMER_13_BIT
#define LCD_BL_FREQUENCY_HZ   4000
#define LCD_BL_MAX_DUTY       8192  // 2 ^ duty resolution

#define LCD_SPI_HOST          SPI2_HOST