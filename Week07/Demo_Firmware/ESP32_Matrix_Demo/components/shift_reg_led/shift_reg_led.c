/**
 * Shift Register C File
 *
 * Function and struct definitions for the Shift Register Controller
 *
 * @version 1.0.0
 */

#include "shift_reg_led.h"
#include "shift_reg_led_core.h"

#include "freertos/semphr.h"

static const char *shift_reg_led_TAG = "shift_reg_led";

static SemaphoreHandle_t update_shift_reg_led;

/*  =================================================================
 *
 *                     Internal Utility Functions
 *
 *  ================================================================= */

static esp_err_t shift_reg_led_update(uint16_t data_word, uint16_t bitmask)
{
    // Prevent Concurrent Matrix Modification
    while (xSemaphoreTake(update_shift_reg_led, (TickType_t)10) == pdFALSE)
    {
        ESP_LOGE(shift_reg_led_TAG, "Waiting...");
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    ESP_LOGD(shift_reg_led_TAG, "Update Shift Register");

    esp_err_t err = shift_reg_led_core_set(data_word, bitmask);

    if (err != ESP_OK)
    {
        ESP_LOGE(shift_reg_led_TAG, "Failed to set shift register: 0x%x & 0x%x", data_word, bitmask);
        return ESP_FAIL;
    }

    err = shift_reg_led_core_write();

    if (err != ESP_OK)
    {
        ESP_LOGE(shift_reg_led_TAG, "Failed to write shift register");
        return ESP_FAIL;
    }

    xSemaphoreGive(update_shift_reg_led);

    return err;
}

/*  =================================================================
 *
 *                         Utility Functions
 *
 *  ================================================================= */

esp_err_t shift_reg_led_enable()
{
    return shift_reg_led_core_enable();
}

esp_err_t shift_reg_led_disable()
{
    return shift_reg_led_core_disable();
}

esp_err_t shift_reg_led_clear()
{
    return shift_reg_led_update(0x0000, 0xFFFF);
}

esp_err_t shift_reg_led_write(uint16_t data_word, uint16_t bitmask)
{
    return shift_reg_led_update(data_word, bitmask);
}

/*  =================================================================
 *
 *                      Initialization Functions
 *
 *  ================================================================= */

esp_err_t shift_reg_led_init()
{
    shift_reg_led_core_init();

    update_shift_reg_led = xSemaphoreCreateMutex();

    shift_reg_led_clear();

    ESP_LOGE(shift_reg_led_TAG, "Shift Register Initialized");

    return ESP_OK;
}