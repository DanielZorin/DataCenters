
from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_FilesGenerator(object):
    def setupUi(self, FilesGenerator):
        FilesGenerator.setObjectName(_fromUtf8("FilesGenerator"))
        FilesGenerator.resize(219, 179)
        FilesGenerator.setStyleSheet(_fromUtf8("QWidget, QMenuBar::item, QHeaderView::section {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #c5d8ef, stop: 1 #89a5c3);\n"
"}\n"
"\n"
"QLabel, QSlider {\n"
"    background-color: transparent;\n"
"}"))
        FilesGenerator.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout_2 = QtGui.QVBoxLayout(FilesGenerator)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetNoConstraint)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.namelabel = QtGui.QLabel(FilesGenerator)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.namelabel.sizePolicy().hasHeightForWidth())
        self.namelabel.setSizePolicy(sizePolicy)
        self.namelabel.setObjectName(_fromUtf8("namelabel"))
        self.verticalLayout.addWidget(self.namelabel)
        self.num = QtGui.QSpinBox(FilesGenerator)
        self.num.setMinimum(1)
        self.num.setObjectName(_fromUtf8("num"))
        self.verticalLayout.addWidget(self.num)
        self.volumelabel = QtGui.QLabel(FilesGenerator)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.volumelabel.sizePolicy().hasHeightForWidth())
        self.volumelabel.setSizePolicy(sizePolicy)
        self.volumelabel.setObjectName(_fromUtf8("volumelabel"))
        self.verticalLayout.addWidget(self.volumelabel)
        self.topologies = QtGui.QComboBox(FilesGenerator)
        self.topologies.setObjectName(_fromUtf8("topologies"))
        self.topologies.addItem(_fromUtf8(""))
        self.topologies.addItem(_fromUtf8(""))
        self.topologies.addItem(_fromUtf8(""))
        self.verticalLayout.addWidget(self.topologies)
        self.typelabel = QtGui.QLabel(FilesGenerator)
        self.typelabel.setObjectName(_fromUtf8("typelabel"))
        self.verticalLayout.addWidget(self.typelabel)
        self.generators = QtGui.QComboBox(FilesGenerator)
        self.generators.setObjectName(_fromUtf8("generators"))
        self.verticalLayout.addWidget(self.generators)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.OK = QtGui.QPushButton(FilesGenerator)
        self.OK.setObjectName(_fromUtf8("OK"))
        self.horizontalLayout.addWidget(self.OK)
        self.Cancel = QtGui.QPushButton(FilesGenerator)
        self.Cancel.setObjectName(_fromUtf8("Cancel"))
        self.horizontalLayout.addWidget(self.Cancel)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(FilesGenerator)
        QtCore.QObject.connect(self.OK, QtCore.SIGNAL(_fromUtf8("clicked()")), FilesGenerator.accept)
        QtCore.QObject.connect(self.Cancel, QtCore.SIGNAL(_fromUtf8("clicked()")), FilesGenerator.reject)
        QtCore.QMetaObject.connectSlotsByName(FilesGenerator)

    def retranslateUi(self, FilesGenerator):
        FilesGenerator.setWindowTitle(QtGui.QApplication.translate("FilesGenerator", "Generate Projects", None, QtGui.QApplication.UnicodeUTF8))
        self.namelabel.setText(QtGui.QApplication.translate("FilesGenerator", "Number of files:", None, QtGui.QApplication.UnicodeUTF8))
        self.volumelabel.setText(QtGui.QApplication.translate("FilesGenerator", "Topology:", None, QtGui.QApplication.UnicodeUTF8))
        self.topologies.setItemText(0, QtGui.QApplication.translate("FilesGenerator", "Common DC Topology", None, QtGui.QApplication.UnicodeUTF8))
        self.topologies.setItemText(1, QtGui.QApplication.translate("FilesGenerator", "Tree-like (2 switch layers)", None, QtGui.QApplication.UnicodeUTF8))
        self.topologies.setItemText(2, QtGui.QApplication.translate("FilesGenerator", "Tree-like (3 switch layers)", None, QtGui.QApplication.UnicodeUTF8))
        self.typelabel.setText(QtGui.QApplication.translate("FilesGenerator", "Generator:", None, QtGui.QApplication.UnicodeUTF8))
        self.OK.setText(QtGui.QApplication.translate("FilesGenerator", "OK", None, QtGui.QApplication.UnicodeUTF8))
        self.Cancel.setText(QtGui.QApplication.translate("FilesGenerator", "Cancel", None, QtGui.QApplication.UnicodeUTF8))

from . import resources_rc
