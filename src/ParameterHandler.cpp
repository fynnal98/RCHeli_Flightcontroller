// ParameterHandler.cpp
#include "ParameterHandler.h"

// Definition der Servo-Pins
int pinServo1 = 13;
int pinServo2 = 14;
int pinServo3 = 15;

// Definition der PID-Controller
PID pidRoll(70.0, 0.0, 0);
PID pidPitch(70.0, 0.0, 0);
PID pidYaw(90.0, 0.0, 5);

// Motor-Pins
const int mainMotorPin = 5;
const int tailMotorPin = 17;
const int tailRotorFactor = 1;

// SBUS-Pin
const int sbusPin = 16;

// Wire-Pins (SDA, SCL)
const int wireSDA = 21;
const int wireSCL = 22;

// Filterparameter
float lowPassAlpha = 0.2;
float highPassAlpha = 0.9;
int movingAvgWindowSize = 10;
float kalmanQ = 0.02;
float kalmanR = 0.2;
float kalmanEstimateError = 1.0;
float kalmanInitialEstimate = 0.0;

// Flags, um Filter zu aktivieren oder deaktivieren
bool useLowPass = true;
bool useHighPass = false;
bool useMovingAvg = false;
bool useKalman = false;

// CG-Offsets für den MPU
float cgOffsetX = -0.09;
float cgOffsetY = 0.0;
float cgOffsetZ = 0.0;

// Gyro Drift-Offsets
float gyroDriftOffsetX = 0.0;
float gyroDriftOffsetY = 0.0;
float gyroDriftOffsetZ = 0.0;

// Kalibrierungsstatus
bool calibrationCompleted = false;

void initializeParameters() {
    // Falls zusätzliche Initialisierungslogik benötigt wird, kann sie hier hinzugefügt werden.
}
