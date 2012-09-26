from PyQt4.QtGui import QMainWindow, QFileDialog
from DCGUI.Windows.ui_Vis import Ui_Vis
from DCGUI.VisCanvas import VisCanvas

class Vis(QMainWindow):
    xmlfile = None

    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_Vis()
        self.ui.setupUi(self)
        self.canvas = VisCanvas(self.ui.graphArea)
        self.ui.graphArea.setWidget(self.canvas)

    def setData(self, data):
        self.resources = data
        self.canvas.Clear()
        self.canvas.Visualize(self.resources)
        
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
