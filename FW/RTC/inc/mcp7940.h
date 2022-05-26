#ifndef __MCP7940_H__
#define __MCP7940_H__

#include <stdio.h>
#include <stdbool.h>




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
#define MCP7940_ALM0_MASK_REG 0x0D
#define MCP7940_ALM1_MASK_REG 0x14

//maschere impostazione allarmi     !!! aggiunto lo zero per lo shift a sx !!!
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


//SERVONO I REGISTRI DA 0x00 A 0x06
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
uint8_t BCD_to_num(uint8_t valueBCD, uint8_t unitBits, uint8_t decBits);
//__MCP7940_H__
#endif 
