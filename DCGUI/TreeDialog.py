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
                "channelsBandwidth2": int(self.ui.channelsBandwidth2.text()),
                "computersNum": int(self.ui.computersNum.text()),
                "storagesNum": int(self.ui.storagesNum.text()),
                "performance": int(self.ui.performance.text()),
                "numTypes": int(self.ui.numTypes.text()),
                "capacity": int(self.ui.capacity.text()),
                "routersNum1": int(self.ui.routersNum1.text())
                }