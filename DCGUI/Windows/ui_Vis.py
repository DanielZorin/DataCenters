
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_Vis(object):
    def setupUi(self, Vis):
        Vis.setObjectName(_fromUtf8("Vis"))
        Vis.setWindowModality(QtCore.Qt.ApplicationModal)
        Vis.resize(800, 283)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/line_chart.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
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
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.centralwidget)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.splitter = QtGui.QSplitter(self.centralwidget)
        self.splitter.setOrientation(QtCore.Qt.Horizontal)
        self.splitter.setObjectName(_fromUtf8("splitter"))
        self.layoutWidget = QtGui.QWidget(self.splitter)
        self.layoutWidget.setObjectName(_fromUtf8("layoutWidget"))
        self.verticalLayout = QtGui.QVBoxLayout(self.layoutWidget)
        self.verticalLayout.setMargin(0)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName(_fromUtf8("horizontalLayout_4"))
        self.label = QtGui.QLabel(self.layoutWidget)
        self.label.setObjectName(_fromUtf8("label"))
        self.horizontalLayout_4.addWidget(self.label)
        self.timeSlider = QtGui.QSlider(self.layoutWidget)
        self.timeSlider.setOrientation(QtCore.Qt.Horizontal)
        self.timeSlider.setObjectName(_fromUtf8("timeSlider"))
        self.horizontalLayout_4.addWidget(self.timeSlider)
        self.timeSpinBox = QtGui.QSpinBox(self.layoutWidget)
        self.timeSpinBox.setMinimum(0)
        self.timeSpinBox.setMaximum(99)
        self.timeSpinBox.setObjectName(_fromUtf8("timeSpinBox"))
        self.horizontalLayout_4.addWidget(self.timeSpinBox)
        self.verticalLayout.addLayout(self.horizontalLayout_4)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.assignedDemands = QtGui.QTreeWidget(self.layoutWidget)
        self.assignedDemands.setMinimumSize(QtCore.QSize(0, 0))
        self.assignedDemands.setMaximumSize(QtCore.QSize(111, 9999))
        self.assignedDemands.setSelectionMode(QtGui.QAbstractItemView.ExtendedSelection)
        self.assignedDemands.setObjectName(_fromUtf8("assignedDemands"))
        self.horizontalLayout.addWidget(self.assignedDemands)
        self.graphArea = QtGui.QScrollArea(self.layoutWidget)
        self.graphArea.setWidgetResizable(False)
        self.graphArea.setObjectName(_fromUtf8("graphArea"))
        self.scrollAreaWidgetContents = QtGui.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 405, 230))
        self.scrollAreaWidgetContents.setObjectName(_fromUtf8("scrollAreaWidgetContents"))
        self.graphArea.setWidget(self.scrollAreaWidgetContents)
        self.horizontalLayout.addWidget(self.graphArea)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.info = QtGui.QTextBrowser(self.splitter)
        self.info.setObjectName(_fromUtf8("info"))
        self.verticalLayout_2.addWidget(self.splitter)
        Vis.setCentralWidget(self.centralwidget)
        self.actionExit = QtGui.QAction(Vis)
        self.actionExit.setObjectName(_fromUtf8("actionExit"))

        self.retranslateUi(Vis)
        QtCore.QObject.connect(self.actionExit, QtCore.SIGNAL(_fromUtf8("triggered()")), Vis.close)
        QtCore.QObject.connect(self.timeSlider, QtCore.SIGNAL(_fromUtf8("valueChanged(int)")), Vis.UpdateTimeFromSlider)
        QtCore.QObject.connect(self.timeSpinBox, QtCore.SIGNAL(_fromUtf8("valueChanged(int)")), Vis.UpdateTimeFromSpinBox)
        QtCore.QObject.connect(self.assignedDemands, QtCore.SIGNAL(_fromUtf8("itemSelectionChanged()")), Vis.demandSelected)
        QtCore.QMetaObject.connectSlotsByName(Vis)

    def retranslateUi(self, Vis):
        Vis.setWindowTitle(QtGui.QApplication.translate("Vis", "Results Visualizer", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Vis", "Time:", None, QtGui.QApplication.UnicodeUTF8))
        self.assignedDemands.headerItem().setText(0, QtGui.QApplication.translate("Vis", "Assigned Requests", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("Vis", "Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setShortcut(QtGui.QApplication.translate("Vis", "Ctrl+X", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
