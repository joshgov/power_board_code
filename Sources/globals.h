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

/***************************************************************************
 *  GLOBAL ADC VARIABLES
 ***************************************************************************/

  extern int16_t average_temp, average_bandgap;
  extern int32_t sum_temp, sum_bandgap;
  extern int16_t temperature;
  extern int16_t reference_temp;
  extern int16_t supply_voltage;
  extern uint16_t timeout;
  extern int16_t m_slope;

#endif