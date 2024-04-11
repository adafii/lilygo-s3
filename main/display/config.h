#pragma once

#define LCD_WIDTH             240
#define LCD_HEIGHT            240

#define LCD_BL                45
#define LCD_MISO              -1
#define LCD_MOSI              13
#define LCD_SCLK              18
#define LCD_CS                12
#define LCD_DC                38
#define LCD_RST               -1

#define LCD_CMD_BITS          8
#define LCD_PARAM_BITS        8
#define LCD_PIXEL_CLOCK_HZ    (20 * 1000 * 1000)

#define SPI_MODE              0
#define SPI_TRANS_QUEUE_DEPTH 10