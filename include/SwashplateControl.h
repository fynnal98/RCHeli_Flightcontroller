#ifndef SWASHPLATECONTROL_H
#define SWASHPLATECONTROL_H

#include <ESP32Servo.h>

class SwashplateControl {
public:
    SwashplateControl(int servo1Pin, int servo2Pin, int servo3Pin);  // Konstruktor

    void setup();  // Initialisierung der Servos
    void combineServos(unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse);  // Kombiniere die Servo-Bewegungen basierend auf den Eingabewerten
    void applyServoOutputs();  // Sendet die kombinierten Servo-Werte an die Servos

    // Zugriff auf die kombinierten Pulse (nach Roll und Pitch)
    unsigned long getServo1Pulse();  
    unsigned long getServo2Pulse();
    unsigned long getServo3Pulse();

private:
    Servo servo1, servo2, servo3;  // Servos für die Taumelscheibe (Aft, Left, Right)
    int servo1Pin, servo2Pin, servo3Pin;  // Pins der Servos

    unsigned long servo1Pulse, servo2Pulse, servo3Pulse;  // Kombinierte Servopulsweiten für Roll und Pitch
};

#endif // SWASHPLATECONTROL_H
