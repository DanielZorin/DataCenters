from PyQt4.QtCore import QString
from PyQt4.QtGui import QMainWindow, QFileDialog, QTextEdit
from DCGUI.Windows.ui_Vis import Ui_Vis
from DCGUI.VisCanvas import VisCanvas
from Core.Demands import VM, DemandStorage

class Vis(QMainWindow):
    xmlfile = None

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

    def setData(self, data):
        self.resources = data
        self.canvas.Clear()
        self.canvas.Visualize(self.resources)
        self.ui.info.setText("")
        
    def resizeEvent(self, e):
        super(QMainWindow, self).resizeEvent(e)
        self.canvas.ResizeCanvas()

    def showEvent(self, e):
        super(QMainWindow, self).showEvent(e)
        self.canvas.ResizeCanvas()

    def Open(self):
        name = QFileDialog.getOpenFileName(filter="*.xml")
        if name == None or name == '':
            return
        self.resources.LoadFromXML(name)
        self.canvas.Clear()
        self.canvas.Visualize(self.resources)
        self.canvas.changed = True
        self.xmlfile = name

    def Save(self):
        if self.xmlfile == None:
            self.SaveAs()
        else:
            output = open(self.xmlfile, 'w')
            output.write(self.resources.ExportToXml())
            output.close()

    def SaveAs(self):
        self.xmlfile = QFileDialog.getSaveFileName(directory=".xml", filter="*.xml")
        if self.xmlfile != '':
            output = open(self.xmlfile, 'w')
            output.write(self.resources.ExportToXml())
            output.close()

    def ShowRouterInfo(self):
        v = next(v for v in self.canvas.vertices.keys() if self.canvas.vertices[v] == self.canvas.selectedVertex)
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
        self.ui.info.setText(str)

    def ShowComputerInfo(self):
        v = next(v for v in self.canvas.vertices.keys() if self.canvas.vertices[v] == self.canvas.selectedVertex)
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
        self.ui.info.setText(str)

    def ShowStorageInfo(self):
        v = next(v for v in self.canvas.vertices.keys() if self.canvas.vertices[v] == self.canvas.selectedVertex)
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
        self.ui.info.setText(str)

    def ShowEdgeInfo(self):
        e = self.canvas.selectedEdge
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
        self.ui.info.setText(str)

    def UpdateTimeFromSlider(self,value):
        self.time = value
        self.ui.timeSpinBox.setValue(value)

    def UpdateTimeFromSpinBox(self,value):
        self.time = value
        self.ui.timeSlider.setValue(value)

