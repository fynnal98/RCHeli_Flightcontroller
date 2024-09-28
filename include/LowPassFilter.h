#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

class LowPassFilter {
public:
    // Constructor that takes the alpha coefficient as a parameter
    LowPassFilter(float alpha);  
    
    // Function to apply the filter to the current value
    float apply(float currentValue);

private:
    float alpha;  // Filter coefficient
    float previousValue;  // Stores the last filtered value
};

#endif // LOWPASSFILTER_H
