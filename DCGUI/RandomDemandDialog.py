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
                "vms": 5,
                "storages": 5,
                "cap_min": 11,
                "cap_max": 15
                }