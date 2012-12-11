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
        self.project = project
        self.canvas.Clear()
        r = self.project.resources.GetTimeBounds()
        self.time = r[0]
        self.ui.timeSpinBox.setValue(r[0])
        self.ui.timeSpinBox.setMinimum(r[0])
        self.ui.timeSpinBox.setMaximum(r[1])
        self.ui.timeSlider.setValue(r[0])
        self.ui.timeSlider.setMinimum(r[0])
        self.ui.timeSlider.setMaximum(r[1])
        self.ui.info.setText("")
        timeInt = self.project.resources.GetTimeInterval(self.time)
        self.ui.assignedDemands.clear()
        for d in self.project.demands:
            if d.assigned and (d.startTime <= self.time) and (d.endTime >= self.time):
                it = QTreeWidgetItem(self.ui.assignedDemands, [d.id])
                it.setFlags(Qt.ItemIsEnabled | Qt.ItemIsSelectable)
        self.canvas.Visualize(self.project.resources, timeInt)
        
    def resizeEvent(self, e):
        super(QMainWindow, self).resizeEvent(e)
        self.canvas.ResizeCanvas()

    def showEvent(self, e):
        super(QMainWindow, self).showEvent(e)
        self.canvas.ResizeCanvas()

    def ShowRouterInfo(self):
        v = next(v for v in self.canvas.vertices.keys() if self.canvas.vertices[v] == self.canvas.selectedVertex)
        timeInt = self.project.resources.GetTimeInterval(self.time)
        if timeInt==None:
            return
        link_num = 0
        for d in v.intervals[timeInt].demands.keys():
            link_num += len(v.intervals[timeInt].demands[d])
        replicalinks = {}
        for d in self.project.demands:
            replicalinks[d.id] = []
            if d.startTime<=timeInt[0] and d.endTime>=timeInt[1]:
                for r in d.replicalinks:
                    if r.path.count(v) != 0:
                        link_num += 1
                        replicalinks[d.id].append(r)
        str = QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Statistics"))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Router id")).arg(v.id)
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Bandwidth")).arg(v.capacity)
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2 (%3%)</font><br />").arg(self.tr("Used Bandwidth")).arg(v.intervals[timeInt].usedResource).arg(v.getUsedCapacityPercent(timeInt))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned requests")).arg(len(v.intervals[timeInt].demands.keys()))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned channels")).arg(link_num)
        str += QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Assigned Requests"))
        demands = v.intervals[timeInt].demands.keys()
        for id in replicalinks.keys():
            if demands.count(id) == 0 and replicalinks[id] != []:
                demands.append(id)
        demands.sort()
        for id in demands:
            d = self.project.FindDemand(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            if v.intervals[timeInt].demands.has_key(id):
                for link in v.intervals[timeInt].demands[d.id]:
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

    def ShowComputerInfo(self):
        v = next(v for v in self.canvas.vertices.keys() if self.canvas.vertices[v] == self.canvas.selectedVertex)
        timeInt = self.project.resources.GetTimeInterval(self.time)
        if timeInt==None:
            return
        vm_num = 0
        for d in v.intervals[timeInt].demands.keys():
            vm_num += len(v.intervals[timeInt].demands[d])
        str = QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Statistics"))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Computer id")).arg(v.id)
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Performance")).arg(v.speed)
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2 (%3%)</font><br />").arg(self.tr("Used Performance")).arg(v.intervals[timeInt].usedResource).arg(v.getUsedSpeedPercent(timeInt))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned requests")).arg(len(v.intervals[timeInt].demands.keys()))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned VMs")).arg(vm_num)
        str += QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Assigned Requests"))
        demands = v.intervals[timeInt].demands.keys()
        demands.sort()
        for id in demands:
            d = self.project.FindDemand(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            for v1 in v.intervals[timeInt].demands[id]:
                str += QString("&nbsp;&nbsp;&nbsp;&nbsp;%3: <font color=blue>%1</font>&nbsp;&nbsp;%4: <font color=blue>%2</font><br />").arg(d.FindVertex(v1).id).arg(d.FindVertex(v1).speed).arg(self.tr("VM id")).arg(self.tr("Performance"))
        self.ui.info.setText(str)

    def ShowStorageInfo(self):
        v = next(v for v in self.canvas.vertices.keys() if self.canvas.vertices[v] == self.canvas.selectedVertex)
        timeInt = self.project.resources.GetTimeInterval(self.time)
        if timeInt==None:
            return
        storage_num = 0
        replicas = {}
        for d in v.intervals[timeInt].demands.keys():
            storage_num += len(v.intervals[timeInt].demands[d])
        for d in self.project.demands:
            replicas[d.id] = []
            if d.startTime<=timeInt[0] and d.endTime>=timeInt[1]:
                for r in d.replications:
                    if r.assignedto==v:
                        storage_num += 1
                        replicas[d.id].append(r)
        str = QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Statistics"))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Storage id")).arg(v.id)
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Type")).arg(v.type)
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Capacity")).arg(v.volume)
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2 (%3%)</font><br />").arg(self.tr("Used Capacity")).arg(v.intervals[timeInt].usedResource).arg(v.getUsedVolumePercent(timeInt))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned requests")).arg(len(v.intervals[timeInt].demands.keys()))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned storages")).arg(storage_num)
        str += QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Assigned Requests"))
        demands = v.intervals[timeInt].demands.keys()
        for id in replicas.keys():
            if demands.count(id) == 0 and replicas[id] != []:
                demands.append(id)
        demands.sort()
        for id in demands:
            d = self.project.FindDemand(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            if v.intervals[timeInt].demands.has_key(id):
                for v1 in v.intervals[timeInt].demands[id]:
                    str += QString("&nbsp;&nbsp;&nbsp;&nbsp;%3: <font color=blue>%1</font>&nbsp;&nbsp;%4: <font color=blue>%2</font><br />").arg(d.FindVertex(v1).id).arg(d.FindVertex(v1).volume).arg(self.tr("Storage id")).arg(self.tr("Capacity"))
            for r in replicas[id]:
                    str += QString("&nbsp;&nbsp;&nbsp;&nbsp;%3: <font color=blue>%1</font>&nbsp;&nbsp;%4: <font color=blue>%2</font><br />").arg(r.replica.id).arg(r.replica.volume).arg(self.tr("Replica of")).arg(self.tr("Capacity"))
        self.ui.info.setText(str)

    def ShowEdgeInfo(self):
        e = self.canvas.selectedEdge
        if e == None:
            return
        timeInt = self.project.resources.GetTimeInterval(self.time)
        if timeInt==None:
            return
        link_num = 0
        for d in e.intervals[timeInt].demands.keys():
            link_num += len(e.intervals[timeInt].demands[d])
        replicalinks = {}
        for d in self.project.demands:
            replicalinks[d.id] = []
            if d.startTime<=timeInt[0] and d.endTime>=timeInt[1]:
                for r in d.replicalinks:
                    if r.path.count(e) != 0:
                        link_num += 1
                        replicalinks[d.id].append(r)
        str = QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Statistics"))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Bandwidth")).arg(e.capacity)
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2 (%3%)</font><br />").arg(self.tr("Used Bandwidth")).arg(e.intervals[timeInt].usedResource).arg(e.getUsedCapacityPercent(timeInt))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned requests")).arg(len(e.intervals[timeInt].demands.keys()))
        str += QString("&nbsp;&nbsp;%1:<font color=blue> %2</font><br />").arg(self.tr("Number of assigned channels")).arg(link_num)
        str += QString("<b><font size=\"+1\">%1</font></b><br />").arg(self.tr("Assigned Requests"))
        demands = e.intervals[timeInt].demands.keys()
        for id in replicalinks.keys():
            if demands.count(id) == 0 and replicalinks[id] != []:
                demands.append(id)
        demands.sort()
        for id in demands:
            d = self.project.FindDemand(id)
            str += QString("&nbsp;&nbsp;<font size=\"+1\">%1</font>:<br />").arg(id)
            if e.intervals[timeInt].demands.has_key(id):
                for link in e.intervals[timeInt].demands[id]:
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

    def UpdateTimeFromSlider(self,value):
        self.time = value
        self.ui.timeSpinBox.setValue(value)
        self.Update()        

    def UpdateTimeFromSpinBox(self,value):
        self.time = value
        self.ui.timeSlider.setValue(value)
        self.Update()

    def Update(self):
        timeInt = self.project.resources.GetTimeInterval(self.time)
        self.ui.assignedDemands.clear()
        for d in self.project.demands:
            if (d.startTime <= self.time) and (d.endTime >= self.time) and d.assigned:
                it = QTreeWidgetItem(self.ui.assignedDemands, [d.id])
                it.setFlags(Qt.ItemIsEnabled | Qt.ItemIsSelectable)
        self.canvas.demandVertices = []
        self.canvas.demandEdges = []
        self.canvas.Visualize(self.project.resources, timeInt)
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
            
    def demandSelected(self):
        self.canvas.demandVertices = []
        self.canvas.demandEdges = []
        if self.ui.assignedDemands.selectedItems()==[]:
            timeInt = self.project.resources.GetTimeInterval(self.time)
            self.canvas.Visualize(self.project.resources, timeInt)
            return
        for it in self.ui.assignedDemands.selectedItems():
            id = it.text(0)
            d = self.project.FindDemand(id)
            for v in d.vertices:
                self.canvas.demandVertices.append(v.resource)
            for r in d.replications:
                self.canvas.demandVertices.append(r.assignedto)
            links = d.edges + d.replicalinks
            for e in links:
                for e1 in e.path[1:len(e.path)-1]:
                    if isinstance(e1,Router):
                        self.canvas.demandVertices.append(e1)
                    else:
                        self.canvas.demandEdges.append(self.project.resources.FindEdge(e1.e1, e1.e2))
        timeInt = self.project.resources.GetTimeInterval(self.time)
        self.canvas.Visualize(self.project.resources, timeInt)