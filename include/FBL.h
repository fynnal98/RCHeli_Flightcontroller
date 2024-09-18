#ifndef FBL_H
#define FBL_H

#include <ESP32Servo.h>
#include "PID.h"
#include "MPU6050.h"

class FBL {
public:
    FBL(int pin1, int pin2, int pin3, float offsetX, float offsetY, float offsetZ);
    void setup();
    void update(MPU6050& mpu, PID& pidRoll, PID& pidPitch, unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse);

private:
    Servo servo1, servo2, servo3;
    int servo1Pin, servo2Pin, servo3Pin;
    float dx, dy, dz;  // Offsets für den Sensor
};

#endif // FBL_H
