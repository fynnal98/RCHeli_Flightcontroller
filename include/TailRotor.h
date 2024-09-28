#ifndef TAILROTOR_H
#define TAILROTOR_H

#include <ESP32Servo.h>
#include "PID.h"

class TailRotor {
public:
    TailRotor(int motorPin, float scaleFactor, PID& pidYaw);  // Übergibt den PID-Controller als Referenz
    void setup();
    void update(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawRate);

private:
    int motorPin;
    float scaleFactor;
    Servo motorServo;
    PID& pidYaw;  // Speichere den PID-Regler als Referenz
    unsigned long computeTailMotorPulse(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawCorrection);
};

#endif  // TAILROTOR_H
