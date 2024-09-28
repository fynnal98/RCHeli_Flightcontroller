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

float lowPassAlpha = 0.2;
float highPassAlpha = 1.0;
int movingAvgWindowSize = 5;

int pinServo1 = 13;
int pinServo2 = 14;
int pinServo3 = 15;

float cgOffsetX = 0.0;
float cgOffsetY = -0.09;
float cgOffsetZ = 0.0;

float gyroDriftOffsetX = 0.0;
float gyroDriftOffsetY = 0.0;
float gyroDriftOffsetZ = 0.0;

bool calibrationCompleted = false;

FBL fbl(pinServo1, pinServo2, pinServo3, lowPassAlpha, highPassAlpha, movingAvgWindowSize);

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
