#include "FBL.h"
#include "DataLogger.h"

DataLogger dataLogger;

// Constructor now includes moving average window size
FBL::FBL(int pin1, int pin2, int pin3, float lowPassAlpha, float highPassAlpha, int movingAvgWindowSize)
    : servo1Pin(pin1), servo2Pin(pin2), servo3Pin(pin3),
      rollLowPassFilter(lowPassAlpha), pitchLowPassFilter(lowPassAlpha),
      rollHighPassFilter(highPassAlpha), pitchHighPassFilter(highPassAlpha),
      rollMovingAvgFilter(movingAvgWindowSize), pitchMovingAvgFilter(movingAvgWindowSize)  // Initialize moving average filters
{}

void FBL::setup() {
    servo1.attach(servo1Pin); // Back
    servo2.attach(servo2Pin); // Left
    servo3.attach(servo3Pin); // Right
}

void FBL::update(MPU6050& mpu, PID& pidRoll, PID& pidPitch, unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse) {
    // Fetch sensor data
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Corrected accelerations now handled inside MPU6050
    float ax_corrected, ay_corrected;
    mpu.calculateCorrectedAccelerations(&a, &g, ax_corrected, ay_corrected);

    // Apply low-pass filter
    float rollLowPassed = rollLowPassFilter.apply(ax_corrected);
    float pitchLowPassed = pitchLowPassFilter.apply(ay_corrected);

    // Apply high-pass filter after low-pass filter
    float rollHighPassed = rollHighPassFilter.apply(rollLowPassed);
    float pitchHighPassed = pitchHighPassFilter.apply(pitchLowPassed);

    // Apply moving average filter after high-pass filter
    float rollFiltered = rollMovingAvgFilter.apply(rollHighPassed);
    float pitchFiltered = pitchMovingAvgFilter.apply(pitchHighPassed);

    // Compute PID corrections based on filtered values
    float rollCorrection = pidRoll.compute(0, rollFiltered);
    float pitchCorrection = pidPitch.compute(0, pitchFiltered);

    // Add corrections to the channel data
    unsigned long servo1Pulse = channel2Pulse + pitchCorrection; // Back
    unsigned long servo2Pulse = channel6Pulse + (-0.5 * pitchCorrection + 0.866 * rollCorrection); // Left
    unsigned long servo3Pulse = channel1Pulse - (-0.5 * pitchCorrection - 0.866 * rollCorrection); // Right

    // Set the pulse lengths for the servos
    servo1.writeMicroseconds(servo1Pulse); // Back
    servo2.writeMicroseconds(servo2Pulse); // Left
    servo3.writeMicroseconds(servo3Pulse); // Right
}
