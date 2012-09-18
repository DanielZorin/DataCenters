from PyQt4.QtGui import QMainWindow, qApp, QListWidgetItem, QDialog
from PyQt4.QtCore import Qt
from DCGUI.Windows.ui_MainWindow import Ui_MainWindow
from DCGUI.ResourcesGraphEditor import ResourcesGraphEditor
from DCGUI.DemandGraphEditor import DemandGraphEditor
from DCGUI.RandomDemandDialog import RandomDemandDialog
from DCGUI.Project import Project

class MainWindow(QMainWindow):
    project = None
    demands = {}

    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.resourcesGraphEditor = ResourcesGraphEditor()
        self.demandGraphEditor = DemandGraphEditor()
        self.project = Project()
        self.resourcesGraphEditor.setData(self.project.resources)


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
        self.resourcesGraphEditor.show()
        while self.resourcesGraphEditor.isVisible():
            qApp.processEvents()

    def AddDemand(self):
        d = self.project.CreateDemand()
        it = QListWidgetItem("New demand", self.ui.demands)
        it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
        self.demands[it] = d
        self.ui.demands.editItem(it)

    def DeleteDemand(self):
        pass

    def RenameDemand(self, item):
        if item in self.demands:
            self.demands[item].id = str(item.text())

    def EditDemand(self):
        if (self.demands == {}) or (self.ui.demands.currentItem() == None):
            return
        self.demandGraphEditor.setData(self.demands[self.ui.demands.currentItem()])
        self.demandGraphEditor.show()
        while self.demandGraphEditor.isVisible():
            qApp.processEvents()

    def RandomDemand(self):
        d = RandomDemandDialog()
        d.exec_()
        if d.result() == QDialog.Accepted: 
            dict = d.GetResult()
            for i in range(dict["n"]):
                demand = self.project.CreateRandomDemand(dict)
                it = QListWidgetItem(demand.id, self.ui.demands)
                it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
                self.demands[it] = demand

    def About(self):
        pass

    def Exit(self):
        pass

