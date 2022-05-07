#ifndef __MCP7940_H__
#define __MCP7940_H__

#include <stdio.h>
#include <stdbool.h>



//nuova versione -- pagina datasheet 6 e 23
#define RTC_MCP7940_I2C_ADR 0x6F //7-bit address!
#define MCP7940_SECS_REG 0x00    //puntatore registro secondi
#define MCP7940_MINS_REG 0x01
#define MCP7940_HOURS_REG 0x02
#define MCP7940_DAY_REG 0x03
#define MCP7940_DATE_REG 0x04
#define MCP7940_MTH_REG 0x05
#define MCP7940_YEAR_REG 0x06
#define MCP7940_DATA_NBYTES 7 //SERVONO I REGISTRI DA 0x00 A 0x06
/*
00      ST  SECTEN2 SECTEN1 SECTEN0 SECONE3 SECONE2 SECONE1 SECONE0
01      RES MINTEN2 MINTEN1 MINTEN0 MINONE3 MINONE2 MINONE1 MINONE0
02      RES 12/n24  HTEN1   HTEN0   HONE3   HONE2   HONE1   HONE0
03
04
05
06
*/

#define START_MILLENNIUM 2

uint8_t mcp7940_get_all_data(uint8_t* data, uint8_t base_address, uint8_t n_bytes);

uint8_t mcp7940_write_single_register(uint8_t reg_address, uint8_t valueBCD);
uint8_t mcp7940_write_multiple_registers(uint8_t base_reg_address, uint8_t* buf, uint8_t nregs);
uint8_t mcp7940_add_hour_format(bool bool12Hours,uint8_t valueBCD);
uint8_t num_to_BCD(uint8_t value, uint8_t unitBits);
uint8_t BCD_to_num(uint8_t valueBCD, uint8_t unitBits, uint8_t decBits);
//__MCP7940_H__
#endif 
