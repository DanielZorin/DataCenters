from PyQt4.QtCore import QString, Qt
from PyQt4.QtGui import QMainWindow, QFileDialog, QTextEdit, QTreeWidgetItem
from DCGUI.Windows.ui_GraphVis import Ui_GraphVis
from Core.Demands import VM, DemandStorage
from Core.Resources import Computer, Storage, Router

class GraphVis(QMainWindow):

    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_GraphVis()
        self.ui.setupUi(self)

    def setData(self, project):
        self.project = project
        self.Paint()

    def Paint(self):
        pass