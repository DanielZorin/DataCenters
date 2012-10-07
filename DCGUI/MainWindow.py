from PyQt4.QtGui import QMainWindow, qApp, QTreeWidgetItem, QDialog, QFileDialog, QMessageBox, QAction, QKeySequence
from PyQt4.QtCore import Qt, QObject, SIGNAL, QSettings, QStringList
from DCGUI.Windows.ui_MainWindow import Ui_MainWindow
from DCGUI.ResourcesGraphEditor import ResourcesGraphEditor
from DCGUI.DemandGraphEditor import DemandGraphEditor
from DCGUI.RandomDemandDialog import RandomDemandDialog
from DCGUI.Vis import Vis
from DCGUI.Project import Project
from Core.Resources import Storage
import os

class MainWindow(QMainWindow):
    project = None
    projectFile = None
    demands = {}

    MaxRecentFiles = 10
    ''' Limit on the number of items in recent files list'''

    recentFileActions = []
    ''' Recent files list'''

    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.settings = QSettings("LVK Inc", "DataCenters")   
        self.projFilter = self.tr("Data centers projects (*.dcxml)")
        self.resourcesGraphEditor = ResourcesGraphEditor()
        self.demandGraphEditor = DemandGraphEditor()
        self.Vis = Vis()
        self.project = Project()
        self.resourcesGraphEditor.setData(self.project.resources)
        for i in range(self.MaxRecentFiles):
            a = QAction(self)
            a.setVisible(False)
            a.setEnabled(False)
            if i <= 9:
                a.setShortcut(QKeySequence(self.tr("Alt+") + str(i + 1)))
            QObject.connect(a, SIGNAL("triggered()"), self.OpenRecentFile);
            self.ui.menuFile.insertAction(self.ui.actionExit, a)
            self.recentFileActions.append(a)
        self.UpdateRecentFileActions()
        self.basename = self.windowTitle()
        self.setWindowTitle("Untitled" + " - " + self.basename)
        self.demandGraphEditor.id_changed.connect(self.demandIdChanged)

    def NewProject(self):
        self.project = Project()
        self.resourcesGraphEditor.setData(self.project.resources)
        self.projectFile = None
        self.demands = {}
        self.ui.demands.clear()
        self.setWindowTitle("Untitled" + " - " + self.basename)
    
    def OpenProject(self):
        name = unicode(QFileDialog.getOpenFileName(filter=self.projFilter))
        if name == None or name == '':
            return
        self.OpenProjectFromFile(name)
        
    def OpenProjectFromFile(self, name):
        self.project = Project()
        
        #try:
        self.project.Load(name)
        #except :
            # TODO: proper exceptioning
        #    QMessageBox.critical(self, self.tr("An error occured"), self.tr("File is not a valid project file: ") + name)
        #    return
        self.projectFile = name
        self.resourcesGraphEditor.setData(self.project.resources)
        self.ui.demands.clear()
        for d in self.project.demands:
            it = QTreeWidgetItem(self.ui.demands, QStringList([d.id, str(d.startTime), str(d.endTime), "Yes" if d.assigned else "No"]))
            it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
            self.demands[it] = d
        self.UpdateRecentFiles()
        self.setWindowTitle(self.projectFile.split('/').pop().split('.')[0] + " - " + self.basename)

    def OpenRecentFile(self):
        ''' Opens a project from recent files list'''
        text = unicode(self.sender().data().toString())
        if os.path.exists(text):
            self.OpenProjectFromFile(text)
            self.UpdateRecentFiles()
        else:
            QMessageBox.critical(self, "Error", "Project not found")
            self.RemoveFromRecentFiles(text)
    
    def SaveProject(self):
        if self.projectFile == None:
            self.SaveProjectAs()
        else:
            self.project.Save(self.projectFile)
            self.UpdateRecentFiles()
    
    def SaveProjectAs(self):
        self.projectFile = unicode(QFileDialog.getSaveFileName(directory=self.project.name + ".dcxml", filter=self.projFilter))
        if self.projectFile != '':
            self.project.Save(self.projectFile)
            self.UpdateRecentFiles()
        self.setWindowTitle(self.projectFile.split('/').pop().split('.')[0] + " - " + self.basename)

    def Run(self):
        self.project.resources._buildPaths()
        self.project.method.Clear()
        self.project.method.Run()

    def Settings(self):
        pass

    def EditProgram(self):
        self.resourcesGraphEditor.show()
        while self.resourcesGraphEditor.isVisible():
            qApp.processEvents()

    def AddDemand(self):
        d = self.project.CreateDemand()
        it = QTreeWidgetItem(self.ui.demands, QStringList(["New demand", "0", "1", "No"]))
        it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
        self.demands[it] = d
        self.ui.demands.editItem(it)
        self.demands[it].id = unicode(it.text(0))
        self.demands[it].startTime = int(it.text(1))
        self.demands[it].endTime = int(it.text(2))

    def DeleteDemand(self):
        item = self.ui.demands.currentItem()
        if (item == None):
            return
        row = self.ui.demands.currentRow()
        self.project.RemoveDemand(self.demands[item])
        del self.demands[item]
        self.ui.demands.takeItem(row)

    def RenameDemand(self, item):
        if item in self.demands:
            self.demands[item].id = unicode(item.text())

    def EditDemand(self):
        if (self.demands == {}) or (self.ui.demands.currentItem() == None):
            return
        self.demandGraphEditor.setData(self.demands[self.ui.demands.currentItem()])
        self.demandGraphEditor.show()

    def RandomDemand(self):
        d = RandomDemandDialog()
        d.exec_()
        if d.result() == QDialog.Accepted: 
            dict = d.GetResult()
            types = []
            for v in self.project.resources.vertices:
                if isinstance(v,Storage) and (types.count(v.type)==0):
                    types.append(v.type)
            dict["types"] = types
            for i in range(dict["n"]):
                demand = self.project.CreateRandomDemand(dict)
                it = QTreeWidgetItem(self.ui.demands, QStringList([demand.id, str(demand.startTime), str(demand.endTime), "No"]))
                it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
                self.demands[it] = demand

    def About(self):
        pass

    def Exit(self):
        pass

    def RemoveFromRecentFiles(self, s):
        ''' Removes an item from recent files list'''
        files = self.settings.value("recentFileList").toStringList();
        files.removeAll(s);
        self.settings.setValue("recentFileList", files);
        self.UpdateRecentFileActions()

    def UpdateRecentFiles(self):
        ''' Updates the recent files list to keep the chronological order'''
        files = self.settings.value("recentFileList").toStringList();
        files.removeAll(self.projectFile);
        files.prepend(self.projectFile);
        while files.count() > self.MaxRecentFiles:
            files.removeLast()

        self.settings.setValue("recentFileList", files);
        self.UpdateRecentFileActions()

    def UpdateRecentFileActions(self):
        ''' Updates the list of QActions for recent files'''
        files = self.settings.value("recentFileList").toStringList();
        numRecentFiles = min(files.count(), self.MaxRecentFiles);

        for i in range(self.MaxRecentFiles):
            if i < numRecentFiles:
                text = self.tr("&%1: %2").arg(i + 1).arg(os.path.basename(unicode(files[i]))[:-6])
                self.recentFileActions[i].setText(text)
                self.recentFileActions[i].setData(files[i])
                self.recentFileActions[i].setVisible(True)
                self.recentFileActions[i].setEnabled(True)
            else:
                self.recentFileActions[i].setVisible(False)
                self.recentFileActions[i].setEnabled(False)

    def demandIdChanged(self):
        it = self.ui.demands.currentItem()
        it.setText(self.demands[it].id)

    def ShowResults(self):
        self.Vis.setData(self.project.resources)
        self.Vis.show()
