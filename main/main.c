#include "hardware/display.h"
#include "gui/gui.h"

static lv_display_t* display = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;

void app_main() {
    init_display(&panel_handle);
    init_gui(&panel_handle, display);
    set_backlight_level(50);
    hello_world();
}