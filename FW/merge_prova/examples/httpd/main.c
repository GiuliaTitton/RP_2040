/**
* FILENAME  :                         main.c             
* PROJECT   :   PSE_2021_2022_SERVER_NTP_LAN
* FOLDER    :               /FW/TEST_RTC_GPS
*
* DESCRIPTION :
*       Il programma acquisisce l'orario di GPS e RTC e ne confronta i valori.
*       In caso di valori differenti, si reimposta l'orario del RTC con i 
*       valori acquisiti via GPS. Il programma si suddivide in:
*       - setup led GPIO per feedback visivo;
*       - setup interrupt e interfaccia uart GPS;
*       - lampeggio led;
*       - setup I2C e orario iniziale RTC;
*       - impostazione allarme;
*       - loop continuo composto da:
*           - lampeggio led;
*           - acquisizione dati GPS;
*           - acquisizione dati RTC;
*           - confronto dati GPS e RTC;
*       - controllo messaggi in ricezione GPS
*
* NOTES :
*       Allarme non impostato. L'allarme può essere 
*       utilizzato per il confronto dei dati acquisiti.
*
*
* AUTHORS :    
*       PAGANIN ANDREA, PASQUINI GIUSEPPE, TRENTI ELIA, TITTON GIULIA, AMORTH MATTEO;
*
* CHANGES :
*
* REF NO  VERSION DATE    WHO     DETAIL
* F26/05  A.00.01 26May22 M. A.   Aggiunte descrizioni funzioni e intestazione
*
* */

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include <stdbool.h>

#include "hardware/clocks.h"

#include "lwip/dhcp.h"
#include "lwip/init.h"

#include <stdio.h>
#include <stdlib.h>
#include <hardware/i2c.h>
#include "hardware/uart.h"
#include "hardware/irq.h"

//user inclusions
#include "mcp7940.h"
#include "pam7q.h"
#include <string.h>



//loop constants
#define ONE_SECOND   1000000
#define TEN_SECONDS 10000000
#define SET_ALLARM         0



  
#include "lwip/apps/httpd.h"

#include "rmii_ethernet/netif.h"


//var
uint8_t string_index = 0;
uint8_t commasCounter = 0;
uint8_t nmea_star_index;
uint8_t nmea_rcvd_sntnc=false;
uint8_t nmea_sntnc[MAX_NMEA_LEN];
uint8_t nmea_field =0;
uint8_t index_data_gps;

void on_uart_rx();


void netif_link_callback(struct netif *netif)
{
    //printf("A:netif link status changed %s\n", netif_is_link_up(netif) ? "up" : "down");
    asm("nop");
}


void netif_status_callback(struct netif *netif)
{
    //printf("netif status changed %s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
   ///printf("lol");
}



const uint LED_PIN = PICO_DEFAULT_LED_PIN;
void led_task(void);


/*******************************/
uint64_t my_period=1000*1000, my_timestamp;
 

  



int main() {
/*************************************/
    /*GPIO*/
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);  //led off
	
	
	    gpio_init(13);
    gpio_set_dir(13, GPIO_OUT);
    gpio_put(13, 1);  //led off
/*************************************/

#if GPS_ENABLE
/***********************************/
/*var*/
    uint32_t time_GPS;
    uint32_t datum_GPS;
    bool time_GPS_valid = false;
    bool data_GPS_valid = false;

/*PPS IRQ*/
    void gpio_callback(uint gpio, uint32_t events); //callback  declaration

    gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

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

    stdio_init_all();

/*************************************/
    //led flashing
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
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); //  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); //  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  
/*mcp init*/
    uint8_t mcp_data_buf[MCP7940_DATA_NBYTES];
    uint64_t timestamp_RTC = time_us_64();
    uint64_t timestamp_correction_RTC = time_us_64();
    uint32_t time_RTC;
    uint32_t datum_RTC;

    mcp_data_buf[0]= num_to_BCD(10,4); //sec
    mcp_data_buf[1]= num_to_BCD(43,4);//min
    mcp_data_buf[2]= mcp7940_add_hour_format(false, num_to_BCD(16,4));//hour + f. 24h

    mcp_data_buf[3]= num_to_BCD(5,4);//day
    mcp_data_buf[4]= num_to_BCD(3,4);//date
    mcp_data_buf[5]= num_to_BCD(5,4);//month
    mcp_data_buf[6]= num_to_BCD(22,4);//year

    mcp7940_write_multiple_registers(MCP7940_SECS_REG, mcp_data_buf, sizeof(mcp_data_buf));
    mcp7940_write_single_register(MCP7940_SECS_REG,mcp_data_buf[0] | 0x80);
