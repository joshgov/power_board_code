#ifndef _MAIN_H
#define _MAIN_H

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "types.h"
#include "defines.h"
#include "hardware.h"
#include "globals.h"
#include "sci.h"
#include "adc.h"
#include "spi.h"
#include "tpm.h"

#define CODE_REVISION 1

/*******************************************************
* Clock/Delays
********************************************************/

#define BUSCLK 40000000UL

/***************************************************************************
 *  Other hardware REGISTER SETUP
 ***************************************************************************/

#define TPM1SC_INIT 				TPM1SC_TOIE_MASK | 0x08   	///< turn on timer, running on busclk / 1

#define TPM1MODH_INIT				0x1F   
#define TPM1MODL_INIT				0x40   					    ///< busclk/8000 = 1 kHz sample rate 
#define TPM1MOD_INIT				0x1F40                      //change the two above together  !! 


#define TPM2SC_INIT					TPM2SC_TOIE_MASK | 0x08		///< J1772 pilot capture

#define TPM2MODH_INIT				0x27   
#define TPM2MODL_INIT				0x10    
#define TPM2MOD_INIT				0x2710                      //change the two above together   

// #define SCIBDH_INIT					0x41
// #define SCIBDL_INIT         0x104
// #define SCIC1_INIT          0x00
// #define SCIC2_INIT          0xEC

#define SCIC1_INIT 		 			0x00						///< serial port, tx only, 38400 baud
#define SCIC2_INIT					SCIC2_TE_MASK | SCIC2_RE_MASK | SCIC2_RIE_MASK
#define SCIC2_SETUP					{0x7F, SCIC2_INIT}  
#define SCIC3_INIT					0x00 
#define SCIBDH_INIT					0x00
#define SCIBDL_INIT                 10  // 115200 baud 

#define TEMP_SENSOR_CH                 26
#define BANDGAP_CH                     0b00011011

#define SIGN_MASK                    0b0010000000000000
#define _2_10_MASK                   0b0001000000000000
#define _2_9_MASK                    0b0000100000000000
#define _2_8_MASK                    0b0000010000000000
#define _2_7_MASK                    0b0000001000000000
#define _2_6_MASK                    0b0000000100000000
#define _2_5_MASK                    0b0000000010000000
#define _2_4_MASK                    0b0000000001000000
#define _2_3_MASK                    0b0000000000100000
#define _2_2_MASK                    0b0000000000010000
#define _2_1_MASK                    0b0000000000001000
#define _2_0_MASK                    0b0000000000000100
#define _2_M1_MASK                   0b0000000000000010
#define _2_M2_MASK                   0b0000000000000001

#define REF_SIGN_MASK                0b0000100000000000
#define REF_2_6_MASK                 0b0000010000000000
#define REF_2_5_MASK                 0b0000001000000000
#define REF_2_4_MASK                 0b0000000100000000
#define REF_2_3_MASK                 0b0000000010000000
#define REF_2_2_MASK                 0b0000000001000000
#define REF_2_1_MASK                 0b0000000000100000
#define REF_2_0_MASK                 0b0000000000010000
#define REF_2_M1_MASK                0b0000000000001000
#define REF_2_M2_MASK                0b0000000000000100
#define REF_2_M3_MASK                0b0000000000000010
#define REF_2_M4_MASK                0b0000000000000001

#define MAX31855_THERMO_MASK        0xFFFC0000
#define MAX31855_REF_MASK           0x0000FFF8
#define MAX31855_FAULT_FLAG_MASK   0x00010000
#define MAX31855_FAULT_MASK         0x0000000F

#define SCV_FAULT_MASK              0b0100
#define SCG_FAULT_MASK              0b0010
#define OC_FAULT_MASK               0b0001

#define UC_THERMAL_RESISTANCE       5
#define AERO_THERMAL_LIMIT          7900      // #FIXME this needs to be a real number and needs to be 4 digits because themocouple data has 2 decimal places
#define AERO_THERMAL_OK_LIMIT       6000      // #FIXME chosen because it's room temp.
#define COLD_THERMAL_LIMIT          -17
#define COLD_THERMAL_TURN_ON_LIMIT  0


int16_t max31855_thc_to_number(uint16_t data);
int16_t max31855_ref_to_number(uint16_t data);

// LBKDIE = 0 | RXEDGIE = 1 |

#endif