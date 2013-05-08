
from PyQt4 import QtCore, QtGui

class Ui_TopologyDialog(object):
    def setupUi(self, TopologyDialog):
        TopologyDialog.setObjectName("TopologyDialog")
        TopologyDialog.resize(231, 131)
        TopologyDialog.setStyleSheet("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}")
        TopologyDialog.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        TopologyDialog.setModal(False)
        self.verticalLayout = QtGui.QVBoxLayout(TopologyDialog)
        self.verticalLayout.setObjectName("verticalLayout")
        self.label = QtGui.QLabel(TopologyDialog)
        self.label.setObjectName("label")
        self.verticalLayout.addWidget(self.label)
        self.common = QtGui.QRadioButton(TopologyDialog)
        self.common.setEnabled(True)
        self.common.setAutoFillBackground(False)
        self.common.setChecked(True)
        self.common.setObjectName("common")
        self.verticalLayout.addWidget(self.common)
        self.tree2 = QtGui.QRadioButton(TopologyDialog)
        self.tree2.setEnabled(True)
        self.tree2.setAutoFillBackground(False)
        self.tree2.setChecked(False)
        self.tree2.setObjectName("tree2")
        self.verticalLayout.addWidget(self.tree2)
        self.tree3 = QtGui.QRadioButton(TopologyDialog)
        self.tree3.setObjectName("tree3")
        self.verticalLayout.addWidget(self.tree3)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.OK = QtGui.QPushButton(TopologyDialog)
        self.OK.setObjectName("OK")
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(TopologyDialog)
        self.Cancel.setObjectName("Cancel")
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)

        self.retranslateUi(TopologyDialog)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL("clicked()"), TopologyDialog.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL("clicked()"), TopologyDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(TopologyDialog)

    def retranslateUi(self, TopologyDialog):
        TopologyDialog.setWindowTitle(QtGui.QApplication.translate("TopologyDialog", "Choose Topology", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("TopologyDialog", "Choose topology:", None, QtGui.QApplication.UnicodeUTF8))
        self.common.setText(QtGui.QApplication.translate("TopologyDialog", "Common DC topology", None, QtGui.QApplication.UnicodeUTF8))
        self.tree2.setText(QtGui.QApplication.translate("TopologyDialog", "Tree-like (2 switch layers)", None, QtGui.QApplication.UnicodeUTF8))
        self.tree3.setText(QtGui.QApplication.translate("TopologyDialog", "Tree-like (3 switch layers)", None, QtGui.QApplication.UnicodeUTF8))
        self.OK.setText(QtGui.QApplication.translate("TopologyDialog", "OK", None, QtGui.QApplication.UnicodeUTF8))
        self.Cancel.setText(QtGui.QApplication.translate("TopologyDialog", "Cancel", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
