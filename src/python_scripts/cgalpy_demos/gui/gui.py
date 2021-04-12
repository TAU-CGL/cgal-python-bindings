from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtOpenGL import QGLWidget, QGLFormat, QGL
from PyQt5.QtWidgets import (QApplication, QGraphicsView,
                             QGraphicsPixmapItem, QGraphicsScene, QGraphicsPolygonItem,
                             QGraphicsEllipseItem, QGraphicsLineItem, QGraphicsTextItem, QOpenGLWidget)
from PyQt5.QtGui import QPainter, QPixmap, QPolygonF, QPen, QVector3D, QPalette, QFont
from PyQt5.QtCore import (QObject, QPointF, QPoint, QRectF,
                          QPropertyAnimation, pyqtProperty, QSequentialAnimationGroup,
                          QParallelAnimationGroup, QPauseAnimation, Qt, pyqtSignal)
from .RDiscRobot import RDiscRobot

from .RPolygon import RPolygon
from .RDisc import RDisc
from .RSegment import RSegment
from .RText import RText
from .MainWindowsPlus import MainWindowPlus
from .GraphicsScenePlus import GraphicsScenePlus
from .RCircleSegment import RCircleSegment
import time


class GUI(object):
    width = 1600
    height = 1000
    zoom = 50.0
    base_line_width = 3.5
    base_text_size = 2 * zoom
    animation_finished_action = lambda: None
    lineEdits = []
    pushButtons = []
    labels = []
    progressBars = []
    scene = None
    mainWindow = None
    sequence = None
    graphicsView = None

    def __init__(self):
        self.mainWindow = MainWindowPlus(self)
        self.scene = GraphicsScenePlus(self)
        self.sequence = QSequentialAnimationGroup()
        self.sequence.finished.connect(self.animation_finished)
        self.setupUi()
        self.setup_view()

    def setupUi(self):
        pass

    def setup_view(self):
        if self.graphicsView is not None:
            self.graphicsView.setScene(self.scene)
            self.graphicsView.setSceneRect(0, 0, 0, 0)
            self.graphicsView.setRenderHints(QPainter.Antialiasing)
            # self.graphicsView.fitInView(self.scene.sceneRect(), Qt.KeepAspectRatio)
            # self.graphicsView.setViewport(QGLWidget(QGLFormat(QGL.SampleBuffers)))
            self.graphicsView.scale(self.zoom, -self.zoom)
            self.graphicsView.setDragMode(1)

    # Add a disc to the scene with radius r centered at (x, y) and return the object associated with it
    def add_disc(self, r, x, y, fill_color=QtCore.Qt.black, line_color=QtCore.Qt.black):
        d = RDisc(r, x, y, fill_color, line_color=line_color, line_width=self.base_line_width / self.zoom)
        self.scene.addItem(d.disc)
        return d

    # Add a disc robot to the scene with radius r centered at (x, y) and return the object associated with it
    def add_disc_robot(self, r, x, y, text="", fill_color=QtCore.Qt.black):
        d = RDiscRobot(r, x, y, fill_color, line_width=self.base_line_width / self.zoom, text=text)
        self.scene.addItem(d.disc)
        self.scene.addItem(d._text)
        return d

    # Add a polygon to the scene and return the object associated with it
    def add_polygon(self, points, fill_color=QtCore.Qt.black, line_color=QtCore.Qt.black):
        p = RPolygon(points, fill_color=fill_color, line_color=line_color, line_width=self.base_line_width / self.zoom)
        self.scene.addItem(p.polygon)
        return p

    # Add a segment to the scene and return the object associated with it
    def add_segment(self, x1, y1, x2, y2, line_color=QtCore.Qt.black):
        s = RSegment(x1, y1, x2, y2, color=line_color, line_width=self.base_line_width / self.zoom)
        self.scene.addItem(s.line)
        return s

    # Add a circle segment to the scene and return the object associated with it
    def add_circle_segment(self, radius: float, center_x: float, center_y: float, start_angle: float,
                           end_angle: float,
                           clockwise, fill_color=QtCore.Qt.transparent, line_color=QtCore.Qt.black):
        s = RCircleSegment(radius, center_x, center_y, start_angle, end_angle,
                           line_width=self.base_line_width / self.zoom, clockwise=clockwise,
                           fill_color=fill_color, line_color=line_color)
        self.scene.addItem(s.path)
        return s

    def add_text(self, text, x, y, size, color=QtCore.Qt.black):
        t = RText(text, x, y, size, color)
        self.scene.addItem(t.text)
        return t

    # Create a new linear translation animation for obj starting at ix, iy and ending at x, y
    def linear_translation_animation(self, obj, ix, iy, x, y, duration=1000):
        anim = QPropertyAnimation(obj, b'pos')
        anim.setDuration(duration)
        anim.setStartValue(QPointF(ix, iy))
        anim.setEndValue(QPointF(x, y))
        return anim

    # Create a general translation animation for obj. func is path from the unit interval I to R^2
    def translation_animation(self, obj, func, duration=1000):
        anim = QPropertyAnimation(obj, b'pos')
        anim.setDuration(duration)
        anim.setStartValue(QPointF(func(0)[0], func(0)[1]))
        anim.setEndValue(QPointF(func(1)[0], func(1)[1]))
        vals = [p / 100 for p in range(0, 101)]
        for i in vals:
            anim.setKeyValueAt(i, (QPointF(func(i)[0], func(i)[1])))
        return anim

    # Create an animation the changes the visibility of an object
    def visibility_animation(self, obj, visible):
        anim = QPropertyAnimation(obj, b'visible')
        anim.setDuration(0)
        if visible:
            anim.setEndValue(1)
        else:
            anim.setEndValue(0)
        return anim

    # Create an animation that does nothing
    def pause_animation(self, duration=1000):
        anim = QPauseAnimation(duration)
        return anim

    # Create an animation that changes the value of an object
    def value_animation(self, obj, v_begin, v_end, duration=1000):
        anim = QPropertyAnimation(obj, b'value')
        anim.setDuration(duration)
        anim.setStartValue(v_begin)
        anim.setEndValue(v_end)
        return anim

    # Create an animation that changes the text of an object
    def text_animation(self, obj, text: int):
        anim = QPropertyAnimation(obj, b'text')
        anim.setDuration(0)
        anim.setEndValue(text)
        return anim

    # Create an animation from a set of animations that will run in parallel
    def parallel_animation(self, *animations):
        group = QParallelAnimationGroup()
        for anim in animations:
            group.addAnimation(anim)
        return group

    # Add an animation to the animation queue
    def queue_animation(self, *animations):
        for anim in animations:
            self.sequence.addAnimation(anim)

    # Play (and empty) the animation queue
    def play_queue(self):
        self.sequence.start()

    # Empty the animation queue queue
    def empty_queue(self):
        self.sequence.clear()

    # Clear the scene of all objects
    def clear_scene(self):
        self.scene.clear()

    # Redraw the scene with updated parameters
    def redraw(self):
        line_width = self.base_line_width / self.zoom
        text_size = max(1, self.base_text_size / self.zoom)
        for item in self.graphicsView.items():
            if not isinstance(item, QGraphicsTextItem):
                pen = item.pen()
                pen.setWidthF(line_width)
                item.setPen(pen)
            else:
                item.setFont(QFont("Times", text_size))
        self.graphicsView.resetTransform()
        self.graphicsView.scale(self.zoom, -self.zoom)

    def animation_finished(self):
        self.empty_queue()
        print("Finished playing animation")
        self.animation_finished_action()

    def set_animation_finished_action(self, action):
        self.animation_finished_action = action

    # Set the i'th text of field in the GUI
    def set_field(self, i, s):
        self.lineEdits[i].setText(s)

    # Get the text of the i'th text field in the GUI
    def get_field(self, i):
        return self.lineEdits[i].text()

    # Set the text and color of the i'th label in the GUI
    def set_label(self, i, s, color=Qt.black):
        self.labels[i].setText(s)
        palette = self.labels[i].palette()
        palette.setColor(QPalette.WindowText, color)
        self.labels[i].setPalette(palette)

    # Set the function to be called when the i'th button in the GUI is pressed
    def set_logic(self, i, logic):
        try:
            self.pushButtons[i].clicked.disconnect()
        except Exception:
            pass
        self.pushButtons[i].clicked.connect(logic)

    # Set the i'th button text in the GUI
    def set_button_text(self, i, s):
        self.pushButtons[i].setText(s)

    # Set the value of the i'th progressBar
    def set_progressbar_value(self, i, n: int):
        self.progressBars[i].setValue(n)

    # Set the program's name
    def set_program_name(self, s):
        self.mainWindow.setWindowTitle(s)
