import math
from Core.Resources import Computer, Storage, Router, Link
from PyQt4 import QtGui, QtCore
from PyQt4.QtCore import QPointF, QRect
from PyQt4.QtGui import QImage, QWidget, QPainter, QPainterPath, QColor, QCursor, QDialog, QIntValidator, QTableWidgetItem
from DCGUI.Windows.ui_ComputerDialog import Ui_ComputerDialog
from DCGUI.Windows.ui_RouterDialog import Ui_RouterDialog
from DCGUI.Windows.ui_EdgeDialog import Ui_EdgeDialog
from DCGUI.Windows.ui_StorageDialog import Ui_StorageDialog

class Vert:
    rect = QRect()
    type = 0

class ComputerDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)
        self.ui = Ui_ComputerDialog()
        self.ui.setupUi(self)
        self.valid = QIntValidator(0, 1000000, self)
        self.ui.speed.setValidator(self.valid)
        
    def Load(self, v):
        self.ui.id.setText(v.id)
        self.ui.speed.setText(str(v.speed))
        
    def SetResult(self, v):
        v.id = self.ui.id.text()
        v.speed = int(self.ui.speed.text())

class StorageDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)
        self.ui = Ui_StorageDialog()
        self.ui.setupUi(self)
        self.valid = QIntValidator(0, 1000000, self)
        self.ui.volume.setValidator(self.valid)
        
    def Load(self, v):
        self.ui.id.setText(v.id)
        self.ui.volume.setText(str(v.volume))
        
    def SetResult(self, v):
        v.id = self.ui.id.text()
        v.volume = int(self.ui.volume.text())

class RouterDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)
        self.ui = Ui_RouterDialog()
        self.ui.setupUi(self)
        
    def Load(self, v):
        self.ui.id.setText(v.id)
        
    def SetResult(self, v):
        v.id = self.ui.id.text()
        

class EdgeDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)
        self.ui = Ui_EdgeDialog()
        self.ui.setupUi(self)
        #self.valid = QIntValidator(0, 1000000, self)
        #self.ui.volume.setValidator(self.valid)

    def Load(self, e):
        #self.ui.name.setText(e.name)
        #self.ui.volume.setText(str(e.volume))
        pass

    def SetResult(self, e):
        #e.name = self.ui.name.text()
        #e.volume = int(self.ui.volume.text())
        pass

class State:
    ''' Enum representing current editing mode '''
    Select = 0
    Computer = 1
    Storage = 2
    Router = 3
    Edge = 4

class VMDialog(QDialog):
    pass

