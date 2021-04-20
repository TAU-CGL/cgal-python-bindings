from PyQt5 import QtWidgets
from PyQt5.QtCore import pyqtSignal, Qt


class GraphicsScenePlus(QtWidgets.QGraphicsScene):
    right_click_signal = pyqtSignal(float, float)
    left_click_signal = pyqtSignal(float, float)

    def __init__(self, gui):
        super().__init__()
        self.gui = gui

    def wheelEvent(self, event):
        angle = event.delta() / 8
        if angle > 0:  # scroll up
            self.gui.zoom /= 0.9
        else:  # scroll up
            self.gui.zoom *= 0.9
        self.gui.redraw()
        event.accept()

    def mousePressEvent(self, event):
        x, y = round(event.scenePos().x()), round(event.scenePos().y())
        if event.button() == Qt.RightButton:
            self.right_click_signal.emit(x, y)
        elif event.button() == Qt.LeftButton:
            self.left_click_signal.emit(x, y)
        super(GraphicsScenePlus, self).mousePressEvent(event)