#include "globals.h"
volatile struct _STATE STATE;

void init_global_vars(void){

    STATE.system_state = STARTUP; 
	STATE.rpi_state = RELAY_OFF;
    STATE.aero_state = RELAY_OFF;
	STATE.rpi_cmd = RELAY_OFF;
    STATE.aero_cmd = RELAY_OFF;
    return;
}

