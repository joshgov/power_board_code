#include "utility_funcs.h"
#include "types.h"
#include "defines.h"
#include "derivative.h"

/***************************************************************************
 *  GENERAL UTILITY FUNCTIONS
 ***************************************************************************/

void wait(uint16_t waittime)                  
  {
  uint16_t i=0, j=0;  
  for(i=0;i<waittime;i++)
    for(j=0; j < WAIT_1MS_LOOPS; j++)
      __RESET_WATCHDOG();
  }