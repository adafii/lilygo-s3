#pragma once
#include "esp_lcd_types.h"
#include "hardware_types.h"

/**
 * Install panel driver and get the panel handle
 * @param[out] panel_handle Panel handle
 */
void init_display(hardware_context_t* hardware_context);

/**
 * Set display backlight level
 * @param level Backlight level 0-100%
 */
void set_backlight_level(uint8_t level);