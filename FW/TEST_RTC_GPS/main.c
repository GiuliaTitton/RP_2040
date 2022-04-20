/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *THIS CODE COMES FROM "EXAMPLES" FOLDER. ONLY ADDED stdio and printf
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include <hardware/i2c.h>
#include <hardware/uart.h>

#include "file.h"
#include "bmp180.h"















int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);


    stdio_init_all();




    i2c_init(i2c0, 10 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  //  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  //  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
uint8_t id =4;

    while (true) {

        id = bmp_180_getID();
        printf("chip id: 0x%02X\n", id);
        sleep_ms(750);
    }
#endif
}
