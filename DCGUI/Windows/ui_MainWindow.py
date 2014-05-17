
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(746, 602)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/star.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        MainWindow.setWindowIcon(icon)
        MainWindow.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #a0a0a0, stop: 1 #f0f0f0);\n"
"}\n"
"\n"
"QLabel, QSlider, QCheckBox {\n"
"    background-color: transparent;\n"
"}"))
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setStyleSheet(_fromUtf8(""))
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.horizontalLayout = QtGui.QHBoxLayout(self.centralwidget)
        self.horizontalLayout.setSizeConstraint(QtGui.QLayout.SetDefaultConstraint)
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.projectname = QtGui.QLabel(self.centralwidget)
        font = QtGui.QFont()
        font.setPointSize(14)
        self.projectname.setFont(font)
        self.projectname.setObjectName(_fromUtf8("projectname"))
        self.horizontalLayout_3.addWidget(self.projectname)
        self.editname = QtGui.QPushButton(self.centralwidget)
        self.editname.setMaximumSize(QtCore.QSize(32, 16777215))
        self.editname.setText(_fromUtf8(""))
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/page_edit.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.editname.setIcon(icon1)
        self.editname.setAutoRepeatInterval(100)
        self.editname.setFlat(True)
        self.editname.setObjectName(_fromUtf8("editname"))
        self.horizontalLayout_3.addWidget(self.editname)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem)
        self.verticalLayout.addLayout(self.horizontalLayout_3)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName(_fromUtf8("horizontalLayout_4"))
        self.label_2 = QtGui.QLabel(self.centralwidget)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.horizontalLayout_4.addWidget(self.label_2)
        self.algorithm = QtGui.QComboBox(self.centralwidget)
        self.algorithm.setMinimumSize(QtCore.QSize(100, 0))
        self.algorithm.setObjectName(_fromUtf8("algorithm"))
        self.algorithm.addItem(_fromUtf8(""))
        self.algorithm.addItem(_fromUtf8(""))
        self.algorithm.addItem(_fromUtf8(""))
        self.algorithm.addItem(_fromUtf8(""))
        self.algorithm.addItem(_fromUtf8(""))
        self.horizontalLayout_4.addWidget(self.algorithm)
        self.runall = QtGui.QPushButton(self.centralwidget)
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/play.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.runall.setIcon(icon2)
        self.runall.setFlat(True)
        self.runall.setObjectName(_fromUtf8("runall"))
        self.horizontalLayout_4.addWidget(self.runall)
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem1)
        self.verticalLayout.addLayout(self.horizontalLayout_4)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        self.tenants = QtGui.QTreeWidget(self.centralwidget)
        self.tenants.setObjectName(_fromUtf8("tenants"))
        self.horizontalLayout_2.addWidget(self.tenants)
        self.groupBox = QtGui.QGroupBox(self.centralwidget)
        self.groupBox.setObjectName(_fromUtf8("groupBox"))
        self.verticalLayout_3 = QtGui.QVBoxLayout(self.groupBox)
        self.verticalLayout_3.setObjectName(_fromUtf8("verticalLayout_3"))
        self.label_13 = QtGui.QLabel(self.groupBox)
        self.label_13.setObjectName(_fromUtf8("label_13"))
        self.verticalLayout_3.addWidget(self.label_13)
        self.tenantcount = QtGui.QLabel(self.groupBox)
        self.tenantcount.setObjectName(_fromUtf8("tenantcount"))
        self.verticalLayout_3.addWidget(self.tenantcount)
        self.label_12 = QtGui.QLabel(self.groupBox)
        self.label_12.setObjectName(_fromUtf8("label_12"))
        self.verticalLayout_3.addWidget(self.label_12)
        self.ratio = QtGui.QLabel(self.groupBox)
        self.ratio.setObjectName(_fromUtf8("ratio"))
        self.verticalLayout_3.addWidget(self.ratio)
        self.label = QtGui.QLabel(self.groupBox)
        self.label.setObjectName(_fromUtf8("label"))
        self.verticalLayout_3.addWidget(self.label)
        self.vmavg = QtGui.QLabel(self.groupBox)
        self.vmavg.setObjectName(_fromUtf8("vmavg"))
        self.verticalLayout_3.addWidget(self.vmavg)
        self.label_3 = QtGui.QLabel(self.groupBox)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.verticalLayout_3.addWidget(self.label_3)
        self.vmmax = QtGui.QLabel(self.groupBox)
        self.vmmax.setObjectName(_fromUtf8("vmmax"))
        self.verticalLayout_3.addWidget(self.vmmax)
        self.label_8 = QtGui.QLabel(self.groupBox)
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.verticalLayout_3.addWidget(self.label_8)
        self.ramavg = QtGui.QLabel(self.groupBox)
        self.ramavg.setObjectName(_fromUtf8("ramavg"))
        self.verticalLayout_3.addWidget(self.ramavg)
        self.label_10 = QtGui.QLabel(self.groupBox)
        self.label_10.setObjectName(_fromUtf8("label_10"))
        self.verticalLayout_3.addWidget(self.label_10)
        self.rammax = QtGui.QLabel(self.groupBox)
        self.rammax.setObjectName(_fromUtf8("rammax"))
        self.verticalLayout_3.addWidget(self.rammax)
        self.label_6 = QtGui.QLabel(self.groupBox)
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.verticalLayout_3.addWidget(self.label_6)
        self.stavg = QtGui.QLabel(self.groupBox)
        self.stavg.setObjectName(_fromUtf8("stavg"))
        self.verticalLayout_3.addWidget(self.stavg)
        self.label_7 = QtGui.QLabel(self.groupBox)
        self.label_7.setObjectName(_fromUtf8("label_7"))
        self.verticalLayout_3.addWidget(self.label_7)
        self.stmax = QtGui.QLabel(self.groupBox)
        self.stmax.setObjectName(_fromUtf8("stmax"))
        self.verticalLayout_3.addWidget(self.stmax)
        self.label_9 = QtGui.QLabel(self.groupBox)
        self.label_9.setObjectName(_fromUtf8("label_9"))
        self.verticalLayout_3.addWidget(self.label_9)
        self.netavg = QtGui.QLabel(self.groupBox)
        self.netavg.setObjectName(_fromUtf8("netavg"))
        self.verticalLayout_3.addWidget(self.netavg)
        self.label_11 = QtGui.QLabel(self.groupBox)
        self.label_11.setObjectName(_fromUtf8("label_11"))
        self.verticalLayout_3.addWidget(self.label_11)
        self.netmax = QtGui.QLabel(self.groupBox)
        self.netmax.setObjectName(_fromUtf8("netmax"))
        self.verticalLayout_3.addWidget(self.netmax)
        self.label_4 = QtGui.QLabel(self.groupBox)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.verticalLayout_3.addWidget(self.label_4)
        self.leafavg = QtGui.QLabel(self.groupBox)
        self.leafavg.setObjectName(_fromUtf8("leafavg"))
        self.verticalLayout_3.addWidget(self.leafavg)
        self.label_5 = QtGui.QLabel(self.groupBox)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.verticalLayout_3.addWidget(self.label_5)
        self.leafmax = QtGui.QLabel(self.groupBox)
        self.leafmax.setObjectName(_fromUtf8("leafmax"))
        self.verticalLayout_3.addWidget(self.leafmax)
        self.horizontalLayout_2.addWidget(self.groupBox)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.horizontalLayout.addLayout(self.verticalLayout)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 746, 21))
        self.menubar.setStyleSheet(_fromUtf8(""))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName(_fromUtf8("menuFile"))
        self.menuProject = QtGui.QMenu(self.menubar)
        self.menuProject.setObjectName(_fromUtf8("menuProject"))
        self.menuGenerators = QtGui.QMenu(self.menuProject)
        self.menuGenerators.setObjectName(_fromUtf8("menuGenerators"))
        MainWindow.setMenuBar(self.menubar)
        self.toolBar = QtGui.QToolBar(MainWindow)
        self.toolBar.setObjectName(_fromUtf8("toolBar"))
        MainWindow.addToolBar(QtCore.Qt.TopToolBarArea, self.toolBar)
        self.actionExit = QtGui.QAction(MainWindow)
        self.actionExit.setObjectName(_fromUtf8("actionExit"))
        self.actionAbout = QtGui.QAction(MainWindow)
        self.actionAbout.setObjectName(_fromUtf8("actionAbout"))
        self.actionNew_Project = QtGui.QAction(MainWindow)
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/page.png")), QtGui.QIcon.Normal, QtGui.QIcon.On)
        self.actionNew_Project.setIcon(icon3)
        self.actionNew_Project.setObjectName(_fromUtf8("actionNew_Project"))
        self.actionOpen_Project = QtGui.QAction(MainWindow)
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/folder.png")), QtGui.QIcon.Normal, QtGui.QIcon.On)
        self.actionOpen_Project.setIcon(icon4)
        self.actionOpen_Project.setObjectName(_fromUtf8("actionOpen_Project"))
        self.actionSave_Project = QtGui.QAction(MainWindow)
        icon5 = QtGui.QIcon()
        icon5.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/cd.png")), QtGui.QIcon.Normal, QtGui.QIcon.On)
        self.actionSave_Project.setIcon(icon5)
        self.actionSave_Project.setObjectName(_fromUtf8("actionSave_Project"))
        self.actionSave_Project_As = QtGui.QAction(MainWindow)
        self.actionSave_Project_As.setObjectName(_fromUtf8("actionSave_Project_As"))
        self.actionStart = QtGui.QAction(MainWindow)
        self.actionStart.setEnabled(True)
        icon6 = QtGui.QIcon()
        icon6.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/play.png")), QtGui.QIcon.Normal, QtGui.QIcon.On)
        self.actionStart.setIcon(icon6)
        self.actionStart.setObjectName(_fromUtf8("actionStart"))
        self.actionSettings = QtGui.QAction(MainWindow)
        icon7 = QtGui.QIcon()
        icon7.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/settings.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionSettings.setIcon(icon7)
        self.actionSettings.setObjectName(_fromUtf8("actionSettings"))
        self.actionEdit_Resources = QtGui.QAction(MainWindow)
        icon8 = QtGui.QIcon()
        icon8.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/graph.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionEdit_Resources.setIcon(icon8)
        self.actionEdit_Resources.setObjectName(_fromUtf8("actionEdit_Resources"))
        self.actionAdd_Demand = QtGui.QAction(MainWindow)
        icon9 = QtGui.QIcon()
        icon9.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/add.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionAdd_Demand.setIcon(icon9)
        self.actionAdd_Demand.setObjectName(_fromUtf8("actionAdd_Demand"))
        self.actionDelete_Demand = QtGui.QAction(MainWindow)
        icon10 = QtGui.QIcon()
        icon10.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/delete.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionDelete_Demand.setIcon(icon10)
        self.actionDelete_Demand.setObjectName(_fromUtf8("actionDelete_Demand"))
        self.actionCreate_Random_Demands = QtGui.QAction(MainWindow)
        self.actionCreate_Random_Demands.setEnabled(False)
        icon11 = QtGui.QIcon()
        icon11.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/dice.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionCreate_Random_Demands.setIcon(icon11)
        self.actionCreate_Random_Demands.setObjectName(_fromUtf8("actionCreate_Random_Demands"))
        self.actionEdit_Demand = QtGui.QAction(MainWindow)
        self.actionEdit_Demand.setIcon(icon1)
        self.actionEdit_Demand.setObjectName(_fromUtf8("actionEdit_Demand"))
        self.actionShow_Results = QtGui.QAction(MainWindow)
        icon12 = QtGui.QIcon()
        icon12.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/line_chart.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionShow_Results.setIcon(icon12)
        self.actionShow_Results.setObjectName(_fromUtf8("actionShow_Results"))
        self.actionShow_Statistics = QtGui.QAction(MainWindow)
        icon13 = QtGui.QIcon()
        icon13.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/chart.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionShow_Statistics.setIcon(icon13)
        self.actionShow_Statistics.setObjectName(_fromUtf8("actionShow_Statistics"))
        self.actionReset = QtGui.QAction(MainWindow)
        self.actionReset.setObjectName(_fromUtf8("actionReset"))
        self.actionSchedule_selected = QtGui.QAction(MainWindow)
        icon14 = QtGui.QIcon()
        icon14.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/fast_forward.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionSchedule_selected.setIcon(icon14)
        self.actionSchedule_selected.setObjectName(_fromUtf8("actionSchedule_selected"))
        self.actionRunMultiple = QtGui.QAction(MainWindow)
        icon15 = QtGui.QIcon()
        icon15.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/multtests.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionRunMultiple.setIcon(icon15)
        self.actionRunMultiple.setObjectName(_fromUtf8("actionRunMultiple"))
        self.actionFds = QtGui.QAction(MainWindow)
        self.actionFds.setObjectName(_fromUtf8("actionFds"))
        self.menuFile.addAction(self.actionNew_Project)
        self.menuFile.addAction(self.actionOpen_Project)
        self.menuFile.addAction(self.actionSave_Project)
        self.menuFile.addAction(self.actionSave_Project_As)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionExit)
        self.menuProject.addAction(self.actionEdit_Resources)
        self.menuProject.addAction(self.actionAdd_Demand)
        self.menuProject.addAction(self.actionDelete_Demand)
        self.menuProject.addAction(self.actionEdit_Demand)
        self.menuProject.addAction(self.actionCreate_Random_Demands)
        self.menuProject.addAction(self.menuGenerators.menuAction())
        self.menuProject.addAction(self.actionStart)
        self.menuProject.addAction(self.actionSchedule_selected)
        self.menuProject.addAction(self.actionShow_Statistics)
        self.menuProject.addAction(self.actionShow_Results)
        self.menuProject.addAction(self.actionReset)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuProject.menuAction())
        self.toolBar.addAction(self.actionNew_Project)
        self.toolBar.addAction(self.actionOpen_Project)
        self.toolBar.addAction(self.actionSave_Project)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionEdit_Resources)
        self.toolBar.addAction(self.actionAdd_Demand)
        self.toolBar.addAction(self.actionDelete_Demand)
        self.toolBar.addAction(self.actionEdit_Demand)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionStart)
        self.toolBar.addAction(self.actionSchedule_selected)
        self.toolBar.addAction(self.actionShow_Results)
        self.toolBar.addAction(self.actionSettings)

        self.retranslateUi(MainWindow)
        QtCore.QObject.connect(self.actionExit, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.Exit)
        QtCore.QObject.connect(self.actionAbout, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.About)
        QtCore.QObject.connect(self.actionNew_Project, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.NewProject)
        QtCore.QObject.connect(self.actionOpen_Project, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.OpenProject)
        QtCore.QObject.connect(self.actionSave_Project, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.SaveProject)
        QtCore.QObject.connect(self.actionSave_Project_As, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.SaveProjectAs)
        QtCore.QObject.connect(self.actionStart, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.Run)
        QtCore.QObject.connect(self.actionSettings, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.Settings)
        QtCore.QObject.connect(self.actionEdit_Resources, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.EditProgram)
        QtCore.QObject.connect(self.actionAdd_Demand, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.AddTenant)
        QtCore.QObject.connect(self.actionDelete_Demand, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.DeleteTenant)
        QtCore.QObject.connect(self.actionEdit_Demand, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.EditTenant)
        QtCore.QObject.connect(self.actionCreate_Random_Demands, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.RandomTenant)
        QtCore.QObject.connect(self.actionShow_Results, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.ShowResults)
        QtCore.QObject.connect(self.actionReset, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.Reset)
        QtCore.QObject.connect(self.editname, QtCore.SIGNAL(_fromUtf8("clicked()")), MainWindow.EditName)
        QtCore.QObject.connect(self.runall, QtCore.SIGNAL(_fromUtf8("clicked()")), MainWindow.Run)
        QtCore.QObject.connect(self.actionSchedule_selected, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.RunSelected)
        QtCore.QObject.connect(self.actionShow_Statistics, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.ShowGraphVis)
        QtCore.QObject.connect(self.tenants, QtCore.SIGNAL(_fromUtf8("itemChanged(QTreeWidgetItem*,int)")), MainWindow.UpdateTenant)
        QtCore.QObject.connect(self.actionRunMultiple, QtCore.SIGNAL(_fromUtf8("triggered()")), MainWindow.RunMultipleTests)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "Data Centers GUI", None, QtGui.QApplication.UnicodeUTF8))
        self.projectname.setText(QtGui.QApplication.translate("MainWindow", "Project name", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("MainWindow", "Algorithm:", None, QtGui.QApplication.UnicodeUTF8))
        self.algorithm.setItemText(0, QtGui.QApplication.translate("MainWindow", "Ant Colony", None, QtGui.QApplication.UnicodeUTF8))
        self.algorithm.setItemText(1, QtGui.QApplication.translate("MainWindow", "Centralized", None, QtGui.QApplication.UnicodeUTF8))
        self.algorithm.setItemText(2, QtGui.QApplication.translate("MainWindow", "Decentralized", None, QtGui.QApplication.UnicodeUTF8))
        self.algorithm.setItemText(3, QtGui.QApplication.translate("MainWindow", "FirstFit", None, QtGui.QApplication.UnicodeUTF8))
        self.algorithm.setItemText(4, QtGui.QApplication.translate("MainWindow", "RandomFit", None, QtGui.QApplication.UnicodeUTF8))
        self.runall.setText(QtGui.QApplication.translate("MainWindow", "Schedule", None, QtGui.QApplication.UnicodeUTF8))
        self.tenants.headerItem().setText(0, QtGui.QApplication.translate("MainWindow", "Name", None, QtGui.QApplication.UnicodeUTF8))
        self.tenants.headerItem().setText(1, QtGui.QApplication.translate("MainWindow", "Starting time", None, QtGui.QApplication.UnicodeUTF8))
        self.tenants.headerItem().setText(2, QtGui.QApplication.translate("MainWindow", "End time", None, QtGui.QApplication.UnicodeUTF8))
        self.tenants.headerItem().setText(3, QtGui.QApplication.translate("MainWindow", "Allow rescheduling", None, QtGui.QApplication.UnicodeUTF8))
        self.tenants.headerItem().setText(4, QtGui.QApplication.translate("MainWindow", "Scheduled", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("MainWindow", "Statistics", None, QtGui.QApplication.UnicodeUTF8))
        self.label_13.setText(QtGui.QApplication.translate("MainWindow", "Total requests scheduled", None, QtGui.QApplication.UnicodeUTF8))
        self.tenantcount.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_12.setText(QtGui.QApplication.translate("MainWindow", "Assignment ratio", None, QtGui.QApplication.UnicodeUTF8))
        self.ratio.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("MainWindow", "Average computational nodes CPU load", None, QtGui.QApplication.UnicodeUTF8))
        self.vmavg.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("MainWindow", "Maximal computational nodes CPU load", None, QtGui.QApplication.UnicodeUTF8))
        self.vmmax.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_8.setText(QtGui.QApplication.translate("MainWindow", "Average computational nodes RAM load", None, QtGui.QApplication.UnicodeUTF8))
        self.ramavg.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_10.setText(QtGui.QApplication.translate("MainWindow", "Maximal computational nodes RAM load", None, QtGui.QApplication.UnicodeUTF8))
        self.rammax.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_6.setText(QtGui.QApplication.translate("MainWindow", "Average stores load", None, QtGui.QApplication.UnicodeUTF8))
        self.stavg.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_7.setText(QtGui.QApplication.translate("MainWindow", "Maximal stores load", None, QtGui.QApplication.UnicodeUTF8))
        self.stmax.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_9.setText(QtGui.QApplication.translate("MainWindow", "Average network load", None, QtGui.QApplication.UnicodeUTF8))
        self.netavg.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_11.setText(QtGui.QApplication.translate("MainWindow", "Maximal network load", None, QtGui.QApplication.UnicodeUTF8))
        self.netmax.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("MainWindow", "Average leaf channels load", None, QtGui.QApplication.UnicodeUTF8))
        self.leafavg.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("MainWindow", "Maximal leaf channels load", None, QtGui.QApplication.UnicodeUTF8))
        self.leafmax.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("MainWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuProject.setTitle(QtGui.QApplication.translate("MainWindow", "Project", None, QtGui.QApplication.UnicodeUTF8))
        self.menuGenerators.setTitle(QtGui.QApplication.translate("MainWindow", "Generate Requests", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBar.setWindowTitle(QtGui.QApplication.translate("MainWindow", "toolBar", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("MainWindow", "Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+X", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAbout.setText(QtGui.QApplication.translate("MainWindow", "About DC GUI", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAbout.setToolTip(QtGui.QApplication.translate("MainWindow", "About DC GUI", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew_Project.setText(QtGui.QApplication.translate("MainWindow", "New Project", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew_Project.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+N", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen_Project.setText(QtGui.QApplication.translate("MainWindow", "Open Project", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen_Project.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+O", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_Project.setText(QtGui.QApplication.translate("MainWindow", "Save Project", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_Project.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+S", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_Project_As.setText(QtGui.QApplication.translate("MainWindow", "Save Project As...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_Project_As.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+Shift+S", None, QtGui.QApplication.UnicodeUTF8))
        self.actionStart.setText(QtGui.QApplication.translate("MainWindow", "Schedule All", None, QtGui.QApplication.UnicodeUTF8))
        self.actionStart.setToolTip(QtGui.QApplication.translate("MainWindow", "Schedule All", None, QtGui.QApplication.UnicodeUTF8))
        self.actionStart.setShortcut(QtGui.QApplication.translate("MainWindow", "F5", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSettings.setText(QtGui.QApplication.translate("MainWindow", "Settings...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSettings.setShortcut(QtGui.QApplication.translate("MainWindow", "F12", None, QtGui.QApplication.UnicodeUTF8))
        self.actionEdit_Resources.setText(QtGui.QApplication.translate("MainWindow", "Edit Resources", None, QtGui.QApplication.UnicodeUTF8))
        self.actionEdit_Resources.setToolTip(QtGui.QApplication.translate("MainWindow", "Edit Resources", None, QtGui.QApplication.UnicodeUTF8))
        self.actionEdit_Resources.setShortcut(QtGui.QApplication.translate("MainWindow", "F3", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAdd_Demand.setText(QtGui.QApplication.translate("MainWindow", "Add Tenant", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAdd_Demand.setToolTip(QtGui.QApplication.translate("MainWindow", "Add Request", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAdd_Demand.setShortcut(QtGui.QApplication.translate("MainWindow", "Alt+N", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDelete_Demand.setText(QtGui.QApplication.translate("MainWindow", "Delete Tenant", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDelete_Demand.setToolTip(QtGui.QApplication.translate("MainWindow", "Delete Tenant", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDelete_Demand.setShortcut(QtGui.QApplication.translate("MainWindow", "Alt+X", None, QtGui.QApplication.UnicodeUTF8))
        self.actionCreate_Random_Demands.setText(QtGui.QApplication.translate("MainWindow", "Create Random Requests", None, QtGui.QApplication.UnicodeUTF8))
        self.actionCreate_Random_Demands.setToolTip(QtGui.QApplication.translate("MainWindow", "Create Random Requests", None, QtGui.QApplication.UnicodeUTF8))
        self.actionCreate_Random_Demands.setShortcut(QtGui.QApplication.translate("MainWindow", "F9", None, QtGui.QApplication.UnicodeUTF8))
        self.actionEdit_Demand.setText(QtGui.QApplication.translate("MainWindow", "Edit Tenant", None, QtGui.QApplication.UnicodeUTF8))
        self.actionEdit_Demand.setToolTip(QtGui.QApplication.translate("MainWindow", "Edit Tenant", None, QtGui.QApplication.UnicodeUTF8))
        self.actionEdit_Demand.setShortcut(QtGui.QApplication.translate("MainWindow", "F4", None, QtGui.QApplication.UnicodeUTF8))
        self.actionShow_Results.setText(QtGui.QApplication.translate("MainWindow", "Show Results", None, QtGui.QApplication.UnicodeUTF8))
        self.actionShow_Results.setToolTip(QtGui.QApplication.translate("MainWindow", "Show Results", None, QtGui.QApplication.UnicodeUTF8))
        self.actionShow_Results.setShortcut(QtGui.QApplication.translate("MainWindow", "F7", None, QtGui.QApplication.UnicodeUTF8))
        self.actionShow_Statistics.setText(QtGui.QApplication.translate("MainWindow", "Show Statistics", None, QtGui.QApplication.UnicodeUTF8))
        self.actionShow_Statistics.setToolTip(QtGui.QApplication.translate("MainWindow", "Show Statistics", None, QtGui.QApplication.UnicodeUTF8))
        self.actionShow_Statistics.setShortcut(QtGui.QApplication.translate("MainWindow", "F8", None, QtGui.QApplication.UnicodeUTF8))
        self.actionReset.setText(QtGui.QApplication.translate("MainWindow", "Reset", None, QtGui.QApplication.UnicodeUTF8))
        self.actionReset.setShortcut(QtGui.QApplication.translate("MainWindow", "F10", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSchedule_selected.setText(QtGui.QApplication.translate("MainWindow", "Run All Algorithms", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSchedule_selected.setShortcut(QtGui.QApplication.translate("MainWindow", "F6, Ctrl+S", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRunMultiple.setText(QtGui.QApplication.translate("MainWindow", "runMultiple", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRunMultiple.setToolTip(QtGui.QApplication.translate("MainWindow", "Run Multiple Tests", None, QtGui.QApplication.UnicodeUTF8))
        self.actionFds.setText(QtGui.QApplication.translate("MainWindow", "fds", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
