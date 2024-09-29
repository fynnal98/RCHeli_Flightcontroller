#ifndef FILTERHANDLER_H
#define FILTERHANDLER_H

#include "LowPassFilter.h"
#include "HighPassFilter.h"
#include "MovingAverageFilter.h"
#include "KalmanFilter.h"
#include "PID.h"

class FilterHandler {
public:
    FilterHandler(float lowPassAlpha, float highPassAlpha, int movingAvgWindowSize, float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate, PID& pid);

    // Apply all filters based on flags (true/false for each filter)
    float apply(float value, bool useLowPass, bool useHighPass, bool useMovingAvg, bool useKalman);

private:
    LowPassFilter lowPassFilter;
    HighPassFilter highPassFilter;
    MovingAverageFilter movingAvgFilter;
    KalmanFilter kalmanFilter;
    PID& pid;
};

#endif // FILTERHANDLER_H
