
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_DemandStorageDialog(object):
    def setupUi(self, DemandStorageDialog):
        DemandStorageDialog.setObjectName(_fromUtf8("DemandStorageDialog"))
        DemandStorageDialog.resize(231, 229)
        DemandStorageDialog.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}"))
        DemandStorageDialog.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_2 = QtGui.QVBoxLayout(DemandStorageDialog)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.namelabel = QtGui.QLabel(DemandStorageDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.namelabel.sizePolicy().hasHeightForWidth())
        self.namelabel.setSizePolicy(sizePolicy)
        self.namelabel.setObjectName(_fromUtf8("namelabel"))
        self.verticalLayout.addWidget(self.namelabel)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        self.id = QtGui.QLineEdit(DemandStorageDialog)
        self.id.setEnabled(True)
        self.id.setReadOnly(False)
        self.id.setObjectName(_fromUtf8("id"))
        self.horizontalLayout_2.addWidget(self.id)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.volumelabel = QtGui.QLabel(DemandStorageDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.volumelabel.sizePolicy().hasHeightForWidth())
        self.volumelabel.setSizePolicy(sizePolicy)
        self.volumelabel.setObjectName(_fromUtf8("volumelabel"))
        self.verticalLayout.addWidget(self.volumelabel)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.volume = QtGui.QLineEdit(DemandStorageDialog)
        self.volume.setEnabled(True)
        self.volume.setReadOnly(False)
        self.volume.setObjectName(_fromUtf8("volume"))
        self.horizontalLayout_3.addWidget(self.volume)
        self.verticalLayout.addLayout(self.horizontalLayout_3)
        self.typelabel = QtGui.QLabel(DemandStorageDialog)
        self.typelabel.setObjectName(_fromUtf8("typelabel"))
        self.verticalLayout.addWidget(self.typelabel)
        self.type = QtGui.QLineEdit(DemandStorageDialog)
        self.type.setObjectName(_fromUtf8("type"))
        self.verticalLayout.addWidget(self.type)
        self.label = QtGui.QLabel(DemandStorageDialog)
        self.label.setObjectName(_fromUtf8("label"))
        self.verticalLayout.addWidget(self.label)
        self.replcapacity = QtGui.QLineEdit(DemandStorageDialog)
        self.replcapacity.setObjectName(_fromUtf8("replcapacity"))
        self.verticalLayout.addWidget(self.replcapacity)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.OK = QtGui.QPushButton(DemandStorageDialog)
        self.OK.setObjectName(_fromUtf8("OK"))
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(DemandStorageDialog)
        self.Cancel.setObjectName(_fromUtf8("Cancel"))
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(DemandStorageDialog)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL(_fromUtf8("clicked()")), DemandStorageDialog.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL(_fromUtf8("clicked()")), DemandStorageDialog.reject)
        QtCore.QObject.connect(self.type, QtCore.SIGNAL(_fromUtf8("textChanged(QString)")), DemandStorageDialog.typeChanged)
        QtCore.QMetaObject.connectSlotsByName(DemandStorageDialog)

    def retranslateUi(self, DemandStorageDialog):
        DemandStorageDialog.setWindowTitle(QtGui.QApplication.translate("DemandStorageDialog", "Edit Storage", None, QtGui.QApplication.UnicodeUTF8))
        self.namelabel.setText(QtGui.QApplication.translate("DemandStorageDialog", "Name:", None, QtGui.QApplication.UnicodeUTF8))
        self.volumelabel.setText(QtGui.QApplication.translate("DemandStorageDialog", "Capacity:", None, QtGui.QApplication.UnicodeUTF8))
        self.typelabel.setText(QtGui.QApplication.translate("DemandStorageDialog", "Type:", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("DemandStorageDialog", "Consistency link bandwidth:", None, QtGui.QApplication.UnicodeUTF8))
        self.OK.setText(QtGui.QApplication.translate("DemandStorageDialog", "OK", None, QtGui.QApplication.UnicodeUTF8))
        self.Cancel.setText(QtGui.QApplication.translate("DemandStorageDialog", "Cancel", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
