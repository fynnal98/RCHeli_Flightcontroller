#ifndef KALMANFILTER_H
#define KALMANFILTER_H

class KalmanFilter {
public:
    KalmanFilter(float processNoise, float measurementNoise, float estimateError, float initialEstimate);
    float updateEstimate(float measurement);

private:
    float processNoise;      // Process noise covariance (Q)
    float measurementNoise;  // Measurement noise covariance (R)
    float estimateError;     // Error in the estimate (P)
    float lastEstimate;      // Previous estimate
};

#endif // KALMANFILTER_H
