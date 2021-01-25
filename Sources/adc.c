#include "adc.h"

void init_ADC(void){
    // SPMSC1 = 0x01;
    SPMSC1 = SPMSC1_BGBE_MASK;
    //ADCCFG = 0x90;
    // ADCCFG = 0b01011011; // 10 bit
    // ADCCFG = 0b01010011; // 8 bit
    // ADCCFG = 0b00101001;  // example config
    // ADCCFG = (ADCCFG_ADLSMP_MASK |  ADCCFG_MODE1_MASK);  // Andrea code
    /*********************/
    // ADCSC1 must be the last register you write to
    /*********************/
    // ADCCFG = 0b00011000;
    // enable long sampling time, 10-bit mode, ADICLK = 11 (internal clk), BUSCLK % 1
    // ADCCFG = ADCCFG_ADLSMP_MASK | ADCCFG_MODE1_MASK | ADCCFG_ADICLK1_MASK | ADCCFG_ADICLK0_MASK;
    // ADCSC2 = 0x00;
    // APCTL1 = 0b00000000;
    // APCTL2 = 0x00;
    // ADCSC1 = ADCSC1_AIEN_MASK | (ADCSC1_ADCH_MASK & BANDGAP_CH);     // comment out to go back to no Interrupt
    
    #ifdef ADC_POLLING
        /***********************/
        // Polling              //
        /***********************/
        ADCCFG = ADCCFG_ADLSMP_MASK | ADCCFG_MODE1_MASK | ADCCFG_ADICLK1_MASK | ADCCFG_ADICLK0_MASK;
        ADCSC2 = 0x00;
        APCTL1 = 0b00000000;
        APCTL2 = 0x00;
        ADCSC1 = (ADCSC1_ADCH_MASK & BANDGAP_CH); 
    #else
        /*********************/
        // ADCSC1 must be the last register you write to
        /*********************/
        ADCCFG = 0b00011000;
        // enable long sampling time, 10-bit mode, ADICLK = 11 (internal clk), BUSCLK % 1
        ADCCFG = ADCCFG_ADLSMP_MASK | ADCCFG_MODE1_MASK | ADCCFG_ADICLK1_MASK | ADCCFG_ADICLK0_MASK;
        ADCSC2 = 0x00;
        APCTL1 = 0b00000000;
        APCTL2 = 0x00;
        ADCSC1 = ADCSC1_AIEN_MASK | (ADCSC1_ADCH_MASK & BANDGAP_CH);     // comment out to go back to no Interrupt
    #endif

    // ADCSC2 = ADCSC2_ADTRG_MASK;
    //ADCSC1 = 0x1A;
    
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

void interrupt VectorNumber_Vadc adc_isr(void){
    static uint8_t adc_seq;
    blue_led = 0;
    if (!adc_seq)
    {
        sum_temp = sum_temp + ADCR - average_temp;
        ADCSC1_ADCH = BANDGAP_CH;
        adc_seq++;
    }
    else
    {
        sum_bandgap = sum_bandgap + ADCR - average_bandgap;
        ADCSC1_ADCH = TEMP_SENSOR_CH;
        adc_seq = 0;
    }
    
    return;
    
}
