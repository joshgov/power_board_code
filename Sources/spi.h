#ifndef _SPI_H
#define _SPI_H

#include "main.h"

#define SPI_send(a) (void)SPI_transcieve_byte(a)
#define SPI_recieve() SPI_transcieve_byte(0xFF)

uint8_t SPI_transcieve_byte(  uint8_t input );
uint8_t SPISendChar (uint8_t data);
void init_SPI(void);
uint32_t SPI_read_MAX31855(void);

#endif