from PyQt4.QtCore import QString, Qt
from PyQt4.QtGui import QMainWindow, QFileDialog, QTextEdit, QTreeWidgetItem
from DCGUI.Windows.ui_Vis import Ui_Vis
from DCGUI.VisCanvas import VisCanvas
from Core.Demands import VM, DemandStorage
from Core.Resources import Computer, Storage, Router

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
        self.canvas.computer_selected.connect(self.ShowComputerInfo)
        self.canvas.storage_selected.connect(self.ShowStorageInfo)
        self.canvas.router_selected.connect(self.ShowRouterInfo)

    def setData(self, project):
        self.resources = project.resources
        self.demands = project.demands
        self.canvas.Clear()
        r = self.resources.GetTimeBounds()
        self.time = r[0]
        self.ui.timeSpinBox.setValue(r[0])
        self.ui.timeSpinBox.setMinimum(r[0])
        self.ui.timeSpinBox.setMaximum(r[1])
        self.ui.timeSlider.setValue(r[0])
        self.ui.timeSlider.setMinimum(r[0])
        self.ui.timeSlider.setMaximum(r[1])
        self.ui.info.setText("")
        timeInt = self.resources.GetTimeInterval(self.time)
        self.ui.assignedDemands.clear()
        for d in self.demands:
            if d.assigned and (d.startTime <= self.time) and (d.endTime >= self.time):
                it = QTreeWidgetItem(self.ui.assignedDemands, [d.id])
                it.setFlags(Qt.ItemIsEnabled | Qt.ItemIsSelectable)
        self.canvas.Visualize(self.resources, timeInt)
        
    def resizeEvent(self, e):
        super(QMainWindow, self).resizeEvent(e)
        self.canvas.ResizeCanvas()

    def showEvent(self, e):
        super(QMainWindow, self).showEvent(e)
        self.canvas.ResizeCanvas()

    def ShowRouterInfo(self):
        v = next(v for v in self.canvas.vertices.keys() if self.canvas.vertices[v] == self.canvas.selectedVertex)
        timeInt = self.resources.GetTimeInterval(self.time)
        link_num = 0
        for d in v.intervals[timeInt].demands.keys():
            link_num += len(v.intervals[timeInt].demands[d])
        str = QString("<b><font size=\"+1\">Statistics</font></b><br />")
        str += QString("&nbsp;&nbsp;Router id:<font color=blue> %1</font><br />").arg(v.id)
        str += QString("&nbsp;&nbsp;Bandwidth:<font color=blue> %1</font><br />").arg(v.capacity)
        str += QString("&nbsp;&nbsp;Used Bandwidth:<font color=blue> %1 (%2%)</font><br />").arg(v.intervals[timeInt].usedResource).arg(v.getUsedCapacityPercent(timeInt))
        str += QString("&nbsp;&nbsp;Number of assigned requests:<font color=blue> %1</font><br />").arg(len(v.intervals[timeInt].demands.keys()))
        str += QString("&nbsp;&nbsp;Number of assigned channels:<font color=blue> %1</font><br />").arg(link_num)
        str += QString("<b><font size=\"+1\">Assigned Requests</font></b><br />")
        demands = v.intervals[timeInt].demands.keys()
        demands.sort()
        for id in demands:
            d = self.FindDemand(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            for link in v.intervals[timeInt].demands[d.id]:
                l = d.FindEdge(d.FindVertex(link[0]),d.FindVertex(link[1]))
                type1 = "VM" if isinstance(l.e1,VM) else "Storage"
                type2 = "VM" if isinstance(l.e2,VM) else "Storage"
                str += QString("&nbsp;&nbsp;&nbsp;&nbsp;Channel: <font color=blue>%1: %2 &lt;---&gt; %3: %4</font>&nbsp;&nbsp;Bandwidth: <font color=blue>%5</font>&nbsp;&nbsp;<br />").arg(type1).arg(l.e1.id).arg(type2).arg(l.e2.id).arg(l.capacity)
        self.ui.info.setText(str)

    def ShowComputerInfo(self):
        v = next(v for v in self.canvas.vertices.keys() if self.canvas.vertices[v] == self.canvas.selectedVertex)
        timeInt = self.resources.GetTimeInterval(self.time)
        vm_num = 0
        for d in v.intervals[timeInt].demands.keys():
            vm_num += len(v.intervals[timeInt].demands[d])
        str = QString("<b><font size=\"+1\">Statistics</font></b><br />")
        str += QString("&nbsp;&nbsp;Computer id:<font color=blue> %1</font><br />").arg(v.id)
        str += QString("&nbsp;&nbsp;Performance:<font color=blue> %1</font><br />").arg(v.speed)
        str += QString("&nbsp;&nbsp;Used Performance:<font color=blue> %1 (%2%)</font><br />").arg(v.intervals[timeInt].usedResource).arg(v.getUsedSpeedPercent(timeInt))
        str += QString("&nbsp;&nbsp;Number of assigned requests:<font color=blue> %1</font><br />").arg(len(v.intervals[timeInt].demands.keys()))
        str += QString("&nbsp;&nbsp;Number of assigned VMs:<font color=blue> %1</font><br />").arg(vm_num)
        str += QString("<b><font size=\"+1\">Assigned Requests</font></b><br />")
        demands = v.intervals[timeInt].demands.keys()
        demands.sort()
        for id in demands:
            d = self.FindDemand(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            for v1 in v.intervals[timeInt].demands[id]:
                str += QString("&nbsp;&nbsp;&nbsp;&nbsp;VM id: <font color=blue>%1</font>&nbsp;&nbsp;Performance: <font color=blue>%2</font><br />").arg(d.FindVertex(v1).id).arg(d.FindVertex(v1).speed)
        self.ui.info.setText(str)

    def ShowStorageInfo(self):
        v = next(v for v in self.canvas.vertices.keys() if self.canvas.vertices[v] == self.canvas.selectedVertex)
        timeInt = self.resources.GetTimeInterval(self.time)
        storage_num = 0
        for d in v.intervals[timeInt].demands.keys():
            storage_num += len(v.intervals[timeInt].demands[d])
        str = QString("<b><font size=\"+1\">Statistics</font></b><br />")
        str += QString("&nbsp;&nbsp;Storage id:<font color=blue> %1</font><br />").arg(v.id)
        str += QString("&nbsp;&nbsp;Type:<font color=blue> %1</font><br />").arg(v.type)
        str += QString("&nbsp;&nbsp;Capacity:<font color=blue> %1</font><br />").arg(v.volume)
        str += QString("&nbsp;&nbsp;Used Capacity:<font color=blue> %1 (%2%)</font><br />").arg(v.intervals[timeInt].usedResource).arg(v.getUsedVolumePercent(timeInt))
        str += QString("&nbsp;&nbsp;Number of assigned requests:<font color=blue> %1</font><br />").arg(len(v.intervals[timeInt].demands.keys()))
        str += QString("&nbsp;&nbsp;Number of assigned storages:<font color=blue> %1</font><br />").arg(storage_num)
        str += QString("<b><font size=\"+1\">Assigned Requests</font></b><br />")
        demands = v.intervals[timeInt].demands.keys()
        demands.sort()
        for id in demands:
            d = self.FindDemand(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            for v1 in v.intervals[timeInt].demands[id]:
                str += QString("&nbsp;&nbsp;&nbsp;&nbsp;Storage id: <font color=blue>%1</font>&nbsp;&nbsp;Capacity: <font color=blue>%2</font><br />").arg(d.FindVertex(v1).id).arg(d.FindVertex(v1).volume)
        self.ui.info.setText(str)

    def ShowEdgeInfo(self):
        e = self.canvas.selectedEdge
        if e == None:
            return
        timeInt = self.resources.GetTimeInterval(self.time)
        link_num = 0
        for d in e.intervals[timeInt].demands.keys():
            link_num += len(e.intervals[timeInt].demands[d])
        str = QString("<b><font size=\"+1\">Statistics</font></b><br />")
        str += QString("&nbsp;&nbsp;Bandwidth:<font color=blue> %1</font><br />").arg(e.capacity)
        str += QString("&nbsp;&nbsp;Used Bandwidth:<font color=blue> %1 (%2%)</font><br />").arg(e.intervals[timeInt].usedResource).arg(e.getUsedCapacityPercent(timeInt))
        str += QString("&nbsp;&nbsp;Number of assigned requests:<font color=blue> %1</font><br />").arg(len(e.intervals[timeInt].demands.keys()))
        str += QString("&nbsp;&nbsp;Number of assigned channels:<font color=blue> %1</font><br />").arg(link_num)
        str += QString("<b><font size=\"+1\">Assigned Requests</font></b><br />")
        demands = e.intervals[timeInt].demands.keys()
        demands.sort()
        for id in demands:
            d = self.FindDemand(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            for link in e.intervals[timeInt].demands[id]:
                l = d.FindEdge(d.FindVertex(link[0]),d.FindVertex(link[1]))
                type1 = "VM" if isinstance(l.e1,VM) else "Storage"
                type2 = "VM" if isinstance(l.e2,VM) else "Storage"
                str += QString("&nbsp;&nbsp;&nbsp;&nbsp;Channel: <font color=blue>%1: %2 &lt;---&gt; %3: %4</font>&nbsp;&nbsp;Bandwidth: <font color=blue>%5</font>&nbsp;&nbsp;<br />").arg(type1).arg(l.e1.id).arg(type2).arg(l.e2.id).arg(l.capacity)
        self.ui.info.setText(str)

    def UpdateTimeFromSlider(self,value):
        self.time = value
        self.ui.timeSpinBox.setValue(value)
        self.Update()        

    def UpdateTimeFromSpinBox(self,value):
        self.time = value
        self.ui.timeSlider.setValue(value)
        self.Update()

    def Update(self):
        timeInt = self.resources.GetTimeInterval(self.time)
        self.ui.assignedDemands.clear()
        for d in self.demands:
            if (d.startTime <= self.time) and (d.endTime >= self.time) and d.assigned:
                it = QTreeWidgetItem(self.ui.assignedDemands, [d.id])
                it.setFlags(Qt.ItemIsEnabled | Qt.ItemIsSelectable)
        self.canvas.demandVertices = []
        self.canvas.demandEdges = []
        self.canvas.Visualize(self.resources, timeInt)
        self.ShowEdgeInfo()
        if self.canvas.selectedVertex == None:
            return
        v = next(v for v in self.canvas.vertices.keys() if self.canvas.vertices[v] == self.canvas.selectedVertex)
        if isinstance(v,Router):
            self.ShowRouterInfo()
        elif isinstance(v,Computer):
            self.ShowComputerInfo()
        elif isinstance(v,Storage):
            self.ShowStorageInfo()

    def FindDemand(self, id):
        for d in self.demands:
            if d.id == id:
                return d
            
    def demandSelected(self):
        self.canvas.demandVertices = []
        self.canvas.demandEdges = []
        if self.ui.assignedDemands.selectedItems()==[]:
            timeInt = self.resources.GetTimeInterval(self.time)
            self.canvas.Visualize(self.resources, timeInt)
            return
        for it in self.ui.assignedDemands.selectedItems():
            id = it.text(0)
            d = self.FindDemand(id)
            for v in d.vertices:
                self.canvas.demandVertices.append(v.resource)
            for e in d.edges:
                for e1 in e.path[1:len(e.path)-1]:
                    if isinstance(e1,Router):
                        self.canvas.demandVertices.append(e1)
                    else:
                        self.canvas.demandEdges.append(self.resources.FindEdge(e1.e1, e1.e2))
        timeInt = self.resources.GetTimeInterval(self.time)
        self.canvas.Visualize(self.resources, timeInt)