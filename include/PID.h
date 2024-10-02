#ifndef PID_H
#define PID_H

class PID {
public:
    PID(float kp, float ki, float kd, float integralLimit, float factor);
    float compute(float setpoint, float measured);

private:
    float kp, ki, kd;
    float prevError;
    float integral;
    float integralLimit;
    float factor;  // Der neue Faktor
};

#endif // PID_H
