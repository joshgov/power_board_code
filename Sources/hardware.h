#ifndef __Hardware_H_
#define __Hardware_H_
/***************************************************************************
 *  HARDWARE SETUP
 ***************************************************************************/

#define ICSC1_INIT					ICSC1_IREFS_MASK | ICSC1_IRCLKEN_MASK | ICSC1_IREFSTEN_MASK  
#define ICSC2_INIT					0x40

void init_bus_speed(void);
void init_ports(void);

#endif

