#ifndef SBUSRECEIVER_H
#define SBUSRECEIVER_H

#include <Arduino.h>
#include <SBUS.h>

class SBUSReceiver {
public:
    SBUSReceiver(HardwareSerial& serialPort);
    void begin();
    bool readChannels(unsigned long& channel1, unsigned long& channel2, unsigned long& channel4, unsigned long& channel6, unsigned long& channel8);
    
    // Neue Methode zum Abrufen des Werts von Kanal 10
    unsigned long getChannel10Pulse();

private:
    bfs::SbusRx sbus_rx;
    bfs::SbusData data;
};

#endif
