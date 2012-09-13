from PyQt4.QtGui import QMainWindow
from DCGUI.Windows.ui_GraphEditor import Ui_GraphEditor
from DCGUI.GraphCanvas import GraphCanvas, State

class GraphEditor(QMainWindow):
    resources = None

    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_GraphEditor()
        self.ui.setupUi(self)
        self.canvas = GraphCanvas(self.ui.graphArea)
        self.ui.graphArea.setWidget(self.canvas)

    def setData(self, data):
        self.resources = data
        print(self.resources.vertices[1])
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
        pass

    def Open(self):
        pass

    def Save(self):
        pass

    def SaveAs(self):
        pass