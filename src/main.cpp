#include <Arduino.h>
#include <ESP32Servo.h>
#include "SBUSReceiver.h"
#include "MPU6050.h"
#include "PID.h"
#include "FBL.h"
#include "WDT.h"
#include "TailRotor.h"

const int sbusPin = 16;
SBUSReceiver sbusReceiver(Serial2);

MPU6050 mpu;
PID pidRoll(90.0, 0.1, 10);
PID pidPitch(90.0, 0.1, 10);

// Tiefpassfilter-Parameter
float alpha = 0.5;  // Alpha-Wert für den Tiefpassfilter

// Flightcontroller Setup
FBL fbl(13, 14, 15, 0.0, -0.09, 0.0, alpha);  // Pins, Offsets und Alpha

const int mainMotorPin = 5;   // Pin für den ESC des Hauptmotors
const int tailMotorPin = 17;  // Pin für den ESC des Heckmotors

Servo mainMotorServo;  // Servo-Objekt für den Hauptmotor
TailRotor tailRotor(tailMotorPin, 1);

void setup() {
    Serial.begin(115200); 

    initWatchdog(2);  

    sbusReceiver.begin();
    Wire.begin(21, 22);
    mpu.begin();
    mpu.setup();
    fbl.setup();
    tailRotor.setup();  

    mainMotorServo.attach(mainMotorPin);

    Serial.println("Setup abgeschlossen");
}

void loop() {
    unsigned long channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel10Pulse;

    resetWatchdog(); 

    if (sbusReceiver.readChannels(channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse)) {
        channel10Pulse = sbusReceiver.getChannel10Pulse(); 

        if (channel10Pulse > 1500) {
            sensors_event_t a, g, temp;
            mpu.getEvent(&a, &g, &temp); 

            fbl.update(mpu, pidRoll, pidPitch, channel1Pulse, channel2Pulse, channel6Pulse);

            tailRotor.update(channel8Pulse, channel4Pulse);
        } else {
            fbl.servo1.writeMicroseconds(channel2Pulse);  // Back
            fbl.servo2.writeMicroseconds(channel6Pulse);  // Left
            fbl.servo3.writeMicroseconds(channel1Pulse);  // Right

            tailRotor.update(channel8Pulse, channel4Pulse);
        }
        mainMotorServo.writeMicroseconds(channel8Pulse);
    } else {
        Serial.println("Fehler beim Lesen der Kanäle.");
    }

    delay(10);  
}
