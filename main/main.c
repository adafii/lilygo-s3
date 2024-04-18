#include "display/st7789.h"

void app_main() {
    esp_lcd_panel_handle_t panel_handle = NULL;
    init_st7789_display(&panel_handle);
    set_backlight_level(100);
}
