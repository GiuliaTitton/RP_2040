/*
 * Copyright (c) 2021 Sandeep Mistry
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include <stdbool.h>

#include "hardware/clocks.h"

#include "lwip/dhcp.h"
#include "lwip/init.h"

char mybuf [1460]; //massima dimensione payload http
  
#include "lwip/apps/httpd.h"

#include "rmii_ethernet/netif.h"


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
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);  //led off


    gpio_init(13);
    gpio_set_dir(13, GPIO_OUT);
    gpio_put(13, 1);  //led off



    for(uint8_t i=0; i<5;i++){
        gpio_put(LED_PIN, 1);  //led off
        sleep_ms(180);
        gpio_put(LED_PIN, 0);  //led on
        sleep_ms(20);       
    } 




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
        if(my_period <=  time_us_64()- my_timestamp){
            my_timestamp=time_us_64();

        
           // printf("hw:");
            //printf("%06x\n", netif.hwaddr);
           // printf("%08x\n", netif.ip_addr);
        }


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