from PyQt4.QtGui import QMainWindow, QFileDialog
from DCGUI.Windows.ui_ResourcesGraphEditor import Ui_ResourcesGraphEditor
from DCGUI.ResourcesGraphCanvas import ResourcesGraphCanvas, State

class ResourcesGraphEditor(QMainWindow):
    xmlfile = None

    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_ResourcesGraphEditor()
        self.ui.setupUi(self)
        self.canvas = ResourcesGraphCanvas(self.ui.graphArea)
        self.ui.graphArea.setWidget(self.canvas)
        self.basename = self.windowTitle()
        self.setWindowTitle("Untitled" + " - " + self.basename)

    def setData(self, data):
        self.resources = data
        self.canvas.Visualize(self.resources)

    def toggleSelect(self):
        self.ui.actionSelect.setChecked(True)
        self.ui.actionComputer.setChecked(False)
        self.ui.actionStorage.setChecked(False)
        self.ui.actionRouter.setChecked(False)
        self.ui.actionEdge.setChecked(False)
        self.canvas.state = State.Select

    def toggleComputer(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionComputer.setChecked(True)
        self.ui.actionStorage.setChecked(False)
        self.ui.actionRouter.setChecked(False)
        self.ui.actionEdge.setChecked(False)
        self.canvas.state = State.Computer

    def toggleStorage(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionComputer.setChecked(False)
        self.ui.actionStorage.setChecked(True)
        self.ui.actionRouter.setChecked(False)
        self.ui.actionEdge.setChecked(False)
        self.canvas.state = State.Storage

    def toggleRouter(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionComputer.setChecked(False)
        self.ui.actionStorage.setChecked(False)
        self.ui.actionRouter.setChecked(True)
        self.ui.actionEdge.setChecked(False)
        self.canvas.state = State.Router

    def toggleEdge(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionComputer.setChecked(False)
        self.ui.actionStorage.setChecked(False)
        self.ui.actionRouter.setChecked(False)
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
        self.resources.vertices = []
        self.resources.edges = []
        self.canvas.Clear()
        self.canvas.Visualize(self.resources)
        self.canvas.changed = True
        self.xmlfile = None
        self.setWindowTitle("Untitled" + " - " + self.basename)

    def Open(self):
        name = QFileDialog.getOpenFileName(filter="*.xml")
        if name == None or name == '':
            return
        self.resources.LoadFromXML(name)
        self.canvas.Clear()
        self.canvas.Visualize(self.resources)
        self.canvas.changed = True
        self.xmlfile = name
        self.setWindowTitle(str(self.xmlfile).split('/').pop().split('.')[0] + " - " + self.basename)

    def Save(self):
        self.canvas.updatePos()
        if self.xmlfile == None:
            self.SaveAs()
        else:
            output = open(self.xmlfile, 'w')
            output.write(self.resources.ExportToXml())
            output.close()

    def SaveAs(self):
        self.canvas.updatePos()
        self.xmlfile = QFileDialog.getSaveFileName(directory=".xml", filter="*.xml")
        if self.xmlfile != '':
            output = open(self.xmlfile, 'w')
            output.write(self.resources.ExportToXml())
            output.close()
            self.setWindowTitle(str(self.xmlfile).split('/').pop().split('.')[0] + " - " + self.basename)

    def closeEvent(self, e):
        self.canvas.updatePos()