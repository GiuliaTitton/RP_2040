#ifndef __PCF8563_H__
#define __PCF8563_H__

#include <stdio.h>


#define RTC_PCF8563_I2C_ADR 0x51 //7-bit address!
#define PCF8563_VL_SECS_REG 0x02
#define PCF_DATA_NBYTES 7

#define START_MILLENNIUM 2




uint8_t pcf8563_get_all_data(uint8_t* data, uint8_t base_address, uint8_t n_bytes);



#endif //__PCF8563_H__
