#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <Arduino.h>

class DataLogger {
public:
    DataLogger();
    // Methode für ungefilterte Daten (6 Parameter)
    void logData(float ax, float ay, float az, float gx, float gy, float gz);
    
    // Methode für gefilterte Daten (8 Parameter)
    void logData(float ax, float ay, float az, float gx, float gy, float gz, float rollFiltered, float pitchFiltered);
};

#endif // DATALOGGER_H
