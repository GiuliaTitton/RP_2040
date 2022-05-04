#ifndef __MCP7940_H__
#define __MCP7940_H__

#include <stdio.h>



//nuova versione -- pagina datasheet 6 e 23
#define RTC_MCP7940_I2C_ADR 0x6F //7-bit address!
#define MCP7940_SECS_REG 0x00    //puntatore registro secondi
#define PCF_DATA_NBYTES 7 //SERVONO I REGISTRI DA 0x00 A 0x06
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




uint8_t mcp7940_write_secs(uint8_t reg_address, uint8_t secs);


#endif //__PCF8563_H__
