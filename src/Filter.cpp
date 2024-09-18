#include "Filter.h"

LowPassFilter::LowPassFilter(float alpha)
    : alpha(alpha), previousValue(0.0) {}

float LowPassFilter::apply(float currentValue) {
    // Tiefpassfilter-Formel: y[i] = α * x[i] + (1 - α) * y[i-1]
    float filteredValue = alpha * currentValue + (1.0 - alpha) * previousValue;
    previousValue = filteredValue;  // Speichert den gefilterten Wert für den nächsten Zyklus
    return filteredValue;
}
