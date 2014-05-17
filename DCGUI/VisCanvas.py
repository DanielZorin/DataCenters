import math
from Core.Resources import Computer, Storage, Router, Link
from Core.Tenant import VM
from PyQt4 import QtGui, QtCore
from PyQt4.QtCore import QPointF, QRect, QString, pyqtSignal, Qt
from PyQt4.QtGui import QImage, QWidget, QPainter, QPainterPath, QColor, QCursor, QDialog, QIntValidator, QTableWidgetItem, QFont
    
class VisCanvas(QWidget):
    resources = None
    vertices = {}
    edges = {}
    tenantVertices = []
    selectedVertex = None
    pressed = False
    edgeDraw = False
    curEdge = None
    selectedEdge = None
    tenantEdges = []
    size = 25.0
    vertex_selected = pyqtSignal()
    edge_selected = pyqtSignal()

    settings = {
              "line": QColor(10, 34, 200),
              "selected_tenant": QColor(1, 200, 1),
              "selected": QColor(200, 1, 1),
              "text": QColor(0,0,0),
              "node": True,
              "computer": True,
              "storage": True,
              "router": True,
              "channel": True
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
        font = QFont()
        font.setPointSize(7)
        paint.setFont(font)
        for e in self.resources.edges:
                if e == self.selectedEdge:
                    paint.setPen(self.settings["selected"])
                elif self.tenantEdges.count(e):
                    paint.setPen(self.settings["selected_tenant"])
                else:
                    paint.setPen(self.settings["line"])
                x1 = self.vertices[e.e1].x() + self.size / 2
                y1 = self.vertices[e.e1].y() + self.size / 2
                x2 = self.vertices[e.e2].x() + self.size / 2
                y2 = self.vertices[e.e2].y() + self.size / 2
                self.drawArrow(paint, x1, y1, x2, y2)
                
        for v in self.vertices.keys():
            if self.tenantVertices.count(v) != 0:
                paint.fillRect(self.vertices[v],self.settings["selected_tenant"])
            if isinstance(v,Computer):
                if self.selectedVertex != self.vertices[v]:
                    paint.drawImage(self.vertices[v], self.computericon)
                else:
                    paint.drawImage(self.vertices[v], self.computerselectedicon)
                '''n = v.getUsedSpeedPercent(self.time)
                pen = paint.pen()
                if n > 90:
                    paint.setPen(Qt.red)
                elif n > 65:
                    paint.setPen(Qt.yellow)
                elif n > 0.5:
                    paint.setPen(Qt.green)
                else:
                    paint.setPen(Qt.blue)
                paint.drawRect(self.vertices[v])
                paint.setPen(pen)
                if self.settings["computer"]:
                    paint.setPen(self.settings["text"])
                    paint.drawText(self.vertices[v].x(), self.vertices[v].y()+self.size/2, str(int(v.getUsedSpeedPercent(self.time)))+"%")
                    paint.drawText(self.vertices[v].x(), self.vertices[v].y()+self.size/2+10, str(int(v.getUsedRamPercent(self.time)))+"%")'''
            elif isinstance(v,Storage):
                if self.selectedVertex != self.vertices[v]:
                    paint.drawImage(self.vertices[v], self.storageicon)
                else:
                    paint.drawImage(self.vertices[v], self.storageselectedicon)
                '''n = v.getUsedVolumePercent(self.time)
                pen = paint.pen()
                if n > 90:
                    paint.setPen(Qt.red)
                elif n > 65:
                    paint.setPen(Qt.yellow)
                elif n > 0.5:
                    paint.setPen(Qt.green)
                else:
                    paint.setPen(Qt.blue)
                paint.drawRect(self.vertices[v])
                paint.setPen(pen)
                if self.settings["storage"]:
                    paint.setPen(self.settings["text"])
                    paint.drawText(self.vertices[v].x(), self.vertices[v].y() + self.size/2, str(int(v.getUsedVolumePercent(self.time)))+"%")'''
            elif isinstance(v,Router):
                if self.selectedVertex != self.vertices[v]:
                    paint.drawImage(self.vertices[v], self.routericon)
                else:
                    paint.drawImage(self.vertices[v], self.routerselectedicon)
                '''if self.settings["router"]:
                    paint.setPen(self.settings["text"])
                    paint.drawText(self.vertices[v].x(), self.vertices[v].y() + self.size/2, str(int(v.getUsedCapacityPercent(self.time)))+"%")'''
        paint.setPen(self.settings["line"])
        if self.edgeDraw:
            self.drawArrow(paint, self.curEdge[0].x() + self.size / 2, self.curEdge[0].y() + self.size / 2,
                           QCursor.pos().x() - self.mapToGlobal(self.geometry().topLeft()).x(),
                           QCursor.pos().y() - self.mapToGlobal(self.geometry().topLeft()).y())
        '''if self.settings["channel"]:
            for e in self.resources.edges:
                x1 = self.vertices[e.e1].x() + self.size / 2
                y1 = self.vertices[e.e1].y() + self.size / 2
                x2 = self.vertices[e.e2].x() + self.size / 2
                y2 = self.vertices[e.e2].y() + self.size / 2
                paint.setPen(self.settings["text"])
                paint.drawText((x1+x2)/2, (y1+y2)/2, str(int(e.getUsedCapacityPercent(self.time)))+"%")'''
        paint.end()

    def Visualize(self, r):
        self.resources = r
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
                self.vertex_selected.emit()
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