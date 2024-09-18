#include "TailRotor.h"

TailRotor::TailRotor(int motorPin, float scaleFactor)
    : motorPin(motorPin), scaleFactor(scaleFactor) {}

void TailRotor::setup() {
    motorServo.attach(motorPin);  // Servo an den Pin anhängen
}

void TailRotor::update(unsigned long channel8Pulse, unsigned long channel4Pulse) {
    // Berechnen des angepassten PWM-Werts für den Heckmotor
    unsigned long adjustedTailMotorPulse = channel8Pulse * scaleFactor;

    // Berechnen der Änderung für den Heckmotor basierend auf Channel 4
    int adjustment = map(channel4Pulse, 1000, 2000, -200, 200);  // Mappe den Bereich von 1000-2000 auf -200 bis +200

    // Hinzufügen der Anpassung zum skalierten PWM-Wert
    adjustedTailMotorPulse += adjustment;

    // Sicherstellen, dass der Wert innerhalb des gültigen PWM-Bereichs bleibt (1000 bis 2000 us)
    adjustedTailMotorPulse = constrain(adjustedTailMotorPulse, 1000, 2000);

    // Ausgabe des skalierten und angepassten PWM-Werts auf den Heckmotor
    motorServo.writeMicroseconds(adjustedTailMotorPulse);

}
