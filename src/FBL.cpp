#include "FBL.h"
#include "DataLogger.h"

DataLogger dataLogger;

// Constructor initializes the servos and filter handlers
FBL::FBL(int pin1, int pin2, int pin3, FilterHandler& rollFilterHandler, FilterHandler& pitchFilterHandler)
    : servo1Pin(pin1), servo2Pin(pin2), servo3Pin(pin3),
      rollFilterHandler(rollFilterHandler), pitchFilterHandler(pitchFilterHandler)
{}

// Set up servos
void FBL::setup() {
    servo1.attach(servo1Pin); // Back
    servo2.attach(servo2Pin); // Left
    servo3.attach(servo3Pin); // Right
}

// Update method fetches sensor data and applies filters
void FBL::update(MPU6050& mpu, unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse) {
    // Fetch sensor data
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Corrected accelerations are handled inside MPU6050
    float ax_corrected, ay_corrected;
    mpu.calculateCorrectedAccelerations(&a, &g, ax_corrected, ay_corrected);

    // Apply the filter handlers
    float rollCorrection = rollFilterHandler.apply(ax_corrected);
    float pitchCorrection = pitchFilterHandler.apply(ay_corrected);

    // Add corrections to the channel data
    unsigned long servo1Pulse = channel2Pulse + pitchCorrection; // Back
    unsigned long servo2Pulse = channel6Pulse + (-0.5 * pitchCorrection + 0.866 * rollCorrection); // Left
    unsigned long servo3Pulse = channel1Pulse - (-0.5 * pitchCorrection - 0.866 * rollCorrection); // Right

    // Set the pulse lengths for the servos
    servo1.writeMicroseconds(servo1Pulse); // Back
    servo2.writeMicroseconds(servo2Pulse); // Left
    servo3.writeMicroseconds(servo3Pulse); // Right
}
