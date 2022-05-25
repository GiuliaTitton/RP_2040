/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *THIS CODE COMES FROM "EXAMPLES" FOLDER. ONLY ADDED stdio and printf
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <hardware/i2c.h>




//user inclusions

#include "mcp7940.h"
#include <string.h>

int main() {
/*************************************/
/*GPIO*/

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);  //led off
    stdio_init_all();

/*************************************/
/*I2C INIT*/

    i2c_init(i2c0, 10 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  //  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  //  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

/*****************************************************/

    for(uint8_t i=0; i<4;i++){
        gpio_put(LED_PIN, 1);  //led on
        sleep_ms(10);
        gpio_put(LED_PIN, 0);  //led off
        sleep_ms(90);       
    } 

/***************************************/
/*mcp init*/



    uint8_t mcp_data_buf[MCP7940_DATA_NBYTES];
    uint8_t _second, _minute, _hour, _day, _weekday, _month, _year; //, _century; 
    uint16_t current_year;
    enum wd {SUN=0, MON, TUE, WED, THU, FRI, SAT} today;  
    
    bool led_val=0;

    uint64_t my_timestamp= time_us_64();
    uint64_t led_dbg_period=200;
    uint64_t led_timestamp = time_us_64();


    mcp_data_buf[0]= num_to_BCD(10,4); //sec
    mcp_data_buf[1]= num_to_BCD(43,4);//min
    mcp_data_buf[2]= mcp7940_add_hour_format(false, num_to_BCD(16,4));//hour + f. 24h

    mcp_data_buf[3]= num_to_BCD(5,4);//day
    mcp_data_buf[4]= num_to_BCD(3,4);//date
    mcp_data_buf[5]= num_to_BCD(5,4);//month
    mcp_data_buf[6]= num_to_BCD(22,4);//year

    mcp7940_write_multiple_registers(MCP7940_SECS_REG, mcp_data_buf, sizeof(mcp_data_buf));

my_timestamp= time_us_64();

/***************************************************/
//toggle led 

    while (true) {
        if(led_dbg_period <=  time_us_64()- led_timestamp){ //LED TASK
            led_timestamp = time_us_64();
            led_val = !led_val; //toggle led val. ricordare led_val = false -> led ON! 

            if(!led_val)
                led_dbg_period = 75*1000; //800 ms led off
            else led_dbg_period = 925*1000; //200ms on;

            gpio_put(LED_PIN, led_val);
            
        }




        if(1000*1000 <= time_us_64()- my_timestamp){ // ogni secondo (10^6 us..)
        

            my_timestamp = time_us_64();
            mcp7940_get_all_data(mcp_data_buf, MCP7940_SECS_REG, MCP7940_DATA_NBYTES);
            

            _second = BCD_to_num(mcp_data_buf[0], 4,3);

            _minute = BCD_to_num(mcp_data_buf[1], 4,3);

            _hour = BCD_to_num(mcp_data_buf[2], 4,3);

            _weekday = mcp_data_buf[3]; //in BCD
            _weekday = (_weekday & 0x07);
            today = _weekday;

            _day = BCD_to_num(mcp_data_buf[4], 4, 2);

            _month = BCD_to_num(mcp_data_buf[5], 4, 1);     

            
            _year = BCD_to_num(mcp_data_buf[6], 4, 4);

            current_year = START_MILLENNIUM  + _year; //100*_century
            
            mcp7940_write_single_register(MCP7940_YEAR_REG,num_to_BCD(22,4));
            printf("Orario%i:%i:%i\nData:%i/%i/%i\n", _hour, _minute, _second, _weekday, _day, _month, current_year);
        }

        //gpio_put(LED_PIN, led_val);
        
    }

}





