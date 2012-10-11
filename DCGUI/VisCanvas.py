import math
from Core.Resources import Computer, Storage, Router, Link
from Core.Demands import VM
from PyQt4 import QtGui, QtCore
from PyQt4.QtCore import QPointF, QRect, QString, pyqtSignal
from PyQt4.QtGui import QImage, QWidget, QPainter, QPainterPath, QColor, QCursor, QDialog, QIntValidator, QTableWidgetItem
    
class VisCanvas(QWidget):
    resources = None
    vertices = {}
    edges = {}
    demandVertices = []
    selectedVertex = None
    pressed = False
    edgeDraw = False
    curEdge = None
    selectedEdge = None
    demandEdges = []
    size = 25.0
    router_selected = pyqtSignal()
    storage_selected = pyqtSignal()
    computer_selected = pyqtSignal()
    edge_selected = pyqtSignal()

    colors = {
              "line": QColor(10, 34, 200),
              "selected_demand": QColor(1, 200, 1),
              "selected": QColor(200, 1, 1),
              "text": QColor(0,0,0)
              }

    def __init__(self, parent=None):
        QWidget.__init__(self, parent)
        self.setSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        self.setFocusPolicy(QtCore.Qt.StrongFocus)
        self.computericon = QImage(":/pics/pics/computer.png")
        self.storageicon = QImage(":/pics/pics/storage.png")
        self.routericon = QImage(":/pics/pics/router.png")
        self.computerselectedicon = QImage(":/pics/pics/computer_selected.png")
        self.storageselectedicon = QImage(":/pics/pics/storage_selected.png")
        self.routerselectedicon = QImage(":/pics/pics/router_selected.png")
        
    def paintEvent(self, event):
        if not self.resources:
            return
        paint = QPainter(self)
        for e in self.resources.edges:
                if e == self.selectedEdge:
                    paint.setPen(self.colors["selected"])
                elif self.demandEdges.count(e):
                    paint.setPen(self.colors["selected_demand"])
                else:
                    paint.setPen(self.colors["line"])
                x1 = self.vertices[e.e1].x() + self.size / 2
                y1 = self.vertices[e.e1].y() + self.size / 2
                x2 = self.vertices[e.e2].x() + self.size / 2
                y2 = self.vertices[e.e2].y() + self.size / 2
                self.drawArrow(paint, x1, y1, x2, y2)
                paint.setPen(self.colors["text"])
                paint.drawText((x1+x2)/2, (y1+y2)/2, str(int(e.getUsedCapacityPercent(self.time)))+"%")
                
        for v in self.vertices.keys():
            if self.demandVertices.count(v) != 0:
                paint.fillRect(self.vertices[v],self.colors["selected_demand"])
            if isinstance(v,Computer):
                if self.selectedVertex != self.vertices[v]:
                    paint.drawImage(self.vertices[v], self.computericon)
                else:
                    paint.drawImage(self.vertices[v], self.computerselectedicon)
                paint.setPen(self.colors["text"])
                paint.drawText(self.vertices[v].x() + self.size, self.vertices[v].y() + self.size, str(int(v.getUsedSpeedPercent(self.time)))+"%")
            elif isinstance(v,Storage):
                if self.selectedVertex != self.vertices[v]:
                    paint.drawImage(self.vertices[v], self.storageicon)
                else:
                    paint.drawImage(self.vertices[v], self.storageselectedicon)
                paint.setPen(self.colors["text"])
                paint.drawText(self.vertices[v].x() + self.size, self.vertices[v].y() + self.size, str(int(v.getUsedVolumePercent(self.time)))+"%")
            elif isinstance(v,Router):
                if self.selectedVertex != self.vertices[v]:
                    paint.drawImage(self.vertices[v], self.routericon)
                else:
                    paint.drawImage(self.vertices[v], self.routerselectedicon)
                paint.setPen(self.colors["text"])
                paint.drawText(self.vertices[v].x() + self.size, self.vertices[v].y() + self.size, str(int(v.getUsedCapacityPercent(self.time)))+"%")
        paint.setPen(self.colors["line"])
        if self.edgeDraw:
            self.drawArrow(paint, self.curEdge[0].x() + self.size / 2, self.curEdge[0].y() + self.size / 2,
                           QCursor.pos().x() - self.mapToGlobal(self.geometry().topLeft()).x(),
                           QCursor.pos().y() - self.mapToGlobal(self.geometry().topLeft()).y())
        paint.end()

    def Visualize(self, r, time):
        self.resources = r
        self.time = time
        for v in self.resources.vertices:
            rect = QtCore.QRect(v.x - self.size / 2, v.y - self.size / 2, self.size, self.size)
            self.vertices[v] = rect
        self.ResizeCanvas()
        self.repaint()

    def ResizeCanvas(self):
        maxx = 0
        maxy = 0
        for r in self.vertices.values():
            if r.topRight().x() > maxx:
                maxx = r.topRight().x()
            if r.bottomRight().y() > maxy:
                maxy = r.bottomRight().y()
        self.setGeometry(0, 0, max(maxx + 10, self.parent().width()), max(maxy + 10, self.parent().height()))

    def drawArrow(self, paint, x1, y1, x2, y2):
        m = paint.worldMatrix()
        paint.translate(x1,y1)
        pi = 3.1415926
        if abs(x2 - x1) > 0:
            alpha = math.atan(abs(y2-y1)/abs(x2-x1)) * 180 / pi
        else:
            alpha = 90
        if y2 > y1:
            if x2 > x1:
                paint.rotate(alpha)
            else:
                paint.rotate(180-alpha)
        else:
            if x2 > x1:
                paint.rotate(-alpha)
            else:
                paint.rotate(alpha-180)
        endcoord = math.sqrt((x2-x1)**2 + (y2-y1)**2)
        p1 = QPointF(endcoord , 0)
        paint.drawLine(0, 0, p1.x(), 0)
        paint.setWorldMatrix(m)

    def mousePressEvent(self, e):
        for v in self.vertices.keys():
            if self.vertices[v].contains(e.pos()):
                self.selectedVertex = self.vertices[v]
                self.selectedEdge = None
                self.repaint()
                self.pressed = True
                if isinstance(v,Computer):
                    self.computer_selected.emit()
                elif isinstance(v,Storage):
                    self.storage_selected.emit()
                elif isinstance(v,Router):
                    self.router_selected.emit()
                return
        for ed in self.resources.edges:
            a = self.vertices[ed.e1].center()
            b = self.vertices[ed.e2].center()
            c = e.pos()
            ab = math.sqrt((a.x() - b.x())**2 + (a.y() - b.y())**2)
            inner = QtCore.QRect(a, b)
            if inner.contains(c):
                bc = math.sqrt((c.x() - b.x())**2 + (c.y() - b.y())**2)
                ac = math.sqrt((a.x() - c.x())**2 + (a.y() - c.y())**2)
                p = (ab + bc + ac) / 2.0
                area = math.sqrt(p * (p - ab) * (p - ac) * (p - bc))
                if area < 100:
                    self.selectedEdge = ed
                    self.selectedVertex = None
                    self.repaint()
                    self.edge_selected.emit()
                    return
        self.selectedEdge = None
        self.selectedVertex = None
        self.repaint()
        return

    def Clear(self):
        self.vertices = {}
        self.edges = {}
        self.selectedVertex = None
        self.pressed = False
        self.edgeDraw = False
        self.curEdge = None
        self.selectedEdge = None