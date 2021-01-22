#include "adc.h"

void init_ADC(void){
    // SPMSC1 = 0x01;
    SPMSC1 = SPMSC1_BGBE_MASK;
    //ADCCFG = 0x90;
    // ADCCFG = 0b01011011; // 10 bit
    // ADCCFG = 0b01010011; // 8 bit
    // ADCCFG = 0b00101001;  // example config
    ADCCFG = (ADCCFG_ADLSMP_MASK |  ADCCFG_MODE1_MASK);  // Andrea code
    ADCSC2 = 0x00;
    //ADCSC1 = 0x1A;
    APCTL1 = 0b00000000;
    APCTL2 = 0x00;
    return;
}

uint16_t read_adc(uint8_t pin){
    // write ADCSC1 to select channel and initiate conversion
    //ADCSC1_ADCH = pin;
    //ADCSC1 = pin & ADCSC1_ADCH_MASK;
    
    uint16_t temp0 = 0;
    uint16_t temp1 = 0;
    uint16_t combined = 0;
    
    //ADCSC1 = ADCSC1_ADCH_MASK & pin;
    ADCSC1 = (ADCSC1_ADCH_MASK & pin);

    while(!ADCSC1_COCO)			// wait until conversion is complete

    // temp0 = ADCRH;
    // temp1 = ADCRL;

    // combined = (temp0 << 8) | (temp1 & 0xff);
    // return combined;
    return ADCRL;
}

uint16_t ConvertATD(uint8_t adcsc1_inp){
    uint16_t temp0 = 0;
    uint16_t temp1 = 0;
    uint16_t combined = 0;

    ADCSC1 = adcsc1_inp; /* Write to ADCSC1 to start conversion */

    while (ADCSC2_ADACT == 1); /* Wait if the conversion is in progress */

    while (ADCSC1_COCO == 0); /* Wait until the conversion is complete */

    // RAM200 = ADCRH.byte; /* Store result in $0200 and $0201 */
    // RAM201 = ADCRL.byte;
    // temp0 = ADCRH;
    // prints("AH");
    // printhexword(temp0);
    // temp1 = ADCRL;
    // prints("AL");
    // printhexword(temp1);
    // combined = (temp0 << 8) | (temp1 & 0xff);
    // prints("CD");
    // printhexword(combined);
    // asm( lda ADCRL; );
    // combined = ADCR;

    return ADCR;
    
}
