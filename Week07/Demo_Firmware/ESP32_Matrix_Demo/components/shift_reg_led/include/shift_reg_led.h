/**
 * Shift Register H File
 *
 * Function and struct definitions for the Shift Register Controller
 *
 * @version 1.0.0
 */

#ifndef _SHIFT_REG_LED_H_
#define _SHIFT_REG_LED_H_

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"

// Common Control

esp_err_t shift_reg_led_enable();

esp_err_t shift_reg_led_disable();

esp_err_t shift_reg_led_clear();

esp_err_t shift_reg_led_write(uint16_t data_word, uint16_t bitmask);

// Initialization Function

esp_err_t shift_reg_led_init();

#endif