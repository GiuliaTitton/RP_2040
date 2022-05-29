/*
* FILENAME  :                        pam7q.h             
* PROJECT   :   PSE_2021_2022_SERVER_NTP_LAN
* FOLDER    :          /FW/TEST_RTC_GPS/inc/
*
* DESCRIPTION :
*       Il file include la definizione dei parametri per la comunicazione UART 
*       e le costanti legate alle stringhe fornite dal protocollo NMEA. Infine
*       dichiarazione funzioni per l'estrazione dei dati.
*       
* NOTES :
*       Importare variabili legate a void on_uart_rx() da main.c
*
* AUTHORS :    
*       PAGANIN ANDREA, PASQUINI GIUSEPPE, TRENTI ELIA, TITTON GIULIA, AMORTH MATTEO;
*
* CHANGES :
*
* REF NO  VERSION DATE    WHO     DETAIL
* F26/05  A.00.01 26May22 M. A.   Aggiunte descrizioni funzioni e intestazione
*
*/
#ifndef __PAM7Q_H__
#define __PAM7Q_H__

#include "hardware/uart.h"

//gps enabler
#define GPS_ENABLE 1

//uart parameters
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

/*static uint8_t string_index = 0;
static uint8_t commasCounter = 0;
static uint8_t nmea_star_index;
static uint8_t nmea_rcvd_sntnc=false;
static uint8_t nmea_sntnc[MAX_NMEA_LEN];
static uint8_t nmea_field =0;
static uint8_t index_data_gps;*/

static bool valid_sntnc=false;
static bool end_sentence=false;

//function declarations
void on_uart_rx();

/**
 * Returns an integer with
 * time formatted as: HourHourMinuteMinuteSecondSecond
 *
 * @param gps_time_buf address of the register to start the reading
 * @param offset offset used only if we provide bigger arrays with other values like seconds, ...
 */
uint32_t getTime(uint8_t* gps_time_buf, uint8_t offset);

/**
 * Returns an integer with date 
 * formatted as: DayDayMonthMonthYearYear,
 * where the value of YearYear is from 0 to 99
 *
 * @param gps_datum_buf array of values 
 * @param offset offset used only if we provide bigger arrays with other values like seconds, ...
 */
uint32_t getData(uint8_t* gps_datum_buf, uint8_t offset);

/**
 * Not implemented!
 *
 *
 *
 */
void gpio_callback(uint gpio, uint32_t events);
#endif