class GraphCanvas(QWidget):
    resources = None
    vertices = {}
    edges = {}
    selectedVertex = None
    pressed = False
    edgeDraw = False
    curEdge = None
    selectedEdge = None
    state = State.Select
    size = 25.0

    colors = {
              "line": QColor(10, 34, 200),
              "vertex": QColor(123, 34, 100),
              "selected": QColor(1, 200, 1),
              "text": QColor(0, 0, 0)
              }

    def __init__(self, parent=None):
        QWidget.__init__(self, parent)
        self.setSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        self.setFocusPolicy(QtCore.Qt.StrongFocus)
        self.computericon = QImage(":/pics/pics/computer.png")
        self.storageicon = QImage(":/pics/pics/storage.png")
        self.routericon = QImage(":/pics/pics/router.png")
        
    def paintEvent(self, event):
        if not self.resources:
            return
        paint = QPainter(self)
        paint.setPen(self.colors["line"])
        for e in self.resources.edges:
                self.drawArrow(paint, self.vertices[e.e1].rect.x() + self.size / 2, self.vertices[e.e1].rect.y() + self.size / 2,
                             self.vertices[e.e2].rect.x() + self.size / 2, self.vertices[e.e2].rect.y() + self.size / 2)
        for task in self.vertices.values():
            if task.type == 0:
                paint.drawImage(task.rect, self.computericon)
            elif task.type == 1:
                paint.drawImage(task.rect, self.storageicon)
            elif task.type == 2:
                paint.drawImage(task.rect, self.routericon)

        paint.setPen(self.colors["line"])
        if self.edgeDraw:
            self.drawArrow(paint, self.curEdge[0].rect.x() + self.size / 2, self.curEdge[0].rect.y() + self.size / 2,
                           QCursor.pos().x() - self.mapToGlobal(self.geometry().topLeft()).x(),
                           QCursor.pos().y() - self.mapToGlobal(self.geometry().topLeft()).y())
        paint.end()

    def Visualize(self, r):
        self.resources = r
        x = 50
        y = 50
        maxi = 40 * int(math.sqrt(len(self.resources.vertices)))
        maxx = 0
        for v in self.resources.vertices:
            task = Vert()
            if isinstance(v, Computer):
                task.type = 0
            elif isinstance(v, Storage):
                task.type = 1
            elif isinstance(v, Router):
                task.type = 2
            task.rect = QtCore.QRect(x - self.size / 2, y - self.size / 2, self.size, self.size)
            if x < maxi:
                x += 40
                maxx = x
            else:
                y += 40
                x = 50
            self.vertices[v] = task
        self.ResizeCanvas()
        self.repaint()

    def keyPressEvent(self, e):
        pass

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
        endcoord = math.sqrt((x2-x1)**2 + (y2-y1)**2) - self.size / 2
        p1 = QPointF(endcoord , 0)
        paint.drawLine(0, 0, p1.x(), 0)
        
        coord = math.sqrt(9**2 - 6**2)
        p2 = QPointF(endcoord - coord, 6)
        p3 = QPointF(endcoord - coord, -6)
        path = QPainterPath()
        path.moveTo(p1)
        path.lineTo(p2)
        path.lineTo(p3)
        path.lineTo(p1)
        paint.fillPath(path, paint.pen().color())
        paint.setWorldMatrix(m)

    def mousePressEvent(self, e):
        if self.state == State.Select:
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
        elif self.state == State.Computer:
            task = Vert()
            task.rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            task.type = 0
            computer = Computer("id", 1)
            self.vertices[computer] = task
            self.resources.AddVertex(computer)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Storage:
            task = Vert()
            task.rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            task.type = 1
            storage = Storage("id", 1)
            self.vertices[storage] = task
            self.resources.AddVertex(storage)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Router:
            task = Vert()
            task.rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            task.type = 2
            router = Router("id")
            self.vertices[router] = task
            self.resources.AddVertex(router)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Edge:
            for v in self.vertices.keys():
                if self.vertices[v].rect.contains(e.pos()):
                    self.edgeDraw = True
                    self.curEdge = []
                    self.curEdge.append(self.vertices[v])
                    self.curEdge.append(v)
                    self.repaint()

    def mouseMoveEvent(self, e):
        if (self.state == State.Computer) or (self.state == State.Storage) or (self.state == State.Router):
            return
        elif self.state == State.Select:
            if self.pressed:
                self.selectedVertex.rect.moveTo(e.pos().x() - self.size / 2, e.pos().y() - self.size / 2)
                self.ResizeCanvas()
                self.repaint()
        elif self.state == State.Edge:
            if self.edgeDraw:
                self.repaint()

    def mouseReleaseEvent(self, e):
        self.pressed = False
        if self.edgeDraw:
            for v in self.vertices.keys():
                if self.vertices[v].rect.contains(e.pos()):
                    ne = Link(self.curEdge[1], v)
                    self.resources.AddLink(ne)
            self.edgeDraw = False
            self.curEdge = None 
            self.changed = True    
            self.repaint()

    def mouseDoubleClickEvent(self, e):
        if self.selectedVertex != None:
            v = next(v for v in self.vertices.keys() if self.vertices[v] == self.selectedVertex)
            self.EditVertex(v)
            self.repaint()
        elif self.selectedEdge != None:
            self.EditEdge(self.selectedEdge)
            self.repaint()

    def EditEdge(self, e):
        d = EdgeDialog()
        d.Load(e)
        d.exec_()
        if d.result() == QDialog.Accepted:
            d.SetResult(e)
            self.changed = True

    def EditVertex(self, v):
        if self.vertices[v].type == 0:
            d = ComputerDialog()
            d.Load(v)
            d.exec_()
            if d.result() == QDialog.Accepted:
                d.SetResult(v)
        elif self.vertices[v].type == 1:
            d = StorageDialog()
            d.Load(v)
            d.exec_()
            if d.result() == QDialog.Accepted:
                d.SetResult(v)
        elif self.vertices[v].type == 2:
            d = RouterDialog()
            d.Load(v)
            d.exec_()
            if d.result() == QDialog.Accepted:
                d.SetResult(v)
        self.changed = True