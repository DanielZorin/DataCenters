import math
from Core.Tenant import *
from Core.AbstractGraph import Param
from PyQt4 import QtGui, QtCore
from PyQt4.QtCore import QPointF, QRect, Qt
from PyQt4.QtGui import QImage, QWidget, QPainter, QPainterPath, QBrush, QColor, QCursor, QDialog, QIntValidator, QTableWidgetItem
from DCGUI.Windows.ui_TenantVM import Ui_TenantVM
from DCGUI.Windows.ui_TenantStorage import Ui_TenantStorage
from DCGUI.Windows.ui_TenantSwitch import Ui_TenantSwitch
from DCGUI.Windows.ui_TenantVnf import Ui_TenantVnf
from DCGUI.Windows.ui_TenantDomain import Ui_TenantDomain
from DCGUI.Windows.ui_TenantEdge import Ui_TenantEdge

class VertexDialog(QDialog):
    def __init__(self, ui):
        QDialog.__init__(self)        
        self.ui = ui
        self.ui.setupUi(self)
        self.ui.params.verticalHeader().hide()
        self.ui.params.horizontalHeader().setStretchLastSection(True)

    def LoadCommon(self, v):
        self.ui.name.setText(v.id)
        self.ui.service.setChecked(v.service)
        for p in v.params:
            self.ui.params.insertRow(0)
            self.ui.params.setItem(0, 0, QTableWidgetItem(p.name))
            self.ui.params.setItem(0, 1, QTableWidgetItem(p.type))
            self.ui.params.setItem(0, 2, QTableWidgetItem(str(p.value)))
        height = 0
        for i in range(self.ui.params.rowCount()):
            height += self.ui.params.rowHeight(i)
        # Dirty resizing to make the table visible
        self.resize(self.width(), self.height() + height - self.ui.params.height())

    def AddParam(self):
        self.ui.params.insertRow(0)
        self.ui.params.setItem(0, 0, QTableWidgetItem("param"))
        self.ui.params.setItem(0, 1, QTableWidgetItem("int"))
        self.ui.params.setItem(0, 2, QTableWidgetItem(str(1)))

    def RemoveParam(self):
        self.ui.params.removeRow(self.ui.params.currentRow())

    def SetResultCommon(self, v):
        v.id = str(self.ui.name.text())
        v.service = self.ui.service.isChecked()
        v.params = []
        for i in range(self.ui.params.rowCount()):
            p = Param(str(self.ui.params.item(i, 0).text()), 
                      str(self.ui.params.item(i, 1).text()), 
                      str(self.ui.params.item(i, 2).text()))
            v.params.append(p)
        
class VMDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_TenantVM())
        
    def Load(self, v):
        self.LoadCommon(v)
        self.ui.image.setText(v.image)

    def SetResult(self, v):
        self.SetResultCommon(v)
        v.image = str(self.ui.image.text())
        
class StorageDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_TenantStorage())
        
    def Load(self, v):
        self.LoadCommon(v)

    def SetResult(self, v):
        self.SetResultCommon(v)

class DomainDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_TenantDomain())
        
    def Load(self, v):
        self.LoadCommon(v)
        self.ui.type.setText(v.type)

    def SetResult(self, v):
        self.SetResultCommon(v)
        v.type = str(self.ui.type.text())

class SwitchDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_TenantSwitch())
        
    def Load(self, v):
        self.LoadCommon(v)
        self.ui.type.setCurrentIndex(0 if v.type == "Switch" else 1)
        self.ui.ip.setText(v.ip)
        self.ui.router.setChecked(v.router)
        self.ui.serviceasuser.setChecked(v.isservice)
        self.ui.provider.setText(v.provider)
        self.ui.servicename.setText(v.servicename)
        self.ui.port.setText(v.port)
        

    def SetResult(self, v):
        self.SetResultCommon(v)
        v.type = str(self.ui.type.currentText())
        v.router = self.ui.router.isChecked()
        v.ip = str(self.ui.ip.text())
        v.servicename = str(self.ui.servicename.currentText())
        v.provider = str(self.ui.provider.currentText())
        v.port = str(self.ui.port.currentText())
        v.isService = self.ui.serviceasuser.isChecked()

    def ServiceChecked(self):
        pass

class VnfDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_TenantVnf())

    def Load(self, v):
        self.LoadCommon(v)        
        self.ui.type.setText(v.type)
        self.ui.profile.setText(v.profile)
        self.ui.serviceasprovider.setChecked(v.isservice)
        self.ui.servicename.setText(v.servicename)
        self.ui.username.setText(v.username)
        self.ui.set.setText(','.join(v.connectionset))

    def SetResult(self, v):
        self.SetResultCommon(v)
        v.type = str(self.ui.type.text())
        v.profile = str(self.ui.profile.text())
        v.isService = self.ui.serviceasprovider.isChecked()
        v.username = str(self.ui.username.text())
        v.servicename = str(self.ui.servicename.text())
        v.connectionset = str(self.ui.set.text()).split(",")
        
    def ServiceChecked(self):
        pass

class EdgeDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)
        self.ui = Ui_TenantEdge()
        self.ui.setupUi(self)
        self.valid = QIntValidator(0, 1000000, self)
        self.ui.capacity.setValidator(self.valid)

    def Load(self, e):
        self.ui.capacity.setText(str(e.capacity))
        self.ui.service.setChecked(e.service)

    def SetResult(self, e):
        e.capacity = int(self.ui.capacity.text())
        e.service = self.ui.service.isChecked()

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
            computer = VM("id")
            self.vertices[computer] = rect
            self.tenant.AddVertex(computer)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Storage:
            rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            storage = Storage("id")
            self.vertices[storage] = rect
            self.tenant.AddVertex(storage)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Switch:
            rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            router = NetElement("id", "Switch")
            self.vertices[router] = rect
            self.tenant.AddVertex(router)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Domain:
            rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            router = Domain("id", "")
            self.vertices[router] = rect
            self.tenant.AddVertex(router)
            self.changed = True
            self.ResizeCanvas()
            self.repaint()
        elif self.state == State.Vnf:
            rect = QtCore.QRect(e.x() - self.size / 2, e.y() - self.size / 2, self.size, self.size)
            vnf = Vnf()
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
            d = SwitchDialog()
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