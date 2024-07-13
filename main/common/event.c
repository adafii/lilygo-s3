#include "event.h"
#include "config.h"
#include "esp_log.h"
#include "event_types.h"
#include "gui/gui.h"

static const char* TAG = "event";

void init_event_loop(gui_context_t* gui_context, esp_event_loop_handle_t* event_loop) {
    ESP_LOGI(TAG, "Initializing event loop");

    const esp_event_loop_args_t loop_args = {
        .queue_size = MAIN_LOOP_QUEUE_SIZE,
        .task_name = MAIN_LOOP_TASK_NAME,
        .task_priority = MAIN_LOOP_TASK_PRIORITY,
        .task_stack_size = MAIN_LOOP_STACK_SIZE,
        .task_core_id = APP_CPU_NUM
    };

    ESP_ERROR_CHECK(esp_event_loop_create(&loop_args, event_loop));

    ESP_ERROR_CHECK(esp_event_handler_register_with(*event_loop, GUI_EVENT, NEXT_APP, gui_change_app, gui_context));
    ESP_ERROR_CHECK(esp_event_handler_register_with(*event_loop, GUI_EVENT, PREVIOUS_APP, gui_change_app, gui_context));
}