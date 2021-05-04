#include "tpm.h"

uint8_t g_slow_start_flag = True;
uint8_t tpm2_flag = True;
extern struct _STATE STATE;
extern uint16_t thermocouple_temp;
extern uint8_t g_aero_hot_flag;
extern uint16_t temperature_count;
extern uint16_t uc_temp;
extern void wait(uint16_t waittime);
extern uint16_t get_uC_temp();
extern uint16_t max31855_temperature(void);
extern uint16_t calc_uC_temp(void);

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
    TPM2C0V = 2047;    // interrupt created when this number is reached. 
    TPM2C1V = 2047;
    return;
}
// void wait(uint16_t waittime)                  
//   {
//   uint16_t i=0, j=0;  
//   for(i=0;i<waittime;i++)
//     for(j=0; j < WAIT_1MS_LOOPS; j++)
//       __RESET_WATCHDOG();
//   }

void reset_tpm2(){
    TPM2C0SC = TPM2C0SC_MS0B_MASK;
    TPM2C1SC = TPM2C1SC_MS1B_MASK;

    // set tpm pins to low state
    PTADD_PTADD6 = OUTPUT;
    PTAPE_PTAPE6 = 0;
    PTAD_PTAD6 = 0;
    PTADD_PTADD7 = OUTPUT;
    PTAPE_PTAPE7 = 0;
    PTAD_PTAD7 = 0;
    wait(20);
    TPM2C0SC = TPM2C0SC_MS0B_MASK | TPM2C0SC_ELS0B_MASK;
    TPM2C1SC = TPM2C1SC_MS1B_MASK | TPM2C1SC_ELS1B_MASK;
}

void init_TPM1(void){ // setup for 1KHz interrupt 1ms intervals
    SOPT2_T1CH0PS = 1;
    SOPT2_T1CH1PS = 1;
    // TPM1SC = 0b01001000;    // Interrupt enable, busclk / 2
    // TPM1SC = TPM1SC_TOIE_MASK | TPM_BUSCLK_MASK | 0b00000100; // previous version
    TPM1SC = 0b01001000;
    TPM1C0SC = TPM1C0SC_MS0B_MASK | TPM1C0SC_ELS0B_MASK;
    TPM1C1SC = TPM1C1SC_MS1B_MASK | TPM1C1SC_ELS1B_MASK;
    TPM1MOD = 2047;
    // TPM1C0V = 511; // previous setting
    TPM1C0V = 2047;
    TPM1C1V = 2047;
    return;
}

void interrupt VectorNumber_Vtpm1ovf tpm1_overflow_isr(void){
    TPM1SC_TOF = 0;
    if(timeout) timeout--;
    if(!timeout){
        if(thermocouple_temp >= AERO_THERMAL_LIMIT){ 
            STATE.system_state = (AERO_HOT);
        }
        if( (thermocouple_temp <= AERO_THERMAL_OK_LIMIT) & g_aero_hot_flag){
             STATE.system_state = (AERO_COOL);
        }
        if(temperature_count >= 8950){
            thermocouple_temp = max31855_temperature();
            uc_temp = get_uC_temp();
            SCI_send_byte(0x0D);
            temperature_count = 0; 
        }
        else{
            temperature_count += 1;
        }
    }
    return;
}

void interrupt VectorNumber_Vtpm1ch0 tpm1_channel0_isr(void){
    TPM1C0SC_CH0F = 0;
    return;
}

void interrupt VectorNumber_Vtpm1ch1 tpm1_channel1_isr(void){
    TPM1C1SC_CH1F = 0;
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
        if(temperature > COLD_THERMAL_TURN_ON_LIMIT){
            TPM2C0V = 2047;
            TPM2C1V = 2047;
            // TPM2C0V = 1023;
            // TPM2C1V = 1023;

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