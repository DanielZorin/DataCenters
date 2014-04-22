
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

class Ui_TenantSettings(object):
    def setupUi(self, TenantSettings):
        TenantSettings.setObjectName(_fromUtf8("TenantSettings"))
        TenantSettings.resize(324, 231)
        TenantSettings.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #a0a0a0, stop: 1 #f0f0f0);\n"
"}\n"
"\n"
"QLabel, QSlider, QCheckBox {\n"
"    background-color: transparent;\n"
"}"))
        TenantSettings.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_2 = QtGui.QVBoxLayout(TenantSettings)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.formLayout = QtGui.QFormLayout()
        self.formLayout.setObjectName(_fromUtf8("formLayout"))
        self.timelabel = QtGui.QLabel(TenantSettings)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.timelabel.sizePolicy().hasHeightForWidth())
        self.timelabel.setSizePolicy(sizePolicy)
        self.timelabel.setObjectName(_fromUtf8("timelabel"))
        self.formLayout.setWidget(2, QtGui.QFormLayout.LabelRole, self.timelabel)
        self.type = QtGui.QLineEdit(TenantSettings)
        self.type.setEnabled(True)
        self.type.setReadOnly(False)
        self.type.setObjectName(_fromUtf8("type"))
        self.formLayout.setWidget(2, QtGui.QFormLayout.FieldRole, self.type)
        self.namelabel = QtGui.QLabel(TenantSettings)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.namelabel.sizePolicy().hasHeightForWidth())
        self.namelabel.setSizePolicy(sizePolicy)
        self.namelabel.setObjectName(_fromUtf8("namelabel"))
        self.formLayout.setWidget(1, QtGui.QFormLayout.LabelRole, self.namelabel)
        self.name = QtGui.QLineEdit(TenantSettings)
        self.name.setEnabled(True)
        self.name.setReadOnly(False)
        self.name.setObjectName(_fromUtf8("name"))
        self.formLayout.setWidget(1, QtGui.QFormLayout.FieldRole, self.name)
        self.label = QtGui.QLabel(TenantSettings)
        self.label.setObjectName(_fromUtf8("label"))
        self.formLayout.setWidget(3, QtGui.QFormLayout.LabelRole, self.label)
        self.created = QtGui.QLineEdit(TenantSettings)
        self.created.setObjectName(_fromUtf8("created"))
        self.formLayout.setWidget(3, QtGui.QFormLayout.FieldRole, self.created)
        self.label_2 = QtGui.QLabel(TenantSettings)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.formLayout.setWidget(4, QtGui.QFormLayout.LabelRole, self.label_2)
        self.updated = QtGui.QLineEdit(TenantSettings)
        self.updated.setObjectName(_fromUtf8("updated"))
        self.formLayout.setWidget(4, QtGui.QFormLayout.FieldRole, self.updated)
        self.label_4 = QtGui.QLabel(TenantSettings)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.formLayout.setWidget(5, QtGui.QFormLayout.LabelRole, self.label_4)
        self.deleted = QtGui.QLineEdit(TenantSettings)
        self.deleted.setObjectName(_fromUtf8("deleted"))
        self.formLayout.setWidget(5, QtGui.QFormLayout.FieldRole, self.deleted)
        self.delbox = QtGui.QCheckBox(TenantSettings)
        self.delbox.setStyleSheet(_fromUtf8(""))
        self.delbox.setObjectName(_fromUtf8("delbox"))
        self.formLayout.setWidget(7, QtGui.QFormLayout.LabelRole, self.delbox)
        self.expiration = QtGui.QLineEdit(TenantSettings)
        self.expiration.setObjectName(_fromUtf8("expiration"))
        self.formLayout.setWidget(6, QtGui.QFormLayout.FieldRole, self.expiration)
        self.label_3 = QtGui.QLabel(TenantSettings)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.formLayout.setWidget(6, QtGui.QFormLayout.LabelRole, self.label_3)
        self.verticalLayout.addLayout(self.formLayout)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.OK = QtGui.QPushButton(TenantSettings)
        self.OK.setObjectName(_fromUtf8("OK"))
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(TenantSettings)
        self.Cancel.setObjectName(_fromUtf8("Cancel"))
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(TenantSettings)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantSettings.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantSettings.reject)
        QtCore.QMetaObject.connectSlotsByName(TenantSettings)

    def retranslateUi(self, TenantSettings):
        TenantSettings.setWindowTitle(_translate("TenantSettings", "Tenant Settings", None))
        self.timelabel.setText(_translate("TenantSettings", "Type:", None))
        self.namelabel.setText(_translate("TenantSettings", "Name:", None))
        self.label.setText(_translate("TenantSettings", "Created:", None))
        self.label_2.setText(_translate("TenantSettings", "Updated:", None))
        self.label_4.setText(_translate("TenantSettings", "Deleted:", None))
        self.delbox.setText(_translate("TenantSettings", "Deleted", None))
        self.label_3.setText(_translate("TenantSettings", "Expiration time:", None))
        self.OK.setText(_translate("TenantSettings", "OK", None))
        self.Cancel.setText(_translate("TenantSettings", "Cancel", None))

from . import resources_rc
