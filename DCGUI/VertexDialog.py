from Core.Tenant import *
from PyQt4.QtCore import Qt
from PyQt4.QtGui import QDialog, QValidator, QIntValidator, QDoubleValidator, QTableWidgetItem, QLineEdit
from DCGUI.Windows.ui_TenantVM import Ui_TenantVM
from DCGUI.Windows.ui_TenantStorage import Ui_TenantStorage
from DCGUI.Windows.ui_TenantSwitch import Ui_TenantSwitch
from DCGUI.Windows.ui_TenantVnf import Ui_TenantVnf
from DCGUI.Windows.ui_TenantDomain import Ui_TenantDomain
from DCGUI.Windows.ui_TenantEdge import Ui_TenantEdge
from DCGUI.Windows.ui_ResourceServer import Ui_ResourceServer

class VertexDialog(QDialog):
    showLimits = True
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
        self.paramnames = {}
        for p in v.params:
            self.ui.params.insertRow(0)
            name = p.name
            if p.unit:
                name += " (" + p.unit + ")"
            self.paramnames[name] = p.name
            it = QTableWidgetItem(name)
            it.setFlags(Qt.ItemIsEnabled)
            self.ui.params.setItem(0, 0, it)
            types = p.type
            if self.showLimits and (p.type != "string"):
                types += " [" + str(p.minv) + "..." + str(p.maxv) + "]"
            it = QTableWidgetItem(types)
            it.setFlags(Qt.ItemIsEnabled)
            self.ui.params.setItem(0, 1, it)
            it = QTableWidgetItem(str(p.value))
            it.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable)
            le = QLineEdit(self.ui.params)
            le.setText(str(p.value))
            if p.type == "integer":
                valid = QIntValidator(p.minv, p.maxv, self)
                le.setValidator(valid)
            if p.type == "real":
                valid = QDoubleValidator(p.minv, p.maxv, 10, self)
                le.setValidator(valid)
            self.ui.params.setCellWidget(0, 2, le)
            self.ui.params.resizeColumnsToContents()
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
                if p.name == self.paramnames[str(self.ui.params.item(i, 0).text())]:
                    if self.ui.params.cellWidget(i, 2).validator():
                        if self.ui.params.cellWidget(i, 2).validator().validate(self.ui.params.cellWidget(i, 2).text(), 0)[0] == QValidator.Acceptable:
                            p.value = str(self.ui.params.cellWidget(i, 2).text())
        
class VMDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_TenantVM())
        
    def Load(self, v):
        self.LoadCommon(v)
        for s in ParamFactory.images:
            self.ui.image.addItem(s)
        for i in range(self.ui.image.count()):
            if self.ui.image.itemText(i) == v.image:
                self.ui.image.setCurrentIndex(i)
        self.ui.external.insertItem(0, "None")
        self.ui.external.setCurrentIndex(0)

    def SetResult(self, v):
        self.SetResultCommon(v)
        v.image = str(self.ui.image.currentText())
        if self.ui.external.currentIndex() == 0:
            v.external = "False"
        else:
            v.external = str(self.ui.external.currentText())
        
class ServerDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_ResourceServer())
        
    def Load(self, v):
        self.LoadCommon(v)

    def SetResult(self, v):
        self.SetResultCommon(v)
        
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

    def Load(self, v):
        self.LoadCommon(v)
        self.ui.type.setCurrentIndex(0 if v.type == "Switch" else 1)
        self.ui.ip.setText(v.ip)
        self.ui.router.setChecked(v.router)
        self.ui.serviceasuser.setChecked(v.isservice)
        self.ui.servicename.setText(v.servicename)
        self.ui.provider.setText(v.provider)
        self.ui.port.setText(v.port)

    def SetResult(self, v):
        self.SetResultCommon(v)
        v.type = "Switch" if self.ui.type.currentIndex() == 0 else "Router"
        v.router = self.ui.router.isChecked()
        v.ip = str(self.ui.ip.text())
        v.servicename = str(self.ui.servicename.text())
        v.provider = str(self.ui.provider.text())
        v.port = str(self.ui.port.text())
        v.isservice = self.ui.serviceasuser.isChecked()

class VnfDialog(VertexDialog):
    def __init__(self):
        VertexDialog.__init__(self, Ui_TenantVnf())

    def Load(self, v):
        self.LoadCommon(v)        
        for s in ParamFactory.vnfimages:
            self.ui.image.addItem(s)
        for i in range(self.ui.image.count()):
            if self.ui.image.itemText(i) == v.image:
                self.ui.image.setCurrentIndex(i)
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
        v.image = str(self.ui.image.currentText())
        
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