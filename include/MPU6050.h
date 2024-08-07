#ifndef MPU6050_H
#define MPU6050_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>

class MPU6050 {
public:
    MPU6050();
    void begin();
    void setup();
    void getEvent(sensors_event_t* a, sensors_event_t* g, sensors_event_t* temp);

private:
    Adafruit_MPU6050 mpu;
};

#endif // MPU6050_H
