#include "PID.h"

PID::PID(float kp, float ki, float kd)
    : kp(kp), ki(ki), kd(kd), prevError(0), integral(0) {}

float PID::compute(float setpoint, float measured) {
    float error = setpoint - measured;
    integral += error;
    float derivative = error - prevError;
    prevError = error;
    return kp * error + ki * integral + kd * derivative;
}
