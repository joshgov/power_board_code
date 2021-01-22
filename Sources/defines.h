#ifndef __DEFINES_H_
#define __DEFINES_H_

/***************************************************************************
 *  GENERAL MACROS
 ***************************************************************************/

#define NULL            ((void*)0)
#define OFF             0
#define ON              1

#define LOW             0
#define HIGH            1

#define ENABLE          0
#define DISABLE         1

#define INPUT           0
#define OUTPUT          1

#define False           0U
#define True            1U

#define ALL				0xFF                      ///< All bits in byte matter
#define ALL_ZERO		{0xFF, 0}                 ///< all bits in byte must be zero
 
#define WAIT_1MS_LOOPS 		    (587U)
#define invert_pin(pin) 	    pin = (pin)? LOW : HIGH 
#define set_led(x) 	            RED_LED = (x)? LOW : HIGH;
#define NOP() asm(nop;)

/***************************************************************************
 *  POWER_BOARD MACROS
 ***************************************************************************/

#define aero_relay 0
#define rpi_relay 1

#define rpi_pwr_ctrl PTCD_PTCD6
#define aero_pwr_ctrl PTCD_PTCD7

#define rpi_pwr_adc PTDD_PTA0
#define aero_pwr_adc PTDD_PTA1

#define blue_led PTCD_PTCD5
#define green_led PTCD_PTCD4

#define rpi_aero_input PTAD_PTAD7
#define rpi_rpi_input PTAD_PTAD6

#define CS PTBD_PTBD5

#endif