#include "tpm.h"

uint8_t g_slow_start_flag = True;

void init_TPM2(void){
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

void init_TPM1(void){ // setup for 1KHz interrupt 1ms intervals
    SOPT2_T1CH0PS = 1;
    // TPM1SC = 0b01001000;    // Interrupt enable, busclk / 2
    TPM1SC = TPM1SC_TOIE_MASK | TPM_BUSCLK_MASK | 0b00000100;
    TPM1C0SC = TPM1C0SC_MS0B_MASK | TPM1C0SC_ELS0B_MASK;
    TPM1MOD = 1023;
    TPM1C0V = 511;
    return;
}

void interrupt VectorNumber_Vtpm1ovf tpm1_overflow_isr(void){
    TPM1SC_TOF = 0;
    if(timeout) timeout--;
    return;
}

void interrupt VectorNumber_Vtpm1ch0 tpm1_channel0_isr(void){
    TPM1C0SC_CH0F = 0;
    return;
}
void interrupt VectorNumber_Vtpm2ovf tpm2_overflow_isr(void){
    // should probably disable interrupts while in interrupt #FIXME
    TPM2SC_TOF = 0;     // clear interrupt flag

    if (!timeout){       // wait for temperature and bandgap readings to settle.
        
        if(temperature < COLD_THERMAL_LIMIT){
            // #FIXME might need to add hysterious to stop it from oscillating at the limit.
            // maybe set timeout to some value so you have to wait a certain amount of time. 
            TPM2C0V = 0;
            TPM2C1V = 0;
        }
        else{
            TPM2C0V = 1023;
            TPM2C1V = 1023;
        }

    }

    return;
}

void interrupt VectorNumber_Vtpm2ch0 tpm2_channel0_isr(void){
    TPM2C0SC_CH0F = 0;
    return;
}

void interrupt VectorNumber_Vtpm2ch1 tpm2_channel1_isr(void){
    TPM2C1SC_CH1F = 0;
    return;
}