#include "interrupts.h"
#include "derivative.h"
#include "defines.h"

bool_t raw_key_pressed(){
    bool_t val = rpi_input;
    return val;
}

// Service routine called by a timer interrupt
bool_t debounce_switch()
{
    static uint16_t State = 0; // Current debounce status
    State=(State<<1) | !RawKeyPressed() | 0xe000;
    if(State==0xf000)return T;
}

/*******************************************************
 * GENERAL HARDWARE
 ********************************************************/

 //12MHz oscillator
#define SYNR_VALUE      0xC3
#define REFDV_VALUE     0x80
#define POSTDIV_VALUE   0x00

void InitBusSpeed(void){
    
  PLLCTL = 0B10000001;  //CME=1,PLLON=0,FM1=0,FM2=0,FSTWKP=0,PRE=0,PCE=0,SCME=1
  CLKSEL = 0B00000011;  //PLLSEL=0,PSTP=0,PLLWAI=0,RTIWAI=1,COPWAI=1
 
  SYNR = SYNR_VALUE;          //Set the multiplier register
  REFDV = REFDV_VALUE;        //Set the divider register
  POSTDIV = POSTDIV_VALUE;    //Set the post divider register
  PLLCTL |= PLLCTL_PLLON_MASK;  //Enable the Phase Lock Loop
  
  //Wait till the PLL VCO is within tolerance
  while((CRGFLG & CRGFLG_LOCK_MASK) == 0);
  
  CLKSEL |= CLKSEL_PLLSEL_MASK; //system clocks are derived from PLLCLK
}

/* SCI **********************************/

// void interrupt SCI1_ISR(void){
     
//   if(SCI1SR1_RDRF){
//     SCI1_RXRoutine();
//   } 

//   if(SCI1SR1_TDRE){
//     if (print_c1buf.inpos == print_c1buf.outpos){ // buffer empty
// 	    SCI1CR2_TIE = 0;	// disable transmit interrupts	
// 	  } 
// 	  else{
// 	    SCI1DRL = print_c1buf.data[print_c1buf.outpos];
//       (void)CBufPop(&print_c1buf, BUFSIZE);
// 	  } 
//   } 
  
// }

