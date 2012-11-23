
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_EdgeDialog(object):
    def setupUi(self, EdgeDialog):
        EdgeDialog.setObjectName(_fromUtf8("EdgeDialog"))
        EdgeDialog.resize(182, 92)
        EdgeDialog.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}a"))
        EdgeDialog.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_2 = QtGui.QVBoxLayout(EdgeDialog)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.capacitylabel = QtGui.QLabel(EdgeDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.capacitylabel.sizePolicy().hasHeightForWidth())
        self.capacitylabel.setSizePolicy(sizePolicy)
        self.capacitylabel.setObjectName(_fromUtf8("capacitylabel"))
        self.verticalLayout.addWidget(self.capacitylabel)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.capacity = QtGui.QLineEdit(EdgeDialog)
        self.capacity.setEnabled(True)
        self.capacity.setReadOnly(False)
        self.capacity.setObjectName(_fromUtf8("capacity"))
        self.horizontalLayout_3.addWidget(self.capacity)
        self.verticalLayout.addLayout(self.horizontalLayout_3)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.OK = QtGui.QPushButton(EdgeDialog)
        self.OK.setObjectName(_fromUtf8("OK"))
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(EdgeDialog)
        self.Cancel.setObjectName(_fromUtf8("Cancel"))
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(EdgeDialog)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL(_fromUtf8("clicked()")), EdgeDialog.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL(_fromUtf8("clicked()")), EdgeDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(EdgeDialog)

    def retranslateUi(self, EdgeDialog):
        EdgeDialog.setWindowTitle(QtGui.QApplication.translate("EdgeDialog", "Edit Channel", None, QtGui.QApplication.UnicodeUTF8))
        self.capacitylabel.setText(QtGui.QApplication.translate("EdgeDialog", "Bandwidth:", None, QtGui.QApplication.UnicodeUTF8))
        self.OK.setText(QtGui.QApplication.translate("EdgeDialog", "OK", None, QtGui.QApplication.UnicodeUTF8))
        self.Cancel.setText(QtGui.QApplication.translate("EdgeDialog", "Cancel", None, QtGui.QApplication.UnicodeUTF8))

