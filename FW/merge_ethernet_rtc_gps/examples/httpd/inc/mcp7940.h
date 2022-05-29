/*
* FILENAME  :                      mcp7940.h             
* PROJECT   :   PSE_2021_2022_SERVER_NTP_LAN
* FOLDER    :          /FW/TEST_RTC_GPS/inc/
*
* DESCRIPTION :
*       Il file include la definizione dei registri e la dichiarazione delle 
*       funzioni utili per l'utilizzo dell'integrato MCP7940 . Sono incluse le
*       seguenti dichiarazioni:
*       - registri riferiti al tempo e alla data
*       - registri maschere per gli allarmi
*       - registri allarme 0 e 1
*       
* NOTES :
*       Funzionalità anno bisestile e allarmi mancanti.
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

#ifndef __MCP7940_H__
#define __MCP7940_H__

#include <stdio.h>
#include <stdbool.h>


//MCP7940 enabler
#define MCP7940_ENABLE 1

//registri valori tempo
#define RTC_MCP7940_I2C_ADR 0x6F //7-bit address!
#define MCP7940_SECS_REG 0x00    //puntatore registro secondi
#define MCP7940_MINS_REG 0x01
#define MCP7940_HOURS_REG 0x02
#define MCP7940_DAY_REG 0x03
#define MCP7940_DATE_REG 0x04
#define MCP7940_MTH_REG 0x05
#define MCP7940_YEAR_REG 0x06
#define MCP7940_CONTROL_REG 0x07
#define MCP7940_DATA_NBYTES 7


//maschere impostazione allarmi     !!! aggiunto lo zero per lo shift a sx !!!
#define MCP7940_ALM0_MASK_REG 0x0D
#define MCP7940_ALM1_MASK_REG 0x14

#define MCP7940_ALM_MASK_SEC 0x00
#define MCP7940_ALM_MASK_MINS 0x10  
#define MCP7940_ALM_MASK_HOURS 0x20
#define MCP7940_ALM_MASK_DAY 0x30
#define MCP7940_ALM_MASK_DATE 0x40
#define MCP7940_ALM_MASK_ALL 0x70

//registri allarme 0
#define MCP7940_ALM0_SECS_REG 0x0A    //puntatore registro secondi ALLARME 0 
#define MCP7940_ALM0_MINS_REG 0x0B
#define MCP7940_ALM0_HOURS_REG 0x0C
#define MCP7940_ALM0_DAY_REG 0x0D
#define MCP7940_ALM0_DATE_REG 0x0E
#define MCP7940_ALM0_MTH_REG 0x0F

//registri allarme 1
#define MCP7940_ALM1_SECS_REG 0x11    //puntatore registro secondi ALLARME 1
#define MCP7940_ALM1_MINS_REG 0x12
#define MCP7940_ALM1_HOURS_REG 0x13
#define MCP7940_ALM1_DAY_REG 0x14
#define MCP7940_ALM1_DATE_REG 0x15
#define MCP7940_ALM1_MTH_REG 0x16

//maschere abilitazione allarmi
#define MCP7940_ALM0_EN 0x10
#define MCP7940_ALM1_EN 0x20

#define START_MILLENNIUM 2000

/**
 * Returns the values
 * saved in RTC buffers
 *
 * @param data pointer to the array where we save values
 * @param base_address address of the register to start the reading
 * @param n_bytes number of registers to read
 */
uint8_t mcp7940_get_all_data(uint8_t* data, uint8_t base_address, uint8_t n_bytes);

/**
 * Writes a value
 * in a specific register
 *
 * @param reg_address address of the register to edit
 * @param valueBCD value to add in BCD format
 */
uint8_t mcp7940_write_single_register(uint8_t reg_address, uint8_t valueBCD);

/**
 * Writes values
 * in more registers
 *
 * @param base_reg_address address of the first value to edit
 * @param buf array of the values in BCD to insert
 * @param nregs number of registers to write
 */
uint8_t mcp7940_write_multiple_registers(uint8_t base_reg_address, uint8_t* buf, uint8_t nregs);

/**
 * Formats the hour value already in BCD
 * with 12/24 H configuration
 *
 * @param bool12Hours set 12/24 H format: HIGH for 12, LOW for 24
 * @param valueBCD value to add in BCD format
 */
uint8_t mcp7940_add_hour_format(bool bool12Hours,uint8_t valueBCD);

/**
 * Enables the mcp7940 to start counting.
 * Requires the seconds value due to register address shared
 *
 * @param secondsBCD value in BCD of seconds to insert
 */
void mcp7940_start_count(uint8_t secondsBCD);

/**
 * Convertion from normal values to BCD
 *
 * @param value value to convert
 * @param unitBits number of digits for unit part, necessary for masks
 */
uint8_t num_to_BCD(uint8_t value, uint8_t unitBits);

/**
 * Convertion from BCD to normal values
 *
 * @param valueBCD value to convert
 * @param unitBits number of digits for unit part, necessary for masks
 */
uint8_t BCD_to_num(uint8_t valueBCD, uint8_t unitBits, uint8_t decBits);

/**
 * Writes normal values
 * in all registers
 *
 * @param secs seconds
 * @param min minutes
 * @param hour hours
 * @param day day of the week (no constains, just a counter): sunday = 0, monday = 1, ...
 * @param date number of day
 * @param month month
 * @param year year in range 0 to 99
 */
void mcp7940_set_all_data(uint8_t secs, uint8_t min, uint8_t hour, uint8_t day, uint8_t date, uint8_t month, uint8_t year);

/**
 * Writes values of time
 *
 * @param secs seconds
 * @param min minutes
 * @param hour hours
 */
void mcp7940_set_time(uint8_t secs, uint8_t min, uint8_t hour);

/**
 * Returns the string related with day
 * Requires the seconds value due to register address shared
 *
 * @param secondsBCD value in BCD of day of the week
 */
const char* getDayName(uint8_t day);

/**
 * Returns an integer with
 * time formatted as: HourHourMinuteMinuteSecondSecond
 *
 * @param buf array pointer of seconds, minutes and hours
 */
uint32_t mcp7940_get_time(uint8_t* buf);

/**
 * Returns an integer with date 
 * formatted as: DayDayMonthMonthYearYear,
 * where the value of YearYear is from 0 to 99
 *
 * @param buf array of values 
 * @param offset offset used only if we provide bigger arrays with other values like seconds, ...
 */
uint32_t mcp7940_get_data(uint8_t* buf, uint8_t offset);

/**
 * Sets and enables an allarm
 *
 * @param allarmNumber number of allarm to enable (0 or 1)
 * @param allarmMask sets the type of allarm to enable
 * @param reg_address register address of value to write
 * @param value_BCD value in BCD format compared to internal clock and calendar
 * @param interruptHighLow defines type of interrupt: HIGH for high state interrupt and LOW for low state interrupt
 * 
 * @return 1 if completed without errors, otherwise 0
 */
uint8_t mcp7940_set_allarm(uint8_t allarmNumber, uint8_t allarmMask, uint8_t reg_address, uint8_t valueBCD, bool interruptHighLow);
//__MCP7940_H__
#endif 
