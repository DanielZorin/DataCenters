
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
        self.assignedTenants = QtGui.QTreeWidget(self.layoutWidget)
        self.assignedTenants.setMinimumSize(QtCore.QSize(0, 0))
        self.assignedTenants.setMaximumSize(QtCore.QSize(111, 9999))
        self.assignedTenants.setSelectionMode(QtGui.QAbstractItemView.ExtendedSelection)
        self.assignedTenants.setObjectName(_fromUtf8("assignedTenants"))
        self.horizontalLayout.addWidget(self.assignedTenants)
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
        QtCore.QObject.connect(self.assignedTenants, QtCore.SIGNAL(_fromUtf8("itemSelectionChanged()")), Vis.tenantSelected)
        QtCore.QMetaObject.connectSlotsByName(Vis)

    def retranslateUi(self, Vis):
        Vis.setWindowTitle(QtGui.QApplication.translate("Vis", "Results Visualizer", None, QtGui.QApplication.UnicodeUTF8))
        self.assignedTenants.headerItem().setText(0, QtGui.QApplication.translate("Vis", "Assigned Tenants", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("Vis", "Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setShortcut(QtGui.QApplication.translate("Vis", "Ctrl+X", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
