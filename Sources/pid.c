
#include "pid.h"

/*###########
2 Significant figures for everything.
All numbers multiplied by 100
#############*/

void init_pid(uint8_t Kp, uint8_t Ki, uint8_t Kd){
    pid.Kp = Kp;
    pid.Ki = Ki;
    pid.Kd = Kd;

    pid_clear();
    pid.windupGuard = 100;
}

void pid_clear(){
    pid.setpoint = 0;

    pid.PTerm = 0;
    pid.ITerm = 0;
    pid.DTerm = 0;
    pid.lastError = 0;
    
    // windup guard
    pid.windupGuard = 0;
    pid.intError = 0;

    pid.feedback_value = 0;
    pid.output = 0;
}

void pid_update(uint8_t feedback_value){
    uint8_t error = pid.setpoint - pid.feedback_value; 
    uint8_t delta_error = error - pid.lastError;
    uint8_t delta_time = PID_PERIOD_LENGTH;
    

    pid.PTerm = pid.Kp * error;
    pid.ITerm = pid.ITerm + error * (1 / PID_UPDATE_RATE);  // #FIXME need interval length not rate

    if (pid.ITerm < (-1 * pid.windupGuard)){
        pid.ITerm = -1 * pid.windupGuard;
    }
    else if(pid.ITerm > pid.windupGuard){
        pid.ITerm = pid.windupGuard;
    }

    pid.DTerm = 0;
    pid.DTerm = delta_error / delta_time;

    pid.lastError = error;
    pid.output = pid.PTerm + (pid.Ki * pid.ITerm) + (pid.Kd + pid.DTerm)

    return;
}