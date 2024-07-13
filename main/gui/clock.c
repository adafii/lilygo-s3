#include "gui/clock.h"
#include "esp_log.h"
#include "gui_types.h"

static const char* TAG = "clock";

typedef struct {
    int32_t time;
} clock_context_t;

void init_clock(gui_app_t* this_app) {
    ESP_LOGI(TAG, "Initializing clock");

    this_app->app_context = calloc(1, sizeof(clock_context_t));
    assert(this_app->app_context);
}

void show_clock(gui_app_t* this_app) {
    ESP_LOGI(TAG, "Showing clock");

    gui_context_t* gui_context = this_app->gui_context;

    if (xSemaphoreTake(gui_context->lvgl_mutex, portMAX_DELAY) == pdTRUE) {
        lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xff0000), LV_PART_MAIN);

        lv_obj_t* label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "Clock is 12:12:12");
        lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
        lv_obj_align(label, LV_ALIGN_OUT_LEFT_TOP, 10, 10);
        xSemaphoreGive(gui_context->lvgl_mutex);
    }
}

void hide_clock(gui_app_t* this_app) {
    ESP_LOGI(TAG, "Hiding clock");
}