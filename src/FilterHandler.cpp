#include "FilterHandler.h"

FilterHandler::FilterHandler(float lowPassAlpha, float highPassAlpha, int movingAvgWindowSize, float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate, PID& pid)
    : lowPassFilter(lowPassAlpha), highPassFilter(highPassAlpha),
      movingAvgFilter(movingAvgWindowSize), kalmanFilter(kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate),
      pid(pid) {}

float FilterHandler::apply(float value, bool useLowPass, bool useHighPass, bool useMovingAvg, bool useKalman) {
    float filteredValue = value;

    // Apply Low-pass filter if enabled
    if (useLowPass) {
        filteredValue = lowPassFilter.apply(filteredValue);
    }

    // Apply High-pass filter if enabled
    if (useHighPass) {
        filteredValue = highPassFilter.apply(filteredValue);
    }

    // Apply Moving average filter if enabled
    if (useMovingAvg) {
        filteredValue = movingAvgFilter.apply(filteredValue);
    }

    // Apply Kalman filter if enabled
    if (useKalman) {
        filteredValue = kalmanFilter.updateEstimate(filteredValue);
    }

    // Apply PID correction and return final result
    return pid.compute(0, filteredValue);
}
