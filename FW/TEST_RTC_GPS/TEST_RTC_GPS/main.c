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
#include "pcf8563_h.h"














int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);  //led off

    stdio_init_all();




    i2c_init(i2c0, 10 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  //  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  //  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    for(uint8_t i=0; i<4;i++){
        gpio_put(LED_PIN, 1);  //led on
        sleep_ms(10);
        gpio_put(LED_PIN, 0);  //led off
        sleep_ms(90);       
    } 

    //uint8_t id =4;


    uint8_t pcf_data_buf[PCF_DATA_NBYTES];
    uint8_t _second, _minute, _hour, _day, _weekday, _month, _year, _century; uint16_t current_year;
    enum wd {SUN=0, MON, TUE, WED, THU, FRI, SAT} today;  
    
    bool led_val=0;

    uint64_t my_timestamp= time_us_64();
    while (true) {

        /*id = bmp_180_getID(); //prova comunicazione con bmp180
        printf("chip id: 0x%02X\n", id);
        sleep_ms(750);*/

        if(1000*1000 == time_us_64()- my_timestamp){
            
            led_val = !led_val;

            my_timestamp = time_us_64();
            pcf8563_get_all_data(pcf_data_buf, PCF8563_VL_SECS_REG, PCF_DATA_NBYTES);
            _second = pcf_data_buf[0]; //in BCD
            _second = ((_second>>4) & 0x07)*10 + (_second & 0x0F);

            _minute = pcf_data_buf[1]; //in BCD
            _minute = ((_minute>>4) & 0x07)*10 + (_minute & 0x0F);

            _hour = pcf_data_buf[2]; //in BCD
            _hour = ((_hour>>4) & 0x03)*10 + (_hour & 0x0F);

            _day = pcf_data_buf[3]; //in BCD
            _day = ((_day>>4) & 0x03)*10 + (_day & 0x0F);

            _weekday = pcf_data_buf[4]; //in BCD
            _weekday = (_weekday & 0x07);
            today = _weekday;

            _month = pcf_data_buf[5]; //in BCD
            _month = ((_month>>4) & 0x01)*10 + (_month & 0x0F);               
            
            _century = pcf_data_buf[5]; //in BCD
            _century = ((_century>>7) & 0x1F) ; //pcf Ritorna solo 0 o 1 se passato il secolo

            _year = pcf_data_buf[6]; //in BCD su 8bit, da 0 a 99 => nessuna conversione

            current_year =1000 * START_MILLENNIUM + 100*_century + _year;

            printf("%i:%i:%i, %S, %i/%i/%i\n", _second, _minute, _hour, today, current_year, _month, _day);
        }

        gpio_put(LED_PIN, led_val);
        
    }
#endif
}
