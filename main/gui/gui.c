#include "gui.h"
#include "display/config.h"
#include "driver/gptimer.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_types.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gui/config.h"
#include "lvgl.h"

static const char* TAG = "GUI";
static SemaphoreHandle_t lvgl_mutex = NULL;

static void init_lvgl_tick_timer();
static bool lvgl_timer_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx);
static void flush_cb(lv_display_t* display, const lv_area_t* area, uint8_t* px_map);
static void gui_task(void* arg);

// Public API

void init_gui(esp_lcd_panel_handle_t* panel_handle, lv_display_t* display) {
    ESP_LOGI(TAG, "Initializing LVGL library");

    lvgl_mutex = xSemaphoreCreateMutex();

    init_lvgl_tick_timer();
    lv_init();
    display = lv_display_create(LCD_WIDTH_PX, LCD_HEIGHT_PX);
    lv_display_set_user_data(display, panel_handle);

    lv_color_t* buffer1 = heap_caps_malloc(LVGL_DISPLAY_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    lv_color_t* buffer2 = heap_caps_malloc(LVGL_DISPLAY_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buffer1 && buffer2);
    lv_display_set_buffers(display, buffer1, buffer2, LVGL_DISPLAY_BUFFER_SIZE * sizeof(lv_color_t),
                           LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_display_set_flush_cb(display, flush_cb);

    ESP_LOGI(TAG, "Starting GUI task on CPU %d", APP_CPU_NUM);
    xTaskCreatePinnedToCore(gui_task, "gui", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL, APP_CPU_NUM);
}

void hello_world() {
    if(xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE) {
        ESP_LOGI(TAG, "Hello world display test");
        lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);

        lv_obj_t* label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "Hello, World!");
        lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
        lv_obj_align(label, LV_ALIGN_OUT_LEFT_TOP, 10, 10);
        xSemaphoreGive(lvgl_mutex);
    }
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

    gptimer_alarm_config_t alarm_config = {.alarm_count = LVGL_TICK_PERIOD_MS * LVGL_TIMER_MS_COUNT,
                                           .reload_count = 0,
                                           .flags.auto_reload_on_alarm = true};
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

static void gui_task(void* arg) {
    ESP_LOGI(TAG, "GUI task started");
    uint32_t task_delay_ms = LVGL_TASK_MAX_DELAY_MS;

    while (1) {
        if(xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE) {
            task_delay_ms = lv_timer_handler();
            xSemaphoreGive(lvgl_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}