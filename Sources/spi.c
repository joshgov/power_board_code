#include "spi.h"

void init_SPI(void){
    SPIC1 = SPIC1_SPE_MASK | SPIC1_MSTR_MASK;
    // SPIC1 = 0b01010100;
    // SPIC1 = 0xD0;
    SPIC2 = 0x00;
    
    // SPIBR = SPIBR_SPR1_MASK;  // divide by 8 to get 5MHz for max temp IC clock rate
    SPIBR = 0b01110111; // prescale 5 divisor 64
    CS = 1; 

    return;
}
    

uint8_t SPI_transcieve_byte(  uint8_t input )
    {
    uint8_t x = SPIS;
    while (!SPIS_SPTEF);
    SPID = input; 
    while (!SPIS_SPRF);
    return SPID;
    }

uint32_t SPI_read_MAX31855(void){
    uint32_t data = 0;
    uint32_t temp0 = 0;
    uint32_t temp1 = 0;
    uint32_t temp2 = 0;
    uint32_t temp3 = 0;
    uint16_t thermocouple = 0;

    CS = 0;
    temp0 = SPI_recieve();
    temp1 = SPI_recieve();
    temp2 = SPI_recieve();
    temp3 = SPI_recieve();
    CS = 1;

    data = (temp0 << 24) | (temp1 << 16) | (temp2 << 8) | (temp3);
    return data;
}

uint8_t SPISendChar (uint8_t data){
    CS=1;
    while (!SPIS_SPTEF); /* wait until transmit buffer is empty*/
    SPID = data; /* Transmit counter*/
    while (!SPIS_SPRF);
    data = SPID;
    CS=0;
    return data;
}