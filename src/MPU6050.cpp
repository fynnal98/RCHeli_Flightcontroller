#include "MPU6050.h"
#include "DataLogger.h"

#define MPU6050_ADDRESS 0x68  // Standard-I2C-Adresse des MPU6050
#define MPU6050_SMPLRT_DIV 0x19  // Register für die Abtastrate

MPU6050::MPU6050() : mpu() {}

void MPU6050::begin() {
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);  // Bandbreite auf 260 Hz setzen

    // Abtastrate explizit auf 1 kHz setzen
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPU6050_SMPLRT_DIV);
    Wire.write(0x00);  // Setze Sample Rate Divider auf 0 (1 kHz)
    Wire.endTransmission();
}

void MPU6050::setup() {
    // Keine zusätzliche Einrichtung erforderlich
}

void MPU6050::getEvent(sensors_event_t* a, sensors_event_t* g, sensors_event_t* temp) {
    // Erfassung der MPU6050-Sensordaten
    mpu.getEvent(a, g, temp);

    // Logger-Objekt erstellen
    DataLogger dataLogger;

    // Logge die ungefilterten Daten (nur 6 Parameter)
    dataLogger.logData(a->acceleration.x, a->acceleration.y, a->acceleration.z,
                       g->gyro.x, g->gyro.y, g->gyro.z);
}
