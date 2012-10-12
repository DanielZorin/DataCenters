
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_RouterDialog(object):
    def setupUi(self, RouterDialog):
        RouterDialog.setObjectName(_fromUtf8("RouterDialog"))
        RouterDialog.resize(231, 145)
        RouterDialog.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}"))
        RouterDialog.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_2 = QtGui.QVBoxLayout(RouterDialog)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.namelabel = QtGui.QLabel(RouterDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.namelabel.sizePolicy().hasHeightForWidth())
        self.namelabel.setSizePolicy(sizePolicy)
        self.namelabel.setObjectName(_fromUtf8("namelabel"))
        self.verticalLayout.addWidget(self.namelabel)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        self.id = QtGui.QLineEdit(RouterDialog)
        self.id.setEnabled(True)
        self.id.setReadOnly(False)
        self.id.setObjectName(_fromUtf8("id"))
        self.horizontalLayout_2.addWidget(self.id)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.label = QtGui.QLabel(RouterDialog)
        self.label.setObjectName(_fromUtf8("label"))
        self.verticalLayout.addWidget(self.label)
        self.capacity = QtGui.QLineEdit(RouterDialog)
        self.capacity.setObjectName(_fromUtf8("capacity"))
        self.verticalLayout.addWidget(self.capacity)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.verticalLayout.addLayout(self.horizontalLayout_3)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.OK = QtGui.QPushButton(RouterDialog)
        self.OK.setObjectName(_fromUtf8("OK"))
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(RouterDialog)
        self.Cancel.setObjectName(_fromUtf8("Cancel"))
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(RouterDialog)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL(_fromUtf8("clicked()")), RouterDialog.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL(_fromUtf8("clicked()")), RouterDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(RouterDialog)

    def retranslateUi(self, RouterDialog):
        RouterDialog.setWindowTitle(QtGui.QApplication.translate("RouterDialog", "Edit Router", None, QtGui.QApplication.UnicodeUTF8))
        self.namelabel.setText(QtGui.QApplication.translate("RouterDialog", "Name:", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("RouterDialog", "Bandwidth:", None, QtGui.QApplication.UnicodeUTF8))
        self.OK.setText(QtGui.QApplication.translate("RouterDialog", "OK", None, QtGui.QApplication.UnicodeUTF8))
        self.Cancel.setText(QtGui.QApplication.translate("RouterDialog", "Cancel", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
