#ifndef __PAM7Q_H__
#define __PAM7Q_H__

#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE
// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

//NMEA constants
#define MESSAGE_OFFSET_TIME 7
#define COMMAS_BEFORE_DATUM 9
#define MAX_NMEA_LEN 82

//gps enable

#define GPS_ENABLED 1


void on_uart_rx();
uint32_t getTime(uint8_t* gps_time_buf, uint8_t offset);
uint32_t getData(uint8_t* gps_datum_buf, uint8_t offset);
#endif