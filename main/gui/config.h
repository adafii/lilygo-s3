#pragma once
#include "hardware/config.h"

#define LVGL_TIMER_RESOLUTION_HZ (1 * 1000 * 1000)
#define LVGL_TIMER_MS_COUNT      1000
#define LVGL_TICK_PERIOD_MS      1
#define LVGL_DISPLAY_BUFFER_SIZE (LCD_WIDTH_PX * LCD_HEIGHT_PX / 10)

#define LVGL_TASK_STACK_SIZE   (4 * 1024)
#define LVGL_TASK_PRIORITY     0
#define LVGL_TASK_MAX_DELAY_MS 500