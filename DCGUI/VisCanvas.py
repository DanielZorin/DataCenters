import math
from Core.Resources import Computer, Storage, Router, Link
from Core.Demands import VM
from PyQt4 import QtGui, QtCore
from PyQt4.QtCore import QPointF, QRect, QString
from PyQt4.QtGui import QImage, QWidget, QPainter, QPainterPath, QColor, QCursor, QDialog, QIntValidator, QTableWidgetItem
from DCGUI.Windows.ui_Info import Ui_Info

class Info(QWidget):
    def __init__(self):
        QDialog.__init__(self)
        self.ui = Ui_Info()
        self.ui.setupUi(self)
        
    def LoadComputerInfo(self, v):
        vm_num = 0
        for d in v.assignedDemands.keys():
            vm_num += len(v.assignedDemands[d])
        str = QString("<b><font size=\"+1\">Statistics</font></b><br />")
        str += QString("&nbsp;&nbsp;Computer id:<font color=blue> %1</font><br />").arg(v.id)
        str += QString("&nbsp;&nbsp;Speed:<font color=blue> %1</font><br />").arg(v.speed)
        str += QString("&nbsp;&nbsp;Used Speed:<font color=blue> %1 (%2%)</font><br />").arg(v.usedSpeed).arg(v.getUsedSpeedPercent())
        str += QString("&nbsp;&nbsp;Number of assigned demands:<font color=blue> %1</font><br />").arg(len(v.assignedDemands.keys()))
        str += QString("&nbsp;&nbsp;Number of assigned VMs:<font color=blue> %1</font><br />").arg(vm_num)
        str += QString("<b><font size=\"+1\">Assigned Demands</font></b><br />")
        demands = v.assignedDemands.keys()
        demands.sort()
        for d in demands:
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(d.id)
            for v1 in v.assignedDemands[d]:
                str += QString("&nbsp;&nbsp;&nbsp;&nbsp;VM id: <font color=blue>%1</font>&nbsp;&nbsp;Speed: <font color=blue>%2</font><br />").arg(v1.id).arg(v1.speed)
        self.ui.textBrowser.setText(str)
        self.setWindowTitle(QString("%1 - Computer Info").arg(v.id))

    def LoadStorageInfo(self, v):
        storage_num = 0
        for d in v.assignedDemands.keys():
            storage_num += len(v.assignedDemands[d])
        str = QString("<b><font size=\"+1\">Statistics</font></b><br />")
        str += QString("&nbsp;&nbsp;Storage id:<font color=blue> %1</font><br />").arg(v.id)
        str += QString("&nbsp;&nbsp;Type:<font color=blue> %1</font><br />").arg(v.type)
        str += QString("&nbsp;&nbsp;Volume:<font color=blue> %1</font><br />").arg(v.volume)
        str += QString("&nbsp;&nbsp;Used Volume:<font color=blue> %1 (%2%)</font><br />").arg(v.usedVolume).arg(v.getUsedVolumePercent())
        str += QString("&nbsp;&nbsp;Number of assigned demands:<font color=blue> %1</font><br />").arg(len(v.assignedDemands.keys()))
        str += QString("&nbsp;&nbsp;Number of assigned storages:<font color=blue> %1</font><br />").arg(storage_num)
        str += QString("<b><font size=\"+1\">Assigned Demands</font></b><br />")
        demands = v.assignedDemands.keys()
        demands.sort()
        for d in demands:
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(d.id)
            for v1 in v.assignedDemands[d]:
                str += QString("&nbsp;&nbsp;&nbsp;&nbsp;Storage id: <font color=blue>%1</font>&nbsp;&nbsp;Volume: <font color=blue>%2</font><br />").arg(v1.id).arg(v1.volume)
        self.ui.textBrowser.setText(str)
        self.setWindowTitle(QString("%1 - Storage Info").arg(v.id))

    def LoadRouterInfo(self, v):
        link_num = 0
        for d in v.assignedDemands.keys():
            link_num += len(v.assignedDemands[d])
        str = QString("<b><font size=\"+1\">Statistics</font></b><br />")
        str += QString("&nbsp;&nbsp;Router id:<font color=blue> %1</font><br />").arg(v.id)
        str += QString("&nbsp;&nbsp;Capacity:<font color=blue> %1</font><br />").arg(v.capacity)
        str += QString("&nbsp;&nbsp;Used Capacity:<font color=blue> %1 (%2%)</font><br />").arg(v.usedCapacity).arg(v.getUsedCapacityPercent())
        str += QString("&nbsp;&nbsp;Number of assigned demands:<font color=blue> %1</font><br />").arg(len(v.assignedDemands.keys()))
        str += QString("&nbsp;&nbsp;Number of assigned links:<font color=blue> %1</font><br />").arg(link_num)
        str += QString("<b><font size=\"+1\">Assigned Demands</font></b><br />")
        demands = v.assignedDemands.keys()
        demands.sort()
        for d in demands:
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(d.id)
            for link in v.assignedDemands[d]:
                type1 = "VM" if isinstance(link.e1,VM) else "Storage"
                type2 = "VM" if isinstance(link.e2,VM) else "Storage"
                str += QString("&nbsp;&nbsp;&nbsp;&nbsp;Link: <font color=blue>%1: %2 &lt;---&gt; %3: %4</font>&nbsp;&nbsp;Capacity: <font color=blue>%5</font>&nbsp;&nbsp;<br />").arg(type1).arg(link.e1.id).arg(type2).arg(link.e2.id).arg(link.capacity)
        self.ui.textBrowser.setText(str)
        self.setWindowTitle(QString("%1 - Router Info").arg(v.id))

    def LoadEdgeInfo(self, e):
        link_num = 0
        for d in e.assignedDemands.keys():
            link_num += len(e.assignedDemands[d])
        str = QString("<b><font size=\"+1\">Statistics</font></b><br />")
        str += QString("&nbsp;&nbsp;Capacity:<font color=blue> %1</font><br />").arg(e.capacity)
        str += QString("&nbsp;&nbsp;Used Capacity:<font color=blue> %1 (%2%)</font><br />").arg(e.usedCapacity).arg(e.getUsedCapacityPercent())
        str += QString("&nbsp;&nbsp;Number of assigned demands:<font color=blue> %1</font><br />").arg(len(e.assignedDemands.keys()))
        str += QString("&nbsp;&nbsp;Number of assigned links:<font color=blue> %1</font><br />").arg(link_num)
        str += QString("<b><font size=\"+1\">Assigned Demands</font></b><br />")
        demands = e.assignedDemands.keys()
        demands.sort()
        for d in demands:
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(d.id)
            for link in e.assignedDemands[d]:
                type1 = "VM" if isinstance(link.e1,VM) else "Storage"
                type2 = "VM" if isinstance(link.e2,VM) else "Storage"
                str += QString("&nbsp;&nbsp;&nbsp;&nbsp;Link: <font color=blue>%1: %2 &lt;---&gt; %3: %4</font>&nbsp;&nbsp;Capacity: <font color=blue>%5</font>&nbsp;&nbsp;<br />").arg(type1).arg(link.e1.id).arg(type2).arg(link.e2.id).arg(link.capacity)
        self.ui.textBrowser.setText(str)
        self.setWindowTitle("Edge Info")
        

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
                else:
                    paint.setPen(self.colors["line"])
                x1 = self.vertices[e.e1].x() + self.size / 2
                y1 = self.vertices[e.e1].y() + self.size / 2
                x2 = self.vertices[e.e2].x() + self.size / 2
                y2 = self.vertices[e.e2].y() + self.size / 2
                self.drawArrow(paint, x1, y1, x2, y2)
                paint.setPen(self.colors["text"])
                paint.drawText((x1+x2)/2, (y1+y2)/2, str(int(e.getUsedCapacityPercent()))+"%")
                
        for v in self.vertices.keys():
            if isinstance(v,Computer):
                if self.selectedVertex != self.vertices[v]:
                    paint.drawImage(self.vertices[v], self.computericon)
                else:
                    paint.drawImage(self.vertices[v], self.computerselectedicon)
                paint.drawText(self.vertices[v].x() + self.size, self.vertices[v].y() + self.size, str(int(v.getUsedSpeedPercent()))+"%")
            elif isinstance(v,Storage):
                if self.selectedVertex != self.vertices[v]:
                    paint.drawImage(self.vertices[v], self.storageicon)
                else:
                    paint.drawImage(self.vertices[v], self.storageselectedicon)
                paint.drawText(self.vertices[v].x() + self.size, self.vertices[v].y() + self.size, str(int(v.getUsedVolumePercent()))+"%")
            elif isinstance(v,Router):
                if self.selectedVertex != self.vertices[v]:
                    paint.drawImage(self.vertices[v], self.routericon)
                else:
                    paint.drawImage(self.vertices[v], self.routerselectedicon)
                paint.drawText(self.vertices[v].x() + self.size, self.vertices[v].y() + self.size, str(int(v.getUsedCapacityPercent()))+"%")
        paint.setPen(self.colors["line"])
        if self.edgeDraw:
            self.drawArrow(paint, self.curEdge[0].x() + self.size / 2, self.curEdge[0].y() + self.size / 2,
                           QCursor.pos().x() - self.mapToGlobal(self.geometry().topLeft()).x(),
                           QCursor.pos().y() - self.mapToGlobal(self.geometry().topLeft()).y())
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
                self.ShowVertexInfo()
                self.pressed = True
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
                    self.ShowEdgeInfo()
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

    def ShowVertexInfo(self):
        v = next(v for v in self.vertices.keys() if self.vertices[v] == self.selectedVertex)
        self.i = Info()
        if isinstance(v,Computer):
            self.i.LoadComputerInfo(v)
        if isinstance(v,Storage):
            self.i.LoadStorageInfo(v)
        if isinstance(v,Router):
            self.i.LoadRouterInfo(v)
        self.i.show()
        
    def ShowEdgeInfo(self):
        self.i = Info()
        self.i.LoadEdgeInfo(self.selectedEdge)
        self.i.show()