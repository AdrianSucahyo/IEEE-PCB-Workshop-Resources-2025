#include "freertos/FreeRTOS.h"
#include <stdio.h>
#include <string.h>

#include "shift_reg.h"
#include "shift_reg_led.h"

static uint16_t frame[8];
static uint16_t frame_buffer[8];

#define BITMASK_SR_UPPER 0b1111111100000000
#define BITMASK_SR_LOWER 0b0000000011111111

static void loop_leds(void *params)
{
    uint16_t current_row_word;
    uint16_t current_row;

    while (1)
    {

        current_row = 0b1;
        // ESP_LOGI("MAIN", "Reset Row");

        for (uint8_t i = 0; i < 8; i++)
        {
            // current_row_word = ((frame[i] >> 2) & BITMASK_SR_UPPER) | ((frame[i] >> 3) & BITMASK_SR_LOWER);
            current_row_word = 0xFFFF;

            // ESP_LOGI("MAIN", "Writing: %x", current_row_word);

            shift_reg_write(current_row_word, BITMASK_SR_UPPER | BITMASK_SR_LOWER);
            shift_reg_led_write(current_row, 0b0000000011111111);

            current_row <<= 1;
        }
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}

void app_main()
{
    vTaskDelay(pdMS_TO_TICKS(1000));

    shift_reg_init();
    shift_reg_led_init();

    shift_reg_enable();
    shift_reg_led_enable();

    xTaskCreate(loop_leds, "Main LED Task", 4096, NULL, 1, NULL);
}