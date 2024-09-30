#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <Arduino.h>

class DataLogger {
public:
    DataLogger();
    void logData(float ax, float ay, float rollFiltered = -1, float pitchFiltered = -1);  // Einheitliche Methode
};

#endif // DATALOGGER_H
