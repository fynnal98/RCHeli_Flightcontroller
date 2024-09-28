#include "TailRotor.h"

// Konstruktor: Übergibt Pin und Skalierungsfaktor, initialisiert den PID-Regler für Yaw
TailRotor::TailRotor(int motorPin, float scaleFactor)
    : motorPin(motorPin), scaleFactor(scaleFactor), pidYaw(90.0, 0.1, 10) {}  // Initialisiere den PID-Regler mit Standardwerten

// Setup: Initialisiert den Servo und verbindet ihn mit dem entsprechenden Pin
void TailRotor::setup() {
    motorServo.attach(motorPin);  // Servo an den angegebenen Pin anhängen
}

// Update: Berechnet den PWM-Wert basierend auf den Kanalwerten und der Yaw-Rate
void TailRotor::update(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawRate) {
    // Berechnung der Yaw-Korrektur basierend auf Gyroskop-Daten
    float yawCorrection = pidYaw.compute(0, yawRate);  // Verwende 0 als Sollwert (kein Yaw), yawRate als Istwert

    // Berechnung des PWM-Werts für den Heckrotor basierend auf Kanalwerten und Yaw-Korrektur
    unsigned long adjustedTailMotorPulse = computeTailMotorPulse(channel8Pulse, channel4Pulse, yawCorrection);

    // Ausgabe des skalierten und angepassten PWM-Werts auf den Heckmotor
    motorServo.writeMicroseconds(adjustedTailMotorPulse);
}

// Hilfsfunktion: Berechnet den PWM-Wert für den Heckrotor
unsigned long TailRotor::computeTailMotorPulse(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawCorrection) {
    // Berechne den PWM-Wert des Heckmotors basierend auf dem Skalierungsfaktor und der Yaw-Korrektur
    unsigned long adjustedTailMotorPulse = channel8Pulse * scaleFactor + yawCorrection;

    // Berechne die Anpassung für den Heckrotor basierend auf Channel 4 (manuelle Steuerung)
    int adjustment = map(channel4Pulse, 1000, 2000, -200, 200);  // Mappe den Bereich von 1000-2000 auf -200 bis +200

    // Hinzufügen der Anpassung zum PWM-Wert
    adjustedTailMotorPulse += adjustment;

    // Sicherstellen, dass der Wert im gültigen PWM-Bereich bleibt (1000 bis 2000 us)
    return constrain(adjustedTailMotorPulse, 1000, 2000);
}
