#ifndef NOTCHFILTER_H
#define NOTCHFILTER_H

class NotchFilter {
public:
    NotchFilter(float frequency, float bandwidth, float sampleRate);
    float apply(float input);

private:
    float a0, a1, a2, b1, b2;
    float x1, x2, y1, y2;
};

#endif // NOTCHFILTER_H
