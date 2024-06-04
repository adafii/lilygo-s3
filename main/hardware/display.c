#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_types.h"
#include "esp_log.h"
#include "hardware/config.h"

static const char* TAG = "display";

static void init_display_backlight();

// Public API

void init_display(esp_lcd_panel_handle_t* panel_handle) {
    ESP_LOGI(TAG, "Initializing LCD");
    const spi_bus_config_t bus_config = {
        .sclk_io_num = LCD_SCLK,
        .mosi_io_num = LCD_MOSI,
        .miso_io_num = LCD_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_WIDTH_PX * 80 * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI_HOST, &bus_config, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_DC,
        .cs_gpio_num = LCD_CS,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = SPI_MODE,
        .trans_queue_depth = SPI_TRANS_QUEUE_DEPTH,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, panel_handle));

    ESP_LOGI(TAG, "Turning display on");
    ESP_ERROR_CHECK(esp_lcd_panel_reset(*panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(*panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(*panel_handle, true));
}

void set_backlight_level(uint8_t level) {
    static bool is_initialized = false;
    if (!is_initialized) {
        init_display_backlight();
    }

    if (level > 100) {
        ESP_LOGW(TAG, "Backlight level %d is over 100%%", level);
        level = 100;
    }

    uint32_t duty = level * LCD_BL_MAX_DUTY / 100;
    ESP_LOGI(TAG, "Setting backlight to %d%%", level);

    ledc_set_duty(LCD_BL_MODE, LCD_BL_CHANNEL, duty);
    ledc_update_duty(LCD_BL_MODE, LCD_BL_CHANNEL);
}

// Internal API

static void init_display_backlight() {
    gpio_set_direction(LCD_BL, GPIO_MODE_OUTPUT);

    ESP_LOGI(TAG, "Initializing LCD backlight led controller");
    ledc_timer_config_t bl_ledc_timer = {
        .speed_mode = LCD_BL_MODE,
        .timer_num = LCD_BL_TIMER,
        .duty_resolution = LCD_BL_DUTY_RES,
        .freq_hz = LCD_BL_FREQUENCY_HZ,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&bl_ledc_timer));

    ledc_channel_config_t bl_ledc_channel = {
        .speed_mode = LCD_BL_MODE,
        .channel = LCD_BL_CHANNEL,
        .timer_sel = LCD_BL_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LCD_BL,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&bl_ledc_channel));
}
