from PyQt5 import QtWidgets, QtCore, QtGui
from gui.gui import GUI


class GUI_demo(GUI):
    def __init__(self):
        super().__init__()

    def setupUi(self):
        self.zoom = 1
        MainWindow = self.mainWindow
        MainWindow.setObjectName("MainWindow")
        # todo: icon
        # icon = QtGui.QIcon()
        # icon.addFile('icon.png', QtCore.QSize(48, 48))
        # MainWindow.setWindowIcon(icon)
        MainWindow.resize(self.width, self.height)

        # qt designer generated code
        MainWindow.setStyleSheet("QMainWindow { background-color : rgb(54, 57, 63); color : rgb(220, 221, 222); }\n"
                                 "#centralwidget { background-color : rgb(54, 57, 63); color : rgb(220, 221, 222); }\n"
                                 "QLabel { background-color : rgb(54, 57, 63); color : rgb(220, 221, 222); }")
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayout = QtWidgets.QGridLayout(self.centralwidget)
        self.gridLayout.setObjectName("gridLayout")
        self.graphicsView = QtWidgets.QGraphicsView(self.centralwidget)
        self.graphicsView.setEnabled(True)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(1)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.graphicsView.sizePolicy().hasHeightForWidth())
        self.graphicsView.setSizePolicy(sizePolicy)
        self.graphicsView.setObjectName("graphicsView")
        self.gridLayout.addWidget(self.graphicsView, 3, 1, 1, 1)
        self.gridLayout_0 = QtWidgets.QGridLayout()
        self.gridLayout_0.setObjectName("gridLayout_0")
        self.lineEdit_1 = QtWidgets.QLineEdit(self.centralwidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.lineEdit_1.setFont(font)
        self.lineEdit_1.setObjectName("lineEdit_1")
        self.gridLayout_0.addWidget(self.lineEdit_1, 2, 0, 1, 1)
        self.toolButton_0 = QtWidgets.QToolButton(self.centralwidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.toolButton_0.setFont(font)
        self.toolButton_0.setObjectName("toolButton_0")
        self.gridLayout_0.addWidget(self.toolButton_0, 0, 1, 1, 1)
        self.pushButton_1 = QtWidgets.QPushButton(self.centralwidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.pushButton_1.setFont(font)
        self.pushButton_1.setObjectName("pushButton_1")
        self.gridLayout_0.addWidget(self.pushButton_1, 9, 0, 1, 1)
        self.lineEdit_0 = QtWidgets.QLineEdit(self.centralwidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.lineEdit_0.setFont(font)
        self.lineEdit_0.setObjectName("lineEdit_0")
        self.gridLayout_0.addWidget(self.lineEdit_0, 0, 0, 1, 1)
        self.lineEdit_2 = QtWidgets.QLineEdit(self.centralwidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.lineEdit_2.setFont(font)
        self.lineEdit_2.setObjectName("lineEdit_2")
        self.gridLayout_0.addWidget(self.lineEdit_2, 7, 0, 1, 1)
        spacerItem = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.gridLayout_0.addItem(spacerItem, 10, 0, 1, 1)
        spacerItem1 = QtWidgets.QSpacerItem(300, 20, QtWidgets.QSizePolicy.MinimumExpanding,
                                            QtWidgets.QSizePolicy.Minimum)
        self.gridLayout_0.addItem(spacerItem1, 12, 0, 1, 1)
        self.toolButton_1 = QtWidgets.QToolButton(self.centralwidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.toolButton_1.setFont(font)
        self.toolButton_1.setObjectName("toolButton_1")
        self.gridLayout_0.addWidget(self.toolButton_1, 2, 1, 1, 1)
        self.pushButton_0 = QtWidgets.QPushButton(self.centralwidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.pushButton_0.setFont(font)
        self.pushButton_0.setObjectName("pushButton_0")
        self.gridLayout_0.addWidget(self.pushButton_0, 8, 0, 1, 1)
        self.gridLayout.addLayout(self.gridLayout_0, 3, 0, 1, 1)
        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        QtCore.QMetaObject.connectSlotsByName(MainWindow)

        # end of eq designer generated code

        self.lineEdits.append(self.lineEdit_0)
        self.lineEdits.append(self.lineEdit_1)
        self.lineEdits.append(self.lineEdit_2)
        self.pushButtons.append(self.pushButton_0)
        self.pushButtons.append(self.pushButton_1)
        self.pushButtons.append(self.toolButton_0)
        self.pushButtons.append(self.toolButton_1)
