#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(float processNoise, float measurementNoise, float estimateError, float initialEstimate)
    : processNoise(processNoise), measurementNoise(measurementNoise), estimateError(estimateError), lastEstimate(initialEstimate) {}

float KalmanFilter::updateEstimate(float measurement) {
    // Predict the estimate error
    estimateError += processNoise;

    // Kalman gain
    float kalmanGain = estimateError / (estimateError + measurementNoise);

    // Update estimate with the Kalman gain
    lastEstimate += kalmanGain * (measurement - lastEstimate);

    // Update error in estimate
    estimateError *= (1 - kalmanGain);

    return lastEstimate;
}
