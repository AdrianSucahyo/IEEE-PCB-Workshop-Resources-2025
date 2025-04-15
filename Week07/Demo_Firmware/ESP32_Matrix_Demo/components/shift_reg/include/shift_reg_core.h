#ifndef _MATRIX_CORE_H_
#define _MATRIX_CORE_H_

#include <stdio.h>

#include "esp_log.h"
#include "driver/gpio.h"
#include "soc/gpio_reg.h"
#include "soc/io_mux_reg.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TRANS_PIN_NUM_BITMASK(n) (n > 31) ? (1ULL << (n - 32)) : 1ULL << n
#define TRANS_PIN_NUM_W1TS(n) (n > 31) ? GPIO_OUT1_W1TS_REG : GPIO_OUT_W1TS_REG
#define TRANS_PIN_NUM_W1TC(n) (n > 31) ? GPIO_OUT1_W1TC_REG : GPIO_OUT_W1TC_REG
#define TRANS_PIN_NUM_IN(n) (n > 31) ? GPIO_IN1_REG : GPIO_IN_REG

esp_err_t shift_reg_core_enable();

esp_err_t shift_reg_core_disable();

esp_err_t shift_reg_core_write();

esp_err_t shift_reg_core_set(uint16_t data_word, uint16_t bitmask);

void shift_reg_core_init();

#endif