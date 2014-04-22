
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

class Ui_TopologyDialog(object):
    def setupUi(self, TopologyDialog):
        TopologyDialog.setObjectName(_fromUtf8("TopologyDialog"))
        TopologyDialog.resize(231, 131)
        TopologyDialog.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}"))
        TopologyDialog.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        TopologyDialog.setModal(False)
        self.verticalLayout = QtGui.QVBoxLayout(TopologyDialog)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.label = QtGui.QLabel(TopologyDialog)
        self.label.setObjectName(_fromUtf8("label"))
        self.verticalLayout.addWidget(self.label)
        self.common = QtGui.QRadioButton(TopologyDialog)
        self.common.setEnabled(True)
        self.common.setAutoFillBackground(False)
        self.common.setChecked(True)
        self.common.setObjectName(_fromUtf8("common"))
        self.verticalLayout.addWidget(self.common)
        self.tree2 = QtGui.QRadioButton(TopologyDialog)
        self.tree2.setEnabled(True)
        self.tree2.setAutoFillBackground(False)
        self.tree2.setChecked(False)
        self.tree2.setObjectName(_fromUtf8("tree2"))
        self.verticalLayout.addWidget(self.tree2)
        self.tree3 = QtGui.QRadioButton(TopologyDialog)
        self.tree3.setObjectName(_fromUtf8("tree3"))
        self.verticalLayout.addWidget(self.tree3)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.OK = QtGui.QPushButton(TopologyDialog)
        self.OK.setObjectName(_fromUtf8("OK"))
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(TopologyDialog)
        self.Cancel.setObjectName(_fromUtf8("Cancel"))
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)

        self.retranslateUi(TopologyDialog)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL(_fromUtf8("clicked()")), TopologyDialog.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL(_fromUtf8("clicked()")), TopologyDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(TopologyDialog)

    def retranslateUi(self, TopologyDialog):
        TopologyDialog.setWindowTitle(_translate("TopologyDialog", "Choose Topology", None))
        self.label.setText(_translate("TopologyDialog", "Choose topology:", None))
        self.common.setText(_translate("TopologyDialog", "Common DC topology", None))
        self.tree2.setText(_translate("TopologyDialog", "Tree-like (2 switch layers)", None))
        self.tree3.setText(_translate("TopologyDialog", "Tree-like (3 switch layers)", None))
        self.OK.setText(_translate("TopologyDialog", "OK", None))
        self.Cancel.setText(_translate("TopologyDialog", "Cancel", None))

from . import resources_rc
