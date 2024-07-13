#pragma once
#include "esp_lcd_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "gui/config.h"
#include "lvgl.h"

#define REGISTER_GUI_APP(gui_context, app_name) \
    gui_register_app(&(gui_app_t                \
    ){.name = #app_name,                        \
      .init_app = init_##app_name,              \
      .show_app = show_##app_name,              \
      .hide_app = hide_##app_name,              \
      .app_context = NULL,                      \
      .gui_context = &gui_context})

typedef struct gui_app_t gui_app_t;
typedef struct gui_context_t gui_context_t;

struct gui_app_t {
    const char* name;
    void* app_context;
    gui_context_t* gui_context;

    void (*init_app)(gui_app_t* this_app);
    void (*show_app)(gui_app_t* this_app);
    void (*hide_app)(gui_app_t* this_app);
};

struct gui_context_t {
    SemaphoreHandle_t lvgl_mutex;
    lv_display_t* display;
    gui_app_t gui_apps[GUI_MAX_APPS];
    int32_t registered_apps;
    int32_t current_app_index;
};