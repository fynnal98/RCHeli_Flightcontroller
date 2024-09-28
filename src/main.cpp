#include <Arduino.h>
#include <ESP32Servo.h>
#include "SBUSReceiver.h"
#include "MPU6050.h"
#include "PID.h"
#include "FBL.h"
#include "WDT.h"
#include "MainMotor.h"
#include "Util.h"
#include "TailRotor.h"

const int sbusPin = 16;
SBUSReceiver sbusReceiver(Serial2);

MPU6050 mpu;
PID pidRoll(90.0, 0.1, 10);
PID pidPitch(90.0, 0.1, 10);
PID pidYaw(0.0, 0.0, 0);  

// Filter parameters
float lowPassAlpha = 0.5;  // Low-pass filter alpha
float highPassAlpha = 1;  // High-pass filter alpha
int movingAvgWindowSize = 5;  // Moving average filter window size (N)

int pinServo1 = 13;
int pinServo2 = 14;
int pinServo3 = 15;

float offsetGyroX = 0.0;
float offsetGyroY = -0.09;
float offsetGyroZ = 0.0;

// Flight controller setup
FBL fbl(pinServo1, pinServo2, pinServo3, offsetGyroX, offsetGyroY, offsetGyroZ, lowPassAlpha, highPassAlpha, movingAvgWindowSize);  // Includes moving average filter size

const int mainMotorPin = 5;   // Pin for the ESC of the main motor
const int tailMotorPin = 17;  // Pin for the ESC of the tail motor

MainMotor mainMotorServo(mainMotorPin);  // Main motor servo object
float scalingFactorTailRotor = 1;

TailRotor tailRotor(tailMotorPin, scalingFactorTailRotor);

void setup() {
    Serial.begin(115200);  // Start serial communication

    initWatchdog(2);  // Initialize the watchdog timer

    sbusReceiver.begin();
    Wire.begin(21, 22);
    mpu.begin();
    mpu.setup();
    fbl.setup();
    tailRotor.setup();  // Setup for the tail rotor

    // Attach the main motor to its pin
    mainMotorServo.setup();

    Serial.println("Setup complete");
}

void loop() {
    unsigned int channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel10Pulse;

    resetWatchdog();  // Reset the watchdog timer

    // Read channel values for channels 1, 2, 4, 6, and 8
    if (sbusReceiver.readChannels(channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel10Pulse)) {
        
        // Fetch the current sensor data (including yaw rate)
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        float yawRate = g.gyro.z;  // Yaw rate from the gyroscope

        if (Util::correctionEnabled(channel10Pulse)) {
            // Perform the FBL update using filtered values (with low-pass, high-pass, and moving average filters)
            fbl.update(mpu, pidRoll, pidPitch, channel1Pulse, channel2Pulse, channel6Pulse);

            // Update the tail rotor with yaw correction
            tailRotor.update(channel8Pulse, channel4Pulse, yawRate);  
        } else {
            // FBL and filters are disabled – send raw servo values
            fbl.servo1.writeMicroseconds(channel2Pulse);  // Back
            fbl.servo2.writeMicroseconds(channel6Pulse);  // Left
            fbl.servo3.writeMicroseconds(channel1Pulse);  // Right

            // Update the tail rotor directly using channel values (without gyro influence)
            tailRotor.update(channel8Pulse, channel4Pulse, 0);  
        }

        // Control the main motor
        mainMotorServo.setPulse(channel8Pulse);
    } else {
        Serial.println("Error reading channels.");
    }

    delay(10);
}
