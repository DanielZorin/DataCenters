
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

class Ui_TenantEdge(object):
    def setupUi(self, TenantEdge):
        TenantEdge.setObjectName(_fromUtf8("TenantEdge"))
        TenantEdge.resize(324, 101)
        TenantEdge.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #a0a0a0, stop: 1 #f0f0f0);\n"
"}\n"
"\n"
"QLabel, QSlider, QCheckBox {\n"
"    background-color: transparent;\n"
"}"))
        TenantEdge.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_2 = QtGui.QVBoxLayout(TenantEdge)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.formLayout = QtGui.QFormLayout()
        self.formLayout.setFieldGrowthPolicy(QtGui.QFormLayout.AllNonFixedFieldsGrow)
        self.formLayout.setObjectName(_fromUtf8("formLayout"))
        self.service = QtGui.QCheckBox(TenantEdge)
        self.service.setObjectName(_fromUtf8("service"))
        self.formLayout.setWidget(1, QtGui.QFormLayout.FieldRole, self.service)
        self.label_4 = QtGui.QLabel(TenantEdge)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.formLayout.setWidget(2, QtGui.QFormLayout.LabelRole, self.label_4)
        self.capacity = QtGui.QLineEdit(TenantEdge)
        self.capacity.setObjectName(_fromUtf8("capacity"))
        self.formLayout.setWidget(2, QtGui.QFormLayout.FieldRole, self.capacity)
        self.verticalLayout.addLayout(self.formLayout)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.OK = QtGui.QPushButton(TenantEdge)
        self.OK.setObjectName(_fromUtf8("OK"))
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(TenantEdge)
        self.Cancel.setObjectName(_fromUtf8("Cancel"))
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(TenantEdge)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantEdge.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL(_fromUtf8("clicked()")), TenantEdge.reject)
        QtCore.QMetaObject.connectSlotsByName(TenantEdge)

    def retranslateUi(self, TenantEdge):
        TenantEdge.setWindowTitle(_translate("TenantEdge", "Edit Edge", None))
        self.service.setText(_translate("TenantEdge", "Service", None))
        self.label_4.setText(_translate("TenantEdge", "Capacity:", None))
        self.OK.setText(_translate("TenantEdge", "OK", None))
        self.Cancel.setText(_translate("TenantEdge", "Cancel", None))

from . import resources_rc
