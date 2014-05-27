from Core.Tenant import *
from PyQt4.QtCore import Qt
from PyQt4.QtGui import QDialog, QIntValidator, QTableWidgetItem
from DCGUI.Windows.ui_TenantVM import Ui_TenantVM
from DCGUI.Windows.ui_TenantStorage import Ui_TenantStorage
from DCGUI.Windows.ui_TenantSwitch import Ui_TenantSwitch
from DCGUI.Windows.ui_TenantVnf import Ui_TenantVnf
from DCGUI.Windows.ui_TenantDomain import Ui_TenantDomain
from DCGUI.Windows.ui_TenantEdge import Ui_TenantEdge

class VertexDialog(QDialog):
    def __init__(self, ui):
        QDialog.__init__(self)        
        self.ui = ui
        self.ui.setupUi(self)
        self.ui.params.verticalHeader().hide()
        self.ui.params.horizontalHeader().setStretchLastSection(True)
        self.ui.add.setEnabled(False)
        self.ui.remove.setEnabled(False)

    def LoadCommon(self, v):
        ind = v.id.find("!")
        if ind != -1:
            self.hash = v.id[ind:]
            id = v.id[:ind]
        else:
            self.hash = ''
            id = v.id
        self.ui.name.setText(id)
        self.ui.service.setChecked(v.service)
        for p in v.params:
            self.ui.params.insertRow(0)
            it = QTableWidgetItem(p.name)
            it.setFlags(Qt.ItemIsEnabled)
            self.ui.params.setItem(0, 0, it)
            it = QTableWidgetItem(p.type + " [" + p.minv + "..." + p.maxv + "]")
            it.setFlags(Qt.ItemIsEnabled)
            self.ui.params.setItem(0, 1, it)
            it = QTableWidgetItem(str(p.value))
            it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
            self.ui.params.setItem(0, 2, it)
        height = 0
        for i in range(self.ui.params.rowCount()):
            height += self.ui.params.rowHeight(i)
        # Dirty resizing to make the table visible
        self.resize(self.width(), self.height() + height - self.ui.params.height())

    def AddParam(self):
        self.ui.params.insertRow(0)
        self.ui.params.setItem(0, 0, QTableWidgetItem("param"))
        self.ui.params.setItem(0, 1, QTableWidgetItem("int"))
        self.ui.params.setItem(0, 2, QTableWidgetItem(str(1)))

    def RemoveParam(self):
        self.ui.params.removeRow(self.ui.params.currentRow())

    def SetResultCommon(self, v):
        v.id = str(self.ui.name.text() + self.hash)
        v.service = self.ui.service.isChecked()
        for i in range(self.ui.params.rowCount()):
            for p in v.params:
                if p.name == str(self.ui.params.item(i, 0).text()):
                    p.value = str(self.ui.params.item(i, 2).text())
        
class VMDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_TenantVM())
        
    def Load(self, v):
        self.LoadCommon(v)
        self.ui.image.setText(v.image)

    def SetResult(self, v):
        self.SetResultCommon(v)
        v.image = str(self.ui.image.text())
        
class StorageDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_TenantStorage())
        
    def Load(self, v):
        self.LoadCommon(v)

    def SetResult(self, v):
        self.SetResultCommon(v)

class DomainDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_TenantDomain())
        
    def Load(self, v):
        self.LoadCommon(v)
        self.ui.type.setText(v.type)

    def SetResult(self, v):
        self.SetResultCommon(v)
        v.type = str(self.ui.type.text())

class SwitchDialog(VertexDialog):
    block = False
    def __init__(self, curname = "", tenants = []):
        VertexDialog.__init__(self, Ui_TenantSwitch())
        self.services = {}
        if tenants == None:
            block = True
            self.ui.provider.setEnabled(False)
            self.ui.servicename.setEnabled(False)
            self.ui.port.setEnabled(False)
            self.ui.serviceasuser.setEnabled(False)
            return
        for t in tenants:
            for v in t.vertices:
                if isinstance(v, Vnf) and v.isservice:
                    if v.username == curname:
                        name = t.name
                        servicename = v.servicename
                        conset = v.connectionset
                        if not name in self.services:
                            self.services[name] = {}
                        self.services[name][servicename] = conset
        for t in self.services.keys():
            self.ui.provider.addItem(t)
        #if self.services:
        #    self.ProviderChanged(0)

    def Load(self, v):
        self.LoadCommon(v)
        self.ui.type.setCurrentIndex(0 if v.type == "Switch" else 1)
        self.ui.ip.setText(v.ip)
        self.ui.router.setChecked(v.router)
        self.ui.serviceasuser.setChecked(v.isservice)
        if not v.isservice:
            return
        for i in range(self.ui.provider.count()):
            if self.ui.provider.itemText(i) == v.provider:
                self.ui.provider.setCurrentIndex(i)
        #self.ProviderChanged(0)
        for i in range(self.ui.servicename.count()):
            if self.ui.servicename.itemText(i) == v.servicename:
                self.ui.servicename.setCurrentIndex(i)
        #self.ServiceChanged(0)
        for i in range(self.ui.port.count()):
            if self.ui.port.itemText(i) == v.port:
                self.ui.port.setCurrentIndex(i)

    def SetResult(self, v):
        self.SetResultCommon(v)
        v.type = str(self.ui.type.currentText())
        v.router = self.ui.router.isChecked()
        v.ip = str(self.ui.ip.text())
        v.servicename = str(self.ui.servicename.currentText())
        v.provider = str(self.ui.provider.currentText())
        v.port = str(self.ui.port.currentText())
        v.isservice = self.ui.serviceasuser.isChecked()

    def ServiceChecked(self):
        pass

    def ProviderChanged(self, index):
        while self.ui.servicename.count() > 0:
            self.ui.servicename.removeItem(0)
        name = str(self.ui.provider.currentText())
        for p in self.services[name].keys():
            self.ui.servicename.addItem(p)
        #self.ServiceChanged(0)

    def ServiceChanged(self, index):
        while self.ui.port.count() > 0:
            self.ui.port.removeItem(0)
        prov = str(self.ui.provider.currentText())
        name = str(self.ui.servicename.currentText())
        for p in self.services[prov][name]:
            self.ui.port.addItem(p)

class VnfDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_TenantVnf())

    def Load(self, v):
        self.LoadCommon(v)        
        self.ui.type.setText(v.type)
        self.ui.profile.setText(v.profile)
        self.ui.serviceasprovider.setChecked(v.isservice)
        self.ui.servicename.setText(v.servicename)
        self.ui.username.setText(v.username)
        self.ui.set.setText(','.join(v.connectionset))

    def SetResult(self, v):
        self.SetResultCommon(v)
        v.type = str(self.ui.type.text())
        v.profile = str(self.ui.profile.text())
        v.isservice = self.ui.serviceasprovider.isChecked()
        v.username = str(self.ui.username.text())
        v.servicename = str(self.ui.servicename.text())
        v.connectionset = str(self.ui.set.text()).split(",")
        
    def ServiceChecked(self):
        pass

class EdgeDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)
        self.ui = Ui_TenantEdge()
        self.ui.setupUi(self)
        self.valid = QIntValidator(0, 1000000, self)
        self.ui.capacity.setValidator(self.valid)

    def Load(self, e):
        self.ui.capacity.setText(str(e.capacity))
        self.ui.service.setChecked(e.service)

    def SetResult(self, e):
        e.capacity = int(self.ui.capacity.text())
        e.service = self.ui.service.isChecked()