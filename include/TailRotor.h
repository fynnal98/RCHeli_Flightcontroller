#ifndef TAILROTOR_H
#define TAILROTOR_H

#include <ESP32Servo.h>

class TailRotor {
public:
    TailRotor(int motorPin, float scaleFactor);  // Konstruktor
    void setup();
    void update(unsigned long channel8Pulse, unsigned long channel4Pulse);

private:
    int motorPin;  // Pin für den Heckmotor
    float scaleFactor;  // Skalierungsfaktor für den Heckmotor
    Servo motorServo;  // Servo für den Heckmotor
};

#endif // TAILROTOR_H
