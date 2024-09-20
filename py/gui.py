import sys
import serial
import numpy as np
import matplotlib.pyplot as plt
from PyQt5.QtWidgets import (QApplication, QMainWindow, QVBoxLayout, QWidget, QTabWidget, QScrollArea)
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from PyQt5.QtCore import QTimer
import qdarkstyle  # Für ein modernes dunkles Thema
from matplotlib.animation import FuncAnimation

# Seriellen Port für ESP32 konfigurieren (COM-Port anpassen)
try:
    ser = serial.Serial('COM5', 115200, timeout=1)  # COM5 anpassen
except serial.SerialException as e:
    print(f"Fehler beim Öffnen des seriellen Ports: {e}")
    ser = None  # Falls der Port nicht geöffnet werden kann

class MplCanvas(FigureCanvas):
    def __init__(self, parent=None, width=5, height=4, dpi=100, subplots=2):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes1 = fig.add_subplot(211)  # Oberer Plot für Amplitudengang
        self.axes2 = fig.add_subplot(212)  # Unterer Plot für Phasengang
        super(MplCanvas, self).__init__(fig)

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        # Setze Titel und Größe des Fensters
        self.setWindowTitle("Flight Controller GUI")
        self.resize(1200, 800)

        # Haupt-Tabs: "Data"
        self.tabs = QTabWidget()
        self.setCentralWidget(self.tabs)

        # Tab für "Data"
        self.tab_data = QTabWidget()  # Untergeordnete Tabs für Beschleunigung und Gyroskop
        self.tabs.addTab(self.tab_data, "Data")
        self.init_data_tab()

        # Datenpuffer für Echtzeitvisualisierung
        self.acc_buffer_x = np.zeros(200)  # Puffergröße auf 200 erhöhen (1 Sekunde Daten)
        self.gyro_buffer_x = np.zeros(200)
        self.acc_buffer_y = np.zeros(200)
        self.gyro_buffer_y = np.zeros(200)

        # Setze die Animation für Echtzeit-Updates
        self.anim_bode_acc_x = FuncAnimation(self.canvas_bode_acc_x.figure, self.update_bode_plot_acc_x, interval=50, blit=True)
        self.anim_bode_gyro_x = FuncAnimation(self.canvas_bode_gyro_x.figure, self.update_bode_plot_gyro_x, interval=50, blit=True)
        self.anim_bode_acc_y = FuncAnimation(self.canvas_bode_acc_y.figure, self.update_bode_plot_acc_y, interval=50, blit=True)
        self.anim_bode_gyro_y = FuncAnimation(self.canvas_bode_gyro_y.figure, self.update_bode_plot_gyro_y, interval=50, blit=True)

        # Timer für serielle Daten
        self.timer = QTimer()
        self.timer.setInterval(1)  # Timer auf 5 ms einstellen, um 200 Hz Daten zu verarbeiten
        self.timer.timeout.connect(self.read_serial_data)
        self.timer.start()

    def init_data_tab(self):
        # Tabs für x_acc_Bode und x_gyro_Bode
        self.tab_acc_x = QWidget()
        self.tab_data.addTab(self.tab_acc_x, "x_acc_Bode")
        layout_acc_x = QVBoxLayout()
        self.canvas_bode_acc_x = MplCanvas(self, width=5, height=4, dpi=100, subplots=2)
        layout_acc_x.addWidget(self.canvas_bode_acc_x)
        self.tab_acc_x.setLayout(layout_acc_x)

        self.tab_gyro_x = QWidget()
        self.tab_data.addTab(self.tab_gyro_x, "x_gyro_Bode")
        layout_gyro_x = QVBoxLayout()
        self.canvas_bode_gyro_x = MplCanvas(self, width=5, height=4, dpi=100, subplots=2)
        layout_gyro_x.addWidget(self.canvas_bode_gyro_x)
        self.tab_gyro_x.setLayout(layout_gyro_x)

        # Tabs für y_acc_Bode und y_gyro_Bode
        self.tab_acc_y = QWidget()
        self.tab_data.addTab(self.tab_acc_y, "y_acc_Bode")
        layout_acc_y = QVBoxLayout()
        self.canvas_bode_acc_y = MplCanvas(self, width=5, height=4, dpi=100, subplots=2)
        layout_acc_y.addWidget(self.canvas_bode_acc_y)
        self.tab_acc_y.setLayout(layout_acc_y)

        self.tab_gyro_y = QWidget()
        self.tab_data.addTab(self.tab_gyro_y, "y_gyro_Bode")
        layout_gyro_y = QVBoxLayout()
        self.canvas_bode_gyro_y = MplCanvas(self, width=5, height=4, dpi=100, subplots=2)
        layout_gyro_y.addWidget(self.canvas_bode_gyro_y)
        self.tab_gyro_y.setLayout(layout_gyro_y)

    def read_serial_data(self):
        if ser is None:
            return  # Serielle Verbindung nicht verfügbar
        try:
            if ser.in_waiting > 0:  # Wenn Daten vorhanden sind
                line = ser.readline().decode('utf-8').strip()
                print(f"Empfangene Daten: {line}")
                values = list(map(float, line.split(',')))

                if len(values) == 6:
                    self.acc_buffer_x = np.roll(self.acc_buffer_x, -1)
                    self.acc_buffer_x[-1] = values[0]  # ax
                    self.gyro_buffer_x = np.roll(self.gyro_buffer_x, -1)
                    self.gyro_buffer_x[-1] = values[3]  # gx

                    self.acc_buffer_y = np.roll(self.acc_buffer_y, -1)
                    self.acc_buffer_y[-1] = values[1]  # ay
                    self.gyro_buffer_y = np.roll(self.gyro_buffer_y, -1)
                    self.gyro_buffer_y[-1] = values[4]  # gy
        except Exception as e:
            print(f"Fehler beim Lesen der Daten: {e}")

    def update_bode_plot_acc_x(self, frame):
        return self.update_bode_plot(self.canvas_bode_acc_x, self.acc_buffer_x, "Bode-Diagramm Beschleunigung X")

    def update_bode_plot_gyro_x(self, frame):
        return self.update_bode_plot(self.canvas_bode_gyro_x, self.gyro_buffer_x, "Bode-Diagramm Gyroskop X")

    def update_bode_plot_acc_y(self, frame):
        return self.update_bode_plot(self.canvas_bode_acc_y, self.acc_buffer_y, "Bode-Diagramm Beschleunigung Y")

    def update_bode_plot_gyro_y(self, frame):
        return self.update_bode_plot(self.canvas_bode_gyro_y, self.gyro_buffer_y, "Bode-Diagramm Gyroskop Y")

    def update_bode_plot(self, canvas, data, label):
        fft_data = np.fft.fft(data)
        freqs = np.fft.fftfreq(len(data), d=0.001)  # 200 Hz Abtastrate
        positive_freqs = freqs[:len(freqs) // 2]
        positive_fft = fft_data[:len(fft_data) // 2]

        amplitude = 20 * np.log10(np.abs(positive_fft) + 1e-10)
        phase = np.angle(positive_fft)

        # Abstand zwischen den Subplots vergrößern
        canvas.figure.subplots_adjust(hspace=0.4)  # Vergrößere den Abstand zwischen den Diagrammen

        canvas.axes1.clear()
        canvas.axes1.plot(positive_freqs, amplitude)
        canvas.axes1.set_title(f"{label}: Amplitudengang")
        canvas.axes1.set_ylabel("Amplitude (dB)")
        canvas.axes1.set_xscale('linear')
        canvas.axes1.set_xlabel("Frequenz (Hz)")
        canvas.axes1.set_xlim([1, 100])
        canvas.axes1.set_xticks([10, 20, 30, 40, 50, 60, 70, 80, 90, 100])
        canvas.axes1.grid(True)

        canvas.axes2.clear()
        canvas.axes2.plot(positive_freqs, phase)
        canvas.axes2.set_title(f"{label}: Phasengang")
        canvas.axes2.set_ylabel("Phase (Rad)")
        canvas.axes2.set_xscale('linear')
        canvas.axes2.set_xlabel("Frequenz (Hz)")
        canvas.axes2.set_xlim([1, 100])
        canvas.axes2.set_xticks([10, 20, 30, 40, 50, 60, 70, 80, 90, 100])
        canvas.axes2.grid(True)

        return canvas.axes1.lines + canvas.axes2.lines


app = QApplication(sys.argv)
app.setStyleSheet(qdarkstyle.load_stylesheet_pyqt5())  # Lade dunkles Thema
window = MainWindow()
window.show()
app.exec_()
