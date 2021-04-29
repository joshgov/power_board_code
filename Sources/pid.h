#ifndef PID_H
#define PID_H

#include "main.h"

#define PID_PERIOD_LENGTH 10

typedef struct PID PID;

struct PID {
     uint8_t Kp;
     uint8_t Ki;
     uint8_t Kd;

     uint8_t PTerm;
     uint8_t ITerm;
     uint8_t DTerm;

     uint8_t lastError;
     uint8_t lastTime;

     uint8_t setpoint;
     uint8_t intError;
     uint8_t windupGuard;

     uint8_t feedback_value;
     uint8_t output;
} pid;

#endif /* PID_H */