from PyQt4.QtGui import QMainWindow, QFileDialog
from DCGUI.Windows.ui_DemandGraphEditor import Ui_DemandGraphEditor
from DCGUI.DemandGraphCanvas import DemandGraphCanvas, State

class DemandGraphEditor(QMainWindow):
    xmlfile = None

    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_DemandGraphEditor()
        self.ui.setupUi(self)
        self.canvas = DemandGraphCanvas(self.ui.graphArea)
        self.ui.graphArea.setWidget(self.canvas)

    def setData(self, data):
        self.demand = data
        self.canvas.Visualize(self.demand)

    def toggleSelect(self):
        self.ui.actionSelect.setChecked(True)
        self.ui.actionVM.setChecked(False)
        self.ui.actionDemandStorage.setChecked(False)
        self.ui.actionEdge.setChecked(False)
        self.canvas.state = State.Select

    def toggleVM(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionVM.setChecked(True)
        self.ui.actionDemandStorage.setChecked(False)
        self.ui.actionEdge.setChecked(False)
        self.canvas.state = State.VM

    def toggleDemandStorage(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionVM.setChecked(False)
        self.ui.actionDemandStorage.setChecked(True)
        self.ui.actionEdge.setChecked(False)
        self.canvas.state = State.DemandStorage

    def toggleEdge(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionVM.setChecked(False)
        self.ui.actionDemandStorage.setChecked(False)
        self.ui.actionEdge.setChecked(True)
        self.canvas.state = State.Edge
        
    def resizeEvent(self, e):
        super(QMainWindow, self).resizeEvent(e)
        self.canvas.ResizeCanvas()

    def showEvent(self, e):
        super(QMainWindow, self).showEvent(e)
        self.canvas.ResizeCanvas()

    def LoadPositions(self, lst):
        pass

    def SavePositions(self):
        pass

    def New(self):
        self.demand.vertices = []
        self.demand.edges = []
        self.canvas.Clear()
        self.canvas.Visualize(self.demand)
        self.canvas.changed = True
        self.xmlfile = None

    def Open(self):
        name = QFileDialog.getOpenFileName(filter="*.xml")
        if name == None or name == '':
            return
        self.demand.LoadFromXml(name)
        self.canvas.Clear()
        self.canvas.Visualize(self.demand)
        self.canvas.changed = True
        self.xmlfile = name

    def Save(self):
        if self.xmlfile == None:
            self.SaveAs()
        else:
            output = open(self.xmlfile, 'w')
            output.write(self.demand.ExportToXml())
            output.close()

    def SaveAs(self):
        self.xmlfile = QFileDialog.getSaveFileName(directory=".xml", filter="*.xml")
        if self.xmlfile != '':
            output = open(self.xmlfile, 'w')
            output.write(self.demand.ExportToXml())
            output.close()