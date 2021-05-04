#ifndef _SCI_H
#define _SCI_H

#include "main.h"

#define BUFSIZE 256

typedef struct{
  uint8_t uC_temp;
  uint8_t aero_temp;
} temperature_packet;

#define SCIASR1 BDH
#define SCIACR1 BDL
#define SCIACR2 CR1

void init_SCI(void);
void SCI_send_byte(uint8_t c);
void prints(char const * s);
char nibble2asc(uint8_t n);
void printhexbyte( uint8_t n );
void printhexword( uint16_t n );
void printhex2word( uint32_t n);
void prints(char const * s);
void printhexbyte_signed(int8_t n);
void printhexword_signed(int16_t n);
void printhexword_negative(int16_t n);

#endif