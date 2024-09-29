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

const int sbusPin = 16;
SBUSReceiver sbusReceiver(Serial2);

MPU6050 mpu;
PID pidRoll(90.0, 0.0, 10); 
PID pidPitch(90.0, 0.0, 10);  
PID pidYaw(90.0, 0.0, 10); 

// Filterparameter
float lowPassAlpha = 0.9;
float highPassAlpha = 0.95;
int movingAvgWindowSize = 1.0;

float kalmanQ = 0.001;  // Prozessrauschen für den Kalman-Filter // Startwert: 0.001 bis 0.01
float kalmanR = 0.1;    // Messrauschen für den Kalman-Filter // Startwert: 0.1 bis 0.5
float kalmanEstimateError = 1.0;  // Fehler in der anfänglichen Schätzung
float kalmanInitialEstimate = 0.0;  // Anfangswert für die Schätzung

int pinServo1 = 13;
int pinServo2 = 14;
int pinServo3 = 15;

float cgOffsetX = -0.09;
float cgOffsetY = 0.001;
float cgOffsetZ = 0.0;

float gyroDriftOffsetX = 0.0;
float gyroDriftOffsetY = 0.0;
float gyroDriftOffsetZ = 0.0;

bool calibrationCompleted = false;

// FBL-Konstruktor mit allen Filterparametern
FBL fbl(pinServo1, pinServo2, pinServo3, lowPassAlpha, highPassAlpha, movingAvgWindowSize, kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate);

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
            fbl.update(mpu, pidRoll, pidPitch, channel1Pulse, channel2Pulse, channel6Pulse);
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
