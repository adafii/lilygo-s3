#include "common/event.h"
#include "gui/clock.h"
#include "gui/gui.h"
#include "hardware/display.h"
#include "hardware/hardware_types.h"
#include "hardware/rtc.h"

static hardware_context_t hardware_context;
static gui_context_t gui_context;
static esp_event_loop_handle_t event_loop;

void app_main() {
    init_rtc();
    init_display(&hardware_context);
    init_event_loop(&gui_context, &event_loop);

    init_gui(&hardware_context, &gui_context);
    REGISTER_GUI_APP(gui_context, clock);
    start_gui(&gui_context);
}