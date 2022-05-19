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


uint8_t mcp7940_write_single_register(uint8_t reg_address, uint8_t valueBCD){

    // buf[0] is the register to write to
    // buf[1] is the value that will be written to the register
    uint8_t buf[2];

    buf[0] = reg_address;
    buf[1] = valueBCD;
    if(PICO_ERROR_GENERIC == i2c_write_blocking(i2c0, RTC_MCP7940_I2C_ADR, buf, 2, false)) //send stop after write op
        return 0; //write fail
    
    else return 1;//write ok
    
}


uint8_t mcp7940_write_multiple_registers(uint8_t base_reg_address, uint8_t* buf, uint8_t nregs){
   
    uint8_t buf_tx[nregs + 1]; //buffer da trasmettere, formato [base_addr, data0, data1, .... datanregs-1]

    buf_tx[0]=base_reg_address;
    for(uint8_t i=0; i<nregs; i++){ //copio buffer passato in quello di trasmissione
        buf_tx[i+1]=buf[i];
    }

    if(PICO_ERROR_GENERIC == i2c_write_blocking(i2c0, RTC_MCP7940_I2C_ADR, buf_tx, sizeof(buf_tx), false)) //send stop after write op
        return 0; //write fail
    
    else return 1; //write ok    
}



uint8_t mcp7940_add_hour_format(bool bool12Hours,uint8_t valueBCD){

    if(bool12Hours)
        return valueBCD || 0x40;
    return valueBCD;
}

uint8_t num_to_BCD(uint8_t value, uint8_t unitBits){
    return ((value/10)<< unitBits) | ((value % 10) & 0x0F);
}

uint8_t BCD_to_num(uint8_t valueBCD, uint8_t unitBits, uint8_t decBits){
    return (((valueBCD>>unitBits) & ((0x0F >> (4 - decBits))))*10 + (valueBCD & 0x0F));
}

uint8_t set_allarm(uint8_t allarmNumber, uint8_t allarmMask, uint8_t reg_address, uint8_t valueBCD, bool interruptHighLow){
    uint8_t errorTrack = 1;
    uint8_t temp;
    bool skip = 0;

    if(allarmNumber)
        temp = MCP7940_ALM1_MASK_REG;
    else
        temp = MCP7940_ALM0_MASK_REG;

    //eccezione timer giorno settimana
    if(reg_address == temp){
        allarmMask = allarmMask | valueBCD;
        skip = 1;
    }
    //set interrupt H/L + flag cancellato
    if (interruptHighLow)
        allarmMask = allarmMask | 0x80;

    //set allarm mask: definisce la tipologia del timer/calendario
    errorTrack *= mcp7940_write_single_register(temp, allarmMask);

    //eccezione timer giorno della settimana
    if(!skip)
        errorTrack *= mcp7940_write_single_register(reg_address, valueBCD);
    
    //abilitazione timer
    if(allarmNumber)
        temp = MCP7940_ALM1_EN;
    else
        temp = MCP7940_ALM0_EN;

    return errorTrack * mcp7940_write_single_register(MCP7940_CONTROL_REG, temp);

    
}