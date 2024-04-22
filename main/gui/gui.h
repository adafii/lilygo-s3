#pragma once
#include "esp_lcd_types.h"
#include "lvgl.h"

void init_gui(esp_lcd_panel_handle_t* panel_handle, lv_display_t* display);

void hello_world();