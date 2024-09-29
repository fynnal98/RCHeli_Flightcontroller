#include <Arduino.h>
#include <ESP32Servo.h>
#include "SBUSReceiver.h"
#include "MPU6050.h"
#include "MPU6050Calibration.h"
#include "PID.h"
#include "FBL.h"
#include "WDT.h"
#include "MainMotor.h"
#include "Util.h"
#include "TailRotor.h"
#include "FilterHandler.h"

// SBUS Pin und Empfänger
const int sbusPin = 16;
SBUSReceiver sbusReceiver(Serial2);

// MPU und PID
MPU6050 mpu;
PID pidRoll(90.0, 0.0, 10);
PID pidPitch(90.0, 0.0, 10);
PID pidYaw(90.0, 0.0, 10);

// Filterparameter
float lowPassAlpha = 0.9;
float highPassAlpha = 0.95;
int movingAvgWindowSize = 1.0;

float kalmanQ = 0.001;
float kalmanR = 0.1;
float kalmanEstimateError = 1.0;
float kalmanInitialEstimate = 0.0;

// Flags, um Filter zu aktivieren oder deaktivieren
bool useLowPass = true;
bool useHighPass = true;
bool useMovingAvg = true;
bool useKalman = true;

int pinServo1 = 13;
int pinServo2 = 14;
int pinServo3 = 15;

// CG-Offsets für den MPU
float cgOffsetX = -0.09;
float cgOffsetY = 0.001;
float cgOffsetZ = 0.0;

float gyroDriftOffsetX = 0.0;
float gyroDriftOffsetY = 0.0;
float gyroDriftOffsetZ = 0.0;

bool calibrationCompleted = false;

// FilterHandler für Roll und Pitch erstellen
FilterHandler rollFilterHandler(lowPassAlpha, highPassAlpha, movingAvgWindowSize, kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate, pidRoll);
FilterHandler pitchFilterHandler(lowPassAlpha, highPassAlpha, movingAvgWindowSize, kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate, pidPitch);

// FBL-Objekt
FBL fbl(pinServo1, pinServo2, pinServo3, rollFilterHandler, pitchFilterHandler);

// Motoren
const int mainMotorPin = 5;
const int tailMotorPin = 17;

MainMotor mainMotorServo(mainMotorPin);
TailRotor tailRotor(tailMotorPin, 1, pidYaw);

void setup() {
    Serial.begin(115200);

    initWatchdog(2);

    sbusReceiver.begin();
    Wire.begin(21, 22);
    mpu.begin();
    mpu.setup();

    Serial.println("Starte Gyroskop-Kalibrierung...");
    MPU6050Calibration::beginCalibration(1000);

    fbl.setup();
    tailRotor.setup();
    mainMotorServo.setup();

    Serial.println("Setup abgeschlossen");
}

void loop() {
    unsigned int channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel10Pulse;

    resetWatchdog();

    if (!calibrationCompleted) {
        calibrationCompleted = MPU6050Calibration::updateCalibration(mpu, gyroDriftOffsetX, gyroDriftOffsetY, gyroDriftOffsetZ);
        if (calibrationCompleted) {
            Serial.println("Gyroskop-Kalibrierung abgeschlossen");
        }
        return;
    }

    if (sbusReceiver.readChannels(channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel10Pulse)) {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        mpu.applyGyroOffset(g, gyroDriftOffsetX, gyroDriftOffsetY, gyroDriftOffsetZ);
        float yawRate = g.gyro.z;

        if (Util::correctionEnabled(channel10Pulse)) {
            // Übergib die Filter-Flags und wende die Filter an
            fbl.update(mpu, channel1Pulse, channel2Pulse, channel6Pulse, useLowPass, useHighPass, useMovingAvg, useKalman);
            tailRotor.update(channel8Pulse, channel4Pulse, yawRate);
        } else {
            fbl.servo1.writeMicroseconds(channel2Pulse);
            fbl.servo2.writeMicroseconds(channel6Pulse);
            fbl.servo3.writeMicroseconds(channel1Pulse);

            tailRotor.update(channel8Pulse, channel4Pulse, 0);
        }

        mainMotorServo.setPulse(channel8Pulse);
    } else {
        Serial.println("Fehler beim Lesen der Kanäle.");
    }

    delay(20);
}
