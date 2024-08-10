#include "FBL.h"

FBL::FBL(int pin1, int pin2, int pin3, float scaleFactor)
    : servo1Pin(pin1), servo2Pin(pin2), servo3Pin(pin3), scaleFactor(scaleFactor) {}

void FBL::setup() {
    servo1.attach(servo1Pin); // Back
    servo2.attach(servo2Pin); // Left
    servo3.attach(servo3Pin); // Right
}

void FBL::update(MPU6050& mpu, PID& pidRoll, PID& pidPitch, unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse) {
    // Sensordaten abrufen
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // PID-Korrekturwerte berechnen
    float rollCorrection = pidRoll.compute(0, a.acceleration.x) * scaleFactor;
    float pitchCorrection = pidPitch.compute(0, a.acceleration.y) * scaleFactor;

    // Korrekturdaten zu den Kanaldaten addieren
    unsigned long servo1Pulse = channel2Pulse + pitchCorrection; // Back
    unsigned long servo2Pulse = channel6Pulse + (-0.5 * pitchCorrection + 0.866 * rollCorrection); // Left
    unsigned long servo3Pulse = channel1Pulse - (-0.5 * pitchCorrection - 0.866 * rollCorrection); // Right

    // Setze die Pulslängen an die Servos
    servo1.writeMicroseconds(servo1Pulse); // Back
    servo2.writeMicroseconds(servo2Pulse); // Left
    servo3.writeMicroseconds(servo3Pulse); // Right
}
