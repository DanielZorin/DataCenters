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
        for p in v.params:
            if (p.type == "int") or (p.type == "real"):
                name = p.name
                val = int(p.value) if p.type == "int" else float(p.value)
                used = 0
                for v1 in v.assignments:
                    for p1 in v1[0].params:
                        if (p1.name == name) and (p1.type == p.type):
                            used += int(p1.value) if p.type == "int" else float(p1.value)
                str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>: used %2 of %3 (%4 %)<br />").arg(name).arg(used).arg(val).arg(int(float(used)/float(val)*100))
        str += QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Assigned Tenants"))
        for id in tenants:
            d = self.project.FindTenant(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            for v1 in v.assignments:
                if v1[1] == d:
                    str += QString("&nbsp;&nbsp;&nbsp;&nbsp;VM ID: <font color=blue>%1</font> <br />").arg(v1[0].id)
        self.ui.info.setText(str)

    def ShowEdgeInfo(self):
        e = self.canvas.selectedEdge
        if e == None:
            return
        timeInt = self.project.resources.GetTimeInterval(self.time)
        if timeInt==None:
            return
        link_num = 0
        for d in e.intervals[timeInt].tenants.keys():
            link_num += len(e.intervals[timeInt].tenants[d])
        replicalinks = {}
        for d in self.project.tenants:
            replicalinks[d.id] = []
            if d.startTime<=timeInt[0] and d.endTime>=timeInt[1]:
                for r in d.replicalinks:
                    if r.path.count(e) != 0:
                        link_num += 1
                        replicalinks[d.id].append(r)
        str = QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Statistics"))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Bandwidth")).arg(e.capacity)
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2 (%3%)</font><br />").arg(self.tr("Used Bandwidth")).arg(e.intervals[timeInt].usedCapacity).arg(e.getUsedCapacityPercent(timeInt))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned requests")).arg(len(e.intervals[timeInt].tenants.keys()))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned channels")).arg(link_num)
        str += QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Assigned Requests"))
        tenants = e.intervals[timeInt].tenants.keys()
        for id in replicalinks.keys():
            if tenants.count(id) == 0 and replicalinks[id] != []:
                tenants.append(id)
        tenants.sort()
        for id in tenants:
            d = self.project.FindTenant(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            if e.intervals[timeInt].tenants.has_key(id):
                for link in e.intervals[timeInt].tenants[id]:
                    l = d.FindEdge(d.FindVertex(link[0]),d.FindVertex(link[1]))
                    type1 = self.tr("VM") if isinstance(l.e1,VM) else self.tr("Storage")
                    type2 = self.tr("VM") if isinstance(l.e2,VM) else self.tr("Storage")
                    str += QString("&nbsp;&nbsp;&nbsp;&nbsp;%6: <font color=blue>%1: %2 &lt;---&gt; %3: %4</font>&nbsp;&nbsp;%7: <font color=blue>%5</font>&nbsp;&nbsp;<br />").arg(type1).arg(l.e1.id).arg(type2).arg(l.e2.id).arg(l.capacity).arg(self.tr("Channel")).arg(self.tr("Bandwidth"))
            for r in replicalinks[id]:
                if r.e1 == r.e2:
                    str += QString("&nbsp;&nbsp;&nbsp;&nbsp;%1 <font color=blue>%2</font>&nbsp;&nbsp;%3: <font color=blue>%4</font>&nbsp;&nbsp;<br />").arg(self.tr("Consistency channel: <font color=blue>Storage</font>")).arg(r.e1.id).arg(self.tr("<font color=blue>&lt;---&gt; replica</font>. Bandwidth")).arg(r.capacity)
                elif r.toreplica:
                    type1 = self.tr("VM") if isinstance(r.e1,VM) else self.tr("Storage")
                    type2 = self.tr("Replica of storage")
                    str += QString("&nbsp;&nbsp;&nbsp;&nbsp;%6: <font color=blue>%1: %2 &lt;---&gt; %3: %4</font>&nbsp;&nbsp;%7: <font color=blue>%5</font>&nbsp;&nbsp;<br />").arg(type1).arg(r.e1.id).arg(type2).arg(r.e2.id).arg(r.capacity).arg(self.tr("Channel")).arg(self.tr("Bandwidth"))
                else:
                    type1 = self.tr("Replica of storage")
                    type2 = self.tr("VM") if isinstance(r.e2,VM) else self.tr("Storage")
                    str += QString("&nbsp;&nbsp;&nbsp;&nbsp;%6: <font color=blue>%1: %2 &lt;---&gt; %3: %4</font>&nbsp;&nbsp;%7: <font color=blue>%5</font>&nbsp;&nbsp;<br />").arg(type1).arg(r.e1.id).arg(type2).arg(r.e2.id).arg(r.capacity).arg(self.tr("Channel")).arg(self.tr("Bandwidth"))
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