from PyQt4.QtCore import QString, Qt
from PyQt4.QtGui import QMainWindow, QFileDialog, QTextEdit, QTreeWidgetItem
from DCGUI.Windows.ui_Vis import Ui_Vis
from DCGUI.VisCanvas import VisCanvas
from Core.Tenant import *
from Core.Resources import *

class Vis(QMainWindow):
    xmlfile = None
    time = 0

    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_Vis()
        self.ui.setupUi(self)
        self.canvas = VisCanvas(self.ui.graphArea)
        self.ui.graphArea.setWidget(self.canvas)
        self.ui.info.setLineWrapMode(QTextEdit.NoWrap)
        self.canvas.edge_selected.connect(self.ShowEdgeInfo)
        self.canvas.vertex_selected.connect(self.ShowVertexInfo)

    def setData(self, project):
        self.project = project
        self.canvas.Clear()
        self.ui.info.setText("")
        self.ui.assignedTenants.clear()
        for d in self.project.tenants:
            it = QTreeWidgetItem(self.ui.assignedTenants, [d.name])
            it.setFlags(Qt.ItemIsEnabled | Qt.ItemIsSelectable)
        self.canvas.Visualize(self.project.resources)
        
    def resizeEvent(self, e):
        super(QMainWindow, self).resizeEvent(e)
        self.canvas.ResizeCanvas()

    def showEvent(self, e):
        super(QMainWindow, self).showEvent(e)
        self.canvas.ResizeCanvas()

    def ShowVertexInfo(self):
        v = next(v for v in self.canvas.vertices.keys() if self.canvas.vertices[v] == self.canvas.selectedVertex)
        str = QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Statistics"))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Computer id")).arg(v.id)
        tenants = list(set([p[1].name for p in v.assignments]))
        tenants.sort()
        #str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Performance")).arg(v.speed)
        #str += QString("&nbsp;&nbsp;%1:<font color=blue> %2 (%3%)</font><br />").arg(self.tr("Used Performance")).arg(v.intervals[timeInt].usedSpeed).arg(v.getUsedSpeedPercent(timeInt))
        #str += QString("&nbsp;&nbsp;%1:<font color=blue> %2 (%3%)</font><br />").arg(self.tr("Used RAM")).arg(v.intervals[timeInt].usedRam).arg(v.getUsedRamPercent(timeInt))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned tenants")).arg(len(tenants))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned VMs")).arg(len(v.assignments))
        str += QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Parameters"))
        for p in v.params.values():
            if (p.type == "integer") or (p.type == "real"):
                name = p.name
                val = int(p.value) if p.type == "integer" else float(p.value)
                used = 0
                for v1 in v.assignments:
                    for p1 in v1[0].params.values():
                        if (p1.name == name) and (p1.type == p.type):
                            used += int(p1.value) if p.type == "integer" else float(p1.value)
                str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>: %5 %2 %6 %3 (%4 %)<br />").arg(name).arg(used).arg(val).arg(int(float(used)/float(val)*100)).arg(self.tr("used")).arg(self.tr("of"))
        str += QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Assigned Tenants"))
        for id in tenants:
            d = self.project.FindTenant(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            for v1 in v.assignments:
                if v1[1] == d:
                    str += QString("&nbsp;&nbsp;&nbsp;&nbsp;ID: <font color=blue>%1</font> <br />").arg(v1[0].id)
        self.ui.info.setText(str)

    def ShowEdgeInfo(self):
        e = self.canvas.selectedEdge
        if e == None:
            return
        tenants = list(set([p[1].name for p in e.assignments]))
        tenants.sort()
        used = 0
        for v in e.assignments:
            used += v[0].capacity
        str = QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Statistics"))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Bandwidth")).arg(e.capacity)
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2 (%3%)</font><br />").arg(self.tr("Used Bandwidth")).arg(used).arg(int(float(used)/float(e.capacity)*100))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned tenants")).arg(len(tenants))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned channels")).arg(len(e.assignments))
        str += QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Assigned Tenants"))
        classnames = {VM:u"VM", Storage:u"Storage", Vnf:u"VNF", NetElement:u"NetElement", Domain:u"Domain"}
        for id in tenants:
            d = self.project.FindTenant(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            for l in e.assignments:
                t1 = classnames[l[0].e1.__class__]
                t2 = classnames[l[0].e2.__class__]
                str += QString("&nbsp;&nbsp;&nbsp;&nbsp;%6: <font color=blue>%1: %2 &lt;---&gt; %3: %4</font>&nbsp;&nbsp;%7: <font color=blue>%5</font>&nbsp;&nbsp;<br />").arg(t1).arg(l[0].e1.id).arg(t2).arg(l[0].e2.id).arg(l[0].capacity).arg(self.tr("Channel")).arg(self.tr("Bandwidth"))
        self.ui.info.setText(str)
            
    def tenantSelected(self):
        self.canvas.tenantVertices = []
        self.canvas.tenantEdges = []
        if self.ui.assignedTenants.selectedItems()==[]:
            self.canvas.Visualize(self.project.resources)
            return
        for it in self.ui.assignedTenants.selectedItems():
            id = it.text(0)
            d = self.project.FindTenant(id)
            for v in d.vertices:
                self.canvas.tenantVertices.append(v.assigned)
            links = d.edges
            '''for e in links:
                for e1 in e.path[1:len(e.path)-1]:
                    if isinstance(e1,Router):
                        self.canvas.tenantVertices.append(e1)
                    else:
                        self.canvas.tenantEdges.append(self.project.resources.FindEdge(e1.e1, e1.e2))'''
        self.canvas.Visualize(self.project.resources)