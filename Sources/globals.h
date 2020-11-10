#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include "types.h"

struct _STATE{
    
    RELAY_STATE     rpi_state;
    RELAY_STATE     aero_state;
    RELAY_STATE     rpi_cmd;
    RELAY_STATE    aero_cmd;
    SYSTEM_STATE   system_state;

};

void init_global_vars(void);

extern uint16_t temp_fault_count;

#endif