
from PyQt4 import QtCore, QtGui

class Ui_StorageDialog(object):
    def setupUi(self, StorageDialog):
        StorageDialog.setObjectName("StorageDialog")
        StorageDialog.resize(231, 184)
        StorageDialog.setStyleSheet("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}")
        StorageDialog.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_2 = QtGui.QVBoxLayout(StorageDialog)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.verticalLayout.setObjectName("verticalLayout")
        self.namelabel = QtGui.QLabel(StorageDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.namelabel.sizePolicy().hasHeightForWidth())
        self.namelabel.setSizePolicy(sizePolicy)
        self.namelabel.setObjectName("namelabel")
        self.verticalLayout.addWidget(self.namelabel)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.id = QtGui.QLineEdit(StorageDialog)
        self.id.setEnabled(True)
        self.id.setReadOnly(False)
        self.id.setObjectName("id")
        self.horizontalLayout_2.addWidget(self.id)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.volumelabel = QtGui.QLabel(StorageDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.volumelabel.sizePolicy().hasHeightForWidth())
        self.volumelabel.setSizePolicy(sizePolicy)
        self.volumelabel.setObjectName("volumelabel")
        self.verticalLayout.addWidget(self.volumelabel)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.volume = QtGui.QLineEdit(StorageDialog)
        self.volume.setEnabled(True)
        self.volume.setReadOnly(False)
        self.volume.setObjectName("volume")
        self.horizontalLayout_3.addWidget(self.volume)
        self.verticalLayout.addLayout(self.horizontalLayout_3)
        self.typelabel = QtGui.QLabel(StorageDialog)
        self.typelabel.setObjectName("typelabel")
        self.verticalLayout.addWidget(self.typelabel)
        self.type = QtGui.QLineEdit(StorageDialog)
        self.type.setObjectName("type")
        self.verticalLayout.addWidget(self.type)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.OK = QtGui.QPushButton(StorageDialog)
        self.OK.setObjectName("OK")
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(StorageDialog)
        self.Cancel.setObjectName("Cancel")
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(StorageDialog)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL("clicked()"), StorageDialog.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL("clicked()"), StorageDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(StorageDialog)

    def retranslateUi(self, StorageDialog):
        StorageDialog.setWindowTitle(QtGui.QApplication.translate("StorageDialog", "Edit Storage", None, QtGui.QApplication.UnicodeUTF8))
        self.namelabel.setText(QtGui.QApplication.translate("StorageDialog", "Name:", None, QtGui.QApplication.UnicodeUTF8))
        self.volumelabel.setText(QtGui.QApplication.translate("StorageDialog", "Capacity:", None, QtGui.QApplication.UnicodeUTF8))
        self.typelabel.setText(QtGui.QApplication.translate("StorageDialog", "Type:", None, QtGui.QApplication.UnicodeUTF8))
        self.OK.setText(QtGui.QApplication.translate("StorageDialog", "OK", None, QtGui.QApplication.UnicodeUTF8))
        self.Cancel.setText(QtGui.QApplication.translate("StorageDialog", "Cancel", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
