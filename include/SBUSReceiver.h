#ifndef SBUS_RECEIVER_H
#define SBUS_RECEIVER_H

#include "sbus.h"


class SBUSReceiver {
public:
    SBUSReceiver(HardwareSerial& serialPort);
    void begin();
    bool readChannels(unsigned long& channel1, unsigned long& channel2, unsigned long& channel6);

private:
    bfs::SbusRx sbus_rx;  // Wir verwenden jetzt das Objekt direkt und nicht mehr über einen Zeiger
    bfs::SbusData data;   // Dasselbe gilt hier
};

#endif // SBUS_RECEIVER_H
