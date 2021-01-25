#include "main.h"

/***************************************************************************
 *  GLOBAL STRUCTURE PROTOTYPES
 ***************************************************************************/

extern struct _STATE STATE;
extern void init_global_vars(void);
void update_system_state(void);
void wait(uint16_t waittime);

/***************************************************************************
 *  GLOBAL STRUCTURE VARIABLES
 ***************************************************************************/

  uint16_t average_temp, average_bandgap;
  uint32_t sum_temp, sum_bandgap;
  uint16_t temperature;
  uint16_t reference_temp;
  uint16_t supply_voltage;
  uint16_t timeout;
  uint16_t m_slope;

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
    init_SCI();
    init_ADC();
    init_SPI();
    init_TPM1();
    init_TPM2();
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
    green_led = 0;
  #endif

  if(average_bandgap) supply_voltage = 12296/average_bandgap;
  if(supply_voltage) reference_temp  = 13960/supply_voltage;

  if(!timeout){
    if(average_temp >= reference_temp){

      if(supply_voltage)
      {
          m_slope = 3266 / supply_voltage;
          temperature = (uint16_t)25 - ((average_temp - reference_temp) * 100)/m_slope;
      }
    }
    else{
        if(supply_voltage){
          m_slope = 3638 / supply_voltage;
          temperature = (uint16_t)25 + ((reference_temp - average_temp) * 100)/m_slope;
        }
    }
  }
  return temperature;
}

void send_temperature(void){
  
  uint16_t adp6 = 0;
  uint16_t max31855_thermocouple = 0;
  uint16_t max31855_ref = 0;
  uint16_t max31855_fault_flag = 0;
  uint16_t max31855_faults = 0;

  uint16_t multiplier = 1000;
  uint16_t temperature = 0;
  uint16_t ADCR_BG = 0;
  uint16_t ADCR_VDD = 1023;  // max num for 10 bit ADC
  uint16_t V_BG = 1202;     //  mult by 1000
  uint16_t V_TEMP25 = 1396;  // mult by 1000
  uint32_t VDD_CONV = 0;
  uint32_t max31855_data = 0;
  uint8_t error_codes = 0;

  ADCR_BG = ConvertATD(BANDGAP_CH);

  // VDD_CONV = (ADCR_VDD * V_BG) / ADCR_BG;
  temperature = ConvertATD(TEMP_SENSOR_CH);

  max31855_data = SPI_read_MAX31855();
  max31855_thermocouple = max31855_data >> 18;
  max31855_ref = (max31855_data & MAX31855_REF_MASK) >> 4;
  max31855_fault_flag = (uint16_t)((max31855_data & MAX31855_FAULT_FLAG_MASK) >> 16);
  max31855_faults = (uint16_t)(max31855_data & MAX31855_FAULT_MASK);

  
  prints("BGP");
  printhexword(ADCR_BG);
  prints("\0");
  prints("UCT");
  printhexword(temperature);
  prints("\0");
  prints("THC");
  printhexword(max31855_thermocouple);
  prints("\0");
  prints("REF");
  printhexword(max31855_ref);
  prints("\0");

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
    prints("\0"); 
  }

  
  return;
}

void main(void) {
  timeout = 3000;
  init();

  
  average_temp = 0;
  average_bandgap = 0;
  sum_temp = 0; 
  sum_bandgap = 0;

  blue_led = 1;

  EnableInterrupts;


  for(;;) { // main loop
    //update_system_state();
    //send_temperature();
    calc_uC_temp();

    __RESET_WATCHDOG(); /* feeds the dog */
  } /* loop forever */
}