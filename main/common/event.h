#pragma once
#include "esp_event.h"
#include "gui/gui_types.h"

/**
 * Init event loop
 * @param gui_context
 * @param event_loop
 */
void init_event_loop(gui_context_t* gui_context, esp_event_loop_handle_t* event_loop);