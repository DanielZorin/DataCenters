
from PyQt4 import QtCore, QtGui

class Ui_Vis(object):
    def setupUi(self, Vis):
        Vis.setObjectName("Vis")
        Vis.setWindowModality(QtCore.Qt.ApplicationModal)
        Vis.resize(800, 283)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/pics/pics/line_chart.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Vis.setWindowIcon(icon)
        Vis.setStyleSheet("QWidget, QMenuBar::item {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}")
        self.centralwidget = QtGui.QWidget(Vis)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.centralwidget)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.splitter = QtGui.QSplitter(self.centralwidget)
        self.splitter.setOrientation(QtCore.Qt.Horizontal)
        self.splitter.setObjectName("splitter")
        self.layoutWidget = QtGui.QWidget(self.splitter)
        self.layoutWidget.setObjectName("layoutWidget")
        self.verticalLayout = QtGui.QVBoxLayout(self.layoutWidget)
        self.verticalLayout.setObjectName("verticalLayout")
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.label = QtGui.QLabel(self.layoutWidget)
        self.label.setObjectName("label")
        self.horizontalLayout_4.addWidget(self.label)
        self.timeSlider = QtGui.QSlider(self.layoutWidget)
        self.timeSlider.setOrientation(QtCore.Qt.Horizontal)
        self.timeSlider.setObjectName("timeSlider")
        self.horizontalLayout_4.addWidget(self.timeSlider)
        self.timeSpinBox = QtGui.QSpinBox(self.layoutWidget)
        self.timeSpinBox.setMinimum(0)
        self.timeSpinBox.setMaximum(99)
        self.timeSpinBox.setObjectName("timeSpinBox")
        self.horizontalLayout_4.addWidget(self.timeSpinBox)
        self.verticalLayout.addLayout(self.horizontalLayout_4)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.assignedDemands = QtGui.QTreeWidget(self.layoutWidget)
        self.assignedDemands.setMinimumSize(QtCore.QSize(0, 0))
        self.assignedDemands.setMaximumSize(QtCore.QSize(111, 9999))
        self.assignedDemands.setSelectionMode(QtGui.QAbstractItemView.ExtendedSelection)
        self.assignedDemands.setObjectName("assignedDemands")
        self.horizontalLayout.addWidget(self.assignedDemands)
        self.graphArea = QtGui.QScrollArea(self.layoutWidget)
        self.graphArea.setWidgetResizable(False)
        self.graphArea.setObjectName("graphArea")
        self.scrollAreaWidgetContents = QtGui.QWidget(self.graphArea)
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 405, 230))
        self.scrollAreaWidgetContents.setObjectName("scrollAreaWidgetContents")
        self.graphArea.setWidget(self.scrollAreaWidgetContents)
        self.horizontalLayout.addWidget(self.graphArea)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.info = QtGui.QTextBrowser(self.splitter)
        self.info.setObjectName("info")
        self.verticalLayout_2.addWidget(self.splitter)
        Vis.setCentralWidget(self.centralwidget)
        self.actionExit = QtGui.QAction(Vis)
        self.actionExit.setObjectName("actionExit")

        self.retranslateUi(Vis)
        QtCore.QObject.connect(self.actionExit, QtCore.SIGNAL("triggered()"), Vis.close)
        QtCore.QObject.connect(self.timeSlider, QtCore.SIGNAL("valueChanged(int)"), Vis.UpdateTimeFromSlider)
        QtCore.QObject.connect(self.timeSpinBox, QtCore.SIGNAL("valueChanged(int)"), Vis.UpdateTimeFromSpinBox)
        QtCore.QObject.connect(self.assignedDemands, QtCore.SIGNAL("itemSelectionChanged()"), Vis.demandSelected)
        QtCore.QMetaObject.connectSlotsByName(Vis)

    def retranslateUi(self, Vis):
        Vis.setWindowTitle(QtGui.QApplication.translate("Vis", "Results Visualizer", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Vis", "Time:", None, QtGui.QApplication.UnicodeUTF8))
        self.assignedDemands.headerItem().setText(0, QtGui.QApplication.translate("Vis", "Assigned Requests", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("Vis", "Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setShortcut(QtGui.QApplication.translate("Vis", "Ctrl+X", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
