#ifndef __TYPES_H_
#define __TYPES_H_

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned long int   uint32_t;
typedef signed char         int8_t;
typedef short               int16_t;
typedef long int            int32_t;
typedef unsigned char       bool_t;

/*!\enum SYSTEM_STATE
  SYSTEM_STATE can take on values 0x0, 0x1F, 0xE3, and 0xFC.
  See BREAKER_STATE for discussion of max hamming distance.
*/
typedef enum { 
    SYSTEM_ERROR      = 0x00, 
    STARTUP           = 0x1F, 
    READY             = 0x21, 
    MAIN              = 0x3E,
    RPI_OFF           = 0x42  
} SYSTEM_STATE;

// System state enums are magic numbers with minimum hamming distance of 5 bits from each other.
// This means that a minimum of 5 bits needs to be flipped to change one valid state into another.
// These are a very robust type of error-correcting code
       
typedef enum { 
    RELAY_ON     = 0xE3,
    RELAY_OFF    = 0xFC
} RELAY_STATE;

// typedef enum SYSTEM_STATE state_t;

#endif

