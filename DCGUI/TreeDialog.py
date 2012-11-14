from PyQt4.QtGui import QDialog
from DCGUI.Windows.ui_TreeDialog import Ui_TreeDialog

class TreeDialog(QDialog):
    def __init__(self, type):
        QDialog.__init__(self)
        self.type = type
        self.ui = Ui_TreeDialog()
        self.ui.setupUi(self)
        if type==1:
            self.ui.routerChilds0.hide()
            self.ui.label1.hide()
            self.ui.label2.hide()
        elif type==2:
            self.ui.level1.hide()
            self.ui.level2.setTitle("1st tier")
            self.ui.numRouters1Lab.hide()
            self.ui.routersNum1.hide()
        elif type==3:
            self.ui.numRouters1Lab.hide()
            self.ui.routersNum1.hide()
        self.adjustSize()

    def GetResult(self):
        return {"type":self.type,
                "routersNum0":int(self.ui.routersNum0.text()), 
                "routerBandwidth0":int(self.ui.routerBandwidth0.text()),
                "routerChilds0":int(self.ui.routerChilds0.text()), 
                "channelsBandwidth0":int(self.ui.channelsBandwidth0.text()), 
                "routerBandwidth1":int(self.ui.routerBandwidth1.text()),
                "routerChilds1": int(self.ui.routerChilds1.text()),
                "channelsBandwidth1": int(self.ui.channelsBandwidth1.text()),
                "routerBandwidth2": int(self.ui.routerBandwidth2.text()),
                "computerChannelsBandwidth2": int(self.ui.computerChannelsBandwidth2.text()),
                "storageChannelsBandwidth2": int(self.ui.storageChannelsBandwidth2.text()),
                "computersNodes": int(self.ui.computersNodes.text()),
                "storagesNodes": int(self.ui.storagesNodes.text()),
                "computersNum": int(self.ui.computersNum.text()),
                "storagesNum": int(self.ui.storagesNum.text()),
                "performance": int(self.ui.performance.text()),
                "numTypes": int(self.ui.numTypes.text()),
                "capacity": int(self.ui.capacity.text()),
                "routersNum1": int(self.ui.routersNum1.text())
                }

    def nodeNumChanged(self):
        if self.type==1:
            computersNodes = self.ui.routersNum1.value()*self.ui.routerChilds1.value()
            step = self.ui.routersNum1.value()
        elif self.type==2:
            computersNodes = self.ui.routersNum0.value()*self.ui.routerChilds0.value()
            step = self.ui.routersNum0.value()
        else:
            computersNodes = self.ui.routersNum0.value()*self.ui.routerChilds0.value()*self.ui.routerChilds1.value()
            step = self.ui.routerChilds0.value()*self.ui.routersNum0.value()
        self.ui.computersNodes.setMaximum(computersNodes)
        self.ui.computersNodes.setSingleStep(step)
        self.ui.computersNodes.setValue(computersNodes)
        self.ui.storagesNodes.setSingleStep(step)
        self.ui.storagesNodes.setMaximum(computersNodes)
        self.ui.storagesNodes.setValue(0)
        

    def computersNodesChanged(self):
        computersNodes = self.ui.computersNodes.value()
        step = self.ui.computersNodes.singleStep()
        computersNodes = int(computersNodes / step) * step
        self.ui.computersNodes.setValue(computersNodes)
        if self.type==1:
            sum = self.ui.routersNum1.value()*self.ui.routerChilds1.value()
        elif self.type==2:
            sum = self.ui.routersNum0.value()*self.ui.routerChilds0.value()
        else:
            sum = self.ui.routersNum0.value()*self.ui.routerChilds0.value()*self.ui.routerChilds1.value()
        self.ui.storagesNodes.setValue(sum - computersNodes)

    def storagesNodesChanged(self):
        storagesNodes = self.ui.storagesNodes.value()
        step = self.ui.storagesNodes.singleStep()
        storagesNodes = int(storagesNodes / step) * step
        self.ui.storagesNodes.setValue(storagesNodes)
        if self.type==1:
            sum = self.ui.routersNum1.value()*self.ui.routerChilds1.value()
        elif self.type==2:
            sum = self.ui.routersNum0.value()*self.ui.routerChilds0.value()
        else:
            sum = self.ui.routersNum0.value()*self.ui.routerChilds0.value()*self.ui.routerChilds1.value()
        self.ui.computersNodes.setValue(sum - storagesNodes)