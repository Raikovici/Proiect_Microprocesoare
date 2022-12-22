import serial
from PyQt5 import QtWidgets, QtCore
from PyQt5.QtWidgets import QPushButton
import pyqtgraph as pg
import sys
import serial

ser_com = serial.Serial('COM7', 115200)


class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.setWindowTitle("Buzzer Window")

        self.graphWidget = pg.PlotWidget()
        self.setCentralWidget(self.graphWidget)
        self.graphWidget.setBackground((245, 245, 245))
        self.graphWidget.setTitle("\nData\n", color="black", size="25px")

        styles = {'color': 'black', 'font-size': '15px'}
        self.graphWidget.setLabel('left', 'Data', **styles)
        self.graphWidget.setLabel('bottom', 'Time', **styles)
        self.graphWidget.showGrid(x=True, y=True)
        self.graphWidget.setYRange(0, 2000, padding=0)
        self.graphWidget.setXRange(0, 800, padding=0)
        self.graphWidget.setAspectLocked(lock=True, ratio=1)


        self.x = list(range(100))
        self.y = list(range(100))

        pen = pg.mkPen(color=(0, 128, 128), width=5)
        self.data_line = self.graphWidget.plot(self.x, self.y, pen=pen)

        self.timer = QtCore.QTimer()
        self.timer.setInterval(10)
        self.timer.timeout.connect(self.update_plot_data)
        self.timer.start()
        self.initUI()

    def initUI(self):

        self.setGeometry(100, 100, 1600, 900)
        self.buzzerButton = QPushButton(self)
        self.buzzerButton.setText("BUZZER")
        self.buzzerButton.clicked.connect(self.signalBuzzer)
        self.buzzerButton.move(0, 0)

    def update_plot_data(self):
        data = ser_com.readline().decode('utf-8')
        #print(data)
        try:
            value = int(data)
            print(value)

            self.x = self.x[1:]
            self.x.append(self.x[-1] + 1)

            self.y = self.y[1:]

            self.y.append(value)
            self.data_line.setData(self.x, self.y)
        except:
            return

    def signalBuzzer(self):
        ser_com.write(1)


def main():
    ser_com.write(b'0')
    while True:
        data = ser_com.readline()
        data = data.decode("utf-8")

        # print(data)


if __name__ == '__main__':
    # main()
    app = QtWidgets.QApplication(sys.argv)

    SensorWindow = MainWindow()
    SensorWindow.show()
    sys.exit(app.exec_())
