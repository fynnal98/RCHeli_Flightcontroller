#include "MPU6050.h"
#include "DataLogger.h"

#define MPU6050_ADDRESS 0x68  // Standard-I2C-Adresse des MPU6050
#define MPU6050_SMPLRT_DIV 0x19  // Register für die Abtastrate

MPU6050::MPU6050() : mpu(), cgOffsetX(0.0), cgOffsetY(0.0), cgOffsetZ(0.0) {}

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

// Methode zur Anwendung der Gyroskop-Offsets (Driftkorrektur)
void MPU6050::applyGyroOffset(sensors_event_t& g, float gyroXOffset, float gyroYOffset, float gyroZOffset) {
    g.gyro.x -= gyroXOffset;
    g.gyro.y -= gyroYOffset;
    g.gyro.z -= gyroZOffset;
}

// Berechnet die korrigierten Beschleunigungswerte unter Berücksichtigung der CG-Offsets
void MPU6050::calculateCorrectedAccelerations(sensors_event_t* a, sensors_event_t* g, float& ax_corrected, float& ay_corrected) {
    // Korrigierte Beschleunigungen berechnen unter Berücksichtigung der CG-Offsets
    ax_corrected = a->acceleration.x - (g->gyro.y * cgOffsetZ - g->gyro.z * cgOffsetY);
    ay_corrected = a->acceleration.y - (g->gyro.z * cgOffsetX - g->gyro.x * cgOffsetZ);
}

// Setzt die CG-Offsets für die spätere Berechnung
void MPU6050::setCGOffsets(float offsetX, float offsetY, float offsetZ) {
    this->cgOffsetX = offsetX;
    this->cgOffsetY = offsetY;
    this->cgOffsetZ = offsetZ;
}
