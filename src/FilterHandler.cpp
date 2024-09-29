#include "FilterHandler.h"

// Constructor initializes all filters and PID controller
FilterHandler::FilterHandler(float lowPassAlpha, float highPassAlpha, int movingAvgWindowSize, float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate, PID& pid)
    : lowPassFilter(lowPassAlpha),
      highPassFilter(highPassAlpha),
      movingAvgFilter(movingAvgWindowSize),
      kalmanFilter(kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate),
      pid(pid) 
{}

// Apply all filters and return the final PID output
float FilterHandler::apply(float value) {
    // Apply filters in sequence: Low-pass -> High-pass -> Moving average -> Kalman -> PID
    float lowPassed = lowPassFilter.apply(value);
    float movingAvgFiltered = movingAvgFilter.apply(lowPassed);
    float highPassed = highPassFilter.apply(movingAvgFiltered);
    float kalmanFiltered = kalmanFilter.updateEstimate(highPassed);

    // Apply PID correction and return final result
    return pid.compute(0, kalmanFiltered);
}
