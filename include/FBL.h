#ifndef FBL_H
#define FBL_H

#include <ESP32Servo.h>
#include "MPU6050.h"
#include "FilterHandler.h"
#include "DataLogger.h"

class FBL {
public:
    FBL(int pin1, int pin2, int pin3, FilterHandler& rollFilterHandler, FilterHandler& pitchFilterHandler, DataLogger& logger);
    void setup();
    void update(MPU6050& mpu, unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse, bool useLowPass, bool useHighPass, bool useMovingAvg, bool useKalman);

    Servo servo1, servo2, servo3;  // Public servos for direct control mode

private:
    int servo1Pin, servo2Pin, servo3Pin;
    FilterHandler& rollFilterHandler;
    FilterHandler& pitchFilterHandler;
    DataLogger& logger;  // Logger hinzufügen
};

#endif // FBL_H
