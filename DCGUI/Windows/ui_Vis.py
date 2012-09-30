
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_Vis(object):
    def setupUi(self, Vis):
        Vis.setObjectName(_fromUtf8("Vis"))
        Vis.setWindowModality(QtCore.Qt.ApplicationModal)
        Vis.resize(437, 339)
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
        self.verticalLayout = QtGui.QVBoxLayout(self.centralwidget)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        self.label = QtGui.QLabel(self.centralwidget)
        self.label.setObjectName(_fromUtf8("label"))
        self.horizontalLayout_2.addWidget(self.label)
        self.lineEdit = QtGui.QLineEdit(self.centralwidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.lineEdit.sizePolicy().hasHeightForWidth())
        self.lineEdit.setSizePolicy(sizePolicy)
        self.lineEdit.setObjectName(_fromUtf8("lineEdit"))
        self.horizontalLayout_2.addWidget(self.lineEdit)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
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
        self.verticalLayout.addLayout(self.horizontalLayout)
        Vis.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(Vis)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 437, 20))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName(_fromUtf8("menuFile"))
        Vis.setMenuBar(self.menubar)
        self.toolBar = QtGui.QToolBar(Vis)
        self.toolBar.setObjectName(_fromUtf8("toolBar"))
        Vis.addToolBar(QtCore.Qt.TopToolBarArea, self.toolBar)
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
        self.toolBar.addAction(self.actionOpen_System)
        self.toolBar.addAction(self.actionSave_System)
        self.toolBar.addSeparator()

        self.retranslateUi(Vis)
        QtCore.QObject.connect(self.actionOpen_System, QtCore.SIGNAL(_fromUtf8("triggered()")), Vis.Open)
        QtCore.QObject.connect(self.actionSave_System, QtCore.SIGNAL(_fromUtf8("triggered()")), Vis.Save)
        QtCore.QObject.connect(self.actionSave_System_As, QtCore.SIGNAL(_fromUtf8("triggered()")), Vis.SaveAs)
        QtCore.QObject.connect(self.actionExit, QtCore.SIGNAL(_fromUtf8("triggered()")), Vis.close)
        QtCore.QMetaObject.connectSlotsByName(Vis)

    def retranslateUi(self, Vis):
        Vis.setWindowTitle(QtGui.QApplication.translate("Vis", "Results Visualizer", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Vis", "Time", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("Vis", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBar.setWindowTitle(QtGui.QApplication.translate("Vis", "toolBar", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen_System.setText(QtGui.QApplication.translate("Vis", "Open Graph", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen_System.setShortcut(QtGui.QApplication.translate("Vis", "Ctrl+O", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System.setText(QtGui.QApplication.translate("Vis", "Save Graph", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System.setShortcut(QtGui.QApplication.translate("Vis", "Ctrl+S", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System_As.setText(QtGui.QApplication.translate("Vis", "Save Graph As...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System_As.setShortcut(QtGui.QApplication.translate("Vis", "Ctrl+Shift+S", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("Vis", "Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setShortcut(QtGui.QApplication.translate("Vis", "Ctrl+X", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
