#include "esp_event.h"
#include "event.h"

void init_event_loop(esp_event_loop_handle_t* event_loop) {
    const esp_event_loop_args_t loop_args = {
        .queue_size = MAIN_LOOP_QUEUE_SIZE,
        .task_name = MAIN_LOOP_TASK_NAME,
        .task_priority = MAIN_LOOP_TASK_PRIORITY,
        .task_stack_size = MAIN_LOOP_STACK_SIZE,
        .task_core_id = APP_CPU_NUM
    };

    ESP_ERROR_CHECK(esp_event_loop_create(&loop_args, event_loop));
}