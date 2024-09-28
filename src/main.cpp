#include <Arduino.h>
#include <ESP32Servo.h>
#include "SBUSReceiver.h"
#include "MPU6050.h"
#include "MPU6050Calibration.h"  // Füge die asynchrone Kalibrierung hinzu
#include "PID.h"
#include "FBL.h"
#include "WDT.h"
#include "MainMotor.h"
#include "Util.h"
#include "TailRotor.h"

// SBUS Pin und Empfänger
const int sbusPin = 16;
SBUSReceiver sbusReceiver(Serial2);

// MPU und PID
MPU6050 mpu;
PID pidRoll(90.0, 0.1, 10);
PID pidPitch(90.0, 0.1, 10);
PID pidYaw(0.0, 0.0, 0);  

// Filterparameter
float lowPassAlpha = 0.5;  // Low-pass filter alpha
float highPassAlpha = 1.0;  // High-pass filter alpha
int movingAvgWindowSize = 5;  // Moving average filter window size (N)

// CG-Offsets für den MPU vom CG des Helikopters
float cgOffsetX = 0.0;  // X-Offset (Seitliche Verschiebung)
float cgOffsetY = 0;  // Y-Offset (Höhenunterschied)
float cgOffsetZ = 0.0;  // Z-Offset (Vorwärts/Rückwärts Verschiebung)

// Gyroskop-Drift-Offsets
float gyroDriftOffsetX = 0.0;
float gyroDriftOffsetY = 0.0;
float gyroDriftOffsetZ = 0.0;

// Kalibrierung abgeschlossen
bool calibrationCompleted = false;

// Flight controller setup ohne CG Offsets (die werden in MPU6050.cpp verarbeitet)
FBL fbl(13, 14, 15, lowPassAlpha, highPassAlpha, movingAvgWindowSize);  // Includes moving average filter size

const int mainMotorPin = 5;   // Pin für den ESC des Hauptmotors
const int tailMotorPin = 17;  // Pin für den ESC des Heckmotors

MainMotor mainMotorServo(mainMotorPin);  // Hauptmotor-Servo-Objekt
float scalingFactorTailRotor = 1;

TailRotor tailRotor(tailMotorPin, scalingFactorTailRotor);

void setup() {
    Serial.begin(115200);  // Start serial communication

    initWatchdog(2);  // Initialize the watchdog timer

    // SBUS-Empfänger und MPU6050 initialisieren
    sbusReceiver.begin();
    Wire.begin(21, 22);
    mpu.begin();
    mpu.setup();

    // Starte die Kalibrierung
    Serial.println("Starte Gyroskop-Kalibrierung...");
    MPU6050Calibration::beginCalibration(1000);  // Kalibriere mit 1000 Samples

    // Setup für FBL, Hauptmotor und Heckrotor
    fbl.setup();
    tailRotor.setup();  // Setup für den Heckrotor
    mainMotorServo.setup();  // Hauptmotor an den Pin anhängen

    Serial.println("Setup abgeschlossen");
}

void loop() {
    unsigned int channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel10Pulse;

    resetWatchdog();  // Setze den Watchdog zurück

    // Führe die Kalibrierung schrittweise durch
    if (!calibrationCompleted) {
        calibrationCompleted = MPU6050Calibration::updateCalibration(mpu, gyroDriftOffsetX, gyroDriftOffsetY, gyroDriftOffsetZ);
        if (calibrationCompleted) {
            Serial.println("Gyroskop-Kalibrierung abgeschlossen");
        }
        // Warte, bis die Kalibrierung abgeschlossen ist, bevor der Rest des Codes läuft
        return;
    }

    // Hole die Kanalwerte für die Steuerung
    if (sbusReceiver.readChannels(channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel10Pulse)) {
        
        // Hole die aktuellen Sensordaten
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        // Wende die Gyroskop-Driftkorrektur an
        mpu.applyGyroOffset(g, gyroDriftOffsetX, gyroDriftOffsetY, gyroDriftOffsetZ);
        float yawRate = g.gyro.z;  // Yaw-Rate vom Gyroskop

        if (Util::correctionEnabled(channel10Pulse)) {
            // FBL-Update mit den gefilterten Werten (Low-Pass, High-Pass, Moving Average)
            fbl.update(mpu, pidRoll, pidPitch, channel1Pulse, channel2Pulse, channel6Pulse);

            // Aktualisiere den Heckrotor mit Yaw-Korrektur
            tailRotor.update(channel8Pulse, channel4Pulse, yawRate);  
        } else {
            // FBL und Filter deaktiviert – Rohdaten an die Servos weitergeben
            fbl.servo1.writeMicroseconds(channel2Pulse);  // Back
            fbl.servo2.writeMicroseconds(channel6Pulse);  // Left
            fbl.servo3.writeMicroseconds(channel1Pulse);  // Right

            // Aktualisiere den Heckrotor direkt mit Kanalwerten (ohne Gyroskop-Einfluss)
            tailRotor.update(channel8Pulse, channel4Pulse, 0);  
        }

        // Steuere den Hauptmotor
        mainMotorServo.setPulse(channel8Pulse);
    } else {
        Serial.println("Fehler beim Lesen der Kanäle.");
    }

    delay(20);
}
