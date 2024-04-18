#include "display/display.h"
#include "gui/gui.h"

static lv_display_t* display = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;

void app_main() {
    init_display(&panel_handle);
    set_backlight_level(10);

    init_gui(&panel_handle, display);
}