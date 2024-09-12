#include <Arduino.h>
#include <ESP32Servo.h>  // Verwenden Sie die ESP32Servo-Bibliothek
#include "SBUSReceiver.h"
#include "MPU6050.h"
#include "PID.h"
#include "FBL.h"
#include "WDT.h"

const int sbusPin = 16;
SBUSReceiver sbusReceiver(Serial2);

MPU6050 mpu;
PID pidRoll(3.0, 0.0, 0.2);
PID pidPitch(3.0, 0.0, 0.2);
FBL fbl(13, 14, 15, 30.0);

// Konfiguration für den Haupt- und Heckmotor
const int mainMotorPin = 5;   // Pin für den ESC des Hauptmotors
const int tailMotorPin = 17;  // Pin für den ESC des Heckmotors

Servo mainMotorServo;  // Servo-Objekt für den Hauptmotor
Servo tailMotorServo;  // Servo-Objekt für den Heckmotor

float tailMotorScaleFactor = 1; // Skalierungsfaktor für den Heckmotor (z.B. 0.5 für 50% der Hauptmotor-Drehzahl)

void setup() {
    Serial.begin(115200);  // Startet die serielle Kommunikation

    initWatchdog(2);  // Initialisiert den Watchdog-Timer

    sbusReceiver.begin();
    Wire.begin(21, 22);
    mpu.begin();
    mpu.setup();
    fbl.setup();

    // Attache den Servo (Motor) an den Pin
    mainMotorServo.attach(mainMotorPin);
    tailMotorServo.attach(tailMotorPin);

    Serial.println("Setup abgeschlossen");
}

void loop() {
    unsigned long channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse;

    resetWatchdog();  // Setzt den Watchdog-Timer zurück

    // Liest die Werte der Kanäle 1, 2, 4, 6 und 8
    if (sbusReceiver.readChannels(channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse)) {
        Serial.println("Kanäle 1, 2, 4, 6 und 8 erfolgreich gelesen.");

        // Gibt die Werte von Channel 4 in der seriellen Konsole aus
        Serial.print("Channel 4 Pulse Width: ");
        Serial.println(channel4Pulse);

        // Gibt den Wert von Channel 8 in der seriellen Konsole aus
        Serial.print("Channel 8 Pulse Width: ");
        Serial.println(channel8Pulse);

        // Direkte Ausgabe des PWM-Wertes auf Pin 5 für den Hauptmotor
        mainMotorServo.writeMicroseconds(channel8Pulse); // Verwenden Sie den ausgelesenen Kanalwert direkt für den Hauptmotor

        // Berechnen Sie den angepassten PWM-Wert für den Heckmotor
        unsigned long adjustedTailMotorPulse = channel8Pulse * tailMotorScaleFactor;

        // Berechnen Sie die Änderung für den Heckmotor basierend auf Channel 4
        int adjustment = map(channel4Pulse, 1000, 2000, -200, 200); // Mappe den Bereich von 1000-2000 auf -200 bis +200

        // Addieren Sie die Anpassung zum skalierten PWM-Wert
        adjustedTailMotorPulse += adjustment;

        // Stellen Sie sicher, dass der Wert innerhalb des gültigen PWM-Bereichs bleibt (1000 bis 2000 us)
        adjustedTailMotorPulse = constrain(adjustedTailMotorPulse, 1000, 2000);
        
        // Direkte Ausgabe des skalierten und angepassten PWM-Wertes auf Pin 17 für den Heckmotor
        tailMotorServo.writeMicroseconds(adjustedTailMotorPulse); 

        Serial.print("Adjusted Tail Motor Pulse Width: ");
        Serial.println(adjustedTailMotorPulse);

        // Aktualisiert die Flugsteuerung mit den restlichen Kanälen
        fbl.update(mpu, pidRoll, pidPitch, channel1Pulse, channel2Pulse, channel6Pulse);
    } else {
        Serial.println("Fehler beim Lesen der Kanäle.");
    }

    delay(20);  // Verzögert die Ausgabe
}
