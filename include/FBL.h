#ifndef FBL_H
#define FBL_H

#include <ESP32Servo.h>
#include "PID.h"
#include "MPU6050.h"
#include "LowPassFilter.h"
#include "HighPassFilter.h"
#include "MovingAverageFilter.h"
#include "KalmanFilter.h"  // Füge den Kalman-Filter hinzu

class FBL {
public:
    FBL(int pin1, int pin2, int pin3, float lowPassAlpha, float highPassAlpha, int movingAvgWindowSize, float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate);
    void setup();
    void update(MPU6050& mpu, PID& pidRoll, PID& pidPitch, unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse);

    Servo servo1, servo2, servo3;  // Public servos for direct control mode

private:
    int servo1Pin, servo2Pin, servo3Pin;

    LowPassFilter rollLowPassFilter;
    LowPassFilter pitchLowPassFilter;
    HighPassFilter rollHighPassFilter;
    HighPassFilter pitchHighPassFilter;

    MovingAverageFilter rollMovingAvgFilter;  // Moving Average Filter for roll
    MovingAverageFilter pitchMovingAvgFilter;  // Moving Average Filter for pitch

    KalmanFilter rollKalmanFilter;  // Kalman-Filter für die Roll-Achse
    KalmanFilter pitchKalmanFilter;  // Kalman-Filter für die Pitch-Achse
};

#endif // FBL_H
