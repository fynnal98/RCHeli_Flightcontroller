#include <Arduino.h>
#include "SBUSReceiver.h"
#include "MPU6050.h"
#include "PID.h"
#include "FBL.h"
#include "WDT.h"  // Füge die Watchdog-Headerdatei hinzu

const int sbusPin = 16;
SBUSReceiver sbusReceiver(Serial2);

MPU6050 mpu;
PID pidRoll(3.0, 0.0, 0.2);
PID pidPitch(3.0, 0.0, 0.2);
FBL fbl(13, 14, 15, 30.0);

void setup() {
    Serial.begin(115200);

    initWatchdog(2);  // Initialisiere den Watchdog-Timer mit einem Timeout von 3 Sekunden

    sbusReceiver.begin();
    Wire.begin(21, 22);
    mpu.begin();
    mpu.setup();
    fbl.setup();

    Serial.println("Setup abgeschlossen");
}

void loop() {
    unsigned long channel1Pulse, channel2Pulse, channel6Pulse;

    resetWatchdog();  // Setze den Watchdog-Timer zurück

    if (sbusReceiver.readChannels(channel1Pulse, channel2Pulse, channel6Pulse)) {
        fbl.update(mpu, pidRoll, pidPitch, channel1Pulse, channel2Pulse, channel6Pulse);
    }

    delay(20);
}
