#pragma once
#include "esp_lcd_types.h"

/**
 * Install panel driver and get the panel handle
 * @param[out] panel_handle Panel handle
 */
void init_st7789_display(esp_lcd_panel_handle_t* panel_handle);

/**
 * Set display backlight level
 * @param level Backlight level 0-100%
 */
void set_backlight_level(uint8_t level);