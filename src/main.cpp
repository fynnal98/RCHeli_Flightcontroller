#include <Arduino.h>
#include <ESP32Servo.h>
#include "SBUSReceiver.h"
#include "MPU6050.h"
#include "PID.h"
#include "FBL.h"
#include "WDT.h"
#include "MainMotor.h"
#include "Util.h"
#include "TailRotor.h"

const int sbusPin = 16;
SBUSReceiver sbusReceiver(Serial2);

MPU6050 mpu;
PID pidRoll(90.0, 0.1, 10);
PID pidPitch(90.0, 0.1, 10);

// Tiefpassfilter-Parameter
float alpha = 0.15;  // Alpha-Wert für den Tiefpassfilter

int pinServo1 = 13;
int pinServo2 = 14;
int pinServo3 = 15;

float offsetGyroX = 0.0;
float offsetGyroY = -0.09;
float offsetGyroZ = 0.0;

// Flightcontroller Setup
FBL fbl(pinServo1, pinServo2, pinServo3, offsetGyroX, offsetGyroY, offsetGyroZ, alpha);  // Pins, Offsets und Alpha

const int mainMotorPin = 5;   // Pin für den ESC des Hauptmotors
const int tailMotorPin = 17;  // Pin für den ESC des Heckmotors

MainMotor mainMotorServo(mainMotorPin);  // Servo-Objekt für den Hauptmotor
float scalingFactorTailRotor = 1;

TailRotor tailRotor(tailMotorPin, scalingFactorTailRotor);

void setup() {
    Serial.begin(115200);  // Startet die serielle Kommunikation

    initWatchdog(2);  // Initialisiert den Watchdog-Timer

    sbusReceiver.begin();
    Wire.begin(21, 22);
    mpu.begin();
    mpu.setup();
    fbl.setup();
    tailRotor.setup();  // Setup für den Heckrotor

    // Attache den Servo (Motor) an den Pin
    mainMotorServo.setup();

    Serial.println("Setup abgeschlossen");
}

void loop() {
    unsigned int channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel10Pulse;

    resetWatchdog();  // Setzt den Watchdog-Timer zurück

    // Liest die Werte der Kanäle 1, 2, 4, 6 und 8
    if (sbusReceiver.readChannels(channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel10Pulse)) {

        if (Util::correctionEnabled(channel10Pulse)) {
            // FBL-Update mit den gefilterten Werten durchführen
            fbl.update(mpu, pidRoll, pidPitch, channel1Pulse, channel2Pulse, channel6Pulse);

            // Heckrotor aktualisieren
            tailRotor.update(channel8Pulse, channel4Pulse);
        } else {
            // FBL und Filter deaktiviert – direkte Steuerdaten an die Servos weitergeben
            fbl.servo1.writeMicroseconds(channel2Pulse);  // Back
            fbl.servo2.writeMicroseconds(channel6Pulse);  // Left
            fbl.servo3.writeMicroseconds(channel1Pulse);  // Right

            // Heckrotor direkt mit Kanal 8 und 4 steuern
            tailRotor.update(channel8Pulse, channel4Pulse);
        }

        // Hauptmotor steuern
        mainMotorServo.setPulse(channel8Pulse);
    } else {
        Serial.println("Fehler beim Lesen der Kanäle.");
    }

    delay(10);  // Verzögert die Ausgabe, um Stabilität zu gewährleisten
}