#endif   
    bool led_val=0;
    uint64_t led_dbg_period=200;
    uint64_t led_timestamp = time_us_64();








    // LWIP network interface
    struct netif netif;

    //
    struct netif_rmii_ethernet_config netif_config = {
        pio0, // PIO:            0
        0,    // pio SM:         0 and 1
        6,    // rx pin start:   6, 7, 8    => RX0, RX1, CRS
        10,   // tx pin start:   10, 11, 12 => TX0, TX1, TX-EN
        14,   // mdio pin start: 14, 15   => ?MDIO, MDC
        NULL, // MAC address (optional - NULL generates one based on flash id) 
    };

    // change the system clock to use the RMII reference clock from pin 20
    clock_configure_gpin(clk_sys, 20, 50 * MHZ, 50 * MHZ);
    sleep_ms(100);

    // initialize stdio after the clock change
    stdio_init_all();


    sleep_ms(5000);
    
    printf("pico rmii ethernet - httpd\n");

    // initilize LWIP in NO SYS mode
    lwip_init();

    // initialize the PIO base RMII Ethernet network interface
    netif_rmii_ethernet_init(&netif, &netif_config);
    


    // assign callbacks for link and status
    netif_set_link_callback(&netif, netif_link_callback);
    netif_set_status_callback(&netif, netif_status_callback);

    // set the default interface and bring it up
    netif_set_default(&netif);
    netif_set_up(&netif);

    // Start DHCP client and httpd


    dhcp_start(&netif);

    httpd_init();  // @elia Avvia il server HTTP vedere se si può sostituire o aggiungere il client !


    // setup core 1 to monitor the RMII ethernet interface
    // this let's core 0 do other things :)
    multicore_launch_core1(netif_rmii_ethernet_loop);
    
    while (1) {

        /*
        static __always_inline void tight_loop_contents	(void )	

        Np-op function intended to be called by any tight hardware polling loop.
         Using this ubiquitously makes it much easier to find tight loops, 
         but also in the future #ifdef-ed support for lockup debugging might be added
        */
        tight_loop_contents(); // @elia Capire cos'è !!!


        led_task();
    //GPS SECTION
    #if GPS_ENABLE
        if(nmea_rcvd_sntnc){
            nmea_rcvd_sntnc=false;
            printf("Stringa originale: %s", nmea_sntnc);
           
            //time acquisition
            if(nmea_sntnc[MESSAGE_OFFSET_TIME] != ','){
                printf("Tempo acquisito: %i\n", time_GPS);
                time_GPS = getTime(nmea_sntnc, MESSAGE_OFFSET_TIME);
                time_GPS_valid = true;
            }else{
                printf("Tempo non acquisito!\n");
                time_GPS_valid = false;
            }
            
            //data acquisition
            if(nmea_sntnc[index_data_gps] != ','){
                datum_GPS = getData(nmea_sntnc, index_data_gps);
                printf("Data acquisita: %i\n", datum_GPS);
                data_GPS_valid = true;
            }else{
                printf("Data non acquisita!\n");
                data_GPS_valid = false;
            }   
        }
    #endif
    //RTC SECTION
    #if MCP7940_ENABLE
        if(ONE_SECOND <= time_us_64() - timestamp_RTC){
            timestamp_RTC = time_us_64();
            mcp7940_get_all_data(mcp_data_buf, MCP7940_SECS_REG, MCP7940_DATA_NBYTES);
            time_RTC = mcp7940_get_time(mcp_data_buf);
            datum_RTC = mcp7940_get_data(mcp_data_buf, 4);
            
            printf("\nOrario RTC: %i\nData RTC: %s %i\n\n", time_RTC, getDayName(mcp_data_buf[3] & 0x07), datum_RTC);
        }
    #endif

    //RTC TIME CORRECTION WITH GPS
    #if MCP7940_ENABLE & GPS_ENABLE
        if(TEN_SECONDS <= time_us_64() - timestamp_correction_RTC){
            printf("Pre-Correzione tempo\n");
            timestamp_correction_RTC = time_us_64();
            
            if((time_RTC != time_GPS) & time_GPS_valid & data_GPS_valid){
                printf("\n\n\n-----Correzione tempo-----\n\n\n");
                if(datum_RTC != datum_GPS)
                    mcp7940_set_all_data(time_GPS%100, (time_GPS/100)%100,(time_GPS/10000),
                                         1,datum_GPS/10000,(datum_GPS/100)%100,datum_GPS%100);
                else
                    mcp7940_set_time(time_GPS%100, (time_GPS/100)%100,(time_GPS/10000));
            }
        }
    #endif

        


    }

    return 0;
}

uint64_t led_dbg_period=200, led_timestamp;
bool led_val;

void led_task(void){
        if(led_dbg_period <=  time_us_64()- led_timestamp){ //LED TASK
        led_timestamp = time_us_64();
        led_val = !led_val; //toggle led val. ricordare led_val = false -> led ON! 

        if(led_val)
            led_dbg_period = 925*1000; //800 ms led off
        else {
            led_dbg_period = 75*1000; //200ms on;
            
        }
        gpio_put(LED_PIN, led_val);
    }
}
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {

    uint8_t ch=uart_getc(UART_ID);
        
//la prima frase può arrivare corrotta, iniziando da carattere diverso da $
//finché non ricevo il primo terminatore \r\n scarto tutti i caratteri. solo dalla seconda considero i dati ok.
        if(!valid_sntnc)
         asm("nop");
        if(ch=='$'){
                
                valid_sntnc=true;
                string_index=0;
                commasCounter = 0;
                nmea_rcvd_sntnc=false;
        
        }
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
