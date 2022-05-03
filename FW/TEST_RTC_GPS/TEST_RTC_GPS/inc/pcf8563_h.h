#ifndef __PCF8563_H__
#define __PCF8563_H__

#include <stdio.h>

/*versione precedente
#define RTC_PCF8563_I2C_ADR 0x51 //7-bit address!
#define PCF8563_VL_SECS_REG 0x02
*/

//nuova versione -- pagina datasheet 6 e 23
#define RTC_PCF8563_I2C_ADR_WRITE 0xA3 //7-bit address!
#define RTC_PCF8563_I2C_ADR_READ 0xA2
#define PCF8563_VL_SECS_REG 0x02    //puntatore registro secondi
#define PCF_DATA_NBYTES 7

#define START_MILLENNIUM 2




uint8_t pcf8563_get_all_data(uint8_t* data, uint8_t base_address, uint8_t n_bytes);



#endif //__PCF8563_H__
