import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QTabWidget
from BodePlotTab import BodePlotTab
from LoggerTab import LoggerTab

class MainGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("HUBERT | Flightcontroller")
        self.setGeometry(200, 200, 1000, 600)

        # Tab widget
        self.tabs = QTabWidget()

        # Add Logger and Bode Plot tabs
        self.logger_tab = LoggerTab()
        self.bode_plot_tab = BodePlotTab()

        self.tabs.addTab(self.logger_tab, "Logger")
        self.tabs.addTab(self.bode_plot_tab, "Bode Plot")

        self.setCentralWidget(self.tabs)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainGUI()
    window.show()
    sys.exit(app.exec_())
