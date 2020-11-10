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
    if(State==0xf000)return T