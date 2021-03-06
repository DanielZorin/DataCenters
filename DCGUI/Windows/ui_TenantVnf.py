
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

class Ui_TenantVnf(object):
    def setupUi(self, TenantVnf):
        TenantVnf.setObjectName(_fromUtf8("TenantVnf"))
        TenantVnf.resize(324, 451)
        TenantVnf.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #a0a0a0, stop: 1 #f0f0f0);\n"
"}\n"
"\n"
"QLabel, QSlider, QCheckBox, QRadioButton {\n"
"    background-color: transparent;\n"
"}"))
        TenantVnf.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_2 = QtGui.QVBoxLayout(TenantVnf)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.formLayout = QtGui.QFormLayout()
        self.formLayout.setFieldGrowthPolicy(QtGui.QFormLayout.AllNonFixedFieldsGrow)
        self.formLayout.setObjectName(_fromUtf8("formLayout"))
        self.label_7 = QtGui.QLabel(TenantVnf)
        self.label_7.setObjectName(_fromUtf8("label_7"))
        self.formLayout.setWidget(0, QtGui.QFormLayout.LabelRole, self.label_7)
        self.name = QtGui.QLineEdit(TenantVnf)
        self.name.setObjectName(_fromUtf8("name"))
        self.formLayout.setWidget(0, QtGui.QFormLayout.FieldRole, self.name)
        self.service = QtGui.QCheckBox(TenantVnf)
        self.service.setObjectName(_fromUtf8("service"))
        self.formLayout.setWidget(1, QtGui.QFormLayout.FieldRole, self.service)
        self.type = QtGui.QLineEdit(TenantVnf)
        self.type.setObjectName(_fromUtf8("type"))
        self.formLayout.setWidget(3, QtGui.QFormLayout.FieldRole, self.type)
        self.profile = QtGui.QLineEdit(TenantVnf)
        self.profile.setObjectName(_fromUtf8("profile"))
        self.formLayout.setWidget(4, QtGui.QFormLayout.FieldRole, self.profile)
        self.servicename = QtGui.QLineEdit(TenantVnf)
        self.servicename.setObjectName(_fromUtf8("servicename"))
        self.formLayout.setWidget(6, QtGui.QFormLayout.FieldRole, self.servicename)
        self.label_11 = QtGui.QLabel(TenantVnf)
        self.label_11.setObjectName(_fromUtf8("label_11"))
        self.formLayout.setWidget(7, QtGui.QFormLayout.LabelRole, self.label_11)
        self.set = QtGui.QLineEdit(TenantVnf)
        self.set.setObjectName(_fromUtf8("set"))
        self.formLayout.setWidget(7, QtGui.QFormLayout.FieldRole, self.set)
        self.label_10 = QtGui.QLabel(TenantVnf)
        self.label_10.setObjectName(_fromUtf8("label_10"))
        self.formLayout.setWidget(8, QtGui.QFormLayout.LabelRole, self.label_10)
        self.username = QtGui.QLineEdit(TenantVnf)
        self.username.setObjectName(_fromUtf8("username"))
        self.formLayout.setWidget(8, QtGui.QFormLayout.FieldRole, self.username)
        self.label_8 = QtGui.QLabel(TenantVnf)
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.formLayout.setWidget(6, QtGui.QFormLayout.LabelRole, self.label_8)
        self.label_12 = QtGui.QLabel(TenantVnf)
        self.label_12.setObjectName(_fromUtf8("label_12"))
        self.formLayout.setWidget(4, QtGui.QFormLayout.LabelRole, self.label_12)
        self.label_9 = QtGui.QLabel(TenantVnf)
        self.label_9.setObjectName(_fromUtf8("label_9"))
        self.formLayout.setWidget(3, QtGui.QFormLayout.LabelRole, self.label_9)
        self.serviceasprovider = QtGui.QCheckBox(TenantVnf)
        self.serviceasprovider.setObjectName(_fromUtf8("serviceasprovider"))
        self.formLayout.setWidget(5, QtGui.QFormLayout.FieldRole, self.serviceasprovider)
        self.image = QtGui.QComboBox(TenantVnf)
        self.image.setObjectName(_fromUtf8("image"))
        self.formLayout.setWidget(2, QtGui.QFormLayout.FieldRole, self.image)
        self.label = QtGui.QLabel(TenantVnf)
        self.label.setObjectName(_fromUtf8("label"))
        self.formLayout.setWidget(2, QtGui.QFormLayout.LabelRole, self.label)
        self.verticalLayout.addLayout(self.formLayout)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName(_fromUtf8("horizontalLayout_5"))
        self.label_3 = QtGui.QLabel(TenantVnf)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.horizontalLayout_5.addWidget(self.label_3)
        self.add = QtGui.QPushButton(TenantVnf)
        self.add.setMaximumSize(QtCore.QSize(16, 16777215))
        self.add.setText(_fromUtf8(""))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/pics/pics/add.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.add.setIcon(icon)
        self.add.setFlat(True)
        self.add.setObjectName(_fromUtf8("add"))
        self.horizontalLayout_5.addWidget(self.add)
        self.remove = QtGui.QPushButton(TenantVnf)
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
        self.params = QtGui.QTableWidget(TenantVnf)
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
        self.OK = QtGui.QPushButton(TenantVnf)
        self.OK.setObjectName(_fromUtf8("OK"))
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(TenantVnf)
        self.Cancel.setObjectName(_fromUtf8("Cancel"))
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(TenantVnf)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantVnf.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantVnf.reject)
        QtCore.QObject.connect(self.add, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantVnf.AddParam)
        QtCore.QObject.connect(self.remove, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantVnf.RemoveParam)
        QtCore.QObject.connect(self.serviceasprovider, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantVnf.ServiceChecked)
        QtCore.QMetaObject.connectSlotsByName(TenantVnf)

    def retranslateUi(self, TenantVnf):
        TenantVnf.setWindowTitle(_translate("TenantVnf", "Edit Vnf", None))
        self.label_7.setText(_translate("TenantVnf", "Name:", None))
        self.service.setText(_translate("TenantVnf", "Service", None))
        self.label_11.setText(_translate("TenantVnf", "Exported connection set:", None))
        self.label_10.setText(_translate("TenantVnf", "User name:", None))
        self.label_8.setText(_translate("TenantVnf", "Service Name:", None))
        self.label_12.setText(_translate("TenantVnf", "Profile Type:", None))
        self.label_9.setText(_translate("TenantVnf", "Type:", None))
        self.serviceasprovider.setText(_translate("TenantVnf", "Service as Provider", None))
        self.label.setText(_translate("TenantVnf", "Image ID:", None))
        self.label_3.setText(_translate("TenantVnf", "Parameters:", None))
        item = self.params.horizontalHeaderItem(0)
        item.setText(_translate("TenantVnf", "Name", None))
        item = self.params.horizontalHeaderItem(1)
        item.setText(_translate("TenantVnf", "Type", None))
        item = self.params.horizontalHeaderItem(2)
        item.setText(_translate("TenantVnf", "Value", None))
        self.OK.setText(_translate("TenantVnf", "OK", None))
        self.Cancel.setText(_translate("TenantVnf", "Cancel", None))

from . import resources_rc
