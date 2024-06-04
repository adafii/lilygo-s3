#include "gui/gui.h"
#include "hardware/display.h"
#include "hardware/rtc.h"

static lv_display_t* display = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;

void app_main() {
    init_rtc();
    init_display(&panel_handle);
    init_gui(&panel_handle, display);
    set_backlight_level(100);
    hello_world();
}