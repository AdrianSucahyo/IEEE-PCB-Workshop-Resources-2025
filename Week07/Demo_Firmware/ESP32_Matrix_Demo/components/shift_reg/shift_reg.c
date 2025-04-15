/**
 * Shift Register C File
 *
 * Function and struct definitions for the Shift Register Controller
 *
 * @version 1.0.0
 */

#include "shift_reg.h"
#include "shift_reg_core.h"

#include "freertos/semphr.h"

static const char *SHIFT_REG_TAG = "shift_reg";

static SemaphoreHandle_t update_shift_reg;

/*  =================================================================
 * 
 *                     Internal Utility Functions
 * 
 *  ================================================================= */

static esp_err_t shift_reg_update(uint16_t data_word, uint16_t bitmask)
{
    // Prevent Concurrent Matrix Modification
    while (xSemaphoreTake(update_shift_reg, (TickType_t)10) == pdFALSE)
    {
        ESP_LOGE(SHIFT_REG_TAG, "Waiting...");
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    ESP_LOGD(SHIFT_REG_TAG, "Update Shift Register");

    esp_err_t err = shift_reg_core_set(data_word, bitmask);

    if (err != ESP_OK)
    {
        ESP_LOGE(SHIFT_REG_TAG, "Failed to set shift register: 0x%x & 0x%x", data_word, bitmask);
        return ESP_FAIL;
    }

    err = shift_reg_core_write();

    if (err != ESP_OK)
    {
        ESP_LOGE(SHIFT_REG_TAG, "Failed to write shift register");
        return ESP_FAIL;
    }

    xSemaphoreGive(update_shift_reg);

    return err;
}

/*  =================================================================
 * 
 *                         Utility Functions
 * 
 *  ================================================================= */

esp_err_t shift_reg_enable()
{
    return shift_reg_core_enable();
}

esp_err_t shift_reg_disable()
{
    return shift_reg_core_disable();
}

esp_err_t shift_reg_clear()
{
    return shift_reg_update(0x0000, 0xFFFF);
}

esp_err_t shift_reg_write(uint16_t data_word, uint16_t bitmask)
{
    return shift_reg_update(data_word, bitmask);
}


/*  =================================================================
 * 
 *                      Initialization Functions
 * 
 *  ================================================================= */


esp_err_t shift_reg_init()
{
    shift_reg_core_init();

    update_shift_reg = xSemaphoreCreateMutex();

    shift_reg_clear();

    ESP_LOGE(SHIFT_REG_TAG, "Shift Register Initialized");

    return ESP_OK;
}