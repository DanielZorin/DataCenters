from PyQt4.QtGui import QDialog
from DCGUI.Windows.ui_RandomDemandDialog import Ui_RandomDemandDialog

     
class RandomDemandDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)
        self.ui = Ui_RandomDemandDialog()
        self.ui.setupUi(self)
        
    def GetResult(self):
        return {"n":int(self.ui.n.text()), 
                "vm_min":int(self.ui.t1.text()), 
                "vm_max":int(self.ui.t2.text()), 
                "st_min":int(self.ui.v1.text()), 
                "st_max":int(self.ui.v2.text()),
                "vms": int(self.ui.vms.text()),
                "storages": int(self.ui.storages.text()),
                "cap_min": int(self.ui.c1.text()),
                "cap_max": int(self.ui.c2.text())
                }