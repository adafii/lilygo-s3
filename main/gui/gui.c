#include "gui.h"
#include "common/event_types.h"
#include "driver/gptimer.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_types.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hardware/config.h"
#include "hardware/display.h"
#include "lvgl.h"

static const char* TAG = "gui";

static void init_lvgl_tick_timer();
static bool lvgl_timer_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx);
static void flush_cb(lv_display_t* display, const lv_area_t* area, uint8_t* px_map);
static void gui_task(void* task_context);

// Public API

void init_gui(hardware_context_t* hardware_context, gui_context_t* gui_context) {
    ESP_LOGI(TAG, "Initializing LVGL library");

    gui_context->lvgl_mutex = xSemaphoreCreateMutex();

    init_lvgl_tick_timer();
    lv_init();
    gui_context->display = lv_display_create(LCD_WIDTH_PX, LCD_HEIGHT_PX);
    assert(gui_context->display);
    lv_display_set_user_data(gui_context->display, &hardware_context->panel_handle);

    lv_color_t* buffer1 = heap_caps_malloc(LVGL_DISPLAY_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    lv_color_t* buffer2 = heap_caps_malloc(LVGL_DISPLAY_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);

    assert(buffer1 && buffer2);
    lv_display_set_buffers(
        gui_context->display, buffer1, buffer2, LVGL_DISPLAY_BUFFER_SIZE * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_PARTIAL
    );

    lv_display_set_flush_cb(gui_context->display, flush_cb);

    ESP_LOGI(TAG, "Starting GUI task on CPU %d", APP_CPU_NUM);
    xTaskCreatePinnedToCore(gui_task, "gui", LVGL_TASK_STACK_SIZE, &gui_context->lvgl_mutex, LVGL_TASK_PRIORITY, NULL, APP_CPU_NUM);
}

void start_gui(gui_context_t* gui_context) {
    ESP_LOGI(TAG, "Starting the first GUI app");

    if (gui_context->registered_apps > 0) {
        set_backlight_level(100);
        gui_app_t* first_app = &gui_context->gui_apps[0];
        first_app->show_app(first_app);
    }
}

void gui_register_app(gui_app_t* app) {
    gui_context_t* gui_context = app->gui_context;

    if (gui_context->registered_apps >= GUI_MAX_APPS) {
        ESP_LOGE(TAG, "Could not register app '%s' anymore, maximum number of apps is %d", app->name, GUI_MAX_APPS);
        return;
    }

    ESP_LOGI(TAG, "Registering new app: %s", app->name);

    const int32_t current_index = gui_context->registered_apps;
    memcpy(&gui_context->gui_apps[current_index], app, sizeof(gui_context->gui_apps[current_index]));
    ++gui_context->registered_apps;

    app->init_app(&gui_context->gui_apps[current_index]);
}

void gui_change_app(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    gui_context_t* gui_context = event_handler_arg;

    if (gui_context->registered_apps == 0 || gui_context->registered_apps == 1) {
        ESP_LOGW(TAG, "Could not select the next GUI app. Number of apps currently registered: %ld", gui_context->registered_apps);
        return;
    }

    gui_app_t* previous_app = &gui_context->gui_apps[gui_context->current_app_index];

    if (event_id == NEXT_APP) {
        gui_context->current_app_index = (gui_context->current_app_index + 1) % gui_context->registered_apps;
    } else if (event_id == PREVIOUS_APP) {
        gui_context->current_app_index = (gui_context->current_app_index - 1 + gui_context->registered_apps) % gui_context->registered_apps;
    } else {
        ESP_LOGE(TAG, "Unknown event ID: %ld", event_id);
        return;
    }

    gui_app_t* next_app = &gui_context->gui_apps[gui_context->current_app_index];

    ESP_LOGI(TAG, "Changing app: %s -> %s", previous_app->name, next_app->name);

    previous_app->hide_app(previous_app);
    next_app->show_app(next_app);
}

// Internal API

static void init_lvgl_tick_timer() {
    ESP_LOGI(TAG, "Initializing LVGL tick timer to %d ms", LVGL_TICK_PERIOD_MS);

    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = LVGL_TIMER_RESOLUTION_HZ,
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    gptimer_alarm_config_t alarm_config = {
        .alarm_count = LVGL_TICK_PERIOD_MS * LVGL_TIMER_MS_COUNT, .reload_count = 0, .flags.auto_reload_on_alarm = true
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

    gptimer_event_callbacks_t timer_cb = {.on_alarm = lvgl_timer_cb};
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &timer_cb, NULL));

    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

static bool lvgl_timer_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx) {
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
    return false;
}

static void flush_cb(lv_display_t* display, const lv_area_t* area, uint8_t* px_map) {
    esp_lcd_panel_handle_t* panel_handle = lv_display_get_user_data(display);
    esp_lcd_panel_draw_bitmap(*panel_handle, area->x1, area->y1, area->x2 + 1, area->y2 + 1, px_map);
    lv_display_flush_ready(display);
}

static void gui_task(void* task_context) {
    ESP_LOGI(TAG, "GUI task started");

    SemaphoreHandle_t* lvgl_mutex = task_context;
    uint32_t task_delay_ms = LVGL_TASK_MAX_DELAY_MS;

    while (true) {
        if (xSemaphoreTake(*lvgl_mutex, portMAX_DELAY) == pdTRUE) {
            task_delay_ms = lv_timer_handler();
            xSemaphoreGive(*lvgl_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}