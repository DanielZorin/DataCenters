from PyQt4.QtGui import QMainWindow, QFileDialog, QDialog
from DCGUI.Windows.ui_TenantEditor import Ui_TenantEditor
from DCGUI.Windows.ui_TenantSettings import Ui_TenantSettings
from DCGUI.TenantCanvas import TenantCanvas, State
from DCGUI.TreeDialog import TreeDialog
from Core.Tenant import *

class TenantSettingsDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)        
        self.ui = Ui_TenantSettings()
        self.ui.setupUi(self)
        
    def Load(self, v):
        self.ui.name.setText(v.name)
        self.ui.type.setText(v.type)
        self.ui.created.setText(v.created)
        self.ui.updated.setText(v.updated)
        self.ui.deleted.setText(v.deleted)
        self.ui.delbox.setChecked(v.deleteFlag)
        self.ui.expiration.setText(v.expiration)

    def SetResult(self, v):
        v.name = str(self.ui.name.text())
        v.type = str(self.ui.type.text())
        v.created = str(self.ui.created.text())
        v.updated = str(self.ui.updated.text())
        v.deleted = str(self.ui.deleted.text())
        v.deleteFlag = self.ui.delbox.isChecked()
        v.expiration = str(self.ui.expiration.text())


class TenantEditor(QMainWindow):
    xmlfile = None

    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_TenantEditor()
        self.ui.setupUi(self)
        self.canvas = TenantCanvas(self.ui.graphArea)
        self.ui.graphArea.setWidget(self.canvas)
        self.basename = self.windowTitle()
        self.setWindowTitle(self.tr("Untitled") + " - " + self.basename)
        self.New()

    def setData(self, data):
        self.tenant = data
        self.canvas.Clear()
        self.canvas.Visualize(self.tenant)

    def toggleSelect(self):
        self.ui.actionSelect.setChecked(True)
        self.ui.actionComputer.setChecked(False)
        self.ui.actionStorage.setChecked(False)
        self.ui.actionRouter.setChecked(False)
        self.ui.actionEdge.setChecked(False)
        self.ui.actionDomain.setChecked(False)
        self.ui.actionService.setChecked(False)
        self.canvas.state = State.Select

    def toggleComputer(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionComputer.setChecked(True)
        self.ui.actionStorage.setChecked(False)
        self.ui.actionRouter.setChecked(False)
        self.ui.actionEdge.setChecked(False)
        self.ui.actionDomain.setChecked(False)
        self.ui.actionService.setChecked(False)
        self.canvas.state = State.VM

    def toggleStorage(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionComputer.setChecked(False)
        self.ui.actionStorage.setChecked(True)
        self.ui.actionRouter.setChecked(False)
        self.ui.actionEdge.setChecked(False)
        self.ui.actionDomain.setChecked(False)
        self.ui.actionService.setChecked(False)
        self.canvas.state = State.Storage

    def toggleRouter(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionComputer.setChecked(False)
        self.ui.actionStorage.setChecked(False)
        self.ui.actionRouter.setChecked(True)
        self.ui.actionEdge.setChecked(False)
        self.ui.actionDomain.setChecked(False)
        self.ui.actionService.setChecked(False)
        self.canvas.state = State.Switch

    def toggleEdge(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionComputer.setChecked(False)
        self.ui.actionStorage.setChecked(False)
        self.ui.actionRouter.setChecked(False)
        self.ui.actionEdge.setChecked(True)
        self.ui.actionDomain.setChecked(False)
        self.ui.actionService.setChecked(False)
        self.canvas.state = State.Edge

    def toggleDomain(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionComputer.setChecked(False)
        self.ui.actionStorage.setChecked(False)
        self.ui.actionRouter.setChecked(False)
        self.ui.actionEdge.setChecked(False)
        self.ui.actionDomain.setChecked(True)
        self.ui.actionService.setChecked(False)
        self.canvas.state = State.Domain

    def toggleService(self):
        self.ui.actionSelect.setChecked(False)
        self.ui.actionComputer.setChecked(False)
        self.ui.actionStorage.setChecked(False)
        self.ui.actionRouter.setChecked(False)
        self.ui.actionEdge.setChecked(False)
        self.ui.actionDomain.setChecked(False)
        self.ui.actionService.setChecked(True)
        self.canvas.state = State.Vnf
    
    def Settings(self):
        d = TenantSettingsDialog()
        d.Load(self.tenant)
        d.exec_()
        if d.result() == QDialog.Accepted:
            d.SetResult(self.tenant)
            
    def resizeEvent(self, e):
        super(QMainWindow, self).resizeEvent(e)
        self.canvas.ResizeCanvas()

    def showEvent(self, e):
        super(QMainWindow, self).showEvent(e)
        self.canvas.ResizeCanvas()

    def New(self):
        self.tenant = Tenant()
        self.canvas.Clear()
        self.canvas.Visualize(self.tenant)
        self.canvas.changed = True
        self.xmlfile = None
        self.setWindowTitle(self.tr("Untitled") + " - " + self.basename)

    def Open(self):
        return
        name = QFileDialog.getOpenFileName(filter="*.xml")
        if name == None or name == '':
            return
        self.tenant.LoadFromXML(name)
        self.canvas.Clear()
        self.canvas.Visualize(self.tenant)
        self.canvas.changed = True
        self.xmlfile = name
        self.setWindowTitle(str(self.xmlfile).split('/').pop().split('.')[0] + " - " + self.basename)

    def Save(self):
        self.canvas.updatePos()
        if self.xmlfile == None:
            self.SaveAs()
        else:
            output = open(self.xmlfile, 'w')
            output.write(self.tenant.ExportToXml())
            output.close()

    def SaveAs(self):
        self.canvas.updatePos()
        self.xmlfile = QFileDialog.getSaveFileName(directory=".xml", filter="*.xml")
        if self.xmlfile != '':
            output = open(self.xmlfile, 'w')
            output.write(self.tenant.ExportToXml())
            output.close()
            self.setWindowTitle(str(self.xmlfile).split('/').pop().split('.')[0] + " - " + self.basename)

    def closeEvent(self, e):
        self.canvas.updatePos()
