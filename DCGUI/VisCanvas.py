import math
from Core.Resources import Computer, Storage, Router, Link
from PyQt4 import QtGui, QtCore
from PyQt4.QtCore import QPointF, QRect
from PyQt4.QtGui import QImage, QWidget, QPainter, QPainterPath, QColor, QCursor, QDialog, QIntValidator, QTableWidgetItem

class Vert:
    rect = QRect()
    type = 0

class VisCanvas(QWidget):
    resources = None
    vertices = {}
    edges = {}
    selectedVertex = None
    pressed = False
    edgeDraw = False
    curEdge = None
    selectedEdge = None
    size = 25.0

    colors = {
              "line": QColor(10, 34, 200),
              "selected": QColor(1, 200, 1),
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
                else:
                    paint.setPen(self.colors["line"])
                self.drawArrow(paint, self.vertices[e.e1].rect.x() + self.size / 2, self.vertices[e.e1].rect.y() + self.size / 2,
                             self.vertices[e.e2].rect.x() + self.size / 2, self.vertices[e.e2].rect.y() + self.size / 2)
        for task in self.vertices.values():
            if task.type == 0:
                if self.selectedVertex != task:
                    paint.drawImage(task.rect, self.computericon)
                else:
                    paint.drawImage(task.rect, self.computerselectedicon)
            elif task.type == 1:
                if self.selectedVertex != task:
                    paint.drawImage(task.rect, self.storageicon)
                else:
                    paint.drawImage(task.rect, self.storageselectedicon)
            elif task.type == 2:
                if self.selectedVertex != task:
                    paint.drawImage(task.rect, self.routericon)
                else:
                    paint.drawImage(task.rect, self.routerselectedicon)
        paint.setPen(self.colors["line"])
        if self.edgeDraw:
            self.drawArrow(paint, self.curEdge[0].rect.x() + self.size / 2, self.curEdge[0].rect.y() + self.size / 2,
                           QCursor.pos().x() - self.mapToGlobal(self.geometry().topLeft()).x(),
                           QCursor.pos().y() - self.mapToGlobal(self.geometry().topLeft()).y())
        paint.end()

    def Visualize(self, r):
        self.resources = r
        for v in self.resources.vertices:
            task = Vert()
            if isinstance(v, Computer):
                task.type = 0
            elif isinstance(v, Storage):
                task.type = 1
            elif isinstance(v, Router):
                task.type = 2
            task.rect = QtCore.QRect(v.x - self.size / 2, v.y - self.size / 2, self.size, self.size)
            self.vertices[v] = task
        self.ResizeCanvas()
        self.repaint()

    def ResizeCanvas(self):
        maxx = 0
        maxy = 0
        for r in self.vertices.values():
            if r.rect.topRight().x() > maxx:
                maxx = r.rect.topRight().x()
            if r.rect.bottomRight().y() > maxy:
                maxy = r.rect.bottomRight().y()
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
            if self.vertices[v].rect.contains(e.pos()):
                self.selectedVertex = self.vertices[v]
                self.selectedEdge = None
                self.repaint()
                self.pressed = True
                return
        for ed in self.resources.edges:
            a = self.vertices[ed.e1].rect.center()
            b = self.vertices[ed.e2].rect.center()
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