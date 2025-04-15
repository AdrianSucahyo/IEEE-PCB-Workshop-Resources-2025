/**
 * Shift Register H File
 *
 * Function and struct definitions for the Shift Register Controller
 *
 * @version 1.0.0
 */

#include "shift_reg_core.h"

static const char *SHIFT_REG_CORE_TAG = "shift_reg_core";

#define SHIFT_REG_DELAY 0

static uint16_t current_state = 0;

/*  =================================================================
 *
 *                     Internal Utility Functions
 *
 *  ================================================================= */

static void shift_reg_shift_out(uint16_t data_byte)
{
    // Set the shift register pins to their base state
    REG_SET_BIT(TRANS_PIN_NUM_W1TC(CONFIG_SHIFT_REGISTER_CLK_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_CLK_PIN));
    REG_SET_BIT(TRANS_PIN_NUM_W1TC(CONFIG_SHIFT_REGISTER_LATCH_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_LATCH_PIN));
    // REG_SET_BIT(TRANS_PIN_NUM_W1TS(CONFIG_SHIFT_REGISTER_ENABLE_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_ENABLE_PIN)); // Disable Shift Register Output

    // Shift All 16 Bits Out
    uint16_t pos = 0x8000;
    for (uint8_t i = 0; i < 16; i++)
    {
        if (data_byte & pos)
        {
            // ESP_LOGV(SHIFT_REG_CORE_TAG, "HIGH");
            REG_SET_BIT(TRANS_PIN_NUM_W1TS(CONFIG_SHIFT_REGISTER_DATA_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_DATA_PIN));
        }
        else
        {
            // ESP_LOGV(SHIFT_REG_CORE_TAG, "LOW");
            REG_SET_BIT(TRANS_PIN_NUM_W1TC(CONFIG_SHIFT_REGISTER_DATA_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_DATA_PIN));
        }

        vTaskDelay(pdMS_TO_TICKS(SHIFT_REG_DELAY));
        // ESP_LOGV(SHIFT_REG_CORE_TAG, "PULSE UP %u", i);
        REG_SET_BIT(TRANS_PIN_NUM_W1TS(CONFIG_SHIFT_REGISTER_CLK_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_CLK_PIN));
        // ESP_LOGV(SHIFT_REG_CORE_TAG, "PULSE DOWN");
        vTaskDelay(pdMS_TO_TICKS(SHIFT_REG_DELAY));
        REG_SET_BIT(TRANS_PIN_NUM_W1TC(CONFIG_SHIFT_REGISTER_CLK_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_CLK_PIN));

        pos >>= 1;
    }

    // Latch the Data
    vTaskDelay(pdMS_TO_TICKS(SHIFT_REG_DELAY));
    // ESP_LOGV(SHIFT_REG_CORE_TAG, "LATCH UP");
    REG_SET_BIT(TRANS_PIN_NUM_W1TS(CONFIG_SHIFT_REGISTER_LATCH_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_LATCH_PIN));
    // ESP_LOGV(SHIFT_REG_CORE_TAG, "LATCH DOWN");
    vTaskDelay(pdMS_TO_TICKS(SHIFT_REG_DELAY));
    REG_SET_BIT(TRANS_PIN_NUM_W1TC(CONFIG_SHIFT_REGISTER_LATCH_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_LATCH_PIN));
}

static void shift_reg_core_gpio_init()
{
    gpio_config_t shift_reg_clock, shift_reg_latch, shift_reg_en, shift_reg_data = {};

    shift_reg_clock.intr_type = GPIO_INTR_DISABLE;
    shift_reg_clock.mode = GPIO_MODE_OUTPUT;
    shift_reg_clock.pin_bit_mask = 1ULL << CONFIG_SHIFT_REGISTER_CLK_PIN;
    shift_reg_clock.pull_down_en = GPIO_PULLDOWN_ENABLE;
    shift_reg_clock.pull_up_en = GPIO_PULLUP_DISABLE;

    shift_reg_latch.intr_type = GPIO_INTR_DISABLE;
    shift_reg_latch.mode = GPIO_MODE_OUTPUT;
    shift_reg_latch.pin_bit_mask = 1ULL << CONFIG_SHIFT_REGISTER_LATCH_PIN;
    shift_reg_latch.pull_down_en = GPIO_PULLDOWN_ENABLE;
    shift_reg_latch.pull_up_en = GPIO_PULLUP_DISABLE;

    shift_reg_en.intr_type = GPIO_INTR_DISABLE;
    shift_reg_en.mode = GPIO_MODE_OUTPUT;
    shift_reg_en.pin_bit_mask = 1ULL << CONFIG_SHIFT_REGISTER_ENABLE_PIN;
    shift_reg_en.pull_down_en = GPIO_PULLDOWN_ENABLE;
    shift_reg_en.pull_up_en = GPIO_PULLUP_DISABLE;

    shift_reg_data.intr_type = GPIO_INTR_DISABLE;
    shift_reg_data.mode = GPIO_MODE_OUTPUT;
    shift_reg_data.pin_bit_mask = 1ULL << CONFIG_SHIFT_REGISTER_DATA_PIN;
    shift_reg_data.pull_down_en = GPIO_PULLDOWN_ENABLE;
    shift_reg_data.pull_up_en = GPIO_PULLUP_DISABLE;

    gpio_config(&shift_reg_clock);
    gpio_config(&shift_reg_latch);
    gpio_config(&shift_reg_en);
    gpio_config(&shift_reg_data);

    // Set the shift register pins to default state
    REG_SET_BIT(TRANS_PIN_NUM_W1TC(CONFIG_SHIFT_REGISTER_CLK_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_CLK_PIN));
    REG_SET_BIT(TRANS_PIN_NUM_W1TC(CONFIG_SHIFT_REGISTER_LATCH_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_LATCH_PIN));
    REG_SET_BIT(TRANS_PIN_NUM_W1TS(CONFIG_SHIFT_REGISTER_ENABLE_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_ENABLE_PIN));
    REG_SET_BIT(TRANS_PIN_NUM_W1TC(CONFIG_SHIFT_REGISTER_DATA_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_DATA_PIN));
}

// ===============================================================================================
//  Utility Functions
// ===============================================================================================

esp_err_t shift_reg_core_enable()
{
    REG_SET_BIT(TRANS_PIN_NUM_W1TC(CONFIG_SHIFT_REGISTER_ENABLE_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_ENABLE_PIN));
    return ESP_OK;
}

esp_err_t shift_reg_core_disable()
{
    REG_SET_BIT(TRANS_PIN_NUM_W1TS(CONFIG_SHIFT_REGISTER_ENABLE_PIN), TRANS_PIN_NUM_BITMASK(CONFIG_SHIFT_REGISTER_ENABLE_PIN));
    return ESP_OK;
}

esp_err_t shift_reg_core_write()
{
    ESP_LOGD(SHIFT_REG_CORE_TAG, "Write Matrix: 0x%x", current_state);
    shift_reg_shift_out(current_state);
    return ESP_OK;
}

esp_err_t shift_reg_core_set(uint16_t data_word, uint16_t bitmask)
{
    ESP_LOGD(SHIFT_REG_CORE_TAG, "Set Matrix: 0x%x & 0x%x", data_word, bitmask);

    current_state = (current_state & ~bitmask) | (data_word & bitmask);
    return ESP_OK;
}

/*  =================================================================
 *
 *                      Initialization Functions
 *
 *  ================================================================= */

void shift_reg_core_init()
{
    shift_reg_core_gpio_init();
}