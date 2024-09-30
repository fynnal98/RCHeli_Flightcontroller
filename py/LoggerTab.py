import serial
import csv
import threading
import time
from PyQt5.QtWidgets import QWidget, QPushButton, QLabel, QVBoxLayout, QFileDialog, QMessageBox

class LoggerTab(QWidget):
    def __init__(self):
        super().__init__()

        # Variables
        self.serial_port = None
        self.is_logging = False
        self.filename = ""

        # GUI elements
        self.start_button = QPushButton("Start Recording")
        self.start_button.clicked.connect(self.start_recording)

        self.stop_button = QPushButton("Stop Recording")
        self.stop_button.setEnabled(False)
        self.stop_button.clicked.connect(self.stop_recording)

        self.status_label = QLabel("Status: Not Recording")

        # Layout setup
        layout = QVBoxLayout()
        layout.addWidget(self.start_button)
        layout.addWidget(self.stop_button)
        layout.addWidget(self.status_label)
        self.setLayout(layout)

    def start_recording(self):
        self.filename = QFileDialog.getSaveFileName(self, "Save CSV File", "", "CSV Files (*.csv)")[0]
        if not self.filename:
            return

        try:
            self.serial_port = serial.Serial('COM5', 115200, timeout=1)
        except serial.SerialException as e:
            QMessageBox.critical(self, "Error", f"Failed to open serial port: {e}")
            return

        self.is_logging = True
        self.start_button.setEnabled(False)
        self.stop_button.setEnabled(True)
        self.status_label.setText("Status: Recording...")

        self.thread = threading.Thread(target=self.log_data)
        self.thread.start()

    def log_data(self):
        with open(self.filename, mode='w', newline='') as csv_file:
            csv_writer = csv.writer(csv_file)
            csv_writer.writerow(['ax', 'ay', 'rollFiltered', 'pitchFiltered'])

            while self.is_logging:
                if self.serial_port.in_waiting > 0:
                    line = self.serial_port.readline().decode('utf-8').strip()
                    if line.startswith("Raw:"):
                        raw_data, filtered_data = line.split(" | Filtered: ")
                        raw_values = raw_data.replace("Raw: ", "").split(",")
                        filtered_values = filtered_data.split(",")
                        csv_writer.writerow(raw_values + filtered_values)
                time.sleep(0.1)

        self.serial_port.close()

    def stop_recording(self):
        self.is_logging = False
        self.start_button.setEnabled(True)
        self.stop_button.setEnabled(False)
        self.status_label.setText(f"Status: Not Recording - Saved to {self.filename}")
