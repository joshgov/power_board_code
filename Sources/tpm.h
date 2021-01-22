#ifndef _TPM_H
#define _TPM_H

#include "main.h"

#define TPM_BUSCLK_MASK 0b00001000
#define TPM_PS_2_MASK     0x01

// extern uint8_t g_flag;     // used for a slow start on fan motors #FIXME

void init_TPM(void);
void interrupt VectorNumber_Vtpm2ovf tpm2_overflow_isr(void);
void interrupt VectorNumber_Vtpm2ch0 tpm2_channel0_isr(void);
void interrupt VectorNumber_Vtpm2ch1 tpm2_channel1_isr(void);
#endif
