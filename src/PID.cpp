#include "PID.h"

PID::PID(float kp, float ki, float kd, float integralLimit, float factor)
    : kp(kp), ki(ki), kd(kd), prevError(0), integral(0), integralLimit(integralLimit), factor(factor) {}

float PID::compute(float setpoint, float measured) {
    float error = setpoint - measured;

    // Berechnung des Integrals (mit Begrenzung)
    integral += error;
    if (integral > integralLimit) {
        integral = integralLimit;
    } else if (integral < -integralLimit) {
        integral = -integralLimit;
    }

    // Berechnung der Ableitung
    float derivative = error - prevError;
    prevError = error;

    // PID-Ausgabe und Faktor anwenden
    return (kp * error + ki * integral + kd * derivative) * factor;
}
