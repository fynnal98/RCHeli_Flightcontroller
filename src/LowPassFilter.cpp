#include "LowPassFilter.h"

LowPassFilter::LowPassFilter(float alpha)
    : alpha(alpha), previousValue(0.0) {}

float LowPassFilter::apply(float currentValue) {
    // Low-pass filter formula: y[i] = α * x[i] + (1 - α) * y[i-1]
    float filteredValue = alpha * currentValue + (1.0 - alpha) * previousValue;
    previousValue = filteredValue;  // Save the filtered value for the next cycle
    return filteredValue;
}
