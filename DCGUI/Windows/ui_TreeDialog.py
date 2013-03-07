
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_TreeDialog(object):
    def setupUi(self, TreeDialog):
        TreeDialog.setObjectName(_fromUtf8("TreeDialog"))
        TreeDialog.resize(481, 532)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(TreeDialog.sizePolicy().hasHeightForWidth())
        TreeDialog.setSizePolicy(sizePolicy)
        TreeDialog.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}"))
        TreeDialog.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_9 = QtGui.QVBoxLayout(TreeDialog)
        self.verticalLayout_9.setObjectName(_fromUtf8("verticalLayout_9"))
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName(_fromUtf8("horizontalLayout_4"))
        self.label = QtGui.QLabel(TreeDialog)
        self.label.setObjectName(_fromUtf8("label"))
        self.horizontalLayout_4.addWidget(self.label)
        self.routersNum0 = QtGui.QSpinBox(TreeDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.routersNum0.sizePolicy().hasHeightForWidth())
        self.routersNum0.setSizePolicy(sizePolicy)
        self.routersNum0.setMinimum(1)
        self.routersNum0.setObjectName(_fromUtf8("routersNum0"))
        self.horizontalLayout_4.addWidget(self.routersNum0)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem)
        self.verticalLayout_9.addLayout(self.horizontalLayout_4)
        self.rootLevel = QtGui.QGroupBox(TreeDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.rootLevel.sizePolicy().hasHeightForWidth())
        self.rootLevel.setSizePolicy(sizePolicy)
        self.rootLevel.setObjectName(_fromUtf8("rootLevel"))
        self.horizontalLayout_16 = QtGui.QHBoxLayout(self.rootLevel)
        self.horizontalLayout_16.setObjectName(_fromUtf8("horizontalLayout_16"))
        self.verticalLayout_3 = QtGui.QVBoxLayout()
        self.verticalLayout_3.setObjectName(_fromUtf8("verticalLayout_3"))
        self.label_2 = QtGui.QLabel(self.rootLevel)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.verticalLayout_3.addWidget(self.label_2)
        self.label_9 = QtGui.QLabel(self.rootLevel)
        self.label_9.setObjectName(_fromUtf8("label_9"))
        self.verticalLayout_3.addWidget(self.label_9)
        self.horizontalLayout_15 = QtGui.QHBoxLayout()
        self.horizontalLayout_15.setObjectName(_fromUtf8("horizontalLayout_15"))
        self.routerBandwidth0 = QtGui.QSpinBox(self.rootLevel)
        self.routerBandwidth0.setMinimum(1)
        self.routerBandwidth0.setMaximum(99999)
        self.routerBandwidth0.setObjectName(_fromUtf8("routerBandwidth0"))
        self.horizontalLayout_15.addWidget(self.routerBandwidth0)
        spacerItem1 = QtGui.QSpacerItem(13, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_15.addItem(spacerItem1)
        self.verticalLayout_3.addLayout(self.horizontalLayout_15)
        self.horizontalLayout_16.addLayout(self.verticalLayout_3)
        spacerItem2 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_16.addItem(spacerItem2)
        self.verticalLayout_2 = QtGui.QVBoxLayout()
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.label1 = QtGui.QLabel(self.rootLevel)
        self.label1.setObjectName(_fromUtf8("label1"))
        self.verticalLayout_2.addWidget(self.label1)
        self.label2 = QtGui.QLabel(self.rootLevel)
        self.label2.setObjectName(_fromUtf8("label2"))
        self.verticalLayout_2.addWidget(self.label2)
        self.horizontalLayout_14 = QtGui.QHBoxLayout()
        self.horizontalLayout_14.setObjectName(_fromUtf8("horizontalLayout_14"))
        self.routerChilds0 = QtGui.QSpinBox(self.rootLevel)
        self.routerChilds0.setMinimum(1)
        self.routerChilds0.setMaximum(99999)
        self.routerChilds0.setObjectName(_fromUtf8("routerChilds0"))
        self.horizontalLayout_14.addWidget(self.routerChilds0)
        spacerItem3 = QtGui.QSpacerItem(17, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_14.addItem(spacerItem3)
        self.verticalLayout_2.addLayout(self.horizontalLayout_14)
        self.horizontalLayout_16.addLayout(self.verticalLayout_2)
        spacerItem4 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_16.addItem(spacerItem4)
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.label_19 = QtGui.QLabel(self.rootLevel)
        self.label_19.setObjectName(_fromUtf8("label_19"))
        self.verticalLayout.addWidget(self.label_19)
        self.label_10 = QtGui.QLabel(self.rootLevel)
        self.label_10.setObjectName(_fromUtf8("label_10"))
        self.verticalLayout.addWidget(self.label_10)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        self.channelsBandwidth0 = QtGui.QSpinBox(self.rootLevel)
        self.channelsBandwidth0.setMinimum(1)
        self.channelsBandwidth0.setMaximum(99999)
        self.channelsBandwidth0.setObjectName(_fromUtf8("channelsBandwidth0"))
        self.horizontalLayout_2.addWidget(self.channelsBandwidth0)
        spacerItem5 = QtGui.QSpacerItem(28, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem5)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.horizontalLayout_16.addLayout(self.verticalLayout)
        self.verticalLayout_9.addWidget(self.rootLevel)
        self.horizontalLayout_8 = QtGui.QHBoxLayout()
        self.horizontalLayout_8.setObjectName(_fromUtf8("horizontalLayout_8"))
        self.numRouters1Lab = QtGui.QLabel(TreeDialog)
        self.numRouters1Lab.setObjectName(_fromUtf8("numRouters1Lab"))
        self.horizontalLayout_8.addWidget(self.numRouters1Lab)
        self.routersNum1 = QtGui.QSpinBox(TreeDialog)
        self.routersNum1.setMinimum(1)
        self.routersNum1.setObjectName(_fromUtf8("routersNum1"))
        self.horizontalLayout_8.addWidget(self.routersNum1)
        spacerItem6 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_8.addItem(spacerItem6)
        self.verticalLayout_9.addLayout(self.horizontalLayout_8)
        self.level1 = QtGui.QGroupBox(TreeDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.level1.sizePolicy().hasHeightForWidth())
        self.level1.setSizePolicy(sizePolicy)
        self.level1.setObjectName(_fromUtf8("level1"))
        self.horizontalLayout_17 = QtGui.QHBoxLayout(self.level1)
        self.horizontalLayout_17.setObjectName(_fromUtf8("horizontalLayout_17"))
        self.verticalLayout_6 = QtGui.QVBoxLayout()
        self.verticalLayout_6.setObjectName(_fromUtf8("verticalLayout_6"))
        self.label_5 = QtGui.QLabel(self.level1)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.verticalLayout_6.addWidget(self.label_5)
        self.label_12 = QtGui.QLabel(self.level1)
        self.label_12.setObjectName(_fromUtf8("label_12"))
        self.verticalLayout_6.addWidget(self.label_12)
        self.horizontalLayout_13 = QtGui.QHBoxLayout()
        self.horizontalLayout_13.setObjectName(_fromUtf8("horizontalLayout_13"))
        self.routerBandwidth1 = QtGui.QSpinBox(self.level1)
        self.routerBandwidth1.setMinimum(1)
        self.routerBandwidth1.setMaximum(99999)
        self.routerBandwidth1.setObjectName(_fromUtf8("routerBandwidth1"))
        self.horizontalLayout_13.addWidget(self.routerBandwidth1)
        spacerItem7 = QtGui.QSpacerItem(28, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_13.addItem(spacerItem7)
        self.verticalLayout_6.addLayout(self.horizontalLayout_13)
        self.horizontalLayout_17.addLayout(self.verticalLayout_6)
        spacerItem8 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_17.addItem(spacerItem8)
        self.verticalLayout_7 = QtGui.QVBoxLayout()
        self.verticalLayout_7.setObjectName(_fromUtf8("verticalLayout_7"))
        self.label_6 = QtGui.QLabel(self.level1)
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.verticalLayout_7.addWidget(self.label_6)
        self.label_15 = QtGui.QLabel(self.level1)
        self.label_15.setObjectName(_fromUtf8("label_15"))
        self.verticalLayout_7.addWidget(self.label_15)
        self.horizontalLayout_9 = QtGui.QHBoxLayout()
        self.horizontalLayout_9.setObjectName(_fromUtf8("horizontalLayout_9"))
        self.routerChilds1 = QtGui.QSpinBox(self.level1)
        self.routerChilds1.setMinimum(1)
        self.routerChilds1.setMaximum(99999)
        self.routerChilds1.setObjectName(_fromUtf8("routerChilds1"))
        self.horizontalLayout_9.addWidget(self.routerChilds1)
        spacerItem9 = QtGui.QSpacerItem(28, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_9.addItem(spacerItem9)
        self.verticalLayout_7.addLayout(self.horizontalLayout_9)
        self.horizontalLayout_17.addLayout(self.verticalLayout_7)
        spacerItem10 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_17.addItem(spacerItem10)
        self.verticalLayout_8 = QtGui.QVBoxLayout()
        self.verticalLayout_8.setObjectName(_fromUtf8("verticalLayout_8"))
        self.label_11 = QtGui.QLabel(self.level1)
        self.label_11.setObjectName(_fromUtf8("label_11"))
        self.verticalLayout_8.addWidget(self.label_11)
        self.label_3 = QtGui.QLabel(self.level1)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.verticalLayout_8.addWidget(self.label_3)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.channelsBandwidth1 = QtGui.QSpinBox(self.level1)
        self.channelsBandwidth1.setMinimum(1)
        self.channelsBandwidth1.setMaximum(99999)
        self.channelsBandwidth1.setObjectName(_fromUtf8("channelsBandwidth1"))
        self.horizontalLayout_3.addWidget(self.channelsBandwidth1)
        spacerItem11 = QtGui.QSpacerItem(18, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem11)
        self.verticalLayout_8.addLayout(self.horizontalLayout_3)
        self.horizontalLayout_17.addLayout(self.verticalLayout_8)
        self.verticalLayout_9.addWidget(self.level1)
        self.level2 = QtGui.QGroupBox(TreeDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.level2.sizePolicy().hasHeightForWidth())
        self.level2.setSizePolicy(sizePolicy)
        self.level2.setObjectName(_fromUtf8("level2"))
        self.horizontalLayout_18 = QtGui.QHBoxLayout(self.level2)
        self.horizontalLayout_18.setObjectName(_fromUtf8("horizontalLayout_18"))
        self.verticalLayout_10 = QtGui.QVBoxLayout()
        self.verticalLayout_10.setObjectName(_fromUtf8("verticalLayout_10"))
        self.label_17 = QtGui.QLabel(self.level2)
        self.label_17.setObjectName(_fromUtf8("label_17"))
        self.verticalLayout_10.addWidget(self.label_17)
        self.label_16 = QtGui.QLabel(self.level2)
        self.label_16.setObjectName(_fromUtf8("label_16"))
        self.verticalLayout_10.addWidget(self.label_16)
        self.horizontalLayout_12 = QtGui.QHBoxLayout()
        self.horizontalLayout_12.setObjectName(_fromUtf8("horizontalLayout_12"))
        self.routerBandwidth2 = QtGui.QSpinBox(self.level2)
        self.routerBandwidth2.setMinimum(1)
        self.routerBandwidth2.setMaximum(99999)
        self.routerBandwidth2.setObjectName(_fromUtf8("routerBandwidth2"))
        self.horizontalLayout_12.addWidget(self.routerBandwidth2)
        spacerItem12 = QtGui.QSpacerItem(28, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_12.addItem(spacerItem12)
        self.verticalLayout_10.addLayout(self.horizontalLayout_12)
        self.horizontalLayout_18.addLayout(self.verticalLayout_10)
        spacerItem13 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_18.addItem(spacerItem13)
        self.verticalLayout_13 = QtGui.QVBoxLayout()
        self.verticalLayout_13.setObjectName(_fromUtf8("verticalLayout_13"))
        self.label_27 = QtGui.QLabel(self.level2)
        self.label_27.setObjectName(_fromUtf8("label_27"))
        self.verticalLayout_13.addWidget(self.label_27)
        self.label_28 = QtGui.QLabel(self.level2)
        self.label_28.setObjectName(_fromUtf8("label_28"))
        self.verticalLayout_13.addWidget(self.label_28)
        self.horizontalLayout_11 = QtGui.QHBoxLayout()
        self.horizontalLayout_11.setObjectName(_fromUtf8("horizontalLayout_11"))
        self.computersNodes = QtGui.QSpinBox(self.level2)
        self.computersNodes.setMinimum(0)
        self.computersNodes.setMaximum(1)
        self.computersNodes.setObjectName(_fromUtf8("computersNodes"))
        self.horizontalLayout_11.addWidget(self.computersNodes)
        spacerItem14 = QtGui.QSpacerItem(28, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_11.addItem(spacerItem14)
        self.verticalLayout_13.addLayout(self.horizontalLayout_11)
        self.horizontalLayout_18.addLayout(self.verticalLayout_13)
        spacerItem15 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_18.addItem(spacerItem15)
        self.verticalLayout_12 = QtGui.QVBoxLayout()
        self.verticalLayout_12.setObjectName(_fromUtf8("verticalLayout_12"))
        self.label_23 = QtGui.QLabel(self.level2)
        self.label_23.setObjectName(_fromUtf8("label_23"))
        self.verticalLayout_12.addWidget(self.label_23)
        self.label_24 = QtGui.QLabel(self.level2)
        self.label_24.setObjectName(_fromUtf8("label_24"))
        self.verticalLayout_12.addWidget(self.label_24)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName(_fromUtf8("horizontalLayout_5"))
        self.storagesNodes = QtGui.QSpinBox(self.level2)
        self.storagesNodes.setMaximum(1)
        self.storagesNodes.setObjectName(_fromUtf8("storagesNodes"))
        self.horizontalLayout_5.addWidget(self.storagesNodes)
        spacerItem16 = QtGui.QSpacerItem(28, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_5.addItem(spacerItem16)
        self.verticalLayout_12.addLayout(self.horizontalLayout_5)
        self.horizontalLayout_18.addLayout(self.verticalLayout_12)
        self.verticalLayout_9.addWidget(self.level2)
        self.horizontalLayout_7 = QtGui.QHBoxLayout()
        self.horizontalLayout_7.setObjectName(_fromUtf8("horizontalLayout_7"))
        self.horizontalLayout_6 = QtGui.QHBoxLayout()
        self.horizontalLayout_6.setObjectName(_fromUtf8("horizontalLayout_6"))
        self.verticalLayout_5 = QtGui.QVBoxLayout()
        self.verticalLayout_5.setObjectName(_fromUtf8("verticalLayout_5"))
        self.label_13 = QtGui.QLabel(TreeDialog)
        self.label_13.setObjectName(_fromUtf8("label_13"))
        self.verticalLayout_5.addWidget(self.label_13)
        self.label_29 = QtGui.QLabel(TreeDialog)
        self.label_29.setObjectName(_fromUtf8("label_29"))
        self.verticalLayout_5.addWidget(self.label_29)
        self.label_8 = QtGui.QLabel(TreeDialog)
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.verticalLayout_5.addWidget(self.label_8)
        self.label_7 = QtGui.QLabel(TreeDialog)
        self.label_7.setObjectName(_fromUtf8("label_7"))
        self.verticalLayout_5.addWidget(self.label_7)
        self.horizontalLayout_6.addLayout(self.verticalLayout_5)
        self.verticalLayout_4 = QtGui.QVBoxLayout()
        self.verticalLayout_4.setObjectName(_fromUtf8("verticalLayout_4"))
        self.storagesNum = QtGui.QSpinBox(TreeDialog)
        self.storagesNum.setMinimum(1)
        self.storagesNum.setMaximum(99999)
        self.storagesNum.setObjectName(_fromUtf8("storagesNum"))
        self.verticalLayout_4.addWidget(self.storagesNum)
        self.storageChannelsBandwidth2 = QtGui.QSpinBox(TreeDialog)
        self.storageChannelsBandwidth2.setMinimum(1)
        self.storageChannelsBandwidth2.setMaximum(99999)
        self.storageChannelsBandwidth2.setObjectName(_fromUtf8("storageChannelsBandwidth2"))
        self.verticalLayout_4.addWidget(self.storageChannelsBandwidth2)
        self.capacity = QtGui.QSpinBox(TreeDialog)
        self.capacity.setMinimum(1)
        self.capacity.setMaximum(99999)
        self.capacity.setObjectName(_fromUtf8("capacity"))
        self.verticalLayout_4.addWidget(self.capacity)
        self.numTypes = QtGui.QSpinBox(TreeDialog)
        self.numTypes.setMinimum(1)
        self.numTypes.setMaximum(99999)
        self.numTypes.setObjectName(_fromUtf8("numTypes"))
        self.verticalLayout_4.addWidget(self.numTypes)
        self.horizontalLayout_6.addLayout(self.verticalLayout_4)
        self.horizontalLayout_7.addLayout(self.horizontalLayout_6)
        self.verticalLayout_18 = QtGui.QVBoxLayout()
        self.verticalLayout_18.setObjectName(_fromUtf8("verticalLayout_18"))
        self.horizontalLayout_10 = QtGui.QHBoxLayout()
        self.horizontalLayout_10.setObjectName(_fromUtf8("horizontalLayout_10"))
        self.verticalLayout_19 = QtGui.QVBoxLayout()
        self.verticalLayout_19.setObjectName(_fromUtf8("verticalLayout_19"))
        self.label_26 = QtGui.QLabel(TreeDialog)
        self.label_26.setObjectName(_fromUtf8("label_26"))
        self.verticalLayout_19.addWidget(self.label_26)
        self.label_14 = QtGui.QLabel(TreeDialog)
        self.label_14.setObjectName(_fromUtf8("label_14"))
        self.verticalLayout_19.addWidget(self.label_14)
        self.label_30 = QtGui.QLabel(TreeDialog)
        self.label_30.setObjectName(_fromUtf8("label_30"))
        self.verticalLayout_19.addWidget(self.label_30)
        self.horizontalLayout_10.addLayout(self.verticalLayout_19)
        self.verticalLayout_20 = QtGui.QVBoxLayout()
        self.verticalLayout_20.setObjectName(_fromUtf8("verticalLayout_20"))
        self.computersNum = QtGui.QSpinBox(TreeDialog)
        self.computersNum.setMinimum(1)
        self.computersNum.setMaximum(99999)
        self.computersNum.setObjectName(_fromUtf8("computersNum"))
        self.verticalLayout_20.addWidget(self.computersNum)
        self.computerChannelsBandwidth2 = QtGui.QSpinBox(TreeDialog)
        self.computerChannelsBandwidth2.setMinimum(1)
        self.computerChannelsBandwidth2.setMaximum(99999)
        self.computerChannelsBandwidth2.setObjectName(_fromUtf8("computerChannelsBandwidth2"))
        self.verticalLayout_20.addWidget(self.computerChannelsBandwidth2)
        self.performance = QtGui.QSpinBox(TreeDialog)
        self.performance.setMinimum(1)
        self.performance.setMaximum(99999)
        self.performance.setObjectName(_fromUtf8("performance"))
        self.verticalLayout_20.addWidget(self.performance)
        self.horizontalLayout_10.addLayout(self.verticalLayout_20)
        self.verticalLayout_18.addLayout(self.horizontalLayout_10)
        spacerItem17 = QtGui.QSpacerItem(20, 18, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout_18.addItem(spacerItem17)
        self.horizontalLayout_7.addLayout(self.verticalLayout_18)
        self.verticalLayout_9.addLayout(self.horizontalLayout_7)
        self.horizontalLayout_19 = QtGui.QHBoxLayout()
        self.horizontalLayout_19.setObjectName(_fromUtf8("horizontalLayout_19"))
        self.label_4 = QtGui.QLabel(TreeDialog)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.horizontalLayout_19.addWidget(self.label_4)
        self.copyNum = QtGui.QSpinBox(TreeDialog)
        self.copyNum.setMinimum(0)
        self.copyNum.setProperty(_fromUtf8("value"), 0)
        self.copyNum.setObjectName(_fromUtf8("copyNum"))
        self.horizontalLayout_19.addWidget(self.copyNum)
        spacerItem18 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_19.addItem(spacerItem18)
        self.verticalLayout_9.addLayout(self.horizontalLayout_19)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.OK = QtGui.QPushButton(TreeDialog)
        self.OK.setObjectName(_fromUtf8("OK"))
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(TreeDialog)
        self.Cancel.setObjectName(_fromUtf8("Cancel"))
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout_9.addLayout(self.horizontalLayout)

        self.retranslateUi(TreeDialog)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL(_fromUtf8("clicked()")), TreeDialog.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL(_fromUtf8("clicked()")), TreeDialog.reject)
        QtCore.QObject.connect(self.routersNum0, QtCore.SIGNAL(_fromUtf8("valueChanged(int)")), TreeDialog.nodeNumChanged)
        QtCore.QObject.connect(self.routersNum1, QtCore.SIGNAL(_fromUtf8("valueChanged(int)")), TreeDialog.nodeNumChanged)
        QtCore.QObject.connect(self.routerChilds0, QtCore.SIGNAL(_fromUtf8("valueChanged(int)")), TreeDialog.nodeNumChanged)
        QtCore.QObject.connect(self.routerChilds1, QtCore.SIGNAL(_fromUtf8("valueChanged(int)")), TreeDialog.nodeNumChanged)
        QtCore.QObject.connect(self.storagesNodes, QtCore.SIGNAL(_fromUtf8("editingFinished()")), TreeDialog.storagesNodesChanged)
        QtCore.QObject.connect(self.computersNodes, QtCore.SIGNAL(_fromUtf8("editingFinished()")), TreeDialog.computersNodesChanged)
        QtCore.QMetaObject.connectSlotsByName(TreeDialog)

    def retranslateUi(self, TreeDialog):
        TreeDialog.setWindowTitle(QtGui.QApplication.translate("TreeDialog", "Tree parameters", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("TreeDialog", "Number of core switches:", None, QtGui.QApplication.UnicodeUTF8))
        self.rootLevel.setTitle(QtGui.QApplication.translate("TreeDialog", "Core layer", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("TreeDialog", "Switches", None, QtGui.QApplication.UnicodeUTF8))
        self.label_9.setText(QtGui.QApplication.translate("TreeDialog", "bandwidth: ", None, QtGui.QApplication.UnicodeUTF8))
        self.label1.setText(QtGui.QApplication.translate("TreeDialog", "Number of outgoing", None, QtGui.QApplication.UnicodeUTF8))
        self.label2.setText(QtGui.QApplication.translate("TreeDialog", "channels per switch:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_19.setText(QtGui.QApplication.translate("TreeDialog", "Outgoing channels    ", None, QtGui.QApplication.UnicodeUTF8))
        self.label_10.setText(QtGui.QApplication.translate("TreeDialog", "bandwidth:", None, QtGui.QApplication.UnicodeUTF8))
        self.numRouters1Lab.setText(QtGui.QApplication.translate("TreeDialog", "Number of 2nd layer switches:", None, QtGui.QApplication.UnicodeUTF8))
        self.level1.setTitle(QtGui.QApplication.translate("TreeDialog", "2nd layer", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("TreeDialog", "Switches", None, QtGui.QApplication.UnicodeUTF8))
        self.label_12.setText(QtGui.QApplication.translate("TreeDialog", "bandwidth: ", None, QtGui.QApplication.UnicodeUTF8))
        self.label_6.setText(QtGui.QApplication.translate("TreeDialog", "Number of outgoing", None, QtGui.QApplication.UnicodeUTF8))
        self.label_15.setText(QtGui.QApplication.translate("TreeDialog", "channels per switch:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_11.setText(QtGui.QApplication.translate("TreeDialog", "Outgoing channels    ", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("TreeDialog", "bandwidth:", None, QtGui.QApplication.UnicodeUTF8))
        self.level2.setTitle(QtGui.QApplication.translate("TreeDialog", "1st layer", None, QtGui.QApplication.UnicodeUTF8))
        self.label_17.setText(QtGui.QApplication.translate("TreeDialog", "Switches", None, QtGui.QApplication.UnicodeUTF8))
        self.label_16.setText(QtGui.QApplication.translate("TreeDialog", "bandwidth: ", None, QtGui.QApplication.UnicodeUTF8))
        self.label_27.setText(QtGui.QApplication.translate("TreeDialog", "Number of switches to", None, QtGui.QApplication.UnicodeUTF8))
        self.label_28.setText(QtGui.QApplication.translate("TreeDialog", "connect computers:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_23.setText(QtGui.QApplication.translate("TreeDialog", "Number of switches to", None, QtGui.QApplication.UnicodeUTF8))
        self.label_24.setText(QtGui.QApplication.translate("TreeDialog", "connect data stores:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_13.setText(QtGui.QApplication.translate("TreeDialog", "Number of data stores per switch:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_29.setText(QtGui.QApplication.translate("TreeDialog", "Data store channels bandwidth:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_8.setText(QtGui.QApplication.translate("TreeDialog", "Data store capacity:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_7.setText(QtGui.QApplication.translate("TreeDialog", "Number of data store types:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_26.setText(QtGui.QApplication.translate("TreeDialog", "Number of computers per switch:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_14.setText(QtGui.QApplication.translate("TreeDialog", "Computer channels bandwidth:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_30.setText(QtGui.QApplication.translate("TreeDialog", "Computer performance:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("TreeDialog", "Number of high-level switches replicas (per switch):", None, QtGui.QApplication.UnicodeUTF8))
        self.OK.setText(QtGui.QApplication.translate("TreeDialog", "OK", None, QtGui.QApplication.UnicodeUTF8))
        self.Cancel.setText(QtGui.QApplication.translate("TreeDialog", "Cancel", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
