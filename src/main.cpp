#include <Arduino.h>
#include "MPU6050.h"
#include "PID.h"
#include "FBL.h"

// Pinbelegung für Empfangskanäle
const int channel1Pin = 34; // Kanal 1 (Rechter Servo)
const int channel2Pin = 35; // Kanal 2 (Hinterer Servo)
const int channel6Pin = 32; // Kanal 6 (Linker Servo)

// Skalierungsfaktor
const float scaleFactor = 10.0;

MPU6050 mpu;
PID pidRoll(2.0, 0.0, 0.1); // Roll-PID
PID pidPitch(2.0, 0.0, 0.1); // Pitch-PID
FBL fbl(13, 14, 15, scaleFactor); // Pins für die Servos: Back, Left, Right

void setup() {
    Serial.begin(115200);

    // Konfiguriere die Pins als Eingänge
    pinMode(channel1Pin, INPUT);
    pinMode(channel2Pin, INPUT);
    pinMode(channel6Pin, INPUT);

    // Initialisiere MPU und FBL
    Wire.begin(21, 22);
    mpu.begin();
    mpu.setup();
    fbl.setup();
}

void loop() {
    // Lese die Pulslängen von den Kanälen
    unsigned long channel1Pulse = pulseIn(channel1Pin, HIGH);
    unsigned long channel2Pulse = pulseIn(channel2Pin, HIGH);
    unsigned long channel6Pulse = pulseIn(channel6Pin, HIGH);

    // FBL Update aufrufen und Servo-Pulse setzen
    fbl.update(mpu, pidRoll, pidPitch, channel1Pulse, channel2Pulse, channel6Pulse);

    delay(20);
}
