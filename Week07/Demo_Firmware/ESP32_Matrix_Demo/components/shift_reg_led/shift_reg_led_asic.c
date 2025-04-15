/**
 * Shift Register C File
 *
 * Function and struct definitions for the Shift Register Controller
 * This file specifically contains commands that are APPLICATION SPECIFIC
 *
 * @version 1.0.0
 */

#include "shift_reg_led.h"

esp_err_t shift_reg_led_enable_gps()
{
    return shift_reg_led_write(0x0020, 0x0020);
}

esp_err_t shift_reg_led_disable_gps()
{
    return shift_reg_led_write(0x0000, 0x0020);
}

esp_err_t shift_reg_led_enable_gps_power()
{
    return shift_reg_led_write(0x0000, 0x0040);
}

esp_err_t shift_reg_led_disable_gps_power()
{
    return shift_reg_led_write(0x0040, 0x0040);
}

esp_err_t shift_reg_led_select_button_col(uint8_t col)
{
    switch (col)
    {
    case 0:
        return shift_reg_led_write(0x0002, 0x001E);
        break;
    case 1:
        return shift_reg_led_write(0x0004, 0x001E);
        break;
    case 2:
        return shift_reg_led_write(0x0008, 0x001E);
        break;
    case 3:
        return shift_reg_led_write(0x0010, 0x001E);
        break;
    default:
        return ESP_ERR_INVALID_ARG;
        break;
    }
}

esp_err_t shift_reg_led_select_button_row(uint8_t row)
{
    switch (row)
    {
    case 0:
        return shift_reg_led_write(0x0200, 0x0280);
        break;
    case 1:
        return shift_reg_led_write(0x0080, 0x0280);
        break;
    case 2:
        return shift_reg_led_write(0x0000, 0x0280);
        break;
    case 3:
        return shift_reg_led_write(0x0280, 0x0280);
        break;
    default:
        return ESP_ERR_INVALID_ARG;
        break;
    }
}

esp_err_t shift_reg_led_write_leds(uint8_t word)
{
    uint16_t led_word = ((uint16_t)word) << 10;
    return shift_reg_led_write(led_word, 0xFC00);
}
