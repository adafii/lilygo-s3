#pragma once
#include "esp_event.h"

ESP_EVENT_DECLARE_BASE(GUI_EVENT);

typedef enum {
    NEXT_APP,
    PREVIOUS_APP,
} GUI_EVENT_IDS;