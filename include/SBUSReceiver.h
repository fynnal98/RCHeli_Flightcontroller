#ifndef SBUS_RECEIVER_H
#define SBUS_RECEIVER_H

#include "sbus.h"

class SBUSReceiver {
public:
    SBUSReceiver(HardwareSerial& serialPort);
    void begin();
    
    // Methode zum Lesen mehrerer Kanäle gleichzeitig (jetzt mit 5 Argumenten)
    bool readChannels(unsigned long& channel1, unsigned long& channel2, unsigned long& channel4, unsigned long& channel6, unsigned long& channel8);

private:
    bfs::SbusRx sbus_rx;
    bfs::SbusData data;
};

#endif // SBUS_RECEIVER_H
