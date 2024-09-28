#include "MPU6050Calibration.h"

int MPU6050Calibration::calibrationStep = 0;
int MPU6050Calibration::totalSamples = 0;
float MPU6050Calibration::gyroXSum = 0;
float MPU6050Calibration::gyroYSum = 0;
float MPU6050Calibration::gyroZSum = 0;

void MPU6050Calibration::beginCalibration(int numSamples) {
    calibrationStep = 0;
    totalSamples = numSamples;
    gyroXSum = 0;
    gyroYSum = 0;
    gyroZSum = 0;
}

bool MPU6050Calibration::updateCalibration(MPU6050& mpu, float& gyroXOffset, float& gyroYOffset, float& gyroZOffset) {
    if (calibrationStep >= totalSamples) {
        // Berechne die Offsets
        gyroXOffset = gyroXSum / totalSamples;
        gyroYOffset = gyroYSum / totalSamples;
        gyroZOffset = gyroZSum / totalSamples;
        
        // Debug-Ausgabe, wenn die Kalibrierung abgeschlossen ist
        Serial.println("Kalibrierung abgeschlossen");  // Hier wird angezeigt, dass die Kalibrierung fertig ist
        return true;  // Kalibrierung abgeschlossen
    }

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);  // Hole die aktuellen Sensordaten

    // Summiere die Gyroskop-Daten für die Kalibrierung
    gyroXSum += g.gyro.x;
    gyroYSum += g.gyro.y;
    gyroZSum += g.gyro.z;

    calibrationStep++;

    if (calibrationStep % 100 == 0) {
        Serial.print("Kalibrierung Schritt: ");
        Serial.println(calibrationStep);
    }

    return false;  // Kalibrierung noch nicht abgeschlossen
}

void MPU6050Calibration::resetCalibration() {
    calibrationStep = 0;
    gyroXSum = 0;
    gyroYSum = 0;
    gyroZSum = 0;
}
