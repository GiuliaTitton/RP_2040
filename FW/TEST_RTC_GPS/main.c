/**
 * 
 *
 * 
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <hardware/i2c.h>
#include "hardware/uart.h"
#include "hardware/irq.h"



//rtc constant
#define ONE_SECOND   1000000
#define TEN_SECONDS 10000000


//user inclusions

#include "mcp7940.h"
#include "pam7q.h"
#include <string.h>

//static int chars_rxed = 0;




static bool valid_sntnc=false;
static bool end_sentence=false;
uint8_t string_index = 0;
uint8_t commasCounter = 0;
uint8_t nmea_star_index;
uint8_t nmea_rcvd_sntnc=false;
uint8_t nmea_sntnc[MAX_NMEA_LEN];
uint8_t nmea_field =0;
uint32_t time_GPS;
uint32_t datum_GPS;
uint8_t index_data_gps;
uint8_t time_RTC;
uint8_t datum_RTC;

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
#if GPS_ENABLED
    /*UART*/

    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, BAUD_RATE);

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


#endif
/*/////////////////////////////////////*/

    stdio_init_all();

/*****************************************************/
    //led lightning
    for(uint8_t i=0; i<4;i++){
        gpio_put(LED_PIN, 1);  //led on
        sleep_ms(10);
        gpio_put(LED_PIN, 0);  //led off
        sleep_ms(90);       
    } 
/*************************************/
#if MCP7940_ENABLE
/*I2C INIT*/

    i2c_init(i2c0, 10 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  //  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  //  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

/***************************************/
/*mcp init*/
    uint8_t mcp_data_buf[MCP7940_DATA_NBYTES];
    uint8_t _second, _minute, _hour, _day, _weekday, _month, _year;  
    uint16_t current_year;

    uint64_t timestamp_RTC = time_us_64();
    mcp7940_set_all_data(15, 3, 20, 2, 24, 7, 21);
#endif   
    bool led_val=0;

    
    uint64_t led_dbg_period=200;
    uint64_t led_timestamp = time_us_64();
    uint64_t timestamp_correction_RTC = time_us_64();

   

/***************************************************/


    while (true) {
        if(led_dbg_period <=  time_us_64()- led_timestamp){ //LED TASK
                    led_timestamp = time_us_64();
                    led_val = !led_val; //toggle led val. ricordare led_val = false -> led ON! 

                    if(!led_val)
                        led_dbg_period = 75*1000; //800 ms led off
                    else led_dbg_period = 925*1000; //200ms on;

                    gpio_put(LED_PIN, led_val);
                    
            }

        //GPS SECTION
    #if GPS_ENABLED
        if(nmea_rcvd_sntnc){
            nmea_rcvd_sntnc=false;
            printf("Stringa originale: %s", nmea_sntnc);
           
            //time acquisition
            if(nmea_sntnc[MESSAGE_OFFSET_TIME] != ','){
                printf("Tempo acquisito: %i", time_GPS);
                time_GPS = getTime(nmea_sntnc[MESSAGE_OFFSET_TIME]);
                time_GPS_valid = true;
            }else{
                printf("Tempo non acquisito!\n");
                time_GPS_valid = false;
            }
            
            //data acquisition
            if(nmea_sntnc[index_data_gps] != ','){
                datum_GPS = getTime(nmea_sntnc[index_data_gps]);
                printf("Data acquisita: %i\n", datum_GPS);
                data_GPS_valid = true;
            }else{
                printf("Data non acquisita!\n");
                data_GPS_valid = false;
            }
            
            
        }
    #endif

    #if MCP7940_ENABLE & GPS_ENABLED
        //RTC SECTION
        if(ONE_SECOND <= time_us_64() - timestamp_RTC){ // ogni secondo (10^6 us..)
            timestamp_RTC = time_us_64();
            mcp7940_get_all_data(mcp_data_buf, MCP7940_SECS_REG, MCP7940_DATA_NBYTES);
            time_RTC = mcp7940_get_time(mcp_data_buf);
            datum_RTC = mcp7940_get_data(mcp_data_buf[4]);
            
            _weekday = mcp_data_buf[3]; //in BCD
            _weekday = (_weekday & 0x07);
            _day = BCD_to_num(mcp_data_buf[4], 4, 2);
            _month = BCD_to_num(mcp_data_buf[5], 4, 1);     
            _year = BCD_to_num(mcp_data_buf[6], 4, 4);
            current_year = START_MILLENNIUM  + _year; //100*_century
            
            printf("Orario: %i:%i:%i\nData: %s %i/%i/%i\n", _hour, _minute, _second, getDayName(_weekday), _day, _month, current_year);
            
            
        }

        //RTC TIME CORRECTION WITH GPS
        if(TEN_SECONDS <= time_us_64() - timestamp_correction_RTC){
            timestamp_correction_RTC = time_us_64();
            
            if((time_RTC != time_GPS) & time_GPS_valid & data_GPS_valid){
                if(datum_RTC != datum_GPS)
                    mcp7940_set_all_data(time_GPS%100, (time_GPS/100)%100,(time_GPS/10000),1,datum_GPS/10000,(datum_GPS/100)%100,datum_GPS%100);
                else
                    mcp7940_set_time(time_GPS%100, (time_GPS/100)%100,(time_GPS/10000));
            }

        }
    #endif
    }

}




void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        


        uint8_t ch=uart_getc(UART_ID);
        
        
//la prima frase può arrivare corrotta, iniziando da carattere diverso da $
//finché non ricevo il primo terminatore \r\n scarto tutti i caratteri. solo dalla seconda considero i dati ok.
        if(!valid_sntnc)
            if(ch=='$'){
                valid_sntnc=true;
                string_index=0;
                commasCounter = 0;
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
            if (commasCounter == COMMAS_BEFORE_DATUM){
                index_data_gps = string_index;
                commasCounter++;
                
            }
            if (',' == nmea_sntnc[string_index]){
                    commasCounter++;
            }
            //printf("passo dopo if virgola: %i\n", commasCounter);    

            if('*'==nmea_sntnc[string_index]){
                nmea_star_index=string_index;
                
            }

            //printf("passo prima di controllo fine stringa: %i\n", commasCounter);
            if('\n'==nmea_sntnc[string_index]){
                nmea_rcvd_sntnc=true;
                
            }
            //printf("passo dopo controllo fine stringa: %i\n", commasCounter);

            string_index++;

        }
        //printf("interno while %i\n", commasCounter);
    }
    //printf("interno funz: %i\n\n", commasCounter);
}



void gpio_callback(uint gpio, uint32_t events) {
    asm("nop");
}