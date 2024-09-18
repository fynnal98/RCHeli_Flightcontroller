#include "FBL.h"

// Konstruktor mit Offsets
FBL::FBL(int pin1, int pin2, int pin3, float offsetX, float offsetY, float offsetZ)
    : servo1Pin(pin1), servo2Pin(pin2), servo3Pin(pin3), dx(offsetX), dy(offsetY), dz(offsetZ) {}

void FBL::setup() {
    servo1.attach(servo1Pin); // Back
    servo2.attach(servo2Pin); // Left
    servo3.attach(servo3Pin); // Right
}

void FBL::update(MPU6050& mpu, PID& pidRoll, PID& pidPitch, unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse) {
    // Sensordaten abrufen
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Korrigierte Beschleunigungen berechnen unter Berücksichtigung der Offsets
    float ax_corrected = a.acceleration.x - (g.gyro.y * dz - g.gyro.z * dy);
    float ay_corrected = a.acceleration.y - (g.gyro.z * dx - g.gyro.x * dz);
    float az_corrected = a.acceleration.z - (g.gyro.x * dy - g.gyro.y * dx);

    // PID-Korrekturwerte berechnen
    float rollCorrection = pidRoll.compute(0, ax_corrected);
    float pitchCorrection = pidPitch.compute(0, ay_corrected);

    // Korrekturdaten zu den Kanaldaten addieren
    unsigned long servo1Pulse = channel2Pulse + pitchCorrection; // Back
    unsigned long servo2Pulse = channel6Pulse + (-0.5 * pitchCorrection + 0.866 * rollCorrection); // Left
    unsigned long servo3Pulse = channel1Pulse - (-0.5 * pitchCorrection - 0.866 * rollCorrection); // Right

    // Setze die Pulslängen an die Servos
    servo1.writeMicroseconds(servo1Pulse); // Back
    servo2.writeMicroseconds(servo2Pulse); // Left
    servo3.writeMicroseconds(servo3Pulse); // Right
}
