from PyQt4.QtCore import Qt
from PyQt4.QtGui import QMainWindow, QDialog, QFileDialog, QTreeWidgetItem, QGraphicsScene, QColor, QPen, QFont, QImage, QPainter
from DCGUI.Windows.ui_TestsWindow import Ui_TestsWindow
from DCGUI.Windows.ui_FilesGenerator import Ui_FilesGenerator
from DCGUI.Project import Project
from DCGUI.TreeDialog import TreeDialog
from DCGUI.ParamsDialog import ParamsDialog
from Core.Demands import DemandStorage, VM
from Core.Resources import Computer, Storage, Router, ResourceGraph
import os, sys

class FilesGenerator(QDialog):
    def __init__(self):
        QDialog.__init__(self)
        self.ui = Ui_FilesGenerator()
        self.ui.setupUi(self)

class TestsWindow(QMainWindow):
    settings = {"axis": QColor(0, 0, 0),
              "graph": QColor(255, 0, 0)}

    def __init__(self, parent):
        QMainWindow.__init__(self, parent)
        self.ui = Ui_TestsWindow()
        self.ui.setupUi(self)
        self.projects = {}
        self.changed = True

    def Generate(self):
        d = FilesGenerator()
        for generator in self.parent().generators.values():
            d.ui.generators.addItem(generator.GetName())
        d.exec_()
        if not d.result():
            return
        resources = ResourceGraph()
        if not d.result():
            return
        if d.ui.topologies.currentIndex()==0:
            d1 = TreeDialog(1)
        elif d.ui.topologies.currentIndex()==1:
            d1 = TreeDialog(2)
        elif d.ui.topologies.currentIndex()==2:
            d1 = TreeDialog(3)
        d1.exec_()
        if not d1.result():
            return
        dict = d1.GetResult()
        if dict["type"]==1:
            resources.GenerateCommonStructure(dict)
        elif dict["type"]==2:
            resources.GenerateTree2(dict)
        elif dict["type"]==3:
            resources.GenerateTree3(dict)
        generator = self.parent().generators.values()[d.ui.generators.currentIndex()]
        data = generator.GetSettings()
        d2 = ParamsDialog(data, self)
        d2.exec_()
        if d2.result() == QDialog.Accepted:
            generator.UpdateSettings(d2.data)
        for i in range(int(d.ui.num.text())):
            project = Project()
            project.demands = generator.Generate(resources)
            project.resources = resources
            name = "Project" + str(i) + ".dcxml"
            project.Save(name)
            it = QTreeWidgetItem(self.ui.projects, [name])
            self.projects[it] = name
        self.changed = True

    def Add(self):
        names = QFileDialog.getOpenFileNames(filter="*.dcxml")
        if names.isEmpty():
            return
        for name in names:
            it = QTreeWidgetItem(self.ui.projects, [os.path.splitext(os.path.split(str(name))[1])[0]])
            self.projects[it]=str(name)
        self.changed = True

    def Remove(self):
        item = self.ui.projects.currentItem()
        if (item == None):
            return
        del self.projects[item]
        self.ui.projects.takeTopLevelItem(self.ui.projects.indexOfTopLevelItem(item))
        del item
        self.changed = True

    def Run(self):
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
        for p in self.projects.values():
            os.system(name + " \"" + os.path.relpath(p) + "\" \"" + os.path.relpath(p) + "\" " + alg)
        self.changed = True

    def ChangeTab(self):
        if self.ui.tests_tabs.currentIndex() == 1 and self.changed:
            self.getStatistics()
            self.Paint()
            self.changed = False

    def getStatistics(self):
        self.stats = {}
        for name in self.projects.values():
            self.stats[name] = {"assigned":0,
                                "replicas":0,
                                "computersload":0,
                                "storesload":0}
            p = Project()
            p.Load(name,light=True)
            self.stats[name]["assigned"] = 0
            requiredspeed = 0
            requiredvolume = 0
            requiredram = 0
            totalspeed = 0
            totalvolume = 0
            totalram = 0
            for d in p.demands:
                if d.assigned:
                    self.stats[name]["assigned"] += 1
                    self.stats[name]["replicas"] += len(d.replications)
                for v in d.vertices:
                    if isinstance(v, DemandStorage):
                        requiredvolume += v.volume
                    elif isinstance(v, VM):
                        requiredspeed += v.speed
                        requiredram += v.ram
            for v in p.resources.vertices:
                if isinstance(v, Storage):
                    totalvolume += v.volume
                elif isinstance(v, Computer):
                    totalspeed += v.speed
            self.stats[name]["computersload"] = 0 if totalspeed == 0 else float(requiredspeed)/totalspeed
            self.stats[name]["ramload"] = 0 if totalram == 0 else float(requiredram)/totalram
            self.stats[name]["storesload"] = 0 if totalvolume == 0 else float(requiredvolume)/totalvolume

    def Paint(self):
        scene = QGraphicsScene()
        hor = self.ui.horizontal.currentIndex()
        vert = self.ui.vertical.currentIndex()
        v = []
        h = []
        projects = []
        if hor == 0:
            projects = sorted(self.stats.values(),key=lambda x: 1.0 / 3.0 * (x["computersload"] + x["storesload"] + x["ramload"]))
        elif hor == 1:
            projects = sorted(self.stats.values(),key=lambda x: 0.5*(x["computersload"]+x["ramload"]))
        elif hor == 2:
            projects = sorted(self.stats.values(),key=lambda x: x["computersload"])
        elif hor == 3:
            projects = sorted(self.stats.values(),key=lambda x: x["ramload"])
        elif hor == 4:
            projects = sorted(self.stats.values(),key=lambda x: x["storesload"])
        if projects == []:
            return
        for proj in projects:
            if vert == 0:
                v.append(proj["assigned"])
            elif vert == 1:
                v.append(proj["replicas"])
            if hor == 0:
                h.append(1.0 / 3.0 * (proj["computersload"] + proj["storesload"] + proj["ramload"]))
            elif hor == 1:
                h.append(0.5 * (proj["computersload"] + proj["ramload"]))
            elif hor == 2:
                h.append(proj["computersload"])
            elif hor == 3:
                h.append(proj["ramload"])
            elif hor == 4:
                h.append(proj["storesload"])

        maxnum = max(max(v), 1)
        scene.addLine(5, 5, 5, 213, QPen(self.settings["axis"]))
        scene.addLine(2, 210, 210, 210, QPen(self.settings["axis"]))
        for i in range(10):
            scene.addLine(5 + (i + 1) * 20, 209, 5 + (i + 1) * 20, 211, QPen(self.settings["axis"]))
            scene.addLine(4, 210 - (i + 1) * 20, 6, 210 - (i + 1) * 20, QPen(self.settings["axis"]))
            font = QFont()
            font.setPointSize(6)       
            t1 = scene.addText(str(0.1*(i + 1)), font)
            t1.setPos((i + 1) * 20, 212)
            if int(0.1*maxnum*(i + 1)) != 0:
                t2 = scene.addText(str(int(0.1*maxnum*(i + 1))), font)
                t2.setPos(-10, 200 - (i + 1) * 20)

        x0 = h[0]
        y0 = v[0]
        for x,y in zip(h,v):
            scene.addLine(5 + x0 * 200, 210 - float(y0)/maxnum * 200, 5 + x * 200, 210 - float(y)/maxnum * 200, QPen(self.settings["graph"]))
            scene.addLine(5 + x * 200 - 2, 210 - float(y)/maxnum * 200 - 2 , 5 + x * 200 + 2, 210 - float(y)/maxnum * 200 + 2, QPen(self.settings["graph"]))
            scene.addLine(5 + x * 200 + 2, 210 - float(y)/maxnum * 200 - 2, 5 + x * 200 - 2, 210 - float(y)/maxnum * 200 + 2, QPen(self.settings["graph"]))
            x0 = x
            y0 = y
        self.ui.graph.setScene(scene)

    def ScaleUp(self):
        self.ui.graph.scale(1.2, 1.2)

    def ScaleDown(self):
        self.ui.graph.scale(0.8, 0.8)

    def Replot(self, i):
        self.Paint()

    def Save(self):
        fileName = unicode(QFileDialog.getSaveFileName(directory="graph.png", filter="*.png"))
        if fileName == '':
            return
        scene = self.ui.graph.scene()
        scene.clearSelection()
        scene.setSceneRect(scene.itemsBoundingRect())
        img = QImage(scene.sceneRect().size().toSize(), QImage.Format_ARGB32)
        img.fill(Qt.transparent)
        ptr = QPainter(img)
        self.ui.graph.scene().render(ptr)
        ptr.end()
        img.save(fileName)
