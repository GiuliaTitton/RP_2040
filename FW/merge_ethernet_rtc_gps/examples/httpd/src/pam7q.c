/*
* FILENAME  :                        pam7q.c             
* PROJECT   :   PSE_2021_2022_SERVER_NTP_LAN
* FOLDER    :          /FW/TEST_RTC_GPS/src/
*
* DESCRIPTION :
*       Il file include la definizione delle funzioni utili
*       per l'utilizzo dell'integrato PAM7Q.
*
* PUBLIC FUNCTIONS :
*       uint32_t    getTime
*       uint32_t    getData
*       void        gpio_callback
*       
* NOTES :
*       Importare void on_uart_rx() da main.c
*       Implementare interrupt su PPS
*
* AUTHORS :    
*       PAGANIN ANDREA, PASQUINI GIUSEPPE, TRENTI ELIA, TITTON GIULIA, AMORTH MATTEO;
*
* CHANGES :
*
* REF NO  VERSION DATE    WHO     DETAIL
* F26/05  A.00.01 26May22 M. A.   Aggiunta intestazione
*
*/
#include "pam7q.h"

uint32_t getTime(uint8_t* gps_time_buf, uint8_t offset){
    return  (gps_time_buf[(0 + offset)]-48) * 100000 +
            (gps_time_buf[(1 + offset)]-48) * 10000  +
            (gps_time_buf[(2 + offset)]-48) * 1000   +
            (gps_time_buf[(3 + offset)]-48) * 100    +
            (gps_time_buf[(4 + offset)]-48) * 10     +
            (gps_time_buf[(5 + offset)]-48)          +//indice 6: punto
            (gps_time_buf[(7 + offset)]-48) * 0.1    +
            (gps_time_buf[(8 + offset)]-48) * 0.01   +
            (gps_time_buf[(9 + offset)]-48) * 0.001  ;
}

uint32_t getData(uint8_t* gps_datum_buf, uint8_t offset){
    return              (gps_datum_buf[(0 + offset)]-48) * 100000 +
                        (gps_datum_buf[(1 + offset)]-48) * 10000  +
                        (gps_datum_buf[(2 + offset)]-48) * 1000   +
                        (gps_datum_buf[(3 + offset)]-48) * 100    +
                        (gps_datum_buf[(4 + offset)]-48) * 10     +
                        (gps_datum_buf[(5 + offset)]-48)          ;
}

void gpio_callback(uint gpio, uint32_t events) {

    asm("nop");

}
