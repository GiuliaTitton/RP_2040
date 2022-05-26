#include "pam7q.h"

uint8_t getTime(uint8_t* gps_time_buf){
    return  (gps_time_buf[0]-48) * 100000 +
            (gps_time_buf[1]-48) * 10000  +
            (gps_time_buf[2]-48) * 1000   +
            (gps_time_buf[3]-48) * 100    +
            (gps_time_buf[4]-48) * 10     +
            (gps_time_buf[5]-48)          +//indice 6: punto
            (gps_time_buf[7]-48) * 0.1    +
            (gps_time_buf[8]-48) * 0.01   +
            (gps_time_buf[9]-48) * 0.001  ;
}

uint8_t getTime(uint8_t* gps_datum_buf){
    return              (gps_datum_buf[0]-48) * 100000 +
                        (gps_datum_buf[1]-48) * 10000  +
                        (gps_datum_buf[2]-48) * 1000   +
                        (gps_datum_buf[3]-48) * 100    +
                        (gps_datum_buf[4]-48) * 10     +
                        (gps_datum_buf[5]-48)          ;
}