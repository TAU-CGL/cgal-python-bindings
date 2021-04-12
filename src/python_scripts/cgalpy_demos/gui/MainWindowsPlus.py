from PyQt5 import QtCore, QtWidgets
from PyQt5.QtCore import Qt, pyqtSignal


class MainWindowPlus(QtWidgets.QMainWindow):
    signal_drop = pyqtSignal(str)

    def __init__(self, gui):
        super().__init__()
        self.gui = gui
        self.setAcceptDrops(True)

    # Adjust zoom level/scale on +/- key press
    # https://doc.qt.io/qt-5/qt.html#Key-enum
    def keyPressEvent(self, event):
        # if event.modifiers() & Qt.ControlModifier
        if event.key() == QtCore.Qt.Key_Plus:
            self.gui.zoom /= 0.9
        if event.key() == QtCore.Qt.Key_Minus:
            self.gui.zoom *= 0.9
        self.gui.redraw()

    def dragEnterEvent(self, event):
        event.accept()

    def dragMoveEvent(self, event):
        event.accept()

    def dropEvent(self, event):
        if event.mimeData().hasText:
            event.setDropAction(Qt.CopyAction)
            file_path: str = event.mimeData().urls()[0].toLocalFile()
            self.signal_drop.emit(file_path)
        event.accept()
