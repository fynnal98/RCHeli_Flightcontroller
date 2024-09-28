#include "TailRotor.h"

// Konstruktor: Übergibt Pin, Skalierungsfaktor und den bereits erstellten PID-Regler
TailRotor::TailRotor(int motorPin, float scaleFactor, PID& pidYaw)
    : motorPin(motorPin), scaleFactor(scaleFactor), pidYaw(pidYaw) {}  // Verwendet den PID-Regler aus der Main

void TailRotor::setup() {
    motorServo.attach(motorPin);  // Servo an den angegebenen Pin anhängen
}

void TailRotor::update(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawRate) {
    // Berechnung der Yaw-Korrektur basierend auf Gyroskop-Daten
    float yawCorrection = pidYaw.compute(0, yawRate);  // Verwende 0 als Sollwert (kein Yaw), yawRate als Istwert

    // Berechnung des PWM-Werts für den Heckrotor basierend auf Kanalwerten und Yaw-Korrektur
    unsigned long adjustedTailMotorPulse = computeTailMotorPulse(channel8Pulse, channel4Pulse, yawCorrection);

    // Ausgabe des skalierten und angepassten PWM-Werts auf den Heckmotor
    motorServo.writeMicroseconds(adjustedTailMotorPulse);
}

unsigned long TailRotor::computeTailMotorPulse(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawCorrection) {
    unsigned long adjustedTailMotorPulse = channel8Pulse * scaleFactor + yawCorrection;

    // Berechne die Anpassung für den Heckrotor basierend auf Channel 4 (manuelle Steuerung)
    int adjustment = map(channel4Pulse, 1000, 2000, -200, 200);  // Mappe den Bereich von 1000-2000 auf -200 bis +200

    // Hinzufügen der Anpassung zum PWM-Wert
    adjustedTailMotorPulse += adjustment;

    // Sicherstellen, dass der Wert im gültigen PWM-Bereich bleibt (1000 bis 2000 us)
    return constrain(adjustedTailMotorPulse, 1000, 2000);
}
