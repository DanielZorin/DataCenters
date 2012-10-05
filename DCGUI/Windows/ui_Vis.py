
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_Vis(object):
    def setupUi(self, Vis):
        Vis.setObjectName(_fromUtf8("Vis"))
        Vis.setWindowModality(QtCore.Qt.ApplicationModal)
        Vis.resize(685, 339)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/graph.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Vis.setWindowIcon(icon)
        Vis.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}"))
        self.centralwidget = QtGui.QWidget(Vis)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.gridLayout = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.label = QtGui.QLabel(self.centralwidget)
        self.label.setObjectName(_fromUtf8("label"))
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)
        self.timeSpinBox = QtGui.QSpinBox(self.centralwidget)
        self.timeSpinBox.setMinimum(0)
        self.timeSpinBox.setMaximum(99)
        self.timeSpinBox.setObjectName(_fromUtf8("timeSpinBox"))
        self.gridLayout.addWidget(self.timeSpinBox, 0, 2, 1, 1)
        self.info = QtGui.QTextBrowser(self.centralwidget)
        self.info.setObjectName(_fromUtf8("info"))
        self.gridLayout.addWidget(self.info, 0, 3, 2, 1)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.graphArea = QtGui.QScrollArea(self.centralwidget)
        self.graphArea.setWidgetResizable(False)
        self.graphArea.setObjectName(_fromUtf8("graphArea"))
        self.scrollAreaWidgetContents = QtGui.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 398, 230))
        self.scrollAreaWidgetContents.setObjectName(_fromUtf8("scrollAreaWidgetContents"))
        self.graphArea.setWidget(self.scrollAreaWidgetContents)
        self.horizontalLayout.addWidget(self.graphArea)
        self.gridLayout.addLayout(self.horizontalLayout, 1, 0, 1, 3)
        self.timeSlider = QtGui.QSlider(self.centralwidget)
        self.timeSlider.setOrientation(QtCore.Qt.Horizontal)
        self.timeSlider.setObjectName(_fromUtf8("timeSlider"))
        self.gridLayout.addWidget(self.timeSlider, 0, 1, 1, 1)
        Vis.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(Vis)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 685, 20))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName(_fromUtf8("menuFile"))
        Vis.setMenuBar(self.menubar)
        self.actionOpen_System = QtGui.QAction(Vis)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/folder.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionOpen_System.setIcon(icon1)
        self.actionOpen_System.setObjectName(_fromUtf8("actionOpen_System"))
        self.actionSave_System = QtGui.QAction(Vis)
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/cd.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionSave_System.setIcon(icon2)
        self.actionSave_System.setObjectName(_fromUtf8("actionSave_System"))
        self.actionSave_System_As = QtGui.QAction(Vis)
        self.actionSave_System_As.setObjectName(_fromUtf8("actionSave_System_As"))
        self.actionExit = QtGui.QAction(Vis)
        self.actionExit.setObjectName(_fromUtf8("actionExit"))
        self.menuFile.addAction(self.actionOpen_System)
        self.menuFile.addAction(self.actionSave_System)
        self.menuFile.addAction(self.actionSave_System_As)
        self.menuFile.addAction(self.actionExit)
        self.menubar.addAction(self.menuFile.menuAction())

        self.retranslateUi(Vis)
        QtCore.QObject.connect(self.actionOpen_System, QtCore.SIGNAL(_fromUtf8("triggered()")), Vis.Open)
        QtCore.QObject.connect(self.actionSave_System, QtCore.SIGNAL(_fromUtf8("triggered()")), Vis.Save)
        QtCore.QObject.connect(self.actionSave_System_As, QtCore.SIGNAL(_fromUtf8("triggered()")), Vis.SaveAs)
        QtCore.QObject.connect(self.actionExit, QtCore.SIGNAL(_fromUtf8("triggered()")), Vis.close)
        QtCore.QObject.connect(self.timeSlider, QtCore.SIGNAL(_fromUtf8("valueChanged(int)")), Vis.UpdateTimeFromSlider)
        QtCore.QObject.connect(self.timeSpinBox, QtCore.SIGNAL(_fromUtf8("valueChanged(int)")), Vis.UpdateTimeFromSpinBox)
        QtCore.QMetaObject.connectSlotsByName(Vis)

    def retranslateUi(self, Vis):
        Vis.setWindowTitle(QtGui.QApplication.translate("Vis", "Results Visualizer", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Vis", "Time:", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("Vis", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen_System.setText(QtGui.QApplication.translate("Vis", "Open Graph", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen_System.setShortcut(QtGui.QApplication.translate("Vis", "Ctrl+O", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System.setText(QtGui.QApplication.translate("Vis", "Save Graph", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System.setShortcut(QtGui.QApplication.translate("Vis", "Ctrl+S", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System_As.setText(QtGui.QApplication.translate("Vis", "Save Graph As...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System_As.setShortcut(QtGui.QApplication.translate("Vis", "Ctrl+Shift+S", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("Vis", "Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setShortcut(QtGui.QApplication.translate("Vis", "Ctrl+X", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
