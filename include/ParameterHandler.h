// ParameterHandler.h
#ifndef PARAMETERHANDLER_H
#define PARAMETERHANDLER_H

#include "PID.h"  // PID-Klasse einbinden

// Servo-Pins
extern int pinServo1;
extern int pinServo2;
extern int pinServo3;

// PID-Controller
extern PID pidRoll;
extern PID pidPitch;
extern PID pidYaw;

extern float rollPIDFactor;
extern float pitchPIDFactor;
extern float yawPIDFactor;
extern float integralLimit;

// Motor-Pins
extern const int mainMotorPin;
extern const int tailMotorPin;
extern const int tailRotorFactor;

// SBUS
extern const int sbusPin;

// Wire-Pins (SDA, SCL)
extern const int wireSDA;
extern const int wireSCL;

// Filterparameter
extern float lowPassAlpha;
extern float highPassAlpha;
extern int movingAvgWindowSize;
extern float kalmanQ;
extern float kalmanR;
extern float kalmanEstimateError;
extern float kalmanInitialEstimate;

// Flags, um Filter zu aktivieren oder deaktivieren
extern bool useLowPass;
extern bool useHighPass;
extern bool useMovingAvg;
extern bool useKalman;

// CG-Offsets für den MPU
extern float cgOffsetX;
extern float cgOffsetY;
extern float cgOffsetZ;

// Gyro Drift-Offsets
extern float gyroDriftOffsetX;
extern float gyroDriftOffsetY;
extern float gyroDriftOffsetZ;

// Kalibrierungsparameter
extern const int calibrationDuration;

// Kalibrierungsstatus
extern bool calibrationCompleted;

void initializeParameters();

#endif
