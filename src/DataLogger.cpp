#include "DataLogger.h"

DataLogger::DataLogger() {
    // Konstruktor, falls notwendig
}

// Ausgabe sowohl der ungefilterten als auch der gefilterten Daten (ax, ay, rollFiltered, pitchFiltered)
void DataLogger::logData(float ax, float ay, float rollFiltered, float pitchFiltered) {
    Serial.print("Raw: ");
    Serial.print(ax); Serial.print(",");
    Serial.print(ay);

    // Wenn keine gefilterten Werte vorhanden sind, ersetze sie durch Rohwerte
    if (rollFiltered == -1 && pitchFiltered == -1) {
        rollFiltered = ax;
        pitchFiltered = ay;
    }

    Serial.print(" | Filtered: ");
    Serial.print(rollFiltered); Serial.print(",");
    Serial.println(pitchFiltered);  // Gefilterte Roll- und Pitch-Werte
}
