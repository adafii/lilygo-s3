#include "display/display.h"
#include "gui/gui.h"
#include <esp_lcd_panel_ops.h>

static lv_display_t* display = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;

void app_main() {
    init_display(&panel_handle);
    set_backlight_level(100);

    init_gui(&panel_handle, display);
    hello_world();
}