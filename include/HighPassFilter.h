#ifndef HIGHPASSFILTER_H
#define HIGHPASSFILTER_H

class HighPassFilter {
public:
    HighPassFilter(float alpha);  // Constructor that takes the filter coefficient (alpha)
    float apply(float currentValue);  // Applies the filter to the current value and returns the filtered value

private:
    float alpha;  // Filter coefficient
    float previousValue;  // Stores the previous input value
    float filteredValue;  // Stores the last filtered value
};

#endif // HIGHPASSFILTER_H
