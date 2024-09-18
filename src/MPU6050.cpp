#include "MPU6050.h"

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
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void MPU6050::setup() {
    // Keine zusätzliche Einrichtung erforderlich
}

void MPU6050::getEvent(sensors_event_t* a, sensors_event_t* g, sensors_event_t* temp) {
    mpu.getEvent(a, g, temp);
    

    // Serial.print(a->acceleration.x); Serial.print(",");
    // Serial.print(a->acceleration.y); Serial.print(",");
    // Serial.print(a->acceleration.z); Serial.print(",");
    // Serial.print(g->gyro.x); Serial.print(",");
    // Serial.print(g->gyro.y); Serial.println(g->gyro.z); 
}
