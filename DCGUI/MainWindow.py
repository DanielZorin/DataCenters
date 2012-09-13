from PyQt4.QtGui import QMainWindow, qApp, QListWidgetItem
from PyQt4.QtCore import Qt
from DCGUI.Windows.ui_MainWindow import Ui_MainWindow
from DCGUI.GraphEditor import GraphEditor
from Core.Resources import ResourceGraph

class MainWindow(QMainWindow):
    project = None
    
    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.graphEditor = GraphEditor()
        #FIXME
        resources = ResourceGraph()
        self.graphEditor.setData(resources)


    def NewProject(self):
        pass
    
    def OpenProject(self):
        pass
        
    def OpenProjectFromFile(self, name):
        pass
    
    def SaveProject(self):
        pass
    
    def SaveProjectAs(self):
        pass

    def Run(self):
        pass

    def Settings(self):
        pass

    def EditProgram(self):
        self.graphEditor.show()
        while self.graphEditor.isVisible():
            qApp.processEvents()

    def AddDemand(self):
        it = QListWidgetItem("New demand", self.ui.demands)
        it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
        self.ui.demands.editItem(it)

    def DeleteDemand(self):
        pass

    def EditDemand(self):
        pass

    def RandomDemand(self):
        pass

    def About(self):
        pass

    def Exit(self):
        pass

