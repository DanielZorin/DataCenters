
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_Vis(object):
    def setupUi(self, Vis):
        Vis.setObjectName(_fromUtf8("Vis"))
        Vis.setWindowModality(QtCore.Qt.ApplicationModal)
        Vis.resize(800, 283)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/line_chart.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Vis.setWindowIcon(icon)
        Vis.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #a0a0a0, stop: 1 #f0f0f0);\n"
"}\n"
"\n"
"QLabel, QSlider, QCheckBox {\n"
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
        QtCore.QObject.connect(self.assignedDemands, QtCore.SIGNAL(_fromUtf8("itemSelectionChanged()")), Vis.demandSelected)
        QtCore.QMetaObject.connectSlotsByName(Vis)

    def retranslateUi(self, Vis):
        Vis.setWindowTitle(_translate("Vis", "Results Visualizer", None))
        self.assignedDemands.headerItem().setText(0, _translate("Vis", "Assigned Tenants", None))
        self.actionExit.setText(_translate("Vis", "Exit", None))
        self.actionExit.setShortcut(_translate("Vis", "Ctrl+X", None))

from . import resources_rc
