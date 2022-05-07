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
#define BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE
// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1




//user inclusions

#include "mcp7940.h"
#include <string.h>




//user declarations

//static int chars_rxed = 0;
void on_uart_rx();

//NMEA constants
#define MESSAGE_OFFSET_TIME 7
#define COMMAS_BEFORE_DATUM 9
#define MAX_NMEA_LEN 82

static bool valid_sntnc=false;
static bool end_sentence=false;
uint8_t string_index = 0;
uint8_t nmea_star_index;
uint8_t nmea_rcvd_sntnc=false;
uint8_t nmea_sntnc[MAX_NMEA_LEN];
uint8_t nmea_field =0;
uint8_t gps_time_buf[11];
uint8_t gps_datum_buf[7];
uint8_t index_data_gps;





int main() {
/*************************************/
/*GPIO*/


    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);  //led off




/*************************************/
/*PPS IRQ*/
    void gpio_callback(uint gpio, uint32_t events); //callback  declaration

    gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);




/***********************************/
    /*UART*/

    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 9600);

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
    uint64_t led_dbg_period=200, led_timestamp;


    mcp_data_buf[0]= num_to_BCD(10,4); //sec
    mcp_data_buf[1]= num_to_BCD(43,4);//min
    mcp_data_buf[2]= num_to_BCD(16,4)&(~(1<<6));//hour + f. 24h

    mcp_data_buf[3]= num_to_BCD(5,4);//day
    mcp_data_buf[4]= num_to_BCD(3,4);//date
    mcp_data_buf[5]= num_to_BCD(5,4);//month
    mcp_data_buf[6]= num_to_BCD(22,4);//year

    //mcp7940_write_multiple_registers(MCP7940_SECS_REG, mcp_data_buf, sizeof(mcp_data_buf));

my_timestamp= time_us_64();

/***************************************************/
//gps task init



    while (true) {



/*
        if(led_dbg_period <=  time_us_64()- led_timestamp){ //LED TASK
            led_timestamp = time_us_64();
            led_val = !led_val; //toggle led val. ricordare led_val = false -> led ON! 

            if(led_val)
                led_dbg_period = 75*1000; //800 ms led off
            else led_dbg_period = 925*1000; //200ms on;

            gpio_put(LED_PIN, led_val);
            
        }*/



        if(nmea_rcvd_sntnc){
            nmea_rcvd_sntnc=false;
            gpio_put(25,1);  


///TODO: CHECK CHECKSUM
//      ESTRARRE ORA UTC

            
            //$GPRMC,152606.090,V,,,,,0.00,0.00,070522,,,N*40
            for(uint8_t j=0; j<10;j++){
                gps_time_buf[j] = nmea_sntnc[j + MESSAGE_OFFSET_TIME];
            }gps_time_buf[10]='\0';//terminatore di stringa

            printf("%s\n", gps_time_buf);

            for(uint8_t j=0; j<6;j++){
                gps_datum_buf[j] = nmea_sntnc[j+ index_data_gps];
            }gps_datum_buf[6]='\0';//terminatore di stringa
            
            printf("%s\n", gps_datum_buf);

        }


/*
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

            current_year = START_MILLENNIUM  + _year; //100*_century
            

            printf("%i:%i:%i, %S, %i/%i/%i\n", _second, _minute, _hour, today, current_year, _month, _day);
        }
*/
        //gpio_put(LED_PIN, led_val);
        
    }

}




void on_uart_rx() {


    while (uart_is_readable(UART_ID)) {
        


        uint8_t ch=uart_getc(UART_ID);
        uint8_t commasCounter = 0;
        
//la prima frase può arrivare corrotta, iniziando da carattere diverso da $
//finché non ricevo il primo terminatore \r\n scarto tutti i caratteri. solo dalla seconda considero i dati ok.
        if(!valid_sntnc)
            gpio_put(25, 0);
            if(ch=='$'){
                
                valid_sntnc=true;
                string_index=0;
                nmea_rcvd_sntnc=false;
        }//else first_sntnc=false;

        if(valid_sntnc){

            nmea_sntnc[string_index] = ch;

            if(3==string_index) 
                if('R'!= nmea_sntnc[string_index]) valid_sntnc=false;
            if(4==string_index)
                if('M'!= nmea_sntnc[string_index]) valid_sntnc=false; 
            if(5==string_index)
                if('C'!= nmea_sntnc[string_index]) valid_sntnc=false;

            //track datum
            if (commasCounter == COMMAS_BEFORE_DATUM)
                index_data_gps = string_index;

            if (',' == nmea_sntnc[string_index])
                commasCounter++;


            if('*'==nmea_sntnc[string_index]){
                nmea_star_index=string_index;
                
            }
            if('\n'==nmea_sntnc[string_index]){
                nmea_rcvd_sntnc=true;
                
            }

            string_index++;
        }
        
    }
}



void gpio_callback(uint gpio, uint32_t events) {
    asm("nop");

}