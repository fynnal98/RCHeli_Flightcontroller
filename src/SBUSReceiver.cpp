#include "SBUSReceiver.h"

// Verwende die richtigen Pins für SBUS, z.B. 16 für RX
SBUSReceiver::SBUSReceiver(HardwareSerial& serialPort)
    : sbus_rx(&serialPort, 16, -1, true) {}  // Passe die Parameter entsprechend deiner Konfiguration an

void SBUSReceiver::begin() {
    sbus_rx.Begin();
}

bool SBUSReceiver::readChannels(unsigned long& channel1, unsigned long& channel2, unsigned long& channel6) {
    if (sbus_rx.Read()) {
        data = sbus_rx.data();

        // Mappe die SBUS-Kanalwerte zu Servo-Pulsweiten (1000-2000 us)
        channel1 = map(data.ch[0], 172, 1811, 1000, 2000);
        channel2 = map(data.ch[1], 172, 1811, 1000, 2000);
        channel6 = map(data.ch[5], 172, 1811, 1000, 2000);
        return true;
    }
    return false;
}
