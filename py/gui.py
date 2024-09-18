import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft, fftfreq
import serial

# Seriellen Port für die Verbindung mit dem ESP32 konfigurieren (COM5)
ser = serial.Serial('COM5', 115200, timeout=1)  # Timeout auf 1 Sekunde gesetzt

sampling_rate = 1000  # Abtastrate in Hz
duration = 5  # Aufnahmedauer in Sekunden
time = np.linspace(0, duration, sampling_rate * duration, endpoint=False)
accel_data = []

print("Erfasse Sensordaten...")

while len(accel_data) < len(time):
    bytes_available = ser.in_waiting
    if bytes_available > 0:
        line = ser.readline().decode('latin-1').strip()
        print(f"Empfangene Zeile: {line}")  # Debug: Zeige die empfangenen Daten an
        try:
            # Prüfen, ob die Zeile numerische Daten enthält (kommagetrennte Werte)
            if ',' in line and all([char.isdigit() or char == '.' or char == ',' or char == '-' for char in line]):
                ax, ay, az, gx, gy, gz = map(float, line.split(','))
                accel_data.append(ax)  # Beispiel für die X-Achse der Beschleunigung
            else:
                print(f"Ignoriere Zeile: {line}")  # Debug-Ausgabe für nicht passende Zeilen
        except ValueError as e:
            print(f"Fehler beim Parsen der Zeile: {e}")
            continue
    else:
        print("Keine Daten verfügbar, warte...")

print("Sensordaten erfolgreich empfangen.")
