#include "SBUSReceiver.h"

// Verwende die richtigen Pins für SBUS, z.B. 16 für RX
SBUSReceiver::SBUSReceiver(HardwareSerial& serialPort)
    : sbus_rx(&serialPort, 16, -1, true) {}

void SBUSReceiver::begin() {
    sbus_rx.Begin();
}

// Methode zum Lesen mehrerer Kanäle gleichzeitig
bool SBUSReceiver::readChannels(unsigned long& channel1, unsigned long& channel2, unsigned long& channel4, unsigned long& channel6, unsigned long& channel8) {
    if (sbus_rx.Read()) {
        data = sbus_rx.data();

        // Mappe die SBUS-Kanalwerte zu Servo-Pulsweiten (1000-2000 us)
        channel1 = map(data.ch[0], 172, 1811, 1000, 2000);
        channel2 = map(data.ch[1], 172, 1811, 1000, 2000);
        channel4 = map(data.ch[3], 172, 1811, 1000, 2000); // Channel 4
        channel6 = map(data.ch[5], 172, 1811, 1000, 2000);
        channel8 = map(data.ch[7], 172, 1811, 1000, 2000);

        return true;
    }
    Serial.println("SBUS-Daten konnten nicht gelesen werden.");
    return false;
}
