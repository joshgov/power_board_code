#include "derivative.h"
#include "defines.h"
#include "hardware.h"

void init_ports(void){
    /* setup microcontroller ports */
    // FIXME: set all other pins in High Z mode
    // FIXME: set GPIO for RPi input

   /* set relay control ports as outputs */
    PTCDD_PTCDD6 = OUTPUT; // rpi_pwr_ctrl
    PTCDD_PTCDD7 = OUTPUT; // aero_pwr_ctrl
    
    /* turn off pull up resistors for relay control outputs */
    PTCPE_PTCPE6 = 0;
    PTCPE_PTCPE7 = 0;

    /* initialize relay control outputs */
    rpi_pwr_ctrl = OFF;
    aero_pwr_ctrl = OFF;
    
    /* setup power monitor inputs */
    PTADD_PTADD2 = INPUT;
    PTADD_PTADD3 = INPUT;
    PTAD_PTAD2 = OFF;
    PTAD_PTAD3 = OFF;
    PTAPE_PTAPE2 = 0;
    PTAPE_PTAPE3 = 0;

    /* setup RPi input pins */
    PTADD_PTADD7 = INPUT;
    PTAPE_PTAPE7 = 1;   // set pullup resistor
    
    PTADD_PTADD6 = INPUT;
    PTAPE_PTAPE6 = 1;  // set pullup resistor

    /* setup LED ports */
    PTCDD_PTCDD5 = OUTPUT;  // blue led
    PTCD_PTCD5 = OFF;       // blue led off

    PTCDD_PTCDD4 = OUTPUT;  // red led pin output
    PTCD_PTCD4 = OFF;       // red led off
    
}

/************************************************
 * GENERAL HARDWARE
 * **********************************************/

//Oscillator

void init_bus_speed(void){
    // select internal FLL clock and no divider -- 40 MHz 
    ICSC1_CLKS  = 0;  // select internal FLL
    ICSC1_RDIV  = 0;  // divide by 1
    ICSC1_IREFS = 1;  // internal reference clock for FLL
    ICSC1_IRCLKEN = 1; // enable internal clock for ICSIRCLK
    ICSC1_IREFSTEN = 0; // stop the clock in stop mode    


    ICSC2_BDIV  = 0;  // divide clock by 1 for bus clock frequency
    
    // zero out other values in ISCSC2 register -- options for external osc
    ICSC2_RANGE = 0;
    ICSC2_HGO = 0;
    ICSC2_LP = 0;
    ICSC2_EREFS = 0;
    ICSC2_ERCLKEN = 0;
    ICSC2_EREFSTEN = 0; 
}
