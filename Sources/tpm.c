#include "tpm.h"

uint8_t g_slow_start_flag = 5;

void init_TPM(void){
    SOPT2_T2CH0PS = 1;
    SOPT2_T2CH1PS = 1;
    // SOPT2_T2CH1PS = 1;
    //  TPM2SC = TPM2SC_TOIE_MASK | TPM2SC_CLKSA_MASK;
    //  TPM2SC_PS = 1;
    TPM2SC = 0b01001000;    // Interrupt enable, busclk / 2

    TPM2MOD = 2047;     // causes TOF Interrupt when reached by counter
    TPM2C0SC = TPM2C0SC_MS0B_MASK | TPM2C0SC_ELS0B_MASK;
    TPM2C1SC = TPM2C1SC_MS1B_MASK | TPM2C1SC_ELS1B_MASK;
    // TPM2C0SC = 0b01101000;
    TPM2C0V = 511;    // interrupt created when this number is reached. 
    TPM2C1V = 511;
    return;
}

void interrupt VectorNumber_Vtpm2ovf tpm2_overflow_isr(void){
    TPM2SC_TOF = 0;     // clear interrupt flag 
    if(!g_slow_start_flag){         // used for a slow start on the fan motor #FIXME
        TPM2C0V = 1023;
        TPM2C1V = 1023;
    } 
    else
        g_flag -= 1;
    return;
}

void interrupt VectorNumber_Vtpm2ch1 tpm2_channel1_isr(void){
    TPM2C1SC_CH1F = 0;
}

void interrupt VectorNumber_Vtpm2ch0 tpm2_channel0_isr(void){
    TPM2C0SC_CH0F = 0;
    return;
}

