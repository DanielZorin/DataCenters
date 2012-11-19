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
            self.ui.level2.setTitle(self.tr("1st layer"))
            self.ui.numRouters1Lab.hide()
            self.ui.routersNum1.hide()
        elif type==3:
            self.ui.numRouters1Lab.hide()
            self.ui.routersNum1.hide()
        self.adjustSize()

    def GetResult(self):
        return {"type":self.type,
                "routersNum0":self.ui.routersNum0.value(), 
                "routerBandwidth0":self.ui.routerBandwidth0.value(),
                "routerChilds0":self.ui.routerChilds0.value(), 
                "channelsBandwidth0":self.ui.channelsBandwidth0.value(), 
                "routerBandwidth1":self.ui.routerBandwidth1.value(),
                "routerChilds1":self.ui.routerChilds1.value(),
                "channelsBandwidth1":self.ui.channelsBandwidth1.value(),
                "routerBandwidth2":self.ui.routerBandwidth2.value(),
                "computerChannelsBandwidth2":self.ui.computerChannelsBandwidth2.value(),
                "storageChannelsBandwidth2":self.ui.storageChannelsBandwidth2.value(),
                "computersNodes":self.ui.computersNodes.value(),
                "storagesNodes":self.ui.storagesNodes.value(),
                "computersNum":self.ui.computersNum.value(),
                "storagesNum":self.ui.storagesNum.value(),
                "performance":self.ui.performance.value(),
                "numTypes":self.ui.numTypes.value(),
                "capacity":self.ui.capacity.value(),
                "routersNum1":self.ui.routersNum1.value(),
                "copyNum":self.ui.copyNum.value()
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