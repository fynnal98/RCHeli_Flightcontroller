import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from PyQt5.QtWidgets import QWidget, QVBoxLayout, QPushButton, QFileDialog, QLabel

class BodePlotCanvas(FigureCanvas):
    def __init__(self, parent=None, width=5, height=4, dpi=100):
        fig, (self.ax1, self.ax2) = plt.subplots(2, 1, figsize=(width, height), dpi=dpi)
        super(BodePlotCanvas, self).__init__(fig)
        self.setParent(parent)

    def plot_bode(self, freq, magnitude_x, magnitude_y, phase_x, phase_y, title):
        self.ax1.clear()
        self.ax2.clear()

        self.ax1.set_title(f'{title} - Magnitude')
        self.ax1.plot(freq, 20 * np.log10(magnitude_x), label='X-axis')
        self.ax1.plot(freq, 20 * np.log10(magnitude_y), label='Y-axis')
        self.ax1.set_ylabel('Magnitude (dB)')
        self.ax1.set_xscale('log')
        self.ax1.grid(True)
        self.ax1.legend()

        self.ax2.set_title(f'{title} - Phase')
        self.ax2.plot(freq, np.degrees(phase_x), label='X-axis')
        self.ax2.plot(freq, np.degrees(phase_y), label='Y-axis')
        self.ax2.set_ylabel('Phase (degrees)')
        self.ax2.set_xlabel('Frequency (Hz)')
        self.ax2.set_xscale('log')
        self.ax2.grid(True)
        self.ax2.legend()

        self.draw()

class BodePlotTab(QWidget):
    def __init__(self):
        super().__init__()

        self.raw_x = None
        self.raw_y = None
        self.filtered_x = None
        self.filtered_y = None

        # UI Elements
        self.label = QLabel("Load a CSV file to generate Bode plots", self)
        self.load_button = QPushButton("Load CSV", self)
        self.load_button.clicked.connect(self.load_csv)

        # Create canvases for raw and filtered data
        self.raw_canvas = BodePlotCanvas(self, width=5, height=4)
        self.filtered_canvas = BodePlotCanvas(self, width=5, height=4)

        # Layout setup
        layout = QVBoxLayout()
        layout.addWidget(self.label)
        layout.addWidget(self.load_button)

        plot_layout = QVBoxLayout()
        plot_layout.addWidget(self.raw_canvas)
        plot_layout.addWidget(self.filtered_canvas)

        layout.addLayout(plot_layout)
        self.setLayout(layout)

    def load_csv(self):
        options = QFileDialog.Options()
        file_name, _ = QFileDialog.getOpenFileName(self, "Open CSV File", "", "CSV Files (*.csv)", options=options)
        if file_name:
            self.label.setText(f"Loaded: {file_name}")
            self.process_data(file_name)

    def process_data(self, file_name):
        df = pd.read_csv(file_name)
        self.raw_x = df['ax'].values
        self.raw_y = df['ay'].values
        self.filtered_x = df['rollFiltered'].values
        self.filtered_y = df['pitchFiltered'].values

        freq = np.linspace(0.1, 10, len(self.raw_x))

        magnitude_x_raw = np.abs(np.fft.fft(self.raw_x))
        phase_x_raw = np.angle(np.fft.fft(self.raw_x))

        magnitude_y_raw = np.abs(np.fft.fft(self.raw_y))
        phase_y_raw = np.angle(np.fft.fft(self.raw_y))

        magnitude_x_filtered = np.abs(np.fft.fft(self.filtered_x))
        phase_x_filtered = np.angle(np.fft.fft(self.filtered_x))

        magnitude_y_filtered = np.abs(np.fft.fft(self.filtered_y))
        phase_y_filtered = np.angle(np.fft.fft(self.filtered_y))

        self.raw_canvas.plot_bode(freq, magnitude_x_raw, magnitude_y_raw, phase_x_raw, phase_y_raw, "Raw Data (X, Y)")
        self.filtered_canvas.plot_bode(freq, magnitude_x_filtered, magnitude_y_filtered, phase_x_filtered, phase_y_filtered, "Filtered Data (X, Y)")
