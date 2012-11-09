from PyQt4.QtGui import QDialog
from DCGUI.Windows.ui_TreeDialog import Ui_TreeDialog

class TreeDialog(QDialog):
    def __init__(self, levels):
        QDialog.__init__(self)
        self.ui = Ui_TreeDialog()
        self.ui.setupUi(self)
        if levels==2:
            self.ui.level1.hide()
            self.ui.level2.setTitle("1st tier")
        self.adjustSize()

    def GetResult(self):
        return {"routersNum0":int(self.ui.routersNum0.text()), 
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
                "capacity": int(self.ui.capacity.text())
                }