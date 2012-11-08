
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_GraphVis(object):
    def setupUi(self, GraphVis):
        GraphVis.setObjectName(_fromUtf8("GraphVis"))
        GraphVis.setWindowModality(QtCore.Qt.ApplicationModal)
        GraphVis.resize(424, 272)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/graph.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
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
        self.horizontalLayout.addWidget(self.graphtype)
        self.pushButton = QtGui.QPushButton(self.centralwidget)
        self.pushButton.setText(_fromUtf8(""))
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/cd.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.pushButton.setIcon(icon1)
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        self.horizontalLayout.addWidget(self.pushButton)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.verticalLayout_2.addLayout(self.horizontalLayout)
        self.graph = QtGui.QGraphicsView(self.centralwidget)
        self.graph.setObjectName(_fromUtf8("graph"))
        self.verticalLayout_2.addWidget(self.graph)
        GraphVis.setCentralWidget(self.centralwidget)
        self.actionExit = QtGui.QAction(GraphVis)
        self.actionExit.setObjectName(_fromUtf8("actionExit"))

        self.retranslateUi(GraphVis)
        QtCore.QObject.connect(self.graphtype, QtCore.SIGNAL(_fromUtf8("currentIndexChanged(int)")), GraphVis.Replot)
        QtCore.QMetaObject.connectSlotsByName(GraphVis)

    def retranslateUi(self, GraphVis):
        GraphVis.setWindowTitle(QtGui.QApplication.translate("GraphVis", "Graph Plotter", None, QtGui.QApplication.UnicodeUTF8))
        self.graphtype.setItemText(0, QtGui.QApplication.translate("GraphVis", "Average Speed", None, QtGui.QApplication.UnicodeUTF8))
        self.graphtype.setItemText(1, QtGui.QApplication.translate("GraphVis", "Average Volume", None, QtGui.QApplication.UnicodeUTF8))
        self.graphtype.setItemText(2, QtGui.QApplication.translate("GraphVis", "Average Capacity", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("GraphVis", "Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setShortcut(QtGui.QApplication.translate("GraphVis", "Ctrl+X", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
