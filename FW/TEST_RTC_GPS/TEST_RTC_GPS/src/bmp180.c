#include "bmp180.h"
#include <hardware/i2c.h>



uint8_t bmp_180_getID(){
    
    uint8_t reg_bmp_id = 0xD0;
    uint8_t chip_read_id[1];

            i2c_write_blocking(i2c0,0x77,&reg_bmp_id, 1, true);
            i2c_read_blocking(i2c0, 0x77,chip_read_id,1,false);

            return chip_read_id[0];

     
}
