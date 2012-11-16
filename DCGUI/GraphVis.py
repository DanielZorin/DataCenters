from PyQt4.QtCore import QString, Qt
from PyQt4.QtGui import QMainWindow, QFont, QFileDialog, QTextEdit, QImage, QPainter, QPen, QColor, QTreeWidgetItem, QGraphicsScene
from DCGUI.Windows.ui_GraphVis import Ui_GraphVis
from Core.Demands import VM, DemandStorage
from Core.Resources import Computer, Storage, Router

class GraphVis(QMainWindow):
    scaleFactor = 1.0
    settings = {
              "axis": QColor(0, 0, 0),
              "graph": QColor(255, 0, 0),
              "net": True
              }

    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_GraphVis()
        self.ui.setupUi(self)

    def setData(self, project):
        self.project = project
        self.totalSpeed = 0.0;
        self.totalVolume = 0.0;
        self.totalCapacity = 0.0;
        for v in self.project.resources.vertices:
            if isinstance(v, Computer):
                self.totalSpeed += v.speed
            elif isinstance(v, Storage):
                self.totalVolume += v.volume
            elif isinstance(v, Router):
                self.totalCapacity += v.capacity
        for e in self.project.resources.edges:
            self.totalCapacity += e.capacity
        self.time = self.project.resources.GetTimeBounds()[1]
        self.avgSpeed = []
        self.avgVolume = []
        self.avgCapacity = []
        ranges = sorted(self.project.resources.vertices[0].intervals.keys(), key=lambda x:x[0])
        for r in ranges:
            usedSpeed = 0.0
            usedVolume = 0.0
            usedCapacity = 0.0
            for v in self.project.resources.vertices:
                if isinstance(v, Computer):
                    usedSpeed += v.intervals[r].usedResource
                elif isinstance(v, Storage):
                    usedVolume += v.intervals[r].usedResource
                elif isinstance(v, Router):
                    usedCapacity += v.intervals[r].usedResource
            for e in self.project.resources.edges:
                    usedCapacity += e.intervals[r].usedResource
            self.avgSpeed.append([r[0], usedSpeed / self.totalSpeed * 100])
            self.avgVolume.append([r[0], usedVolume / self.totalVolume * 100])
            self.avgCapacity.append([r[0], usedCapacity / self.totalCapacity * 100])
        self.avgSpeed.append([r[1], self.avgSpeed[-1][1]])
        self.avgVolume.append([r[1], self.avgVolume[-1][1]])
        self.avgCapacity.append([r[1], self.avgCapacity[-1][1]])
        self.Paint()

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

    def ScaleUp(self):
        self.ui.graph.scale(1.2, 1.2)

    def ScaleDown(self):
        self.ui.graph.scale(0.8, 0.8)

    def Paint(self):
        scene = QGraphicsScene()

        # Draw the graph
        tt = self.ui.graphtype.currentIndex()
        if tt == 0:
            points = self.avgSpeed
        elif tt == 1:
            points = self.avgVolume
        else:
            points = self.avgCapacity
        p0 = points[0]
        for p in points[1:]:
            scene.addLine(5 + p0[0] * 2, 205 - p0[1] * 2, 5 + p[0] * 2, 205 - p[1] * 2, QPen(QColor(255, 0, 0)))
            p0 = p

        # Draw the axis
        scene.addLine(5, 5, 5, 208, QPen(QColor(0, 0, 0)))
        scene.addLine(2, 205, self.time * 2 + 5, 205, QPen(QColor(0, 0, 0)))
        t = 0
        while t <= self.time:
            scene.addLine(5 + t * 2, 206, 5 + t * 2, 204)
            font = QFont()
            font.setPointSize(6)          
            capt = scene.addText(str(t), font)
            capt.setPos(t * 2, 203)
            t += 10
        self.ui.graph.setScene(scene)