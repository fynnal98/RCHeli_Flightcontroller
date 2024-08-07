#ifndef PID_H
#define PID_H

class PID {
public:
    PID(float kp, float ki, float kd);
    float compute(float setpoint, float measured);

private:
    float kp, ki, kd;
    float prevError, integral;
};

#endif // PID_H
