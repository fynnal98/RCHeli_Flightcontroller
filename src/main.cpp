#include <Arduino.h>
#include "SBUSReceiver.h"
#include "MPU6050.h"
#include "PID.h"
#include "FBL.h"

// Konfiguriere deinen SBUS-Empfänger auf dem entsprechenden Pin
const int sbusPin = 16;  // Pin für SBUS
SBUSReceiver sbusReceiver(Serial2);  // Verwende Serial2 für SBUS

MPU6050 mpu;
PID pidRoll(2.0, 0.0, 0.1);
PID pidPitch(2.0, 0.0, 0.1);
FBL fbl(13, 14, 15, 40.0);

void setup() {
    Serial.begin(115200);
    sbusReceiver.begin();

    Wire.begin(21, 22);
    mpu.begin();
    mpu.setup();
    fbl.setup();
}

void loop() {
    unsigned long channel1Pulse, channel2Pulse, channel6Pulse;

    // SBUS-Daten einlesen und debuggen
    if (sbusReceiver.readChannels(channel1Pulse, channel2Pulse, channel6Pulse)) {
        // Debug-Ausgaben für die empfangenen Kanäle
        Serial.print("Channel 1: "); Serial.println(channel1Pulse);
        Serial.print("Channel 2: "); Serial.println(channel2Pulse);
        Serial.print("Channel 6: "); Serial.println(channel6Pulse);

        // Verarbeite die Kanäle und steuere die Servos
        fbl.update(mpu, pidRoll, pidPitch, channel1Pulse, channel2Pulse, channel6Pulse);
    }

    delay(20);  // Kurze Pause, um das System zu stabilisieren
}
