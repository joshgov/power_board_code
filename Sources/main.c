#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "types.h"
#include "defines.h"
#include "hardware.h"
#include "globals.h"

/***************************************************************************
 *  GLOBAL STRUCTURE PROTOTYPES
 ***************************************************************************/

extern struct _STATE STATE;
extern void init_global_vars(void);
void update_system_state(void);
void wait(uint16_t waittime);


bool_t raw_key_pressed(){
    bool_t val = rpi_aero_input;
    return val;
}

// Service routine called by a timer interrupt
bool_t debounce_switch()                 
{
    static uint16_t State = 0; // Current debounce status
    State=(State<<1) | !raw_key_pressed() | 0xe000;
    if(State==0xf000)return TRUE;
    return FALSE;
}

/*void interrupt VectorNumber_Vtpm1ovf tpm_time_up(void){
    bool_t switch_state = debounce_switch();
    if(switch_state)       // if there is a 0 on the line set TEMP_FAULT state
       STATE.system_state = (TEMP_FAULT);
    else
       STATE.system_state = (
    
}*/

void init(void){
    /* init code for the power board
       setup port directions, ADCs etc. */
    DisableInterrupts;   
    init_bus_speed();
    init_ports();
    init_global_vars();
    update_system_state();
    
}

void set_rpi_relay(RELAY_STATE new_mode){
  switch (STATE.rpi_state)
		{
		case RELAY_OFF:
        if (new_mode == RELAY_ON)
				{
          STATE.rpi_state = RELAY_ON;
          rpi_pwr_ctrl = ON;
				}
        break;

    case RELAY_ON:
        if (new_mode == RELAY_OFF){
          STATE.rpi_state = RELAY_OFF;
          rpi_pwr_ctrl = OFF;
        }
        break;    
    }
}

void set_aero_relay(RELAY_STATE new_mode){

    switch (STATE.aero_state)
		{
		case RELAY_OFF:
        if (new_mode == RELAY_ON)
				{
          STATE.aero_state = RELAY_ON;
          aero_pwr_ctrl = ON;
				}
        break;

    case RELAY_ON:
        if (new_mode == RELAY_OFF){
          STATE.aero_state = RELAY_OFF;
          aero_pwr_ctrl = OFF;
        }
        break;    
    }
    
}

void update_system_state(void){
  switch(STATE.system_state)
		{

		case STARTUP:
        set_rpi_relay(RELAY_OFF);
        set_aero_relay(RELAY_OFF);

        green_led = ON;
        STATE.system_state = (READY);
        break;
        
    
    case READY:
        set_rpi_relay(RELAY_ON);
        set_aero_relay(RELAY_ON);
        blue_led = ON;
        if(rpi_aero_input == 0){
            wait(10);  // wait 10 ms and re-read the pin to make sure it's not bouncing
          if(rpi_aero_input == 0)
            STATE.system_state = (TEMP_FAULT);
        }
        if(rpi_rpi_input == 0){
          wait(10);
          if(rpi_rpi_input == 0)
            STATE.system_state = (RPI_REBOOT);
        }
        break;
    
    case TEMP_FAULT:
        set_aero_relay(RELAY_OFF);
        blue_led = OFF;
        
        if(rpi_aero_input == 1){
          wait(10);
          if(rpi_aero_input == 1)
            STATE.system_state = (READY);
        }
        break;
        
    case RPI_REBOOT:
        set_rpi_relay(RELAY_OFF);
        blue_led = OFF;
        wait(100);
        blue_led = ON;

        if(rpi_rpi_input == 1){
          wait(10);
          if(rpi_rpi_input == 1)
            STATE.system_state = (READY);
        }
        break;
    
    default: 
        // we're in an unknown state AHHH! -- start over
        set_rpi_relay(RELAY_OFF);
        set_aero_relay(RELAY_OFF);
        STATE.system_state = (STARTUP);
        break;
    }
}

void wait(uint16_t waittime)                  
  {
  uint16_t i=0, j=0;  
  for(i=0;i<waittime;i++)
    for(j=0; j < WAIT_1MS_LOOPS; j++)
      __RESET_WATCHDOG();
  }


void main(void) {

  init();

  EnableInterrupts; /* enable interrupts */
  /* include your code here */


  for(;;) { // main loop
    /*set_rpi_relay(RELAY_ON);
    wait(1000);
    set_aero_relay(RELAY_ON);*/
    update_system_state();
    
    
    // rpi_pwr_ctrl = ~rpi_pwr_ctrl;
    // aero_pwr_ctrl = ~aero_pwr_ctrl;
  
    __RESET_WATCHDOG(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
