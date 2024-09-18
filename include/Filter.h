#ifndef FILTER_H
#define FILTER_H

class LowPassFilter {
public:
    LowPassFilter(float alpha);  // Konstruktor, nimmt den Filterkoeffizienten (alpha) als Parameter
    float apply(float currentValue);  // Wendet den Filter auf den aktuellen Wert an und gibt den gefilterten Wert zurück

private:
    float alpha;  // Filterkoeffizient
    float previousValue;  // Speichert den letzten gefilterten Wert
};

#endif // FILTER_H
