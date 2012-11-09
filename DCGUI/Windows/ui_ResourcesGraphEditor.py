
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_ResourcesGraphEditor(object):
    def setupUi(self, ResourcesGraphEditor):
        ResourcesGraphEditor.setObjectName(_fromUtf8("ResourcesGraphEditor"))
        ResourcesGraphEditor.setWindowModality(QtCore.Qt.ApplicationModal)
        ResourcesGraphEditor.resize(427, 312)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/graph.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        ResourcesGraphEditor.setWindowIcon(icon)
        ResourcesGraphEditor.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}"))
        self.centralwidget = QtGui.QWidget(ResourcesGraphEditor)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.verticalLayout = QtGui.QVBoxLayout(self.centralwidget)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
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
        ResourcesGraphEditor.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(ResourcesGraphEditor)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 427, 20))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName(_fromUtf8("menuFile"))
        ResourcesGraphEditor.setMenuBar(self.menubar)
        self.toolBar = QtGui.QToolBar(ResourcesGraphEditor)
        self.toolBar.setObjectName(_fromUtf8("toolBar"))
        ResourcesGraphEditor.addToolBar(QtCore.Qt.TopToolBarArea, self.toolBar)
        self.actionSelect = QtGui.QAction(ResourcesGraphEditor)
        self.actionSelect.setCheckable(True)
        self.actionSelect.setChecked(True)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/select.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionSelect.setIcon(icon1)
        self.actionSelect.setObjectName(_fromUtf8("actionSelect"))
        self.actionComputer = QtGui.QAction(ResourcesGraphEditor)
        self.actionComputer.setCheckable(True)
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/computer.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionComputer.setIcon(icon2)
        self.actionComputer.setObjectName(_fromUtf8("actionComputer"))
        self.actionEdge = QtGui.QAction(ResourcesGraphEditor)
        self.actionEdge.setCheckable(True)
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/edge.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionEdge.setIcon(icon3)
        self.actionEdge.setObjectName(_fromUtf8("actionEdge"))
        self.actionNew_System = QtGui.QAction(ResourcesGraphEditor)
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/page.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionNew_System.setIcon(icon4)
        self.actionNew_System.setObjectName(_fromUtf8("actionNew_System"))
        self.actionOpen_System = QtGui.QAction(ResourcesGraphEditor)
        icon5 = QtGui.QIcon()
        icon5.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/folder.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionOpen_System.setIcon(icon5)
        self.actionOpen_System.setObjectName(_fromUtf8("actionOpen_System"))
        self.actionSave_System = QtGui.QAction(ResourcesGraphEditor)
        icon6 = QtGui.QIcon()
        icon6.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/cd.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionSave_System.setIcon(icon6)
        self.actionSave_System.setObjectName(_fromUtf8("actionSave_System"))
        self.actionSave_System_As = QtGui.QAction(ResourcesGraphEditor)
        self.actionSave_System_As.setObjectName(_fromUtf8("actionSave_System_As"))
        self.actionExit = QtGui.QAction(ResourcesGraphEditor)
        self.actionExit.setObjectName(_fromUtf8("actionExit"))
        self.actionStorage = QtGui.QAction(ResourcesGraphEditor)
        self.actionStorage.setCheckable(True)
        icon7 = QtGui.QIcon()
        icon7.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/storage.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionStorage.setIcon(icon7)
        self.actionStorage.setObjectName(_fromUtf8("actionStorage"))
        self.actionRouter = QtGui.QAction(ResourcesGraphEditor)
        self.actionRouter.setCheckable(True)
        icon8 = QtGui.QIcon()
        icon8.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/router.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionRouter.setIcon(icon8)
        self.actionRouter.setObjectName(_fromUtf8("actionRouter"))
        self.actionTopology = QtGui.QAction(ResourcesGraphEditor)
        icon9 = QtGui.QIcon()
        icon9.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/topology.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionTopology.setIcon(icon9)
        self.actionTopology.setObjectName(_fromUtf8("actionTopology"))
        self.menuFile.addAction(self.actionNew_System)
        self.menuFile.addAction(self.actionOpen_System)
        self.menuFile.addAction(self.actionSave_System)
        self.menuFile.addAction(self.actionSave_System_As)
        self.menuFile.addAction(self.actionExit)
        self.menubar.addAction(self.menuFile.menuAction())
        self.toolBar.addAction(self.actionNew_System)
        self.toolBar.addAction(self.actionOpen_System)
        self.toolBar.addAction(self.actionSave_System)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionSelect)
        self.toolBar.addAction(self.actionComputer)
        self.toolBar.addAction(self.actionStorage)
        self.toolBar.addAction(self.actionRouter)
        self.toolBar.addAction(self.actionEdge)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionTopology)

        self.retranslateUi(ResourcesGraphEditor)
        QtCore.QObject.connect(self.actionSelect, QtCore.SIGNAL(_fromUtf8("triggered()")), ResourcesGraphEditor.toggleSelect)
        QtCore.QObject.connect(self.actionComputer, QtCore.SIGNAL(_fromUtf8("triggered()")), ResourcesGraphEditor.toggleComputer)
        QtCore.QObject.connect(self.actionEdge, QtCore.SIGNAL(_fromUtf8("triggered()")), ResourcesGraphEditor.toggleEdge)
        QtCore.QObject.connect(self.actionNew_System, QtCore.SIGNAL(_fromUtf8("triggered()")), ResourcesGraphEditor.New)
        QtCore.QObject.connect(self.actionOpen_System, QtCore.SIGNAL(_fromUtf8("triggered()")), ResourcesGraphEditor.Open)
        QtCore.QObject.connect(self.actionSave_System, QtCore.SIGNAL(_fromUtf8("triggered()")), ResourcesGraphEditor.Save)
        QtCore.QObject.connect(self.actionSave_System_As, QtCore.SIGNAL(_fromUtf8("triggered()")), ResourcesGraphEditor.SaveAs)
        QtCore.QObject.connect(self.actionExit, QtCore.SIGNAL(_fromUtf8("triggered()")), ResourcesGraphEditor.close)
        QtCore.QObject.connect(self.actionStorage, QtCore.SIGNAL(_fromUtf8("triggered()")), ResourcesGraphEditor.toggleStorage)
        QtCore.QObject.connect(self.actionRouter, QtCore.SIGNAL(_fromUtf8("triggered()")), ResourcesGraphEditor.toggleRouter)
        QtCore.QObject.connect(self.actionTopology, QtCore.SIGNAL(_fromUtf8("triggered()")), ResourcesGraphEditor.generateTopology)
        QtCore.QMetaObject.connectSlotsByName(ResourcesGraphEditor)

    def retranslateUi(self, ResourcesGraphEditor):
        ResourcesGraphEditor.setWindowTitle(QtGui.QApplication.translate("ResourcesGraphEditor", "Resources Graph Editor", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("ResourcesGraphEditor", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBar.setWindowTitle(QtGui.QApplication.translate("ResourcesGraphEditor", "toolBar", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSelect.setText(QtGui.QApplication.translate("ResourcesGraphEditor", "Select", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSelect.setShortcut(QtGui.QApplication.translate("ResourcesGraphEditor", "Alt+1", None, QtGui.QApplication.UnicodeUTF8))
        self.actionComputer.setText(QtGui.QApplication.translate("ResourcesGraphEditor", "Add Computer", None, QtGui.QApplication.UnicodeUTF8))
        self.actionComputer.setToolTip(QtGui.QApplication.translate("ResourcesGraphEditor", "Add Computer", None, QtGui.QApplication.UnicodeUTF8))
        self.actionComputer.setShortcut(QtGui.QApplication.translate("ResourcesGraphEditor", "Alt+2", None, QtGui.QApplication.UnicodeUTF8))
        self.actionEdge.setText(QtGui.QApplication.translate("ResourcesGraphEditor", "Add Edge", None, QtGui.QApplication.UnicodeUTF8))
        self.actionEdge.setShortcut(QtGui.QApplication.translate("ResourcesGraphEditor", "Alt+3", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew_System.setText(QtGui.QApplication.translate("ResourcesGraphEditor", "New Graph", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew_System.setShortcut(QtGui.QApplication.translate("ResourcesGraphEditor", "Ctrl+N", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen_System.setText(QtGui.QApplication.translate("ResourcesGraphEditor", "Open Graph", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen_System.setShortcut(QtGui.QApplication.translate("ResourcesGraphEditor", "Ctrl+O", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System.setText(QtGui.QApplication.translate("ResourcesGraphEditor", "Save Graph", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System.setShortcut(QtGui.QApplication.translate("ResourcesGraphEditor", "Ctrl+S", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System_As.setText(QtGui.QApplication.translate("ResourcesGraphEditor", "Save Graph As...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_System_As.setShortcut(QtGui.QApplication.translate("ResourcesGraphEditor", "Ctrl+Shift+S", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("ResourcesGraphEditor", "Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setShortcut(QtGui.QApplication.translate("ResourcesGraphEditor", "Ctrl+X", None, QtGui.QApplication.UnicodeUTF8))
        self.actionStorage.setText(QtGui.QApplication.translate("ResourcesGraphEditor", "Add Storage", None, QtGui.QApplication.UnicodeUTF8))
        self.actionStorage.setToolTip(QtGui.QApplication.translate("ResourcesGraphEditor", "Add Storage", None, QtGui.QApplication.UnicodeUTF8))
        self.actionStorage.setShortcut(QtGui.QApplication.translate("ResourcesGraphEditor", "Alt+4", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRouter.setText(QtGui.QApplication.translate("ResourcesGraphEditor", "Add Router", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRouter.setToolTip(QtGui.QApplication.translate("ResourcesGraphEditor", "Add Router", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRouter.setShortcut(QtGui.QApplication.translate("ResourcesGraphEditor", "Alt+5", None, QtGui.QApplication.UnicodeUTF8))
        self.actionTopology.setText(QtGui.QApplication.translate("ResourcesGraphEditor", "Generate Standart Topology", None, QtGui.QApplication.UnicodeUTF8))
        self.actionTopology.setToolTip(QtGui.QApplication.translate("ResourcesGraphEditor", "Generate Standart Topology", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
