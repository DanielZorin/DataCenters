
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

class Ui_ResourceServer(object):
    def setupUi(self, ResourceServer):
        ResourceServer.setObjectName(_fromUtf8("ResourceServer"))
        ResourceServer.resize(324, 358)
        ResourceServer.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #a0a0a0, stop: 1 #f0f0f0);\n"
"}\n"
"\n"
"QLabel, QSlider, QCheckBox {\n"
"    background-color: transparent;\n"
"}"))
        ResourceServer.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_2 = QtGui.QVBoxLayout(ResourceServer)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.formLayout = QtGui.QFormLayout()
        self.formLayout.setFieldGrowthPolicy(QtGui.QFormLayout.AllNonFixedFieldsGrow)
        self.formLayout.setObjectName(_fromUtf8("formLayout"))
        self.namelabel = QtGui.QLabel(ResourceServer)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.namelabel.sizePolicy().hasHeightForWidth())
        self.namelabel.setSizePolicy(sizePolicy)
        self.namelabel.setObjectName(_fromUtf8("namelabel"))
        self.formLayout.setWidget(1, QtGui.QFormLayout.LabelRole, self.namelabel)
        self.name = QtGui.QLineEdit(ResourceServer)
        self.name.setEnabled(True)
        self.name.setReadOnly(False)
        self.name.setObjectName(_fromUtf8("name"))
        self.formLayout.setWidget(1, QtGui.QFormLayout.FieldRole, self.name)
        self.service = QtGui.QCheckBox(ResourceServer)
        self.service.setObjectName(_fromUtf8("service"))
        self.formLayout.setWidget(2, QtGui.QFormLayout.FieldRole, self.service)
        self.verticalLayout.addLayout(self.formLayout)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName(_fromUtf8("horizontalLayout_5"))
        self.label_3 = QtGui.QLabel(ResourceServer)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.horizontalLayout_5.addWidget(self.label_3)
        self.add = QtGui.QPushButton(ResourceServer)
        self.add.setMaximumSize(QtCore.QSize(16, 16777215))
        self.add.setText(_fromUtf8(""))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/add.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.add.setIcon(icon)
        self.add.setFlat(True)
        self.add.setObjectName(_fromUtf8("add"))
        self.horizontalLayout_5.addWidget(self.add)
        self.remove = QtGui.QPushButton(ResourceServer)
        self.remove.setMaximumSize(QtCore.QSize(16, 16777215))
        self.remove.setText(_fromUtf8(""))
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/delete.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.remove.setIcon(icon1)
        self.remove.setFlat(True)
        self.remove.setObjectName(_fromUtf8("remove"))
        self.horizontalLayout_5.addWidget(self.remove)
        self.verticalLayout.addLayout(self.horizontalLayout_5)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.horizontalLayout_4.setObjectName(_fromUtf8("horizontalLayout_4"))
        self.params = QtGui.QTableWidget(ResourceServer)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.params.sizePolicy().hasHeightForWidth())
        self.params.setSizePolicy(sizePolicy)
        self.params.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAsNeeded)
        self.params.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.params.setShowGrid(True)
        self.params.setColumnCount(3)
        self.params.setObjectName(_fromUtf8("params"))
        self.params.setRowCount(0)
        item = QtGui.QTableWidgetItem()
        self.params.setHorizontalHeaderItem(0, item)
        item = QtGui.QTableWidgetItem()
        self.params.setHorizontalHeaderItem(1, item)
        item = QtGui.QTableWidgetItem()
        self.params.setHorizontalHeaderItem(2, item)
        self.horizontalLayout_4.addWidget(self.params)
        self.verticalLayout.addLayout(self.horizontalLayout_4)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.OK = QtGui.QPushButton(ResourceServer)
        self.OK.setObjectName(_fromUtf8("OK"))
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(ResourceServer)
        self.Cancel.setObjectName(_fromUtf8("Cancel"))
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(ResourceServer)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL(_fromUtf8("clicked()")), ResourceServer.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL(_fromUtf8("clicked()")), ResourceServer.reject)
        QtCore.QObject.connect(self.add, QtCore.SIGNAL(_fromUtf8("clicked()")), ResourceServer.AddParam)
        QtCore.QObject.connect(self.remove, QtCore.SIGNAL(_fromUtf8("clicked()")), ResourceServer.RemoveParam)
        QtCore.QMetaObject.connectSlotsByName(ResourceServer)

    def retranslateUi(self, ResourceServer):
        ResourceServer.setWindowTitle(_translate("ResourceServer", "Edit Server", None))
        self.namelabel.setText(_translate("ResourceServer", "Name:", None))
        self.service.setText(_translate("ResourceServer", "Service", None))
        self.label_3.setText(_translate("ResourceServer", "Parameters:", None))
        item = self.params.horizontalHeaderItem(0)
        item.setText(_translate("ResourceServer", "Name", None))
        item = self.params.horizontalHeaderItem(1)
        item.setText(_translate("ResourceServer", "Type", None))
        item = self.params.horizontalHeaderItem(2)
        item.setText(_translate("ResourceServer", "Value", None))
        self.OK.setText(_translate("ResourceServer", "OK", None))
        self.Cancel.setText(_translate("ResourceServer", "Cancel", None))

from . import resources_rc
