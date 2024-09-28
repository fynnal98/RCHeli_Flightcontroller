#ifndef TAILROTOR_H
#define TAILROTOR_H

#include <ESP32Servo.h>
#include "PID.h"

class TailRotor {
public:
    TailRotor(int motorPin, float scaleFactor);  // Konstruktor
    void setup();  // Setup des Servos
    void update(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawRate);  // Aktualisiert den Heckrotor

private:
    int motorPin;           // Pin für den Heckmotor
    float scaleFactor;      // Skalierungsfaktor für den Heckmotor
    Servo motorServo;       // Servo für den Heckmotor
    PID pidYaw;             // PID für den Heckrotor

    unsigned long computeTailMotorPulse(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawCorrection);  // Berechnet den PWM-Wert
};

#endif // TAILROTOR_H
