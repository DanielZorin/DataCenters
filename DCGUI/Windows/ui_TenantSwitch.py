
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

class Ui_TenantSwitch(object):
    def setupUi(self, TenantSwitch):
        TenantSwitch.setObjectName(_fromUtf8("TenantSwitch"))
        TenantSwitch.resize(324, 398)
        TenantSwitch.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #a0a0a0, stop: 1 #f0f0f0);\n"
"}\n"
"\n"
"QLabel, QSlider, QCheckBox {\n"
"    background-color: transparent;\n"
"}"))
        TenantSwitch.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_2 = QtGui.QVBoxLayout(TenantSwitch)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.formLayout = QtGui.QFormLayout()
        self.formLayout.setFieldGrowthPolicy(QtGui.QFormLayout.AllNonFixedFieldsGrow)
        self.formLayout.setObjectName(_fromUtf8("formLayout"))
        self.namelabel = QtGui.QLabel(TenantSwitch)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.namelabel.sizePolicy().hasHeightForWidth())
        self.namelabel.setSizePolicy(sizePolicy)
        self.namelabel.setObjectName(_fromUtf8("namelabel"))
        self.formLayout.setWidget(0, QtGui.QFormLayout.LabelRole, self.namelabel)
        self.name = QtGui.QLineEdit(TenantSwitch)
        self.name.setEnabled(True)
        self.name.setReadOnly(False)
        self.name.setObjectName(_fromUtf8("name"))
        self.formLayout.setWidget(0, QtGui.QFormLayout.FieldRole, self.name)
        self.service = QtGui.QCheckBox(TenantSwitch)
        self.service.setObjectName(_fromUtf8("service"))
        self.formLayout.setWidget(1, QtGui.QFormLayout.FieldRole, self.service)
        self.label_4 = QtGui.QLabel(TenantSwitch)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.formLayout.setWidget(2, QtGui.QFormLayout.LabelRole, self.label_4)
        self.type = QtGui.QComboBox(TenantSwitch)
        self.type.setObjectName(_fromUtf8("type"))
        self.type.addItem(_fromUtf8(""))
        self.type.addItem(_fromUtf8(""))
        self.formLayout.setWidget(2, QtGui.QFormLayout.FieldRole, self.type)
        self.router = QtGui.QCheckBox(TenantSwitch)
        self.router.setObjectName(_fromUtf8("router"))
        self.formLayout.setWidget(3, QtGui.QFormLayout.LabelRole, self.router)
        self.ip = QtGui.QLineEdit(TenantSwitch)
        self.ip.setObjectName(_fromUtf8("ip"))
        self.formLayout.setWidget(3, QtGui.QFormLayout.FieldRole, self.ip)
        self.serviceasuser = QtGui.QCheckBox(TenantSwitch)
        self.serviceasuser.setObjectName(_fromUtf8("serviceasuser"))
        self.formLayout.setWidget(4, QtGui.QFormLayout.FieldRole, self.serviceasuser)
        self.label_5 = QtGui.QLabel(TenantSwitch)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.formLayout.setWidget(5, QtGui.QFormLayout.LabelRole, self.label_5)
        self.provider = QtGui.QComboBox(TenantSwitch)
        self.provider.setObjectName(_fromUtf8("provider"))
        self.formLayout.setWidget(5, QtGui.QFormLayout.FieldRole, self.provider)
        self.label_2 = QtGui.QLabel(TenantSwitch)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.formLayout.setWidget(6, QtGui.QFormLayout.LabelRole, self.label_2)
        self.servicename = QtGui.QComboBox(TenantSwitch)
        self.servicename.setObjectName(_fromUtf8("servicename"))
        self.formLayout.setWidget(6, QtGui.QFormLayout.FieldRole, self.servicename)
        self.label_6 = QtGui.QLabel(TenantSwitch)
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.formLayout.setWidget(7, QtGui.QFormLayout.LabelRole, self.label_6)
        self.port = QtGui.QComboBox(TenantSwitch)
        self.port.setObjectName(_fromUtf8("port"))
        self.formLayout.setWidget(7, QtGui.QFormLayout.FieldRole, self.port)
        self.verticalLayout.addLayout(self.formLayout)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName(_fromUtf8("horizontalLayout_5"))
        self.label_3 = QtGui.QLabel(TenantSwitch)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.horizontalLayout_5.addWidget(self.label_3)
        self.add = QtGui.QPushButton(TenantSwitch)
        self.add.setMaximumSize(QtCore.QSize(16, 16777215))
        self.add.setText(_fromUtf8(""))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/add.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.add.setIcon(icon)
        self.add.setFlat(True)
        self.add.setObjectName(_fromUtf8("add"))
        self.horizontalLayout_5.addWidget(self.add)
        self.remove = QtGui.QPushButton(TenantSwitch)
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
        self.params = QtGui.QTableWidget(TenantSwitch)
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
        self.OK = QtGui.QPushButton(TenantSwitch)
        self.OK.setObjectName(_fromUtf8("OK"))
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(TenantSwitch)
        self.Cancel.setObjectName(_fromUtf8("Cancel"))
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(TenantSwitch)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantSwitch.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantSwitch.reject)
        QtCore.QObject.connect(self.add, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantSwitch.AddParam)
        QtCore.QObject.connect(self.remove, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantSwitch.RemoveParam)
        QtCore.QObject.connect(self.serviceasuser, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantSwitch.ServiceChecked)
        QtCore.QObject.connect(self.provider, QtCore.SIGNAL(_fromUtf8("currentIndexChanged(int)")), TenantSwitch.ProviderChanged)
        QtCore.QObject.connect(self.servicename, QtCore.SIGNAL(_fromUtf8("currentIndexChanged(int)")), TenantSwitch.ServiceChanged)
        QtCore.QMetaObject.connectSlotsByName(TenantSwitch)

    def retranslateUi(self, TenantSwitch):
        TenantSwitch.setWindowTitle(_translate("TenantSwitch", "Edit Switch", None))
        self.namelabel.setText(_translate("TenantSwitch", "Name:", None))
        self.service.setText(_translate("TenantSwitch", "Service", None))
        self.label_4.setText(_translate("TenantSwitch", "Type:", None))
        self.type.setItemText(0, _translate("TenantSwitch", "Switch", None))
        self.type.setItemText(1, _translate("TenantSwitch", "Router", None))
        self.router.setText(_translate("TenantSwitch", "Router  IP:", None))
        self.serviceasuser.setText(_translate("TenantSwitch", "Service as User", None))
        self.label_5.setText(_translate("TenantSwitch", "Provider name:", None))
        self.label_2.setText(_translate("TenantSwitch", "Service name:", None))
        self.label_6.setText(_translate("TenantSwitch", "External port:", None))
        self.label_3.setText(_translate("TenantSwitch", "Parameters:", None))
        item = self.params.horizontalHeaderItem(0)
        item.setText(_translate("TenantSwitch", "Name", None))
        item = self.params.horizontalHeaderItem(1)
        item.setText(_translate("TenantSwitch", "Type", None))
        item = self.params.horizontalHeaderItem(2)
        item.setText(_translate("TenantSwitch", "Value", None))
        self.OK.setText(_translate("TenantSwitch", "OK", None))
        self.Cancel.setText(_translate("TenantSwitch", "Cancel", None))

from . import resources_rc
