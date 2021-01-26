#ifndef _ADC_H
#define _ADC_H

#include "main.h"

#define ADC_POLLING

void init_ADC(void);
uint16_t read_adc(uint8_t pin);
uint16_t ConvertATD(uint8_t adcsc1_inp);
void interrupt VectorNumber_Vadc adc_isr(void);


#endif