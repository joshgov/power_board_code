#include "main.h"

/***************************************************************************
 *  GLOBAL STRUCTURE PROTOTYPES
 ***************************************************************************/

struct _STATE STATE;
extern void init_global_vars(void);
void update_system_state();
void wait(uint16_t waittime);

/***************************************************************************
 *  GLOBAL STRUCTURE VARIABLES
 ***************************************************************************/

  int16_t average_temp, average_bandgap;
  int32_t sum_temp, sum_bandgap;
  int16_t temperature;
  int16_t reference_temp;
  int16_t supply_voltage;
  uint16_t timeout;
  int16_t m_slope;
  uint8_t  g_aero_hot_flag;
  int16_t thermocouple_temp;
  int16_t uc_temp;
  int16_t temperature_count;

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

void turn_fan_on(){
    PTADD_PTADD6 = OUTPUT;
    PTAPE_PTAPE6 = 0;
    PTAD_PTAD6 = 1;
    PTADD_PTADD7 = OUTPUT;
    PTAPE_PTAPE7 = 0;
    PTAD_PTAD7 = 1;
}

void init(void){
    /* init code for the power board
       setup port directions, ADCs etc. */
    DisableInterrupts;   
    init_bus_speed();
    init_ports();
    init_global_vars();
    init_SCI();
    init_ADC();
    init_SPI();
    init_TPM1();
    // wait(500);
    init_TPM2();
    // turn_fan_on();
    update_system_state();  // #FIXME might need to change this and pass actual temperatures in.
    
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

void update_system_state(){
  switch(STATE.system_state)
		{

		case STARTUP:
        set_rpi_relay(RELAY_OFF);
        set_aero_relay(RELAY_OFF);

        green_led = OFF;
        blue_led = OFF;
        STATE.system_state = (READY);
        break;
        
    
    case READY:
        set_rpi_relay(RELAY_ON);
        set_aero_relay(RELAY_ON);
        // if(rpi_aero_input == 0){
        //     wait(10);  // wait 10 ms and re-read the pin to make sure it's not bouncing
        //   if(rpi_aero_input == 0)
        //     STATE.system_state = (MAIN);
        // }
        // if(rpi_rpi_input == 0){
        //   wait(10);
        //   if(rpi_rpi_input == 0)
        //     STATE.system_state = (MAIN);
        // }
        STATE.system_state = (MAIN);
        break;
    
    case MAIN:
        blue_led = ON;

        if(rpi_aero_input == 0){
          wait(10);               // wait 10 seconds to see if the value has settled
          if(rpi_aero_input == 0){
            set_aero_relay(RELAY_OFF);
          }
        }
        else{
          if(!g_aero_hot_flag){
            set_aero_relay(RELAY_ON);
          }
        }

        if(rpi_rpi_input == 0){
          wait(10);           // wait 10 seconds to see if the value has settled
          if(rpi_rpi_input == 0){
            set_rpi_relay(RELAY_OFF);
          }
        }
        else{
          set_rpi_relay(RELAY_ON);
        }

        break;

    case AERO_HOT:
      set_aero_relay(RELAY_OFF);
      green_led = ON;
      g_aero_hot_flag = True;
      STATE.system_state = (MAIN);
      break;
    
    case AERO_COOL:
      set_aero_relay(RELAY_ON);
      green_led = OFF;
      g_aero_hot_flag = False;
      STATE.system_state = (MAIN);
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


uint16_t calc_uC_temp(void){

  uint8_t multiplier = 10;

  #ifdef ADC_POLLING
  /***********************/
  /* No ADC Interrupts   */
  /***********************/

  average_temp = ConvertATD(TEMP_SENSOR_CH);       // update the average temperature
  average_bandgap = ConvertATD(BANDGAP_CH); // update the average bandgap value
  

  #else
    /***********************/
    /* ADC Interrupts      */
    /***********************/
    average_temp = sum_temp / 64;       // update the average temperature
    average_bandgap = sum_bandgap / 64; // update the average bandgap value
  #endif

  if(average_bandgap) supply_voltage = 12296/average_bandgap;
  if(supply_voltage) reference_temp  = 13960/supply_voltage;

  if(!timeout){
    if(average_temp >= reference_temp){

      if(supply_voltage)
      {
          m_slope = 3266 / supply_voltage;
          temperature = (int16_t)25 - ((average_temp - reference_temp) * 100)/m_slope;
      }
    }
    else{
        if(supply_voltage){
          m_slope = 3638 / supply_voltage;
          temperature = (int16_t)25 + ((reference_temp - average_temp) * 100)/m_slope;
        }
    }
  }
  return temperature - UC_THERMAL_RESISTANCE;   // subtract 5 for junction thermal resistance
}

int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

int16_t max31855_thc_to_number(uint16_t data){
  int16_t temp = 0;
  temp = 
   ( ( (data & _2_10_MASK) >> 12) * ipow(2, 10) ) +\
   ( ( (data & _2_9_MASK) >> 11)  * ipow(2, 9)  ) +\
   ( ( (data & _2_8_MASK) >> 10)  * ipow(2, 8)  ) +\
   ( ( (data & _2_7_MASK) >> 9 )  * ipow(2, 7)  ) +\
   ( ( (data & _2_6_MASK) >> 8 )  * ipow(2, 6)  ) +\
   ( ( (data & _2_5_MASK) >> 7 )  * ipow(2, 5)  ) +\
   ( ( (data & _2_4_MASK) >> 6 )  * ipow(2, 4)  ) +\
   ( ( (data & _2_3_MASK) >> 5 )  * ipow(2, 3)  ) +\
   ( ( (data & _2_2_MASK) >> 4 )  * ipow(2, 2)  ) +\
   ( ( (data & _2_1_MASK) >> 3 )  * ipow(2, 1)  ) +\
   ( ( (data & _2_0_MASK) >> 2 )  * ipow(2, 0)  );

  temp = temp * 100;        // add 2 significant digits for the decimal numbers

  temp =  temp + \
   ( ( (data & _2_M1_MASK) >> 1 ) * 50) +\
   (   (data & _2_M2_MASK)        * 25);

  temp = temp | (data & SIGN_MASK);  // add the sign bit onto the output
  return temp;
}

int16_t max31855_ref_to_number(uint16_t data){
  int16_t temp = 0;
  temp = 
   ( ( (data & REF_2_6_MASK) >> 10) * ipow(2, 6) ) +\
   ( ( (data & REF_2_5_MASK) >> 9)  * ipow(2, 5)  ) +\
   ( ( (data & REF_2_4_MASK) >> 8)  * ipow(2, 4)  ) +\
   ( ( (data & REF_2_3_MASK) >> 7 )  * ipow(2, 3)  ) +\
   ( ( (data & REF_2_2_MASK) >> 6 )  * ipow(2, 2)  ) +\
   ( ( (data & REF_2_1_MASK) >> 5 )  * ipow(2, 1)  ) +\
   ( ( (data & REF_2_0_MASK) >> 4 )  * ipow(2, 0)  );

  temp = temp * 100;        // add 2 significant digits for the decimal numbers


  // I drop 2 signficant figures because anything beyond 0.25C isn't very useful
  temp = temp + ( ( (data & REF_2_M1_MASK) >> 3 )  * 50  );
  temp = temp + ( ( (data & REF_2_M2_MASK) >> 2 )  * 25  );

  // temp = temp | (data & REF_SIGN_MASK);  // add the sign bit onto the output
  return temp;
}

uint16_t max31855_temperature(void){
  
  uint16_t max31855_thermocouple = 0;
  uint16_t max31855_ref = 0;
  uint16_t max31855_fault_flag = 0;
  uint16_t max31855_faults = 0;

  uint32_t max31855_data = 0;
  uint8_t error_codes = 0;

  max31855_data = SPI_read_MAX31855();
  max31855_thermocouple = max31855_data >> 18;
  max31855_ref = (max31855_data & MAX31855_REF_MASK) >> 4;
  max31855_fault_flag = (uint16_t)((max31855_data & MAX31855_FAULT_FLAG_MASK) >> 16);
  max31855_faults = (uint16_t)(max31855_data & MAX31855_FAULT_MASK);

  max31855_thermocouple = max31855_thc_to_number(max31855_thermocouple);
  max31855_ref          = max31855_ref_to_number(max31855_ref);

  prints("THC");
  printhexword(max31855_thermocouple);
  SCI_send_byte(0x0D);
  //printhexword(0x00);
  
  // prints("REF");
  // printhexword(max31855_ref);
  // prints("\0");

  if(max31855_fault_flag){
    // check what errors we have
    prints("FTS");

    if(max31855_faults & SCV_FAULT_MASK){
      prints("SCV");
    }
    if(max31855_faults & SCG_FAULT_MASK)
      prints("SCG");
    if(max31855_faults & OC_FAULT_MASK)
      prints("OC");
    //printhexword(0x00);
    SCI_send_byte(0x0D);
     
  }

  
  return max31855_thermocouple;
}

uint16_t get_uC_temp(){
  uint16_t uc_temp = calc_uC_temp();
  prints("UCT");
  printhexword(uc_temp);
  SCI_send_byte(0x0D);
  return uc_temp;
}

void main(void) {
  uc_temp = 0;
  thermocouple_temp = 0;
  timeout = 300;
  g_aero_hot_flag = False;
  temperature_count = 0;
  init();

  
  average_temp = 0;
  average_bandgap = 0;
  sum_temp = 0; 
  sum_bandgap = 0;


  EnableInterrupts;


  for(;;) { // main loop
    // thermocouple_temp = max31855_temperature();
    // uc_temp = get_uC_temp();
    // SCI_send_byte(0x0D);
    update_system_state();
    

    __RESET_WATCHDOG(); /* feeds the dog */
  } /* loop forever */
}