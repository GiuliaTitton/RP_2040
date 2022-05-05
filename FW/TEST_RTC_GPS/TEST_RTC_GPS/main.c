/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *THIS CODE COMES FROM "EXAMPLES" FOLDER. ONLY ADDED stdio and printf
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include <hardware/i2c.h>
#include "hardware/uart.h"
#include "hardware/irq.h"


/// \tag::uart_advanced[]

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE
// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1



//user inclusions

#include "mcp7940.h"




//user declarations

static int chars_rxed = 0;
void on_uart_rx();






int main() {
/*************************************/
/*GPIO*/


    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);  //led off


void gpio_callback(uint gpio, uint32_t events); //callback  declaration



/*************************************/
/*PPS IRQ*/

    gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    void gpio_callback(uint gpio, uint32_t events);


/***********************************/
    /*UART*/

    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 2400);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

/*/////////////////////////////////////*/

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
/*pcf init*/



    uint8_t mcp_data_buf[MCP7940_DATA_NBYTES];
    uint8_t _second, _minute, _hour, _day, _weekday, _month, _year; //, _century; 
    uint16_t current_year;
    enum wd {SUN=0, MON, TUE, WED, THU, FRI, SAT} today;  
    
    bool led_val=0;

    uint64_t my_timestamp= time_us_64();


    mcp_data_buf[0]= num_to_BCD(10,4); //sec
    mcp_data_buf[1]= num_to_BCD(43,4);//min
    mcp_data_buf[2]= num_to_BCD(16,4)&(~(1<<6));//hour + f. 24h

    mcp_data_buf[3]= num_to_BCD(5,4);//day
    mcp_data_buf[4]= num_to_BCD(3,4);//date
    mcp_data_buf[5]= num_to_BCD(5,4);//month
    mcp_data_buf[6]= num_to_BCD(22,4);//year

    mcp7940_write_multiple_registers(MCP7940_SECS_REG, mcp_data_buf, sizeof(mcp_data_buf));




    while (true) {


        if(1000*1000 == time_us_64()- my_timestamp){ // ogni secondo (10^6 us..)
            
            led_val = !led_val; //toggle led value

            my_timestamp = time_us_64();
            //mcp7940_write_single_register(MCP7940_SECS_REG, 45);
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

            current_year = START_MILLENNIUM + /*100*_century*/ + _year;

            printf("%i:%i:%i, %S, %i/%i/%i\n", _second, _minute, _hour, today, current_year, _month, _day);
        }

        gpio_put(LED_PIN, led_val);
        
    }

}




void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        // Can we send it back?
        if (uart_is_writable(UART_ID)) {
            // Change it slightly first!
            ch++;
            uart_putc(UART_ID, ch);
        }
        chars_rxed++;
    }
}



void gpio_callback(uint gpio, uint32_t events) {
    asm("nop");

}