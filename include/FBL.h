#ifndef FBL_H
#define FBL_H

#include <ESP32Servo.h>
#include "PID.h"
#include "MPU6050.h"
#include "FilterHandler.h"

class FBL {
public:
    FBL(int pin1, int pin2, int pin3, FilterHandler& rollFilterHandler, FilterHandler& pitchFilterHandler);
    void setup();
    void update(MPU6050& mpu, unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse);

    Servo servo1, servo2, servo3;  // Public servos for direct control mode

private:
    int servo1Pin, servo2Pin, servo3Pin;

    // Filter handlers for roll and pitch
    FilterHandler& rollFilterHandler;
    FilterHandler& pitchFilterHandler;
};

#endif // FBL_H
