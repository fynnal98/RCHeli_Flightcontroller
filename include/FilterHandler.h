#ifndef FILTERHANDLER_H
#define FILTERHANDLER_H

#include "LowPassFilter.h"
#include "HighPassFilter.h"
#include "MovingAverageFilter.h"
#include "KalmanFilter.h"
#include "PID.h"

class FilterHandler {
public:
    // Constructor for initializing the filters and PID controller
    FilterHandler(float lowPassAlpha, float highPassAlpha, int movingAvgWindowSize, float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate, PID& pid);

    // Method to apply all filters and return the final PID output
    float apply(float value);

private:
    // Filters
    LowPassFilter lowPassFilter;
    HighPassFilter highPassFilter;
    MovingAverageFilter movingAvgFilter;
    KalmanFilter kalmanFilter;

    // PID controller
    PID& pid;
};

#endif // FILTERHANDLER_H
