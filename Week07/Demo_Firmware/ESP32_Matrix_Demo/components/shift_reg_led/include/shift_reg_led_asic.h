/**
 * Shift Register H File
 *
 * Function and struct definitions for the Shift Register Controller
 *
 * @version 1.0.0
 */

#ifndef _SHIFT_REG_LED_ASIC_H_
#define _SHIFT_REG_LED_ASIC_H_

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"

esp_err_t shift_reg_led_enable_gps();

esp_err_t shift_reg_led_disable_gps();

esp_err_t shift_reg_led_enable_gps_power();

esp_err_t shift_reg_led_disable_gps_power();

esp_err_t shift_reg_led_select_button_col(uint8_t col);

esp_err_t shift_reg_led_select_button_row(uint8_t row);

esp_err_t shift_reg_led_write_leds(uint8_t word);

#endif