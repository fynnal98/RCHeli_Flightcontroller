#include "HighPassFilter.h"

// Constructor initializes alpha and sets initial previous and filtered values to 0
HighPassFilter::HighPassFilter(float alpha)
    : alpha(alpha), previousValue(0.0), filteredValue(0.0) {}

// Applies the high-pass filter formula
float HighPassFilter::apply(float currentValue) {
    // High-pass filter formula: y[i] = α * (y[i-1] + x[i] - x[i-1])
    filteredValue = alpha * (filteredValue + currentValue - previousValue);
    previousValue = currentValue;  // Store current value as previous for next iteration
    return filteredValue;
}
