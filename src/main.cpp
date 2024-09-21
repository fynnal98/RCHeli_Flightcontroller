#include <Arduino.h>
#include <ESP32Servo.h>
#include "SBUSReceiver.h"
#include "MPU6050.h"
#include "PID.h"
#include "FBL.h"
#include "WDT.h"
#include "TailRotor.h"
#include "NotchFilter.h"  

const int sbusPin = 16;
SBUSReceiver sbusReceiver(Serial2);

MPU6050 mpu;
PID pidRoll(90.0, 0.1, 10);
PID pidPitch(90.0, 0.1, 10);

// Tiefpassfilter-Parameter
float alpha = 0.15;  // Alpha-Wert für den Tiefpassfilter

// Bandsperrfilter-Parameter
float notchFreq = 0.0;  // Notch-Frequenz in Hz
float bandwidth = 0.0;  // Bandbreite des Notch-Filters in Hz
float sampleRate = 500.0;  // Abtastrate in Hz

// Flightcontroller Setup
FBL fbl(13, 14, 15, 0.0, -0.09, 0.0, alpha);  // Pins, Offsets und Alpha

const int mainMotorPin = 5;   // Pin für den ESC des Hauptmotors
const int tailMotorPin = 17;  // Pin für den ESC des Heckmotors

Servo mainMotorServo;  // Servo-Objekt für den Hauptmotor
TailRotor tailRotor(tailMotorPin, 1);

// Initialisiere den Bandsperrfilter
NotchFilter notchFilterX(sampleRate, notchFreq, bandwidth);
NotchFilter notchFilterY(sampleRate, notchFreq, bandwidth);

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
    mainMotorServo.attach(mainMotorPin);

    Serial.println("Setup abgeschlossen");
}

void loop() {
    unsigned long channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse;

    resetWatchdog();  // Setzt den Watchdog-Timer zurück

    // Liest die Werte der Kanäle 1, 2, 4, 6 und 8
    if (sbusReceiver.readChannels(channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse)) {

        // Direkte Ausgabe des PWM-Werts auf Pin 5 für den Hauptmotor
        mainMotorServo.writeMicroseconds(channel8Pulse);

        // MPU-Daten auslesen und über die serielle Schnittstelle senden
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);  // Daten werden automatisch über den DataLogger gesendet

        // FBL-Update und Heckrotor-Steuerung
        fbl.update(mpu, pidRoll, pidPitch, channel1Pulse, channel2Pulse, channel6Pulse);
        tailRotor.update(channel8Pulse, channel4Pulse);  // TailRotor übernimmt jetzt die Berechnung für den Heckrotor
    } else {
        Serial.println("Fehler beim Lesen der Kanäle.");
    }

    delay(10);  // Verzögert die Ausgabe
}
