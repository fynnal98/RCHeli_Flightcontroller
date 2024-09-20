#include "NotchFilter.h"
#include <cmath> // Für cos() und M_PI

NotchFilter::NotchFilter(float frequency, float bandwidth, float sampleRate) {
    float r = 1 - 3 * bandwidth;
    float k = (1 - 2 * r * cos(2 * M_PI * frequency / sampleRate) + r * r) / (2 - 2 * cos(2 * M_PI * frequency / sampleRate));
    a0 = 1 - k;
    a1 = 2 * (k - r) * cos(2 * M_PI * frequency / sampleRate);
    a2 = r * r - k;
    b1 = 2 * r * cos(2 * M_PI * frequency / sampleRate);
    b2 = -r * r;
    x1 = x2 = y1 = y2 = 0;
}

float NotchFilter::apply(float input) {
    float output = a0 * input + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;
    x2 = x1;
    x1 = input;
    y2 = y1;
    y1 = output;
    return output;
}
