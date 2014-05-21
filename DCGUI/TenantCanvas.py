import math, time
from Core.Tenant import *
from PyQt4 import QtGui, QtCore
from PyQt4.QtCore import QPointF, Qt
from PyQt4.QtGui import QImage, QWidget, QPainter, QColor, QCursor, QDialog
from DCGUI.VertexDialog import *



class State:
    ''' Enum representing current editing mode '''
    Select = 0
    VM = 1
    Storage = 2
    Switch = 3
    Vnf = 4
    Domain = 5
    Edge = 6

class TenantCanvas(QWidget):
    tenant = Tenant()
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
              "selected": QColor(1, 200, 1),
              }

    def __init__(self, parent=None):
        QWidget.__init__(self, parent)
        self.setSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        self.setFocusPolicy(QtCore.Qt.StrongFocus)
        self.computericon = QImage(":/pics/pics/computer.png")
        self.storageicon = QImage(":/pics/pics/storage.png")
        self.routericon = QImage(":/pics/pics/router.png")
        self.serviceicon = QImage(":/pics/pics/vnf.png")
        self.domainicon = QImage(":/pics/pics/topology.png")
        self.tenants = []
      
    def paintEvent(self, event):
        if not self.tenant:
            return
        paint = QPainter(self)
        for e in self.tenant.edges:
                if e == self.selectedEdge:
                    paint.setPen(self.colors["selected"])
                else:
                    paint.setPen(self.colors["line"])
                self.drawArrow(paint, self.vertices[e.e1].x() + self.size / 2, self.vertices[e.e1].y() + self.size / 2,
                             self.vertices[e.e2].x() + self.size / 2, self.vertices[e.e2].y() + self.size / 2)
        for v in self.vertices.keys():
            pen = paint.pen()
            paint.setPen(Qt.red)
            if self.selectedVertex == self.vertices[v]:
                paint.drawRect(self.vertices[v])
            if isinstance(v, VM):
                paint.drawImage(self.vertices[v], self.computericon)
            elif isinstance(v, Storage):
                paint.drawImage(self.vertices[v], self.storageicon)
            elif isinstance(v, NetElement):
                paint.drawImage(self.vertices[v], self.routericon)
            elif isinstance(v, Vnf):
                paint.drawImage(self.vertices[v], self.serviceicon)
            elif isinstance(v, Domain):
                paint.drawImage(self.vertices[v], self.domainicon)
        paint.setPen(self.colors["line"])
        if self.edgeDraw:
            self.drawArrow(paint, self.curEdge[0].x() + self.size / 2, self.curEdge[0].y() + self.size / 2,
                           QCursor.pos().x() - self.mapToGlobal(self.geometry().topLeft()).x(),
                           QCursor.pos().y() - self.mapToGlobal(self.geometry().topLeft()).y())
        paint.end()

    def Visualize(self, r):
        self.tenant = r
        for v in self.tenant.vertices:
            rect = QtCore.QRect(v.x - self.size / 2, v.y - self.size / 2, self.size, self.size)
            self.vertices[v] = rect
        self.ResizeCanvas()
        self.repaint()

    def Delete(self):
        if self.selectedVertex != None:
            v = next(v for v in self.vertices.keys() if self.vertices[v] == self.selectedVertex)
            del self.vertices[v]
            self.tenant.DeleteVertex(v)
            del self.selectedVertex
            self.selectedVertex = None
            self.changed = True
            self.repaint()
        elif self.selectedEdge != None:
            self.tenant.DeleteEdge(self.selectedEdge)
            self.selectedEdge = None
            self.changed = True
            self.repaint()

    def keyPressEvent(self, e):
        if e.key() == QtCore.Qt.Key_Return:
            if self.selectedVertex != None:
                v = next(v for v in self.vertices.keys() if self.vertices[v] == self.selectedVertex)
                self.EditVertex(v)
                self.repaint()
            elif self.selectedEdge != None:
                self.EditEdge(self.selectedEdge)
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
        if self.state == State.Select:
            for v in self.vertices.keys():
                if self.vertices[v].contains(e.pos()):
                    self.selectedVertex = self.vertices[v]
                    self.selectedEdge = None
                    self.repaint()
                    self.pressed = True
                    return
            for ed in self.tenant.edges:
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
                    if area < ab:
                        self.selectedEdge = ed
                        self.selectedVertex = None
                        self.repaint()
                        return
            self.selectedEdge = None
            self.selectedVertex = None
            self.repaint()
            return
        elif self.state == State.VM:
            rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            computer = VM(self.genId())
            self.vertices[computer] = rect
            self.tenant.AddVertex(computer)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Storage:
            rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            storage = Storage(self.genId())
            self.vertices[storage] = rect
            self.tenant.AddVertex(storage)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Switch:
            rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            router = NetElement(self.genId(), "Switch")
            self.vertices[router] = rect
            self.tenant.AddVertex(router)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Domain:
            rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            router = Domain(self.genId(), "")
            self.vertices[router] = rect
            self.tenant.AddVertex(router)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Vnf:
            rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            vnf = Vnf(self.genId())
            self.vertices[vnf] = rect
            self.tenant.AddVertex(vnf)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Edge:
            for v in self.vertices.keys():
                if self.vertices[v].contains(e.pos()):
                    self.edgeDraw = True
                    self.curEdge = []
                    self.curEdge.append(self.vertices[v])
                    self.curEdge.append(v)
                    self.repaint()

    def genId(self):
        return "id!" + str(time.time())

    def mouseMoveEvent(self, e):
        if self.state == State.Select:
            if self.pressed:
                self.selectedVertex.moveTo(e.pos().x() - self.size / 2, e.pos().y() - self.size / 2)
                self.ResizeCanvas()
                self.repaint()
        elif self.state == State.Edge:
            if self.edgeDraw:
                self.repaint()
        else:
            return

    def mouseReleaseEvent(self, e):
        self.pressed = False
        if self.edgeDraw:
            for v in self.vertices.keys():
                if self.vertices[v].contains(e.pos()):
                    ne = Link(self.curEdge[1], v, 0)
                    self.tenant.AddLink(ne)
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
        if isinstance(v, VM):
            d = VMDialog()
        elif isinstance(v, Storage):
            d = StorageDialog()
        elif isinstance(v, NetElement):
            d = SwitchDialog(self.tenant.name, self.tenants)
        elif isinstance(v, Vnf):
            d = VnfDialog()
        elif isinstance(v, Domain):
            d = DomainDialog()
        d.Load(v)
        d.exec_()
        if d.result() == QDialog.Accepted:
            d.SetResult(v)
        self.changed = True

    def Clear(self):
        self.vertices = {}
        self.edges = {}
        self.selectedVertex = None
        self.pressed = False
        self.edgeDraw = False
        self.curEdge = None
        self.selectedEdge = None

    def updatePos(self):
        for v in self.vertices.keys():
            rect = self.vertices[v]
            v.x = rect.x() + self.size/2
            v.y = rect.y() + self.size/2