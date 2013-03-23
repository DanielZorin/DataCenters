
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_GraphVis(object):
    def setupUi(self, GraphVis):
        GraphVis.setObjectName(_fromUtf8("GraphVis"))
        GraphVis.setWindowModality(QtCore.Qt.ApplicationModal)
        GraphVis.resize(471, 272)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/chart.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        GraphVis.setWindowIcon(icon)
        GraphVis.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}"))
        self.centralwidget = QtGui.QWidget(GraphVis)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.centralwidget)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.graphtype = QtGui.QComboBox(self.centralwidget)
        self.graphtype.setMinimumSize(QtCore.QSize(200, 0))
        self.graphtype.setObjectName(_fromUtf8("graphtype"))
        self.graphtype.addItem(_fromUtf8(""))
        self.graphtype.addItem(_fromUtf8(""))
        self.graphtype.addItem(_fromUtf8(""))
        self.graphtype.addItem(_fromUtf8(""))
        self.horizontalLayout.addWidget(self.graphtype)
        self.save = QtGui.QPushButton(self.centralwidget)
        self.save.setText(_fromUtf8(""))
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/cd.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.save.setIcon(icon1)
        self.save.setObjectName(_fromUtf8("save"))
        self.horizontalLayout.addWidget(self.save)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.scaledown = QtGui.QPushButton(self.centralwidget)
        self.scaledown.setText(_fromUtf8(""))
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/scaledown.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.scaledown.setIcon(icon2)
        self.scaledown.setObjectName(_fromUtf8("scaledown"))
        self.horizontalLayout.addWidget(self.scaledown)
        self.scaleup = QtGui.QPushButton(self.centralwidget)
        self.scaleup.setText(_fromUtf8(""))
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/scaleup.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.scaleup.setIcon(icon3)
        self.scaleup.setObjectName(_fromUtf8("scaleup"))
        self.horizontalLayout.addWidget(self.scaleup)
        self.pushButton = QtGui.QPushButton(self.centralwidget)
        self.pushButton.setText(_fromUtf8(""))
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/settings.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.pushButton.setIcon(icon4)
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        self.horizontalLayout.addWidget(self.pushButton)
        self.verticalLayout_2.addLayout(self.horizontalLayout)
        self.graph = QtGui.QGraphicsView(self.centralwidget)
        self.graph.setObjectName(_fromUtf8("graph"))
        self.verticalLayout_2.addWidget(self.graph)
        GraphVis.setCentralWidget(self.centralwidget)
        self.actionExit = QtGui.QAction(GraphVis)
        self.actionExit.setObjectName(_fromUtf8("actionExit"))

        self.retranslateUi(GraphVis)
        QtCore.QObject.connect(self.graphtype, QtCore.SIGNAL(_fromUtf8("currentIndexChanged(int)")), GraphVis.Replot)
        QtCore.QObject.connect(self.save, QtCore.SIGNAL(_fromUtf8("clicked()")), GraphVis.Save)
        QtCore.QObject.connect(self.scaledown, QtCore.SIGNAL(_fromUtf8("clicked()")), GraphVis.ScaleDown)
        QtCore.QObject.connect(self.scaleup, QtCore.SIGNAL(_fromUtf8("clicked()")), GraphVis.ScaleUp)
        QtCore.QObject.connect(self.pushButton, QtCore.SIGNAL(_fromUtf8("clicked()")), GraphVis.Settings)
        QtCore.QMetaObject.connectSlotsByName(GraphVis)

    def retranslateUi(self, GraphVis):
        GraphVis.setWindowTitle(QtGui.QApplication.translate("GraphVis", "Graph Plotter", None, QtGui.QApplication.UnicodeUTF8))
        self.graphtype.setItemText(0, QtGui.QApplication.translate("GraphVis", "Cumulative Used Performance", None, QtGui.QApplication.UnicodeUTF8))
        self.graphtype.setItemText(1, QtGui.QApplication.translate("GraphVis", "Cumulative Used Capacity", None, QtGui.QApplication.UnicodeUTF8))
        self.graphtype.setItemText(2, QtGui.QApplication.translate("GraphVis", "Cumulative Used Bandwidth", None, QtGui.QApplication.UnicodeUTF8))
        self.graphtype.setItemText(3, QtGui.QApplication.translate("GraphVis", "Cumulative Used RAM Capacity", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("GraphVis", "Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setShortcut(QtGui.QApplication.translate("GraphVis", "Ctrl+X", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
