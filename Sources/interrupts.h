#ifndef __INTERRUPTS_H_
#define __INTERRUPTS_H_
#include "types.h"
#include "defines.h"
#include "derivative.h"

bool_t debounce_switch();
bool_t raw_key_pressed();
INTERRUPT(VectorNumber_Vtpm1ovf) tpm_time_up(void);

#endif