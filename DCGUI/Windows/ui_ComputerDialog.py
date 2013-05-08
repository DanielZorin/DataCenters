
from PyQt4 import QtCore, QtGui

class Ui_ComputerDialog(object):
    def setupUi(self, ComputerDialog):
        ComputerDialog.setObjectName("ComputerDialog")
        ComputerDialog.resize(184, 184)
        ComputerDialog.setStyleSheet("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}")
        ComputerDialog.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_2 = QtGui.QVBoxLayout(ComputerDialog)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.verticalLayout.setObjectName("verticalLayout")
        self.namelabel = QtGui.QLabel(ComputerDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.namelabel.sizePolicy().hasHeightForWidth())
        self.namelabel.setSizePolicy(sizePolicy)
        self.namelabel.setObjectName("namelabel")
        self.verticalLayout.addWidget(self.namelabel)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.id = QtGui.QLineEdit(ComputerDialog)
        self.id.setEnabled(True)
        self.id.setReadOnly(False)
        self.id.setObjectName("id")
        self.horizontalLayout_2.addWidget(self.id)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.timelabel = QtGui.QLabel(ComputerDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.timelabel.sizePolicy().hasHeightForWidth())
        self.timelabel.setSizePolicy(sizePolicy)
        self.timelabel.setObjectName("timelabel")
        self.verticalLayout.addWidget(self.timelabel)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.speed = QtGui.QLineEdit(ComputerDialog)
        self.speed.setEnabled(True)
        self.speed.setReadOnly(False)
        self.speed.setObjectName("speed")
        self.horizontalLayout_3.addWidget(self.speed)
        self.verticalLayout.addLayout(self.horizontalLayout_3)
        self.label = QtGui.QLabel(ComputerDialog)
        self.label.setObjectName("label")
        self.verticalLayout.addWidget(self.label)
        self.ram = QtGui.QLineEdit(ComputerDialog)
        self.ram.setObjectName("ram")
        self.verticalLayout.addWidget(self.ram)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.OK = QtGui.QPushButton(ComputerDialog)
        self.OK.setObjectName("OK")
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(ComputerDialog)
        self.Cancel.setObjectName("Cancel")
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(ComputerDialog)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL("clicked()"), ComputerDialog.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL("clicked()"), ComputerDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(ComputerDialog)

    def retranslateUi(self, ComputerDialog):
        ComputerDialog.setWindowTitle(QtGui.QApplication.translate("ComputerDialog", "Edit Computational Node", None, QtGui.QApplication.UnicodeUTF8))
        self.namelabel.setText(QtGui.QApplication.translate("ComputerDialog", "Name:", None, QtGui.QApplication.UnicodeUTF8))
        self.timelabel.setText(QtGui.QApplication.translate("ComputerDialog", "Performance:", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("ComputerDialog", "RAM capacity:", None, QtGui.QApplication.UnicodeUTF8))
        self.OK.setText(QtGui.QApplication.translate("ComputerDialog", "OK", None, QtGui.QApplication.UnicodeUTF8))
        self.Cancel.setText(QtGui.QApplication.translate("ComputerDialog", "Cancel", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
