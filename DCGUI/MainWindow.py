from PyQt4.QtGui import QMainWindow, qApp, QTreeWidgetItem, QDialog, QFileDialog, QMessageBox, QActionGroup, QAction, QKeySequence, QLineEdit, QComboBox
from PyQt4.QtCore import Qt, QObject, SIGNAL, QSettings, QStringList, QTimer, QTranslator, QDir
from DCGUI.Windows.ui_MainWindow import Ui_MainWindow
from DCGUI.ResourcesGraphEditor import ResourcesGraphEditor
from DCGUI.DemandGraphEditor import DemandGraphEditor
from DCGUI.RandomDemandDialog import RandomDemandDialog
from DCGUI.Vis import Vis
from DCGUI.GraphVis import GraphVis
from DCGUI.Project import Project
from DCGUI.SettingsDialog import SettingsDialog
from DCGUI.ParamsDialog import ParamsDialog
from DCGUI.TestsWindow import TestsWindow 
from Core.Resources import Storage
import os, re, sys

class MainWindow(QMainWindow):
    project = None
    projectFile = None
    demands = {}
    generators = {}

    MaxRecentFiles = 10
    ''' Limit on the number of items in recent files list'''

    recentFileActions = []
    ''' Recent files list'''

    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.loadTranslations()
        self.settings = QSettings("LVK Inc", "DataCenters")   
        self.resourcesGraphEditor = ResourcesGraphEditor()
        self.demandGraphEditor = DemandGraphEditor()
        self.randomDialog = RandomDemandDialog()
        self.Vis = Vis()
        self.graphvis = GraphVis(self)
        self.demands = {}
        self.project = Project()
        # TODO: Captain, we have a problem!
        # For some reason, in Python 2.7 QSettings converts dicts to QVariant
        # So the ini file is undecypherable
        # This works fine in Python 3.2 by the way
        #if self.settings.value("vis"):
            #self.Vis.canvas.settings = self.settings.value("vis")
        #self.graphvis.settings = self.settings.value("graphVis")
        self.settingsDialog = SettingsDialog(self.Vis.canvas.settings, self.graphvis.settings)
        self.settingsDialog.ui.backup.setChecked(self.settings.value("backup").toBool())
        self.settingsDialog.ui.autosave.setChecked(self.settings.value("autosave").toBool())
        self.settingsDialog.ui.interval.setValue(self.settings.value("interval").toInt()[0])
        i = 0
        for s in self.languages:
            self.settingsDialog.ui.languages.addItem(s)
            if s == str(self.settings.value("language").toString()):
                self.settingsDialog.ui.languages.setCurrentIndex(i)
            i += 1
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
        self.demandGraphEditor.demand_changed.connect(self.demandChanged)
        self.backupTimer = QTimer()
        self.backupTimer.setInterval(60000)
        self.backupTimer.setSingleShot(False)
        QObject.connect(self.backupTimer, SIGNAL("timeout()"), self.Backup)
        self.autosaveTimer = QTimer()
        self.autosaveTimer.setInterval(60000)
        self.autosaveTimer.setSingleShot(False)
        QObject.connect(self.autosaveTimer, SIGNAL("timeout()"), self.Autosave)
        self.Translate(str(self.settings.value("language", "English").toString()))
        self.projFilter = self.tr("Data centers projects (*.dcxml)")
        self.setWindowTitle(self.tr("Untitled") + " - " + self.basename)
        self.ui.actionSchedule_selected.setVisible(False)
        self.ui.runsel.setHidden(True)
        self.loadPlugins()

    def NewProject(self):
        self.project = Project()
        self.resourcesGraphEditor.setData(self.project.resources)
        self.projectFile = None
        self.demands = {}
        self.ui.demands.clear()
        self.setWindowTitle(self.tr("Untitled") + " - " + self.basename)
        self.backupTimer.start()
        self.autosaveTimer.start()
    
    def OpenProject(self):
        name = unicode(QFileDialog.getOpenFileName(filter=self.projFilter))
        if name == None or name == '':
            return
        self.OpenProjectFromFile(name)
        
    def OpenProjectFromFile(self, name):
        self.demands = {}
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
            it = QTreeWidgetItem(self.ui.demands, QStringList([d.id, str(d.startTime), str(d.endTime), self.tr("No") if d.critical else self.tr("Yes"), self.tr("Yes") if d.assigned else self.tr("No")]))
            cb = QComboBox()
            cb.addItems([self.tr("No"),self.tr("Yes")])
            cb.setCurrentIndex(0 if d.critical else 1)
            QObject.connect(cb, SIGNAL("currentIndexChanged(int)"), it.emitDataChanged)
            self.ui.demands.setItemWidget(it,3,cb)
            it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
            self.demands[it] = d
        self.UpdateRecentFiles()
        self.setWindowTitle(self.project.name + " - " + self.basename)
        self.ui.projectname.setText(self.project.name)
        self.showStats()
        self.backupTimer.start()
        self.autosaveTimer.start()

    def OpenRecentFile(self):
        ''' Opens a project from recent files list'''
        text = unicode(self.sender().data().toString())
        if os.path.exists(text):
            self.OpenProjectFromFile(text)
            self.UpdateRecentFiles()
        else:
            QMessageBox.critical(self, self.tr("Error"), self.tr("Project not found"))
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

    def Backup(self):
        if self.settings.value("backup").toBool():
            self.project.Save(self.projectFile + ".bak")

    def Autosave(self):
        if self.settings.value("autosave").toBool():
            self.project.Save(self.projectFile)

    def InitProject(self):
        self.project.resources._buildPaths()
        self.project.method.demand_assigned.connect(self.demandAssigned)

    def Run(self):
        self.Reset()
        self.InitProject()
        #self.project.method.Clear()
        self.Reset()
        self.project.Save(self.projectFile)
        if self.ui.algorithm.currentIndex() == 0:
            alg = "a"
        elif self.ui.algorithm.currentIndex() == 1:
            alg = "c"
        else:
            alg = "d"
        if sys.platform.startswith("win"):
            name = "Algorithm/Algolib.exe"
        else:
            name = "Algorithm/Algolib"
        os.system(name + " \"" + os.path.relpath(self.projectFile) + "\" \"" + os.path.relpath(self.projectFile) + "\" " + alg)
        #self.project.Run()
        self.OpenProjectFromFile(self.projectFile)
        self.showStats()

    def RunMultipleTests(self):
        window = TestsWindow(self)
        window.show()
        while window.isVisible():
            qApp.processEvents()

    def RunSelected(self):
        self.InitProject()
        if self.ui.demands.selectedItems()==[]:
            return
        id = self.ui.demands.selectedItems()[0].text(0)
        d = self.project.FindDemand(id)
        self.project.method.AssignDemand(d)
        self.showStats()

    def showStats(self):
        if self.project.resources.vertices == []:
            return
        stats = self.project.GetStats()
        self.ui.demandcount.setText(str(stats["demands"]))
        self.ui.vmavg.setText(str(stats["vmavg"])+"%")
        self.ui.ramavg.setText(str(stats["ramavg"])+"%")
        self.ui.stavg.setText(str(stats["stavg"])+"%")
        self.ui.netavg.setText(str(stats["netavg"])+"%")
        self.ui.leafavg.setText(str(stats["leafavg"])+"%")
        self.ui.vmmax.setText(str(stats["vmmax"])+"%")
        self.ui.rammax.setText(str(stats["rammax"])+"%")
        self.ui.stmax.setText(str(stats["stmax"])+"%")
        self.ui.netmax.setText(str(stats["netmax"])+"%")
        self.ui.leafmax.setText(str(stats["leafmax"])+"%")

    def EditProgram(self):
        self.resourcesGraphEditor.show()
        while self.resourcesGraphEditor.isVisible():
            qApp.processEvents()

    def AddDemand(self):
        d = self.project.CreateDemand()
        it = QTreeWidgetItem(self.ui.demands, QStringList(["New_demand", "0", "1", self.tr("No"), self.tr("No")]))
        cb = QComboBox()
        cb.addItems([self.tr("No"),self.tr("Yes")])
        self.ui.demands.setItemWidget(it,3,cb)
        QObject.connect(cb, SIGNAL("currentIndexChanged(int)"), it.emitDataChanged)
        it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
        self.demands[it] = d
        self.ui.demands.editItem(it)
        self.demands[it].id = unicode(it.text(0))
        self.demands[it].startTime = int(it.text(1))
        self.demands[it].endTime = int(it.text(2))
        self.demands[it].critical = False if self.ui.demands.itemWidget(it,3).currentText() == self.tr("Yes") else True
    
    def DeleteDemand(self):
        item = self.ui.demands.currentItem()
        if (item == None):
            return
        self.project.RemoveDemand(self.demands[item])
        del self.demands[item]
        self.ui.demands.takeTopLevelItem(self.ui.demands.indexOfTopLevelItem(item))
        del item

    def UpdateDemand(self, item):
        if item in self.demands:
            self.demands[item].id = unicode(item.text(0))
            self.demands[item].startTime = int(item.text(1))
            self.demands[item].endTime = int(item.text(2))
            self.demands[item].critical = False if self.ui.demands.itemWidget(item,3).currentText() == self.tr("Yes") else True

    def EditDemand(self):
        if (self.demands == {}) or (self.ui.demands.currentItem() == None):
            return
        d = self.demands[self.ui.demands.currentItem()]
        if d.assigned:
            self.project.resources.DropDemand(d)
            
            #self.project.method.UpdateIntervals(d)
        self.demandGraphEditor.setData(d)
        self.demandGraphEditor.show()

    def RandomDemand(self):
        d = self.randomDialog
        types = []
        for v in self.project.resources.vertices:
            if isinstance(v,Storage) and (types.count(v.type)==0):
                types.append(v.type)
        if len(types) == 1: #only type 0
            d.ui.cc1.setEnabled(False)
            d.ui.cc2.setEnabled(False)
        d.exec_()
        if d.result() == QDialog.Accepted: 
            dict = d.GetResult()
            dict["types"] = types
            for i in range(dict["n"]):
                demand = self.project.CreateRandomDemand(dict)
                it = QTreeWidgetItem(self.ui.demands, QStringList([demand.id, str(demand.startTime), str(demand.endTime), self.tr("No"), self.tr("No")]))
                cb = QComboBox()
                cb.addItems([self.tr("No"),self.tr("Yes")])
                self.ui.demands.setItemWidget(it,3,cb)
                QObject.connect(cb, SIGNAL("currentIndexChanged(int)"), it.emitDataChanged)
                it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
                self.demands[it] = demand

    def Reset(self):
        self.project.Reset()
        for k in self.demands.keys():
            k.setText(4, self.tr("No"))
        self.showStats()

    def About(self):
        pass

    def Exit(self):
        pass

    def EditName(self):
        self.lineedit = QLineEdit(self.ui.projectname.parentWidget())
        self.lineedit.setGeometry(self.ui.projectname.geometry())
        self.lineedit.setText(self.ui.projectname.text())
        self.lineedit.setFocus()
        self.lineedit.show()
        self.ui.projectname.hide()
        # TODO: what's wrong?
        #self.ui.editname.hide()
        QObject.connect(self.lineedit, SIGNAL("editingFinished()"), self.ChangeName)

    def ChangeName(self):
        s = self.lineedit.text()
        self.ui.projectname.setText(s)
        self.ui.projectname.show()
        #self.ui.editname.show()
        self.lineedit.hide() 
        self.project.name = s
        self.setWindowTitle(self.project.name + " - " + self.tr("Data Centers GUI")) 

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
            files.removeAt(files.count()-1)

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

    def demandChanged(self):
        it = self.ui.demands.currentItem()
        it.setText(0, self.demands[it].id)
        it.setText(1, str(self.demands[it].startTime))
        it.setText(2, str(self.demands[it].endTime))
        it.setText(4, self.tr("No"))
        self.showStats()

    def ShowResults(self):
        self.Vis.setData(self.project)
        self.Vis.show()

    def ShowGraphVis(self):
        self.graphvis.setData(self.project)
        self.graphvis.show()

    def demandAssigned(self, id):
        item = self.ui.demands.findItems(id, Qt.MatchExactly)[0]
        item.setText(4, "Yes")

    def Settings(self):
        self.settingsDialog.exec_()
        if self.settingsDialog.result() == QDialog.Accepted:
            self.settings.setValue("vis", self.Vis.canvas.settings)  
            self.settings.setValue("graphVis", self.graphvis.settings)
            self.settings.setValue("backup", self.settingsDialog.ui.backup.isChecked())
            self.settings.setValue("autosave", self.settingsDialog.ui.autosave.isChecked())
            self.settings.setValue("interval", self.settingsDialog.ui.interval.value())
            self.autosaveTimer.setInterval(self.settings.value("interval").toInt()[0] * 1000)
            newlang = self.settingsDialog.ui.languages.currentText()
            if newlang != self.settings.value("language"):
                self.Translate(newlang)
                self.settings.setValue("language", newlang)

    def Translate(self, lang):
        translator = QTranslator(qApp)
        translator.load(":Translations/dc_" + lang + ".qm")
        qApp.installTranslator(translator)
        self.basename = self.tr("Data Centers GUI")
        self.demandGraphEditor.basename = self.demandGraphEditor.tr("Request Graph Editor")
        self.resourcesGraphEditor.basename = self.resourcesGraphEditor.tr("Resources Graph Editor")
        self.ui.retranslateUi(self)
        self.settingsDialog.ui.retranslateUi(self.settingsDialog)
        self.demandGraphEditor.ui.retranslateUi(self.demandGraphEditor)
        self.resourcesGraphEditor.ui.retranslateUi(self.resourcesGraphEditor)
        self.randomDialog.ui.retranslateUi(self.randomDialog)
        self.Vis.ui.retranslateUi(self.Vis)
        self.graphvis.ui.retranslateUi(self.graphvis)
        self.showStats()
        for k in self.demands.keys():
            cb = QComboBox()
            cb.addItems([self.tr("No"),self.tr("Yes")])
            cb.setCurrentIndex(0 if self.demands[k].critical else 1)
            QObject.connect(cb, SIGNAL("currentIndexChanged(int)"), k.emitDataChanged)
            self.ui.demands.setItemWidget(k,3,cb)
            if self.demands[k].assigned:
                k.setText(4, self.tr("Yes"))
            else:
                k.setText(4, self.tr("No"))

    def loadTranslations(self):
        all = QDir(":Translations").entryList()
        tsfile = re.compile("dc_([a-zA-z]*)\.qm")
        res = []
        for s in all:
            m = tsfile.match(s)
            if m != None:
                res.append(m.group(1))
        self.languages = res

    def loadPlugins(self):
        sys.path.append(os.curdir + os.sep + "plugins")
        plugins = QActionGroup(self)
        for s in os.listdir("plugins"):
            # TODO: check all errors
            if s.endswith(".py"):
                plugin = __import__(s[:-3])
                if plugin == "__init__":
                    continue
                if "pluginMain" in dir(plugin):
                    pluginClass = plugin.pluginMain()
                    name = pluginClass.GetName()
                    action = QAction(name, self)
                    action.setCheckable(False)
                    QObject.connect(action, SIGNAL("triggered()"), self.GenerateRequests)
                    plugins.addAction(action)
                    self.ui.menuGenerators.addAction(action)
                    self.generators[action] = pluginClass()
                else:
                    print("pluginMain not found in " + s)

    def GenerateRequests(self):
        generator = self.generators[self.sender()]
        data = generator.GetSettings()
        d = ParamsDialog(data, self, generator.GetName()=="Tightly coupled")
        d.exec_()
        if d.result() == QDialog.Accepted:
            generator.UpdateSettings(d.data)
            #TODO: populate the table with new demands
            self.project.demands = generator.Generate(self.project.resources)
            self.ui.demands.clear()
            self.demands = {}
            for demand in self.project.demands:
                it = QTreeWidgetItem(self.ui.demands, QStringList([demand.id, str(demand.startTime), str(demand.endTime), self.tr("No"), self.tr("No")]))
                cb = QComboBox()
                cb.addItems([self.tr("No"),self.tr("Yes")])
                self.ui.demands.setItemWidget(it,3,cb)
                QObject.connect(cb, SIGNAL("currentIndexChanged(int)"), it.emitDataChanged)
                it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
                self.demands[it] = demand
