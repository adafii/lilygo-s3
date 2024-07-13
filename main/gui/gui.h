#pragma once
#include "esp_event.h"
#include "gui/gui_types.h"
#include "hardware/hardware_types.h"
#include <stdint.h>

/**
 * Init LVGL GUI
 * @param hardware_context Hardware context
 * @param gui_context GUI context
 */
void init_gui(hardware_context_t* hardware_context, gui_context_t* gui_context);

/**
 * Run the first GUI app
 * @param gui_context GUI context
 */
void start_gui(gui_context_t* gui_context);

/**
 * Register GUI app
 * @param gui_context GUI context
 * @param app App to be registered
 */
void gui_register_app(gui_app_t* app);

/**
 * Change GUI app event handler
 * @param event_handler_arg Event handler data
 * @param event_base Event base
 * @param event_id Event ID
 * @param event_data Event data
 */
void gui_change_app(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data);