#include "mcp7940.h"
#include <hardware/i2c.h>


uint8_t mcp7940_get_all_data(uint8_t* data, uint8_t base_address, uint8_t n_bytes){
 
    if(PICO_ERROR_GENERIC == i2c_write_blocking(i2c0, RTC_MCP7940_I2C_ADR, &base_address, 1, true)) //don't send stop after write op
        return 0;
    else {
        i2c_read_blocking(i2c0, RTC_MCP7940_I2C_ADR, data, n_bytes, false); //send stop after reading
        return 1;
    }
}


uint8_t mcp7940_write_secs(uint8_t reg_address, uint8_t secs){

    // buf[0] is the register to write to
    // buf[1] is the value that will be written to the register
    uint8_t buf[2];

    buf[0] = reg_address;
    buf[1] = secs;
    if(PICO_ERROR_GENERIC == i2c_write_blocking(i2c0, RTC_MCP7940_I2C_ADR, buf, 2, false)) //send stop after write op
        return 0;
    
    else return 1;
    
}